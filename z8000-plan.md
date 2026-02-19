# Z8000 PCC Port — Implementation Plan

## Context

Port the Portable C Compiler (PCC) from the Motorola 68000 backend (`68000/c68/`) to the Zilog Z8002 (nonsegmented, 16-bit flat address space). The result is a full cross-compilation toolchain: compiler backend (`cz8`), assembler (`az8`), linker (`ldz8`), and driver (`ccz8`).

## Target: Z8002 Nonsegmented

- 16 x 16-bit GPRs (R0-R15), with overlapping byte (RH/RL0-7), long-pair (RR0-RR14 even), quad (RQ0-RQ12 multiples of 4)
- R0 **cannot** be used for indirect/indexed addressing
- R15 = hardware stack pointer (grows down)
- 16-bit flat address space (64KB)
- Hardware MULT (16x16→32), DIV (32/16→16q+16r)
- No hardware FPU
- 8 addressing modes: R, IR, IM, DA, X, RA, BA, BX

## Type Sizes

| Type | Bits | Notes |
|------|------|-------|
| char | 8 | |
| short | 16 | = int |
| int | 16 | native word |
| long | 32 | register pair (RRn) |
| pointer | 16 | = int |
| float | 32 | software library |
| double | 64 | software library |

## Calling Convention

- R15 = SP, R14 = FP (frame pointer)
- R0 = return value (word), RR0 = return value (long)
- Arguments on stack, right-to-left, 16-bit aligned
- Callee-saved: R4-R7 (data), R10-R13 (address)
- Caller-saved: R0-R3, R8-R9
- Prologue: `push @.sp,.r14 / ld .r14,.sp / sub .sp,#framesize`
- Epilogue: `ld .sp,.r14 / pop .r14,@.sp / ret`

## Register Classes (PCC model)

- **SAREG** (R0-R7): data/arithmetic registers. R0-R3 scratch, R4-R7 reg vars
- **SBREG** (R8-R13): address/pointer registers. R8-R9 scratch, R10-R13 reg vars
- R14 (FP), R15 (SP): SBREG, not allocatable as temporaries

## Directory Structure

```
z8000/
├── cz8/          # compiler backend
│   ├── macdefs       # type sizes, register constants
│   ├── mac2defs      # register defs, shapes
│   ├── code.c        # prologue/epilogue, switch
│   ├── local.c       # pass 1 tree transforms
│   ├── local2.c      # pass 2: adrput, zzzcode, register names
│   ├── order.c       # register allocation, SU numbers
│   ├── table.c       # instruction templates (biggest file)
│   ├── Makefile
│   └── (copied unchanged: cgram.y cgram.c trees.c pftn.c optim.c
│        scan.c xdefs.c comm1.c allo.c match.c reader.c
│        mfile1 mfile2 manifest common)
├── az8/          # assembler
│   ├── inst.h        # instruction dispatch numbers
│   ├── mical.h       # assembler defs
│   ├── init.c        # instruction table init
│   ├── ins.c         # Z8000 instruction encoding (biggest file)
│   ├── scan.c        # input scanning (register names, addressing syntax)
│   ├── sym.c         # symbol table
│   ├── error.c       # errors
│   ├── rel.c         # relocation (b.out format)
│   ├── ps.c          # pseudo-ops
│   ├── sdi.c         # span-dependent instruction resolution
│   └── Makefile
├── ccz8.c        # driver (based on cc68.c)
├── ldz8.c        # linker (based on ld68.c, minimal changes)
└── crt0.az8      # C runtime startup
```

## Implementation Steps

### Phase 1: Compiler Backend — Machine Definitions & Framework

1. **Create `z8000/cz8/` directory and copy machine-independent files** from `68000/c68/`:
   `cgram.y cgram.c trees.c pftn.c optim.c scan.c xdefs.c comm1.c allo.c match.c reader.c mfile1 mfile2 manifest common`

2. **Write `macdefs`**: SZINT=16, SZPOINT=16, SZLONG=32, ARGINIT=32, AUTOINIT=0, STKREG=14, ARGREG=14, MINRVAR=4, MAXRVAR=7. Define BACKAUTO, BACKTEMP, ONEPASS.

3. **Write `mac2defs`**: R0-R7 (SAREG), R8-R13 (SBREG), FP=14, SP=15. REGSZ=16, TMPREG=FP. Shapes: SCCON (-128..127), SICON (0..32767), S8CON (1..8). No NESTCALLS.

4. **Fix 3 compile errors** inherited from 68000 sources:
   - `code.c`: rename `tmpfile` variable (clashes with stdlib)
   - `local2.c:277`: fix `& =` → `&=`
   - `scan.c:105`: fix `dimtab[NULL]` → `dimtab[0]`

