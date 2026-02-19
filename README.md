# Z8000 PCC Port

## Project

Port of the Portable C Compiler (PCC) from the Motorola 68000 backend (`68000/c68/`) to the Zilog Z8002 (nonsegmented, 16-bit flat address space). 

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

### Completed (Phases 1-6)

- **Phase 1**: Machine-independent PCC files copied, `macdefs` and `mac2defs` written for Z8002
- **Phase 2**: Machine-dependent compiler files written (`code.c`, `local.c`, `local2.c`, `order.c`)
- **Phase 3**: Instruction templates written (`table.c`)
- **Phase 4**: Z8000 assembler (`az8`) written and builds
- **Phase 5**: Driver (`ccz8.c`), linker (`ldz8.c`), and runtime (`crt0.az8`) written and build
- **Phase 6**: Testing — all test programs (`hello.c`, `arith.c`, `control.c`) compile through `cz8` and assemble through `az8` end-to-end

All four binaries (`cz8`, `az8`, `ccz8`, `ldz8`) compile and link successfully.
All three test programs compile and assemble without errors.

### Fixes applied during Phase 6

- `az8/scan.c`: fixed `sopcode()` compound opcode fallback bug, added `!` as comment char
- `az8/ins.c`: fixed `exts` to use size L; added `t_x` indexed addressing to `mult_op`/`div_op`; added memory-immediate compare to `alu_op`
- `cz8/local2.c`: changed `ccbranches[]` to `"jr eq,.L%d"` compound opcode format
- `cz8/table.c`: fixed INTEMP template to route memory sources through `r0`/`rl0` scratch register (Z8000 LD can't do mem-to-mem)

## Key Technical Details

- Z8002 nonsegmented: 16-bit flat address space, 16x16-bit GPRs (R0-R15)
- int = short = pointer = 16 bits, long = 32 bits (register pairs)
- R14 = frame pointer, R15 = stack pointer
- R0 cannot be used for indirect/indexed addressing
- Register classes: SAREG (R0-R7 data), SBREG (R8-R13 address)
- All source is K&R C, requires warning suppression flags for modern clang
- Object format: b.out (big-endian, shared with 68000 toolchain)
