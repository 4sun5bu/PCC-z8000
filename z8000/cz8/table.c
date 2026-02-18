# include "mfile2"

# define TSCALAR TCHAR|TUCHAR|TSHORT|TUSHORT|TINT|TUNSIGNED|TPOINT
# define TWORD TINT|TUNSIGNED|TSHORT|TUSHORT|TPOINT
# define EA SNAME|SOREG|SCON|SAREG|SBREG|STARNM
# define EAA SNAME|SOREG|SAREG|STARNM

struct optab  table[] = {

/* === ASSIGN === */

/* clear word/byte to zero */
ASSIGN,	INAREG|FOREFF,
	EAA,	TSCALAR,
	SZERO,	TANY,
		0,	RLEFT|RRIGHT,
		"	clrZB	AL\n",

/* clear long to zero - pair of clears */
ASSIGN,	INAREG|FOREFF,
	EAA,	TLONG|TULONG,
	SZERO,	TANY,
		0,	RLEFT|RRIGHT,
		"	clr	AL\n	clr	UL\n",

/* assign word/byte: reg = anything */
ASSIGN,	INAREG|FOREFF,
	SAREG|STAREG,	TSCALAR,
	EA,	TSCALAR,
		0,	RLEFT|RRIGHT,
		"	ldZB	AL,AR\n",

/* assign word/byte: mem/starnm = reg */
ASSIGN,	INAREG|FOREFF,
	SNAME|SOREG|STARNM,	TSCALAR,
	SAREG|SBREG,	TSCALAR,
		0,	RLEFT|RRIGHT,
		"	ldZB	AL,AR\n",

/* assign word/byte: mem = mem (through temp register) */
ASSIGN,	INAREG|FOREFF,
	SNAME|SOREG|STARNM,	TSCALAR,
	SNAME|SOREG|STARNM,	TSCALAR,
		NAREG,	RLEFT|RRIGHT,
		"	ldZB	A1,AR\n	ldZB	AL,A1\n",

/* assign word: breg = anything */
ASSIGN, INBREG|FOREFF,
	SBREG,	TWORD,
	EA,	TWORD,
		0,	RLEFT|RRIGHT,
		"	ld	AL,AR\n",

/* assign long: reg = reg (word by word) */
ASSIGN,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SAREG,	TLONG|TULONG,
		0,	RLEFT|RRIGHT,
		"	ld	AL,AR\n	ld	UL,UR\n",

/* assign long: reg = mem */
ASSIGN,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SNAME|SOREG|STARNM,	TLONG|TULONG,
		0,	RLEFT|RRIGHT,
		"	ld	AL,AR\n	ld	UL,UR\n",

/* assign long: mem = reg */
ASSIGN,	INAREG|FOREFF,
	SNAME|SOREG,	TLONG|TULONG,
	SAREG,	TLONG|TULONG,
		0,	RLEFT|RRIGHT,
		"	ld	AL,AR\n	ld	UL,UR\n",

/* assign long: reg = const (word by word) */
ASSIGN,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SCON,	TLONG|TULONG,
		0,	RLEFT|RRIGHT,
		"	ld	AL,AR\n	ld	UL,UR\n",

/* assign to bit field: clear field */
ASSIGN, INAREG|FOREFF,
	SFLD,	TANY,
	SZERO,	TANY,
		0,	RRIGHT,
		"	and	AL,#Z~\n",

/* assign to bit field: general case */
ASSIGN, INTAREG|INAREG|FOREFF,
	SFLD,	TANY,
	SAREG|STAREG,	TANY,
		NAREG,	RRIGHT,
		"F	push	@sp,AR\n	ld	A1,#H\n	sda	AR,A1\n	and	AR,#M\n	and	AL,#N\n	OR	AL,AR\nF	pop	AR,@sp\n",

/* === UNARY MUL (indirect load) === */

/* indirect load word through address register */
UNARY MUL,	INTAREG|INAREG,
	SBREG,	TWORD,
	SANY,	TANY,
		NAREG|NASR,	RESC1,
		"	ld	A1,@AL\n",

/* indirect load byte through address register */
UNARY MUL,	INTAREG|INAREG,
	SBREG,	TCHAR|TUCHAR,
	SANY,	TANY,
		NAREG|NASR,	RESC1,
		"	ldb	A1,@AL\n",

/* indirect load word through data register (must not be r0) */
UNARY MUL,	INTAREG|INAREG,
	SAREG,	TWORD,
	SANY,	TANY,
		NAREG|NASR,	RESC1,
		"	ld	A1,@AL\n",

/* indirect load byte through data register */
UNARY MUL,	INTAREG|INAREG,
	SAREG,	TCHAR|TUCHAR,
	SANY,	TANY,
		NAREG|NASR,	RESC1,
		"	ldb	A1,@AL\n",

/* indirect load into breg */
UNARY MUL,	INTBREG|INBREG,
	SBREG,	TWORD,
	SANY,	TANY,
		NBREG|NBSR,	RESC1,
		"	ld	A1,@AL\n",

/* === OPLTYPE (load) === */

/* throw away useless computation */
OPLTYPE,	FOREFF,
	SNAME|SOREG|SCON|SAREG|SBREG,	TANY,
	EA,	TANY,
		0,	RRIGHT,
		"",

/* test for condition code */
OPLTYPE,	FORCC,
	SANY,	TANY,
	SAREG,	TWORD,
		0,	RESCC,
		"	test	AR\n",

OPLTYPE,	FORCC,
	SANY,	TANY,
	SAREG,	TCHAR|TUCHAR,
		0,	RESCC,
		"	testb	AR\n",

OPLTYPE,	FORCC,
	SANY,	TANY,
	SNAME|SOREG,	TWORD,
		0,	RESCC,
		"	test	AR\n",

/* load zero into register */
OPLTYPE,	INTAREG|INAREG,
	SANY,	TANY,
	SZERO,	TSCALAR,
		NAREG|NASR,	RESC1,
		"	clrZB	A1\n",

/* load word/byte value into data register */
OPLTYPE,	INTAREG|INAREG,
	SANY,	TANY,
	EA,	TSCALAR,
		NAREG|NASR,	RESC1,
		"	ldZB	A1,AR\n",

/* load long value into data register (word by word) */
OPLTYPE,	INTAREG|INAREG,
	SANY,	TANY,
	EA,	TLONG|TULONG,
		NAREG|NASR,	RESC1,
		"	ld	A1,AR\n	ld	U1,UR\n",

/* load word into address register */
OPLTYPE,	INTBREG|INBREG,
	SANY,	TANY,
	EA,	TWORD,
		NBREG|NBSR,	RESC1,
		"	ld	A1,AR\n",

/* load into temp */
OPLTYPE,	INTEMP,
	SANY,	TANY,
	EA,	TSCALAR,
		NTEMP,	RESC1,
		"	ldZB	A1,AR\n",

/* push word argument - register source */
OPLTYPE,	FORARG,
	SANY,	TANY,
	SAREG|SBREG,	TWORD,
		0,	RNULL,
		"	push	@sp,AR\nZ-",

/* push word argument - constant source */
OPLTYPE,	FORARG,
	SANY,	TANY,
	SCON,	TWORD,
		0,	RNULL,
		"	push	@sp,AR\nZ-",

/* push word argument - memory source (load then push) */
OPLTYPE,	FORARG,
	SANY,	TANY,
	SNAME|SOREG|STARNM,	TWORD,
		0,	RNULL,
		"	ld	r0,AR\n	push	@sp,r0\nZ-",

/* push char argument - sign extend to word then push */
OPLTYPE,	FORARG,
	SANY,	TANY,
	EA,	TCHAR,
		0,	RNULL,
		"	ldb	rl0,AR\n	extsb	r0\n	push	@sp,r0\nZ-",

/* push unsigned char argument - zero extend to word then push */
OPLTYPE,	FORARG,
	SANY,	TANY,
	EA,	TUCHAR,
		0,	RNULL,
		"	clr	r0\n	ldb	rl0,AR\n	push	@sp,r0\nZ-",

/* push long argument (high word first to stack, so push low then high) */
OPLTYPE,	FORARG,
	SANY,	TANY,
	SAREG,	TLONG|TULONG,
		0,	RNULL,
		"	push	@sp,UR\n	push	@sp,AR\nZ-Z-",

/* push long argument from memory */
OPLTYPE,	FORARG,
	SANY,	TANY,
	SNAME|SOREG|STARNM,	TLONG|TULONG,
		0,	RNULL,
		"	ld	r0,UR\n	push	@sp,r0\n	ld	r0,AR\n	push	@sp,r0\nZ-Z-",

/* push long constant */
OPLTYPE,	FORARG,
	SANY,	TANY,
	SCON,	TLONG|TULONG,
		0,	RNULL,
		"	push	@sp,UR\n	push	@sp,AR\nZ-Z-",

/* === OPLOG (comparison) === */

/* compare word: reg vs anything */
OPLOG,	FORCC,
	SAREG|STAREG|SBREG|STBREG,	TWORD,
	EA,	TWORD,
		0,	RESCC,
		"	cp	AL,AR\nZI",

/* compare word: mem vs const */
OPLOG,	FORCC,
	SNAME|SOREG|SAREG|SBREG,	TWORD,
	SCON,	TWORD,
		0,	RESCC,
		"	cp	AL,AR\nZI",

/* compare byte: reg vs anything */
OPLOG,	FORCC,
	SAREG|STAREG,	TCHAR|TUCHAR,
	EA,	TCHAR|TUCHAR,
		0,	RESCC,
		"	cpb	AL,AR\nZI",

/* compare long: reg vs reg (word by word via cbgen) */
OPLOG,	FORCC,
	SAREG|STAREG,	TLONG|TULONG,
	SAREG,	TLONG|TULONG,
		0,	RESCC,
		"ZLZI",

/* compare long: reg vs mem */
OPLOG,	FORCC,
	SAREG|STAREG,	TLONG|TULONG,
	SNAME|SOREG|STARNM,	TLONG|TULONG,
		0,	RESCC,
		"ZLZI",

/* compare long: reg vs const */
OPLOG,	FORCC,
	SAREG|STAREG,	TLONG|TULONG,
	SCON,	TLONG|TULONG,
		0,	RESCC,
		"ZLZI",

/* === CCODES (set register to 0/1 based on condition) === */

CCODES,	INTAREG|INAREG,
	SANY,	TANY,
	SANY,	TANY,
		NAREG,	RESC1,
		"	ld	A1,#1\nZN",

/* === UNARY MINUS === */

UNARY MINUS,	INTAREG|INAREG,
	STAREG,	TWORD,
	SANY,	TANY,
		0,	RLEFT,
		"	neg	AL\n",

UNARY MINUS,	INTAREG|INAREG,
	STAREG,	TCHAR|TUCHAR,
	SANY,	TANY,
		0,	RLEFT,
		"	negb	AL\n",

/* === COMPL (bitwise complement) === */

COMPL,	INTAREG|INAREG,
	STAREG,	TWORD,
	SANY,	TANY,
		0,	RLEFT,
		"	com	AL\n",

COMPL,	INTAREG|INAREG,
	STAREG,	TCHAR|TUCHAR,
	SANY,	TANY,
		0,	RLEFT,
		"	comb	AL\n",

/* === INCR/DECR === */

/* post-increment with small constant (1-16) */
INCR,	INTAREG|INAREG|FOREFF,
	EAA,	TSCALAR,
	S16CON,	TSCALAR,
		NAREG,	RESC1,
		"F	ldZB	AL,A1\n	inc	AL,AR\n",

DECR,	INTAREG|INAREG|FOREFF,
	EAA,	TSCALAR,
	S16CON,	TSCALAR,
		NAREG,	RESC1,
		"F	ldZB	AL,A1\n	dec	AL,AR\n",

/* post-increment with general constant */
INCR,	INTAREG|INAREG|FOREFF,
	EAA,	TSCALAR,
	SCON,	TSCALAR,
		NAREG,	RESC1,
		"F	ldZB	AL,A1\n	add	AL,AR\n",

DECR,	INTAREG|INAREG|FOREFF,
	EAA,	TSCALAR,
	SCON,	TSCALAR,
		NAREG,	RESC1,
		"F	ldZB	AL,A1\n	sub	AL,AR\n",

/* post-increment into breg */
INCR,	INTBREG|INBREG|FOREFF,
	SBREG,	TWORD,
	S16CON,	TWORD,
		NBREG,	RESC1,
		"F	ld	AL,A1\n	inc	AL,AR\n",

DECR,	INTBREG|INBREG|FOREFF,
	SBREG,	TWORD,
	S16CON,	TWORD,
		NBREG,	RESC1,
		"F	ld	AL,A1\n	dec	AL,AR\n",

INCR,	INTBREG|INBREG|FOREFF,
	SBREG,	TWORD,
	SCON,	TWORD,
		NBREG,	RESC1,
		"F	ld	AL,A1\n	add	AL,AR\n",

DECR,	INTBREG|INBREG|FOREFF,
	SBREG,	TWORD,
	SCON,	TWORD,
		NBREG,	RESC1,
		"F	ld	AL,A1\n	sub	AL,AR\n",

/* === ASG PLUS / ASG MINUS === */

/* add/sub with small constant (1-16): inc/dec */
ASG PLUS,	INAREG|FOREFF,
	EAA,	TWORD,
	S16CON,	TWORD,
		0,	RLEFT,
		"	inc	AL,AR\n",

ASG MINUS,	INAREG|FOREFF,
	EAA,	TWORD,
	S16CON,	TWORD,
		0,	RLEFT,
		"	dec	AL,AR\n",

/* add/sub: reg op= src */
ASG PLUS,	INAREG|FOREFF,
	SAREG|STAREG,	TWORD,
	EA,	TWORD,
		0,	RLEFT,
		"	add	AL,AR\n",

ASG PLUS,	INBREG|FOREFF,
	SBREG|STBREG,	TWORD,
	EA,	TWORD,
		0,	RLEFT,
		"	add	AL,AR\n",

ASG MINUS,	INAREG|FOREFF,
	SAREG|STAREG,	TWORD,
	EA,	TWORD,
		0,	RLEFT,
		"	sub	AL,AR\n",

ASG MINUS,	INBREG|FOREFF,
	SBREG|STBREG,	TWORD,
	EA,	TWORD,
		0,	RLEFT,
		"	sub	AL,AR\n",

/* add/sub byte */
ASG PLUS,	INAREG|FOREFF,
	SAREG|STAREG,	TCHAR|TUCHAR,
	EA,	TCHAR|TUCHAR,
		0,	RLEFT,
		"	addb	AL,AR\n",

ASG MINUS,	INAREG|FOREFF,
	SAREG|STAREG,	TCHAR|TUCHAR,
	EA,	TCHAR|TUCHAR,
		0,	RLEFT,
		"	subb	AL,AR\n",

/* add/sub: mem op= reg */
ASG PLUS,	INAREG|FOREFF,
	SNAME|SOREG|STARNM,	TWORD,
	SAREG|STAREG,	TWORD,
		0,	RLEFT,
		"	add	AL,AR\n",

ASG MINUS,	INAREG|FOREFF,
	SNAME|SOREG|STARNM,	TWORD,
	SAREG|STAREG,	TWORD,
		0,	RLEFT,
		"	sub	AL,AR\n",

/* add/sub long: reg op= reg (word by word with carry) */
ASG PLUS,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SAREG,	TLONG|TULONG,
		0,	RLEFT,
		"	add	UL,UR\n	adc	AL,AR\n",

ASG MINUS,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SAREG,	TLONG|TULONG,
		0,	RLEFT,
		"	sub	UL,UR\n	sbc	AL,AR\n",

/* add/sub long: reg op= mem */
ASG PLUS,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SNAME|SOREG|STARNM,	TLONG|TULONG,
		0,	RLEFT,
		"	add	UL,UR\n	adc	AL,AR\n",

ASG MINUS,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SNAME|SOREG|STARNM,	TLONG|TULONG,
		0,	RLEFT,
		"	sub	UL,UR\n	sbc	AL,AR\n",

/* add/sub long: reg op= const */
ASG PLUS,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SCON,	TLONG|TULONG,
		0,	RLEFT,
		"	add	UL,UR\n	adc	AL,AR\n",

ASG MINUS,	INAREG|FOREFF,
	SAREG|STAREG,	TLONG|TULONG,
	SCON,	TLONG|TULONG,
		0,	RLEFT,
		"	sub	UL,UR\n	sbc	AL,AR\n",

/* === ASG OR / ASG AND / ASG ER (exclusive or) === */

/* xor must be separate since Z8000 xor syntax differs */
ASG ER,	INAREG|FOREFF,
	EAA,	TWORD,
	SCON,	TWORD,
		0,	RLEFT,
		"	xor	AL,AR\n",

ASG ER,	INAREG|FOREFF,
	EAA,	TWORD,
	SAREG|STAREG,	TWORD,
		0,	RLEFT,
		"	xor	AL,AR\n",

/* or/and: reg op= src */
ASG OPSIMP,	INAREG|FOREFF,
	SAREG|STAREG,	TWORD,
	EA,	TWORD,
		0,	RLEFT,
		"	OI	AL,AR\n",

/* or/and: mem op= const */
ASG OPSIMP,	INAREG|FOREFF,
	SNAME|SOREG,	TWORD,
	SCON,	TWORD,
		0,	RLEFT,
		"	OI	AL,AR\n",

/* or/and: mem op= reg */
ASG OPSIMP,	INAREG|FOREFF,
	SNAME|SOREG|STARNM,	TWORD,
	SAREG|STAREG,	TWORD,
		0,	RLEFT,
		"	OI	AL,AR\n",

/* byte variants */
ASG ER,	INAREG|FOREFF,
	SAREG|STAREG,	TCHAR|TUCHAR,
	EA,	TCHAR|TUCHAR,
		0,	RLEFT,
		"	xorb	AL,AR\n",

ASG OPSIMP,	INAREG|FOREFF,
	SAREG|STAREG,	TCHAR|TUCHAR,
	EA,	TCHAR|TUCHAR,
		0,	RLEFT,
		"	OIb	AL,AR\n",

/* === ASG LS / ASG RS (shifts) === */

/* word shift by immediate count */
ASG LS,	INAREG|FOREFF,
	SAREG|STAREG,	TWORD,
	SCON,	TWORD,
		0,	RLEFT,
		"	sla	AL,AR\n",

ASG RS,	INAREG|FOREFF,
	SAREG|STAREG,	TINT|TSHORT,
	SCON,	TWORD,
		0,	RLEFT,
		"	sra	AL,AR\n",

ASG RS,	INAREG|FOREFF,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	SCON,	TWORD,
		0,	RLEFT,
		"	srl	AL,AR\n",

/* word shift by register count (dynamic shift) */
ASG LS,	INAREG|FOREFF,
	SAREG|STAREG,	TWORD,
	SAREG,	TWORD,
		0,	RLEFT,
		"	sda	AL,AR\n",

ASG RS,	INAREG|FOREFF,
	SAREG|STAREG,	TINT|TSHORT,
	SAREG,	TWORD,
		0,	RLEFT,
		"	negZB	AR\n	sda	AL,AR\n",

ASG RS,	INAREG|FOREFF,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	SAREG,	TWORD,
		0,	RLEFT,
		"	negZB	AR\n	sdl	AL,AR\n",

/* byte shift */
ASG LS,	INAREG|FOREFF,
	SAREG|STAREG,	TCHAR|TUCHAR,
	SCON,	TCHAR|TUCHAR,
		0,	RLEFT,
		"	slab	AL,AR\n",

ASG RS,	INAREG|FOREFF,
	SAREG|STAREG,	TCHAR,
	SCON,	TCHAR|TUCHAR,
		0,	RLEFT,
		"	srab	AL,AR\n",

ASG RS,	INAREG|FOREFF,
	SAREG|STAREG,	TUCHAR,
	SCON,	TCHAR|TUCHAR,
		0,	RLEFT,
		"	srlb	AL,AR\n",

/* === MUL / DIV / MOD (16-bit hardware operations) === */
/* rallo ensures left operand is in r1 (part of pair rr0) */

/* signed multiply: r1 * src -> rr0, low word (result) in r1 */
MUL,	INAREG|INTAREG,
	SAREG|STAREG,	TINT|TSHORT,
	EA,	TINT|TSHORT,
		0,	RLEFT,
		"	mult	rr0,AR\n",

/* unsigned multiply */
MUL,	INAREG|INTAREG,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	EA,	TUNSIGNED|TUSHORT,
		0,	RLEFT,
		"	mult	rr0,AR\n",

/* signed divide: exts r1 -> rr0, then rr0/src -> r1(quot), r0(rem) */
DIV,	INAREG|INTAREG,
	SAREG|STAREG,	TINT|TSHORT,
	EA,	TINT|TSHORT,
		0,	RLEFT,
		"	exts	rr0\n	div	rr0,AR\n",

/* unsigned divide */
DIV,	INAREG|INTAREG,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	EA,	TUNSIGNED|TUSHORT,
		0,	RLEFT,
		"	subl	rr0,rr0\n	ld	r1,AL\n	div	rr0,AR\n",

/* signed modulus: same as div, then move remainder r0 -> r1 */
MOD,	INAREG|INTAREG,
	SAREG|STAREG,	TINT|TSHORT,
	EA,	TINT|TSHORT,
		0,	RLEFT,
		"	exts	rr0\n	div	rr0,AR\n	ld	r1,r0\n",

/* unsigned modulus */
MOD,	INAREG|INTAREG,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	EA,	TUNSIGNED|TUSHORT,
		0,	RLEFT,
		"	subl	rr0,rr0\n	ld	r1,AL\n	div	rr0,AR\n	ld	r1,r0\n",

/* ASG MUL/DIV/MOD */
ASG MUL,	INAREG,
	SAREG|STAREG,	TINT|TSHORT,
	EA,	TINT|TSHORT,
		0,	RLEFT,
		"	mult	rr0,AR\n",

ASG MUL,	INAREG,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	EA,	TUNSIGNED|TUSHORT,
		0,	RLEFT,
		"	mult	rr0,AR\n",

ASG DIV,	INAREG,
	SAREG|STAREG,	TINT|TSHORT,
	EA,	TINT|TSHORT,
		0,	RLEFT,
		"	exts	rr0\n	div	rr0,AR\n",

ASG DIV,	INAREG,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	EA,	TUNSIGNED|TUSHORT,
		0,	RLEFT,
		"	subl	rr0,rr0\n	ld	r1,AL\n	div	rr0,AR\n",

ASG MOD,	INAREG,
	SAREG|STAREG,	TINT|TSHORT,
	EA,	TINT|TSHORT,
		0,	RLEFT,
		"	exts	rr0\n	div	rr0,AR\n	ld	r1,r0\n",

ASG MOD,	INAREG,
	SAREG|STAREG,	TUNSIGNED|TUSHORT,
	EA,	TUNSIGNED|TUSHORT,
		0,	RLEFT,
		"	subl	rr0,rr0\n	ld	r1,AL\n	div	rr0,AR\n	ld	r1,r0\n",

/* === UNARY CALL === */

UNARY CALL,	INTAREG,
	SBREG|SNAME|SOREG|SCON,	TANY,
	SANY,	TANY,
		NAREG|NASL,	RESC1,
		"ZC\nZ0",

UNARY CALL,	INTAREG,
	SAREG,	TANY,
	SANY,	TANY,
		NAREG|NASL,	RESC1,
		"	ld	r8,AL\n	call	@r8\nZ0",

/* === SCONV (type conversions) === */

/* word to word (same size, just type change - no code needed) */
SCONV,	INTAREG,
	STAREG,	TWORD,
	SANY,	TWORD,
		0,	RLEFT,
		"",

/* char -> int/word: sign extend byte to word */
SCONV,	INTAREG,
	STAREG,	TCHAR,
	SANY,	TWORD,
		0,	RLEFT,
		"	extsb	AL\n",

/* uchar -> int/word: zero extend byte (mask) */
SCONV,	INTAREG,
	STAREG,	TUCHAR,
	SANY,	TWORD,
		0,	RLEFT,
		"	and	AL,#0xFF\n",

/* int -> long: sign extend word to long pair */
/* source is in AL (one register), result needs register pair */
/* need escape register pair; move source to low reg, sign extend */
SCONV,	INTAREG|INAREG,
	SAREG|STAREG,	TINT|TSHORT,
	SANY,	TLONG|TULONG,
		NAREG|NASR,	RESC1,
		"	ld	U1,AL\n	exts	ZD\n",

/* uint -> ulong: zero extend word to long pair */
SCONV,	INTAREG|INAREG,
	SAREG|STAREG,	TUNSIGNED|TUSHORT|TPOINT,
	SANY,	TLONG|TULONG,
		NAREG|NASR,	RESC1,
		"	subl	ZD,ZD\n	ld	U1,AL\n",

/* long -> int/word: truncate (take low word) */
SCONV,	INTAREG|INAREG,
	SAREG|STAREG,	TLONG|TULONG,
	SANY,	TWORD,
		NAREG|NASR,	RESC1,
		"	ld	A1,UL\n",

/* long -> int/word from memory: adjust offset and load */
SCONV,	INTAREG|INAREG,
	SNAME|SOREG,	TLONG|TULONG,
	SANY,	TWORD,
		NAREG|NASR,	RESC1,
		"ZT	ld	A1,AL\n",

/* int/word -> char: no code, just use low byte */
SCONV,	INAREG|INTAREG,
	EA,	TWORD,
	SANY,	TCHAR|TUCHAR,
		0,	RLEFT,
		"",

/* long -> char: take low byte of low word */
SCONV,	INTAREG|INAREG,
	SAREG|STAREG,	TLONG|TULONG,
	SANY,	TCHAR|TUCHAR,
		NAREG|NASR,	RESC1,
		"	ld	A1,UL\n",

/* uchar -> char, char -> uchar: no code */
SCONV,	INTAREG,
	STAREG,	TCHAR|TUCHAR,
	SANY,	TCHAR|TUCHAR,
		0,	RLEFT,
		"",

/* === STASG (structure assignment) === */

STASG,	FOREFF,
	SNAME|SOREG,	TANY,
	SCON|SBREG,	TANY,
		0,	RNOP,
		"ZS",

STASG,	INTBREG|INBREG,
	SNAME|SOREG,	TANY,
	STBREG,	TANY,
		0,	RRIGHT,
		"ZS",

STASG, INBREG|INTBREG,
	SNAME|SOREG,	TANY,
	SCON|SBREG,	TANY,
		NBREG,	RESC1,
		"ZS	ld	A1,AR\n",

/* === INIT (data initialization) === */

INIT,	FOREFF,
	SCON,	TANY,
	SANY,	TINT|TUNSIGNED|TPOINT|TSHORT|TUSHORT,
		0,	RNOP,
		"	.word	CL\n",

INIT,	FOREFF,
	SCON,	TANY,
	SANY,	TCHAR|TUCHAR,
		0,	RNOP,
		"	.byte	CL\n",

INIT,	FOREFF,
	SCON,	TANY,
	SANY,	TLONG|TULONG,
		0,	RNOP,
		"	.word	CL\n	.word	CL\n",

	/* Default actions for hard trees ... */

# define DF(x) FORREW,SANY,TANY,SANY,TANY,REWRITE,x,""

UNARY MUL, DF( UNARY MUL ),

INCR, DF(INCR),

DECR, DF(INCR),

ASSIGN, DF(ASSIGN),

STASG, DF(STASG),

OPLEAF, DF(NAME),

OPLOG,	FORCC,
	SANY,	TANY,
	SANY,	TANY,
		REWRITE,	BITYPE,
		"",

OPLOG,	DF(NOT),

COMOP, DF(COMOP),

INIT, DF(INIT),

OPUNARY, DF(UNARY MINUS),


ASG OPANY, DF(ASG PLUS),

OPANY, DF(BITYPE),

FREE,	FREE,	FREE,	FREE,	FREE,	FREE,	FREE,	FREE,	"help; I'm in trouble\n" };