### Phase 2: Compiler Backend — Machine-Dependent Code

5. **Write `local.c`** (adapt from 68000):
   - `clocal()`: NAME→OREG for AUTO/PARAM via R14. PCONV trivial (pointers=int=16 bit)
   - `ctype()`: pass-through (LONG ≠ INT on Z8000, unlike 68000)
   - `cisreg()`: true for all scalar types
   - `notoff()`: offsets -32768..32767 legal for R1-R15, reject R0

6. **Write `code.c`** (adapt from 68000):
   - `bfcode()`: Z8000 prologue (push FP / ld FP,SP / sub SP,#framesize / save regs via LDM)
   - `efcode()`: struct return handling, branch to retlab
   - `genswitch()`: table switch using indexed LD + JP, or binary search using CP/JR
   - `branch()`, `deflab()`, `locctr()`: change `.bra`→`jr`, labels `.L%d`

7. **Write `local2.c`** (major rewrite):
   - `rnames[]`: `.r0`..`.r13`, `.r14`, `.sp`
   - `rstatus[]`: SAREG|STAREG for R0-R7, SBREG|STBREG for R8-R13, SBREG for R14,R15
   - `adrput()`: NAME→symbol, ICON→`#val`, REG→register name, OREG→`offset(.rN)` or `@.rN`
   - `zzzcode()`: ZB (type suffix b/w/l), ZC (call cleanup), ZI (conditional branch), ZS (struct copy via LDIR), ZP (arg stack tracking), Z- (push)
   - `hopcode()`: map operators to Z8000 mnemonics (add/sub/and/or/xor/sla/sra)
   - `ccbranches[]`: `jr eq/ne/lt/le/gt/ge/ult/ule/ugt/uge`
   - `hardops()`: long mul/div/mod → library calls; all float/double → library calls
   - `szty()`: returns 2 for LONG/ULONG/FLOAT, 1 otherwise (DOUBLE handled by hardops, never in regs)
   - `rmove()`: `ld .rN,.rM` for word, `ldl .rrN,.rrM` for long

8. **Write `order.c`** (adapt from 68000):
   - `rallo()`: FORCE→R0|MUSTDO, MUL/DIV→even register for pair result
   - `offstar()`: direct address computations into INTBREG (R8-R13)
   - `indexreg()`: returns true for R1-R15, false for R0
   - `shumul()`: STARNM only (no STARREG — Z8000 has no auto-increment)
   - `argsize()`: int/pointer = 2 bytes, long = 4 bytes, double = 8 bytes (NOT 4 for everything like 68000)
   - `sucomp()`: adjust SU numbers for szty=2 longs

### Phase 3: Instruction Templates

9. **Write `table.c`** — the largest single piece of work. Template categories:

   **ASSIGN**: `ld`/`ldb`/`ldl` for reg-reg, reg-mem, mem-reg, clear with `clr`/`clrb`
   **OPLTYPE** (loads): `ld`/`ldb`/`ldl` into SAREG/SBREG, FORARG templates using `push @.sp,.rN`
   **OPLOG** (compare): `cp`/`cpb`/`cpl` + `ZI` conditional branch; `test`/`testb` for compare-with-zero
   **ASG PLUS/MINUS**: `add`/`addb`/`addl`, `sub`/`subb`/`subl`; `inc`/`dec` for small constants
   **ASG OR/AND/ER**: `or`/`and`/`xor` + byte variants
   **ASG LS/RS**: `sla`/`sra`/`sll`/`srl` with immediate count or register shift via `sda`/`sdl`
   **MUL**: `mult .rrN, src` (16x16→32, extract low word). Needs even-register constraint
   **DIV/MOD**: `exts .rrN` + `div .rrN, src`. Quotient in Rd+1, remainder in Rd
   **UNARY MINUS/COMPL**: `neg`/`negb`, `com`/`comb`
   **INCR/DECR**: `inc`/`dec` with immediate 1-16, or `add`/`sub` for larger
   **SCONV**: char→int via `extsb`, uchar→int via `and #0xFF`, int→long via `exts .rrN`
   **UNARY MUL**: indirect load `ld .rN, @.rM`
   **UNARY CALL**: `call symbol` or `call @.rN`
   **CCODES**: `ld .rN,#1` + conditional `ld .rN,#0`
   **STASG**: struct assignment via `ldir @.rN, @.rM, .r0` for large, individual LD for small
   **FLD**: bit field extract/insert via shifts and masks

### Phase 4: Assembler

10. **Create `z8000/az8/` directory**, copy framework files from `68000/a68/`:
    `sym.c error.c rel.c ps.c sdi.c Makefile`

11. **Write `inst.h`**: dispatch numbers for ~110 Z8000 instructions (ld, ldb, ldl, add, sub, cp, call, ret, jr, jp, push, pop, mult, div, exts, extsb, sla, sra, ldir, etc.)

12. **Write `mical.h`**: operand types (t_reg, t_ireg, t_immed, t_da, t_x, t_ra, t_ba, t_bx), instruction size limits

13. **Write `init.c`**: op_codes[] mapping mnemonic strings to dispatch numbers, register name initialization (`.r0`-`.r15`, `.rr0`-`.rr14`, `.rh0`-`.rh7`, `.rl0`-`.rl7`, `.sp`)

14. **Adapt `scan.c`**: recognize Z8000 register names and `@` for indirect addressing

15. **Write `ins.c`** (major work): Z8000 instruction encoding. The Z8000 has a fairly regular encoding:
    - Bits 15-8: opcode, Bits 7-4: dst reg, Bits 3-0: src reg (for register-register)
    - Multi-word for immediate/address operands
    - Encoding helpers: `encode_ld()`, `encode_alu()`, `encode_shift()`, `encode_call()`, `encode_jr()`, `encode_jp()`, `encode_push()`, etc.

16. **Adapt `sdi.c`**: JR (2 bytes, ±256 range) → JP (4 bytes) expansion

### Phase 5: Driver, Linker, Runtime

17. **Write `ccz8.c`** (adapt from cc68.c): change pass names (cz8, az8, oz8), file extensions (.az8), predefined macros (-Dz8000 -Dz8002)

18. **Adapt `ldz8.c`** from ld68.c: minimal changes — b.out format is machine-independent. Change default load address, use RWORD (16-bit) relocations for pointers

19. **Write `crt0.az8`**: C runtime startup — zero BSS, set up stack, call `main`, call `exit`

20. **Write runtime library stubs**: `lmul`, `ldiv`, `lrem` (32-bit arithmetic), float/double library routines (can be stubbed initially)

### Phase 6: Testing & Verification ✓

21. ✓ **Build the compiler**: `cd z8000/cz8 && make`
22. ✓ **Build the assembler**: `cd z8000/az8 && make`
23. ✓ **Test with simple programs**: `main(){return 42;}` compiles and assembles end-to-end
24. ✓ **Test arithmetic**: int add/sub/mul/div, long arithmetic, type conversions — 7 test programs pass (hello.c, arith.c, control.c, t.c, t6.c, t3.c, x.c)
25. **Test control flow**: if/else, while, for work; switch NOT YET IMPLEMENTED
26. **Test pointers and arrays**: basic indirection works; comprehensive tests needed
27. **Test function calls**: word and long args work; struct return NOT YET IMPLEMENTED
28. **End-to-end**: compile → assemble works; link → binary execution not yet tested on target

#### Bugs found and fixed during Phase 6:
- Assembler: `sopcode()` fallback, `!` comment char, `exts` size, indexed MULT/DIV, memory-immediate CP
- Compiler: INTEMP mem-to-mem, INCR/DECR operand order, callee-saved register saves, `tymatch()` missing LONG case, SCONV template shapes, LONG↔ULONG template, `cbgen()` double-free

### Phase 7: Remaining Work

**High priority:**
- Switch statement codegen (`genswitch()` in code.c)
- Float/double software library (or at minimum stubs)
- Pointer SCONV templates (ptr↔int, trivial on Z8000)

**Medium priority:**
- Struct argument passing (STARG templates)
- Struct-valued function returns (STCALL templates)
- Bitfield read/extraction templates
- Long multiply/divide runtime library (`lmul`, `ldiv`, `lrem`, `ulmul`, `uldiv`, `ulrem`)

**Low priority:**
- Assembler pseudo-ops: `.space`, `.align N`, `.fill`, `.set`
- Variadic function support
- Linker improvements (weak symbols, validation)

## Key Technical Risks

1. **Register pair allocation for longs**: PCC's `allo.c` allocates consecutive registers for szty=2. Z8000 requires even-register pairs. The natural register numbering (R0=0, R2=2, etc.) helps, but odd-starting pairs must be prevented. May need a small `allo.c` tweak or enforce via `rallo()` hints.

2. **No auto-increment addressing**: The 68000 heavily uses `reg@+`. Remove all STARREG templates. Structure copy uses Z8000's `LDIR` block transfer instead.

3. **16-bit int changes propagate widely**: argument sizes, constant ranges, type conversion paths, SU numbers all change from the 68000 baseline.

4. **BA/BX only with LD**: Base Address and Base Index modes are only available with Load instructions, not with arithmetic. This limits which templates can use OREG for source operands in ADD/SUB/etc. — need to load into register first.
