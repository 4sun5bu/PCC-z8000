/*	Layout of a.out file (Z8000):
 *
 *	header of 8 shorts	magic number 405, 407, 410, 411
 *				text size		)
 *				data size		) in bytes
 *				bss size		)
 *				symbol table size	)
 *				entry point
 *				text relocation size	)
 *				data relocation size	)
 *
 *
 *	header:			0
 *	text:			16
 *	data:			16+textsize
 *	text relocation:	16+textsize+datasize
 *	data relocation:	16+textsize+datasize+trelsize
 *	symbol table:		16+textsize+datasize+trelsize+drelsize
 *
 */
/* various parameters */
#define SYMLENGTH	50		/* maximum length of a symbol */
#define PAGESIZE	1024		/* relocation boundry for 410 files */

/* types of files */
#define	ARCMAGIC 0177545
#define OMAGIC	0405
#define	FMAGIC	0407
#define	NMAGIC	0410
#define	IMAGIC	0411

/* symbol types (internal encoding, shifted left 8 from a.out N_* values) */
#define	EXTERN	(040<<8)
#define	UNDEF	(00<<8)
#define	ABS	(01<<8)
#define	TEXT	(02<<8)
#define	DATA	(03<<8)
#define	BSS	(04<<8)
#define	COMM	(05<<8)	/* internal use only */
#define REG	(06<<8)

/*	char rsegment:2;	/* RTEXT, RDATA, RBSS, or REXTERN */
/*	char rsize:2;		/* RBYTE, RWORD, or RLONG */
/*	char rdisp:1;		/* 1 => a displacement */

/* displacement */
#define	RDISP	(1<<(3+8))

/* relocation segments */
#define	RSEGMNT	(03<<(6+8))

#define	RTEXT	(00<<(6+8))
#define	RDATA	(01<<(6+8))
#define	RBSS	(02<<(6+8))
#define	REXT	(03<<(6+8))

/* relocation sizes */
#define	RSIZE	(03<<(4+8))

#define RBYTE	(00<<(4+8))
#define RWORD	(01<<(4+8))
#define RLONG	(02<<(4+8))

/* On-disk sizes (independent of host sizeof(long)) */
#define HDRSIZE		16	/* 8 * 2 bytes */
#define RELOC_DISKSIZE	8	/* short + short + long = 2+2+4 */
#define NLIST_DISKSIZE	12	/* char[8] + short + short = 8+2+2 */

/* macros which define various positions in file based on a bhdr, filhdr */
#define TEXTPOS		HDRSIZE
#define DATAPOS 	TEXTPOS + filhdr.tsize
#define RTEXTPOS	DATAPOS + filhdr.dsize
#define RDATAPOS	RTEXTPOS + filhdr.trsize
#define SYMPOS		RDATAPOS + filhdr.drsize
#define ENDPOS		SYMPOS + filhdr.ssize

/* header of a.out files (internal representation, fields are long for convenience) */
struct bhdr {
	long	fmagic;
	long	tsize;
	long	dsize;
	long	bsize;
	long	ssize;
	long	entry;
	long	trsize;
	long	drsize;
};

/* symbol management (internal representation) */
struct sym {
	short	stype;
	long	svalue;
};

/* relocation commands */
struct reloc {
	short rinfo;		/* rsegment, rsize, and rdisp */
/*	char rsegment:2;	 RTEXT, RDATA, RBSS, or REXTERN */
/*	char rsize:2;		 RBYTE, RWORD, or RLONG */
/*	char rdisp:1;		 1 => a displacement */
	short rsymbol;		/* id of the symbol of external relocations */
	long rpos;			/* position of relocation in segment */
};

/* Stuff for unix compatibility */

#define	A_MAGIC1	FMAGIC	/* normal */
#define	A_MAGIC2	NMAGIC	/* read-only text */

struct	nlist {			/* symbol table entry */
	char n_name[8];		/* symbol name */
	int n_type;			/* type flag */
	unsigned n_value;	/* value */
};

		/* values for type flag */
#define	N_UNDF	0	/* undefined */
#define	N_ABS	01	/* absolute */
#define	N_TEXT	02	/* text symbol */
#define	N_DATA	03	/* data symbol */
#define	N_BSS	04	/* bss symbol */
#define	N_TYPE	037
#define	N_REG	024	/* register name */
#define	N_FN	037	/* file name symbol */
#define	N_EXT	040	/* external bit, or'ed in */
#define	FORMAT	"%06o"	/* to print a value */
