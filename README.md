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
├── include/      # target headers (varargs.h)
├── lib/          # runtime library (arith.az8 — 32-bit multiply/divide/modulo)
└── test/         # 15 end-to-end test programs
pcc-tests/        # submodule: PCC test suite (source for adapted tests)
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
- **Phase 6**: Testing — 15 end-to-end tests compile → assemble → link → execute on Z8002 emulator

All four binaries (`cz8`, `az8`, `ccz8`, `ldz8`) compile and link successfully.
15 test programs execute correctly on the Z8002 emulator (`cd z8000/test && make`):

**Core tests** (7):
`hello.c` (return 42), `arith.c` (recursive factorial), `control.c` (loops/pointers/arrays/structs),
`switch.c` (dense table jump + sparse binary search), `bitfield.c` (read/write), `shift.c` (word + long shifts),
`larith.c` (32-bit signed/unsigned multiply, divide, modulo).

**Adapted from pcc-tests** (8):
`pcc_math.c` (int div/mod/xor/or/and), `pcc_cmp.c` (systematic signed + unsigned comparisons),
`pcc_struct.c` (struct assignment from local/global/static), `pcc_structret.c` (struct return from function),
`pcc_union.c` (union pass-by-value + address-of parameter), `pcc_ptr.c` (pointer arrays + indexing),
`pcc_scope.c` (variable scoping + shadowing), `pcc_optim.c` (constant folding + dead code + loop with multiply).

### Fixes applied during Phase 6

**Assembler (`az8`):**
- `scan.c`: fixed `sopcode()` compound opcode fallback bug, added `!` as comment char
- `ins.c`: fixed `exts` to use size L; added `t_x` indexed addressing to `mult_op`/`div_op`; added memory-immediate compare to `alu_op`
- `ins.c`: fixed INC/DEC count encoding (stored n instead of n-1 in 4-bit field; validated against z8k-coff-as)
- `ins.c`: fixed SRL/SRA/SRLL/SRAL right-shift count not negated (Z8000 uses signed 16-bit count; validated against z8k-coff-as)

**Compiler backend (`cz8`):**
- `local2.c`: changed `ccbranches[]` to `"jr eq,.L%d"` compound opcode format
- `table.c`: fixed INTEMP template to route memory sources through `r0`/`rl0` scratch register (Z8000 LD can't do mem-to-mem)
- `table.c`: fixed INCR/DECR reversed operands
- `local2.c`: added callee-saved register save/restore in prologue/epilogue
- `trees.c`: added `case LONG:` to `tymatch()` logop switch — on Z8000 LONG≠INT (32 vs 16 bits), so LONG reaches `tymatch` unlike 68000/16032 ports where `ctype()` maps LONG→INT
- `table.c`: widened int→long and uint→ulong SCONV source shapes from `SAREG|STAREG` to `EA|STAREG|STBREG`; added LONG↔ULONG no-op SCONV template
- `local2.c`: removed double-free `reclaim()` from `cbgen()` case 'C' (match.c already calls reclaim after expand)
- `code.c`: fixed `genswitch()` table jump to use `jp @r1` instead of `jp @r0` (R0 can't be used for indirect addressing on Z8000)
- `table.c`: fixed bitfield assign templates — Z8000 has no memory-dest AND/OR, so load/modify/store through temp register; also fixed `OR` escape → literal `or`
- `table.c`: added long shift templates (slal/sral/srll for static, sdal/sdll for dynamic)
- `local2.c`: added ZQ escape to print register pair name for left operand
- `include/varargs.h`: added K&R-style variadic function support header

**Assembler (`az8`) — pseudo-ops:**
- `ps.c`/`init.c`/`inst.h`/`ins.c`: added `.zerow` pseudo-op (zero N words) — compiler emits this for zero-initialized word-sized static data; `.zerol` (zero N longs) already existed

**Runtime:**
- `crt0.az8`: fixed `_main`/`_exit` → `main`/`exit` (compiler does not prepend underscore to C symbols)
- `lib/arith.az8`: 32-bit arithmetic runtime library — `lmul`/`ulmul` (signed/unsigned multiply), `ldiv`/`uldiv` (divide), `lrem`/`ulrem` (remainder), plus assignment variants (`almul`, `aldiv`, `alrem`, `aulmul`, `auldiv`, `aulrem`). Unsigned ops use a fast path (two hardware DIV instructions) when divisor < 32768, otherwise binary long division (32 iterations)

### Known gaps (not yet implemented)

**High priority — blocks real programs:**
- **Float/double** — all ops emit library calls (`fadd`, `fsub`, `fmul`, `fdiv`, `fneg`, `float`, `fix` plus assignment variants); none implemented

**Medium priority — assembler encoding bugs (not emitted by compiler, affect hand-written assembly):**
- **BIT/SET/RES register mode** — `bit_op()` uses IR-mode opcodes for R-mode operands; `bitb rl0,#0` → `2680` instead of correct `A680`
- **DJNZ offset** — uses 4-bit offset field instead of 7-bit

**Low priority — cosmetic:**
- Linker prints "Undefined" warnings for local labels that are actually resolved; output is correct
- No standard library headers beyond `varargs.h`

## Key Technical Details

- Z8002 nonsegmented: 16-bit flat address space, 16x16-bit GPRs (R0-R15)
- int = short = pointer = 16 bits, long = 32 bits (register pairs)
- R14 = frame pointer, R15 = stack pointer
- R0 cannot be used for indirect/indexed addressing
- Register classes: SAREG (R0-R7 data), SBREG (R8-R13 address)
- All source is K&R C, requires warning suppression flags for modern clang
- Object format: b.out (big-endian, shared with 68000 toolchain)
