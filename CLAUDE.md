# Z8000 PCC Port

## Project

Port of the Portable C Compiler (PCC) from the Motorola 68000 backend (`68000/c68/`) to the Zilog Z8002 (nonsegmented, 16-bit flat address space). The full implementation plan is at `.claude/plans/kind-tickling-pebble.md` (if available) or can be reconstructed from the code.

## Directory Structure

```
z8000/
├── cz8/          # compiler backend (builds to cz8)
├── az8/          # assembler (builds to az8)
├── ccz8.c        # compiler driver
├── ldz8.c        # linker
├── crt0.az8      # C runtime startup
├── b.out.h       # object file format header
└── test/         # test programs (hello.c, arith.c, control.c)
```

## Build

```bash
cd z8000/cz8 && make          # compiler backend
cd z8000/az8 && make          # assembler
cd z8000 && cc -O -w -Wno-implicit-int -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-int-conversion -Wno-incompatible-function-pointer-types -o ccz8 ccz8.c
cd z8000 && cc -O -w -Wno-implicit-int -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-int-conversion -o ldz8 ldz8.c
```

## Current Status

### Completed (Phases 1-5)

- **Phase 1**: Machine-independent PCC files copied, `macdefs` and `mac2defs` written for Z8002
- **Phase 2**: Machine-dependent compiler files written (`code.c`, `local.c`, `local2.c`, `order.c`)
- **Phase 3**: Instruction templates written (`table.c`)
- **Phase 4**: Z8000 assembler (`az8`) written and builds
- **Phase 5**: Driver (`ccz8.c`), linker (`ldz8.c`), and runtime (`crt0.az8`) written and build

All four binaries (`cz8`, `az8`, `ccz8`, `ldz8`) compile and link successfully.

### In Progress (Phase 6: Testing)

The compiler produces correct-looking Z8000 assembly. The simple test `main(){return 42;}` compiles through `cz8` and assembles through `az8` end-to-end. More complex tests (`test/arith.c`, `test/control.c`) compile but hit assembler limitations.

#### Remaining fixes needed (assembler `az8/ins.c`):

1. **Rebuild az8 and cz8** — source changes made but binaries not yet rebuilt:
   - `az8/scan.c`: fixed `sopcode()` compound opcode fallback bug, added `!` as comment char
   - `az8/ins.c`: fixed `exts` to use size L instead of W
   - `cz8/local2.c`: changed `ccbranches[]` from `"jr eq,.L%d"` to `"jreq .L%d"` format

2. **Add indexed addressing (`t_x`) to `mult_op` and `div_op`** in `az8/ins.c`:
   - `mult .rr0,6(.r14)` currently fails with "Invalid operand"
   - Need to add `t_x` case with addressed-mode opcode (0x5900 for MULT, 0x5B00 for DIV)

3. **Add memory-immediate compare to `alu_op`** in `az8/ins.c`:
   - `cp 4(.r14),#1` fails — `alu_op` only supports register as first operand
   - Need to handle `t_x` or `t_normal` as first operand with `t_immed` as second

4. **Test the full pipeline** with `test/arith.c` and `test/control.c`

## Key Technical Details

- Z8002 nonsegmented: 16-bit flat address space, 16x16-bit GPRs (R0-R15)
- int = short = pointer = 16 bits, long = 32 bits (register pairs)
- R14 = frame pointer, R15 = stack pointer
- R0 cannot be used for indirect/indexed addressing
- Register classes: SAREG (R0-R7 data), SBREG (R8-R13 address)
- All source is K&R C, requires warning suppression flags for modern clang
- Object format: b.out (big-endian, shared with 68000 toolchain)
