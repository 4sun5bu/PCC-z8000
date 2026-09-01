#define	ARMAG	0177545
#define SARMAG 2
struct	ar_hdr {
	char	ar_name[14];
	int		ar_date;
	char	ar_uid;
	char	ar_gid;
	int		ar_mode;
	int		ar_size;
};
