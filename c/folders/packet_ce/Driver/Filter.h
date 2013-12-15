/* Header required for BPF filter */
#ifndef __FILTER_H__
#define __FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef	INT		bpf_int32;
typedef	UINT	bpf_u_int32;

#define BPF_ALIGNMENT		sizeof(bpf_int32)
#define BPF_WORDALIGN(x)	(((x)+(BPF_ALIGNMENT-1))&~(BPF_ALIGNMENT-1))

#define BPF_MAXINSNS	512
#define BPF_MAXBUFSIZE	0x8000
#define BPF_MINBUFSIZE	32


/////////////////////////////////////////////////////////////////////////////////////
// Declaration related to BPF

int bpf_validate(struct bpf_insn *f,int len);

UINT bpf_filter(register struct bpf_insn *pc, 
				register UCHAR *p,
				UINT wirelen, 
				register UINT buflen);
UINT bpf_filter_with_2_buffers(register struct bpf_insn *pc, 
							   register UCHAR *p,
							   register UCHAR *pd, 
							   register int headersize,
							   UINT wirelen, 
							   register UINT buflen);

#define MAX_BUFFER_SPACE	MAX_PACKET_LENGTH + sizeof(struct bpf_hdr)

/*
 * Struct return by BIOCVERSION.  This represents the version number of 
 * the filter language described by the instruction encodings below.
 * bpf understands a program iff kernel_major == filter_major &&
 * kernel_minor >= filter_minor, that is, if the value returned by the
 * running kernel has the same major number and a minor number equal
 * equal to or less than the filter being downloaded.  Otherwise, the
 * results are undefined, meaning an error may be returned or packets
 * may be accepted haphazardly.
 * It has nothing to do with the source code version.
 */
struct bpf_version {
	USHORT bv_major;
	USHORT bv_minor;
};

/* Current version number of filter architecture. */
#define BPF_MAJOR_VERSION 1
#define BPF_MINOR_VERSION 1

/*
 * BPF ioctls
 *
 * The first set is for compatibility with Sun's pcc style
 * header files.  If your using gcc, we assume that you
 * have run fixincludes so the latter set should work.
 */
#if (defined(sun) || defined(ibm032)) && !defined(__GNUC__)
#define	BIOCGBLEN		_IOR(B,102, u_int)
#define	BIOCSBLEN		_IOWR(B,102, u_int)
#define	BIOCSETF		_IOW(B,103, struct bpf_program)
#define	BIOCFLUSH		_IO(B,104)
#define BIOCPROMISC		_IO(B,105)
#define	BIOCGDLT		_IOR(B,106, u_int)
#define BIOCGETIF		_IOR(B,107, struct ifreq)
#define BIOCSETIF		_IOW(B,108, struct ifreq)
#define BIOCSRTIMEOUT	_IOW(B,109, struct timeval)
#define BIOCGRTIMEOUT	_IOR(B,110, struct timeval)
#define BIOCGSTATS		_IOR(B,111, struct bpf_stat)
#define BIOCIMMEDIATE	_IOW(B,112, u_int)
#define BIOCVERSION		_IOR(B,113, struct bpf_version)
#define BIOCSTCPF		_IOW(B,114, struct bpf_program)
#define BIOCSUDPF		_IOW(B,115, struct bpf_program)
#else
#define	BIOCGBLEN		_IOR('B',102, u_int)
#define	BIOCSBLEN		_IOWR('B',102, u_int)
#define	BIOCSETF		_IOW('B',103, struct bpf_program)
#define	BIOCFLUSH		_IO('B',104)
#define BIOCPROMISC		_IO('B',105)
#define	BIOCGDLT		_IOR('B',106, u_int)
#define BIOCGETIF		_IOR('B',107, struct ifreq)
#define BIOCSETIF		_IOW('B',108, struct ifreq)
#define BIOCSRTIMEOUT	_IOW('B',109, struct timeval)
#define BIOCGRTIMEOUT	_IOR('B',110, struct timeval)
#define BIOCGSTATS		_IOR('B',111, struct bpf_stat)
#define BIOCIMMEDIATE	_IOW('B',112, u_int)
#define BIOCVERSION		_IOR('B',113, struct bpf_version)
#define BIOCSTCPF		_IOW('B',114, struct bpf_program)
#define BIOCSUDPF		_IOW('B',115, struct bpf_program)
#endif
/*
 * Because the structure above is not a multiple of 4 bytes, some compilers
 * will insist on inserting padding; hence, sizeof(struct bpf_hdr) won't work.
 * Only the kernel needs to know about it; applications use bh_hdrlen.
 */
