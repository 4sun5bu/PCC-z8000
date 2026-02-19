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
└── test/         # test programs (hello.c, arith.c, control.c, switch.c)
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
- **Phase 6**: Testing — 8 test programs compile through `cz8` and assemble through `az8` end-to-end

All four binaries (`cz8`, `az8`, `ccz8`, `ldz8`) compile and link successfully.
Eight test programs compile and assemble without errors: `hello.c`, `arith.c`, `control.c`, `switch.c`, `t.c`, `t6.c`, `t3.c`, `x.c`.

### Fixes applied during Phase 6

**Assembler (`az8`):**
- `scan.c`: fixed `sopcode()` compound opcode fallback bug, added `!` as comment char
- `ins.c`: fixed `exts` to use size L; added `t_x` indexed addressing to `mult_op`/`div_op`; added memory-immediate compare to `alu_op`

**Compiler backend (`cz8`):**
- `local2.c`: changed `ccbranches[]` to `"jr eq,.L%d"` compound opcode format
- `table.c`: fixed INTEMP template to route memory sources through `r0`/`rl0` scratch register (Z8000 LD can't do mem-to-mem)
- `table.c`: fixed INCR/DECR reversed operands
- `local2.c`: added callee-saved register save/restore in prologue/epilogue
- `trees.c`: added `case LONG:` to `tymatch()` logop switch — on Z8000 LONG≠INT (32 vs 16 bits), so LONG reaches `tymatch` unlike 68000/16032 ports where `ctype()` maps LONG→INT
- `table.c`: widened int→long and uint→ulong SCONV source shapes from `SAREG|STAREG` to `EA|STAREG|STBREG`; added LONG↔ULONG no-op SCONV template
- `local2.c`: removed double-free `reclaim()` from `cbgen()` case 'C' (match.c already calls reclaim after expand)
- `code.c`: fixed `genswitch()` table jump to use `jp @r1` instead of `jp @r0` (R0 can't be used for indirect addressing on Z8000)

### Known gaps (not yet implemented)

**High severity:**
- **Float/double** — all ops redirected to library calls (`fadd`, `fsub`, etc.) which don't exist yet; no float constants or comparisons

**Medium severity:**
- **Struct arguments (STARG)** — no templates for pushing structs as function args
- **Struct-valued returns (STCALL)** — no templates
- **Bitfield reads** — only bitfield write (assign) templates exist
- **Long multiply/divide** — uses library calls (`lmul`, `ulmul`, etc.) which don't exist yet; 16-bit works

**Low severity:**
- **Assembler pseudo-ops** — no `.space`, `.align N`, `.fill`, `.set`
- **Variadic functions** — no va_args/va_list support
- **Linker** — no weak symbols, minimal relocation validation

## Key Technical Details

- Z8002 nonsegmented: 16-bit flat address space, 16x16-bit GPRs (R0-R15)
- int = short = pointer = 16 bits, long = 32 bits (register pairs)
- R14 = frame pointer, R15 = stack pointer
- R0 cannot be used for indirect/indexed addressing
- Register classes: SAREG (R0-R7 data), SBREG (R8-R13 address)
- All source is K&R C, requires warning suppression flags for modern clang
- Object format: b.out (big-endian, shared with 68000 toolchain)