#ifdef KERNEL
#define SIZEOF_BPF_HDR 18
#endif


// Data-link level type codes.
#define DLT_NULL		0	// no link-layer encapsulation 
#define DLT_EN10MB		1	// Ethernet (10Mb) 
#define DLT_EN3MB		2	// Experimental Ethernet (3Mb) 
#define DLT_AX25		3	// Amateur Radio AX.25 
#define DLT_PRONET		4	// Proteon ProNET Token Ring 
#define DLT_CHAOS		5	// Chaos 
#define DLT_IEEE802		6	// IEEE 802 Networks 
#define DLT_ARCNET		7	// ARCNET 
#define DLT_SLIP		8	// Serial Line IP 
#define DLT_PPP			9	// Point-to-point Protocol 
#define DLT_FDDI		10	// FDDI 
#define DLT_ATM_RFC1483	11	// LLC/SNAP encapsulated atm 
#define DLT_RAW			12	// raw IP 
#define DLT_SLIP_BSDOS	13	// BSD/OS Serial Line IP 
#define DLT_PPP_BSDOS	14	// BSD/OS Point-to-point Protocol 


// The instruction encondings.

// instruction classes 
#define		BPF_CLASS(code) ((code) & 0x07)

#define		BPF_LD		0x00
#define		BPF_LDX		0x01
#define		BPF_ST		0x02
#define		BPF_STX		0x03
#define		BPF_ALU		0x04
#define		BPF_JMP		0x05
#define		BPF_RET		0x06
#define		BPF_MISC	0x07

// ld/ldx fields 
#define BPF_SIZE(code)	((code) & 0x18)
#define		BPF_W		0x00
#define		BPF_H		0x08
#define		BPF_B		0x10
#define BPF_MODE(code)	((code) & 0xe0)
#define		BPF_IMM 	0x00
#define		BPF_ABS		0x20
#define		BPF_IND		0x40
#define		BPF_MEM		0x60
#define		BPF_LEN		0x80
#define		BPF_MSH		0xa0

// alu/jmp fields
#define BPF_OP(code)	((code) & 0xf0)
#define		BPF_ADD		0x00
#define		BPF_SUB		0x10
#define		BPF_MUL		0x20
#define		BPF_DIV		0x30
#define		BPF_OR		0x40
#define		BPF_AND		0x50
#define		BPF_LSH		0x60
#define		BPF_RSH		0x70
#define		BPF_NEG		0x80
#define		BPF_JA		0x00
#define		BPF_JEQ		0x10
#define		BPF_JGT		0x20
#define		BPF_JGE		0x30
#define		BPF_JSET	0x40
#define BPF_SRC(code)	((code) & 0x08)
#define		BPF_K		0x00
#define		BPF_X		0x08

// ret - BPF_K and BPF_X also apply 
#define BPF_RVAL(code)	((code) & 0x18)
#define		BPF_A		0x10

// misc 
#define BPF_MISCOP(code) ((code) & 0xf8)
#define		BPF_TAX		0x00
#define		BPF_TXA		0x80


// Macros for insn array initializers.
#define BPF_STMT(code, k) { (u_short)(code), 0, 0, k }
#define BPF_JUMP(code, k, jt, jf) { (u_short)(code), jt, jf, k }


// Number of scratch memory words (for BPF_LD|BPF_MEM and BPF_ST).
#define BPF_MEMWORDS 16


#ifdef __cplusplus
}
#endif

#endif // __FILTER_H__