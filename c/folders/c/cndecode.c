/*
 * cndecode.c
 * By Matthew Skala
 */

/*
 * Utility to decode the Cyber Patrol 4 cyber.not file
 * Usage:
 *    cndecode cyber.not dictionary suppdict iplist
 * all arguments except the first are optional
 * "dictionary" is a list (newline separated) of words for the URL-hash attack
 *    typically /usr/dict/words or equivalent, or use our ready-made one
 * suppdict is another such list; if it's specified, it will be written to
 *    with the list of actual words found (saves time next run)
 * iplist is lines of ip address and domain name, tab separated; it will be
 *    written to with any new addresses looked up (if we compile with reverse
 *    DNS enabled).
 */

/***************************************************************************/
/* System stuff */

/*
 * Compiling notes:
 * This was written under Linux on a PC, but should be portable to any 32-bit
 * little-endian architecture.  Since CP4 is PC-specific, that shouldn't be
 * too much of a limitation.  This program does require structures bigger than
 * 64K, and so it might be touch-and-go on a 16-bit PC compiler.  If you
 * enable reverse DNS with the defines below, you will have to have reasonably
 * Linux-ish (which in turn means reasonably BSD-ish) networking libraries.
 * Reverse queries can take a long time.  If you find this annoying, you
 * can set a time limit, and then after that time expires the system will stop
 * attempting reverse lookups.  This is a win because the reverse lookups it
 * already did are saved; next time, it'll pick up where it left off.  Real
 * Programmers, of course, would run multiple queries at once, but that would
 * mean either splitting into several processes, multithreading, or "fake"
 * multithreading with custom-written resolver routines.  This way is a lot
 * less stressful.  (I have a perl script that spawns 40 processes to max out
 * my modem, but it's a monstrosity.)  The amount of CPU power required for
 * CRC reversal scales exponentially with the number of characters of CRC
 * reversing you choose; that also determines how accurately it'll guess for
 * URL hashes that are not in the dictionary.  It's probably smarter to get a
 * bigger dictionary.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Compile-time configuration */

/* Should we attempt reverse lookups?  Requires Net libraries, active
 * connection, and a certain amount of time. */
#define REVERSE_DNS
/* After this many seconds, no more reverse DNS queries will be performed.
 * Default one hour, and ignored if you turned reverse DNS off. */
#define TIME_LIMIT 3600
/* How many characters of CRC reversal?  Five is essentially free, more takes
 * exponentially longer time. */
#define REVERSE_CRC_LENGTH 8
/* How many entries in the dictionary hash table? */
#define DICTHASH_SIZE 32767
/* How many entries in the IP address hash table? */
#define IPHASH_SIZE 32767
/* What's the longest line length we expect to see? */
#define LINELEN 256

/* Headers we only need if we're doing reverse DNS */
#ifdef REVERSE_DNS
#include <netdb.h>
#include <sys/socket.h>
#include <time.h>
#endif

/****************************************************************************/
/* Reference tables */

/* Forward CRC polynomial table */
unsigned long crctable[256]={
   0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,
   0x076DC419L, 0x706AF48FL, 0xE963A535L, 0x9E6495A3L,
   0x0EDB8832L, 0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
   0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L, 0x90BF1D91L,
   0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
   0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
   0x136C9856L, 0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL,
   0x14015C4FL, 0x63066CD9L, 0xFA0F3D63L, 0x8D080DF5L,
   0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L, 0xA2677172L,
   0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
   0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L,
   0x32D86CE3L, 0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
   0x26D930ACL, 0x51DE003AL, 0xC8D75180L, 0xBFD06116L,
   0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L, 0xB8BDA50FL,
   0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
   0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL,
   0x76DC4190L, 0x01DB7106L, 0x98D220BCL, 0xEFD5102AL,
   0x71B18589L, 0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
   0x7807C9A2L, 0x0F00F934L, 0x9609A88EL, 0xE10E9818L,
   0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
   0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL,
   0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L,
   0x65B0D9C6L, 0x12B7E950L, 0x8BBEB8EAL, 0xFCB9887CL,
   0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
   0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
   0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL,
   0x4369E96AL, 0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
   0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL, 0xDD0D7CC9L,
   0x5005713CL, 0x270241AAL, 0xBE0B1010L, 0xC90C2086L,
   0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
   0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L,
   0x59B33D17L, 0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL,
   0xEDB88320L, 0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
   0xEAD54739L, 0x9DD277AFL, 0x04DB2615L, 0x73DC1683L,
   0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
   0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
   0xF00F9344L, 0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL,
   0xF762575DL, 0x806567CBL, 0x196C3671L, 0x6E6B06E7L,
   0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
   0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
   0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L,
   0xD1BB67F1L, 0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
   0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L, 0x41047A60L,
   0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL, 0x4669BE79L,
   0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
   0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL,
   0xC5BA3BBEL, 0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L,
   0xC2D7FFA7L, 0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
   0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL, 0x026D930AL,
   0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
   0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
   0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L,
   0x86D3D2D4L, 0xF1D4E242L, 0x68DDB3F8L, 0x1FDA836EL,
   0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
   0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
   0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L,
   0xA00AE278L, 0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
   0xA7672661L, 0xD06016F7L, 0x4969474DL, 0x3E6E77DBL,
   0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L, 0x37D83BF0L,
   0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
   0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L,
   0xBAD03605L, 0xCDD70693L, 0x54DE5729L, 0x23D967BFL,
   0xB3667A2EL, 0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
   0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL, 0x2D02EF8DL
};

/* This answers the question: how many freely-chosen bits do I include
 * when I ask for a crc collision with input length (index)? */
char freebits[12]={0,0,0,0,1,4,10,16,22,28,34,40};

/* This says where each forced bit goes */
char bitsforced[32]={0,1,2,3,4,6,
                     8,9,10,11,12,14,
                     16,17,18,19,20,22,
                     24,25,26,27,28,30,
                     32,33,34,35,38,
                     40,41,42};

/* This says where each free bit goes */
char bitsfree[40]={36,43,44,46,
                   48,49,50,51,52,54,
                   56,57,58,59,60,62,
                   64,65,66,67,68,70,
                   72,73,74,75,76,78,
                   80,81,82,83,84,86,
                   88,89,90,91,92,94};

/* The portion of the inverted matrix corresponding to the CRC bits */
unsigned long crcmatrix[32]={
   0x9BF7B4FE,0x10CEBBDB,0x3EC28E73,0xE516F5B2,
   0x3EB07172,0xAC6CB91B,0x2344667F,0x25ECE58C,
   0xD24109C4,0x501CB10A,0x97761211,0x0A2EF700,
   0x0C806D13,0x55AE3901,0x4C147270,0xDAC3C857,
   0x384B8A54,0xF7583CAD,0xA1DA1DC4,0x0028BBDC,
   0xB5BB7FE3,0x99610C1A,0x1FC446C4,0x8DE0FF05,
   0x01D3D128,0x64FAC9B2,0x3BC5E604,0xE564A85C,
   0xADEB84A5,0xCFCDBB2B,0x3E7D9F68,0xA102B971
};

/* The portion of the inverted matrix corresponding to the free bits */
unsigned long freematrix[40]={
   0x0CBFC054,0xAEAB35B2,0x315B20B2,0x1F113696,
   0x6DA65FB4,0x08F3CFCD,0xC0E8FCF1,0xD928FA77,
   0x58C085F6,0x55F7A6A4,0x726948CB,0xBEE706A6,
   0xDE9BCF28,0x539FADD8,0xA5D7713D,0xA6B4900F,
   0x3CA9547B,0xC98AC9B5,0xAF52FA18,0x60098F5B,
   0x142D2C51,0x706AA085,0x46494250,0x54026BCE,
   0xEBE4D0A3,0x673646B9,0x945A22D6,0x7C5347FB,
   0xC61C9B99,0x97780ADB,0x7E9DB1AE,0x88C43E39,
   0x55CEBFB3,0x5C81ADC9,0x0F3DD57C,0x3D44BCF3,
   0x0383F8DD,0x73F38757,0xA8F2D5CF,0x2922BEA9
};

/* Matrix columns to take into account the canonicalization */
unsigned long lengthmatrix[12]={
   0x84741063,0xC5273406,0xE5A222DF,0x9941CB2B,
   0xD9EBE522,0xCB93A8AF,0x962E3D2D,0x90029144,
   0x5B298B04,0x575F1D8A,0x78EE4BEC,0x47B6B86A
};

/* The Cyber Patrol blocking categories */
char *category[16]={
   "Violence / Profanity",
   "Partial Nudity",
   "Full Nudity",
   "Sexual Acts / Text",
   "Gross Depictions / Text",
   "Intolerance",
   "Satanic or Cult",
   "Drugs / Drug Culture",
   "Militant / Extremist",
   "Sex Education",
   "Questionable / Illegal & Gambling",
   "Alcohol & Tobacco",
   "Reserved 4",
   "Reserved 3",
   "Reserved 2",
   "Reserved 1"
};

/* This indicates the "score" for each possible character value.  First
 * 32 entries are for characters 32 to 63, second 32 entries are for
 * characters 96 to 127.  The way this works is that unexpected characters
 * get higher scores and so are less likely to be chosen... this gives us
 * that little bit of extra guidance to help find good reverse CRCs.
 * The baseline is that an ordinary alphabet character is 10 points.
 * Illegal characters count 50, ensuring that they're unlikely to ever be
 * chosen.  Scores assigned manually, and only semi-systematically.
 */
unsigned cscore[64]={
/* SP !  "  #  $  %  &  '  */
   50,20,30,20,17,50,50,30,
/* (  )  *  +  ,  -  .  /  */
   20,20,20,17,20,12,12,50,
/* 0  1  2  3  4  5  6  7  */ 
   14,14,15,15,15,15,15,15,
/* 8  9  :  ;  <  =  >  ?  */
   15,14,20,20,50,20,50,30,
/* `  a b  c  d e f  g */
   30,7,10,10,9,7,10,10,
/* h  i j  k  l m  n o */
   10,7,12,10,9,10,9,7,
/* p  q  r s t u v  w  */
   10,15,9,9,9,7,12,10,
/* x y  z  {  |  }  ~  DEL */
   9,10,12,20,30,20,30,50
};

/***************************************************************************/
/* Data structures */

/* Structures for the hash tables */
typedef struct _DICTHASH_ENT {
   struct _DICTHASH_ENT *next;
   unsigned long hash;
   char *word;
} DICTHASH_ENT;
typedef struct _IPHASH_ENT {
   struct _IPHASH_ENT *next;
   unsigned long ip;
   char *name;
} IPHASH_ENT;

/* Linked list of blocking masks, for key printing */
typedef struct _BLOCKING_MASK {
   struct _BLOCKING_MASK *next;
   unsigned short mask;
} BLOCKING_MASK;

/* Global vars */
char *cyber_not;
long cyber_not_size;
DICTHASH_ENT **dicthash;
IPHASH_ENT **iphash;
BLOCKING_MASK *masks=NULL;
#ifdef REVERSE_DNS
time_t start_time;
#endif

/*************************************************************************/
/* Utility functions */

/* Encryption used to conceal the config files, and the deputy password */
void cpcrypt4(char *data,long length) {
   unsigned char key;
   long i,j;
   
   key=(unsigned char)(length&0xFF);
   for (i=0;i<2;i++) {
      for (j=0;j<length;j++) {
	 key=(key>>1)+(key<<7);
	 key^=(unsigned char)data[j];
	 data[j]=(char)key;
      }
   }
}

/* The slightly nonstandard CRC32 used for URL hashing */
unsigned long forward_crc(char *input,int length) {
   int i;
   unsigned long rval=0;
   
   for (i=0;i<length;i++)
     rval=(rval>>8)^crctable[(rval&0xFF)^(input[i]|0x20)];
   return rval;
}

#define GETBIT(p,b) ((((p)[(b)>>3])>>((b)&7))&1)
#define FLIPBIT(p,b) ((p)[(b)>>3]^=(1<<((b)&7)))
#define FLIPBITR(p,b) { if (length-1-((b)>>3)>=0) \
                        (p)[length-1-((b)>>3)]^=(1<<((b)&7)); }

/* attempt to reverse the CRC32 function */
void reverse_crc(unsigned long crc,int length,char *in,char *out) {
   unsigned long bits;
   int i;
   
   /* correct for output length */
   bits=lengthmatrix[length-1];
   
   /* XOR in the CRC */
   for (i=0;i<32;i++)
     if (crc&(1<<i))
       bits^=crcmatrix[i];
   
   /* XOR in the free bits */
   for (i=0;i<freebits[length-1];i++)
     if (GETBIT(in,i))
       bits^=freematrix[i];
   
   /* set up output */
   for (i=0;i<length;i++)
     out[i]=0x20;
   
   /* output forced bits */
   for (i=0;i<32;i++)
     if (bits&(1<<i))
       FLIPBITR(out,bitsforced[i]);
   
   /* output free bits */
   for (i=0;i<freebits[length-1];i++)
     if (GETBIT(in,i))
       FLIPBITR(out,bitsfree[i]);
}

/* load a word into dictionary, if its hash was in cyber.not */
int guess_word(char *word,int length) {
   unsigned long hash;
   DICTHASH_ENT *tmp;
   
   hash=forward_crc(word,length);
   for (tmp=dicthash[hash%DICTHASH_SIZE];
	tmp && (tmp->hash!=hash);
	tmp=tmp->next);
   if (tmp && !tmp->word) {
      tmp->word=(char *)malloc(length+1);
      if (!tmp->word) {
	 puts("ERROR - out of memory (dicthash entry)");
	 exit(1);
      }
      memcpy(tmp->word,word,length);
      tmp->word[length]='\0';
      return 1;
   } else
     return 0;
}

/* clear out the list of blocking masks */
void clear_blockmask_key(void) {
   BLOCKING_MASK *tmp;
   
   while (masks) {
      tmp=masks;
      masks=tmp->next;
      free(tmp);
   }
}

/* add a mask to the sorted list, if it's not already there.  Yes, this
 * is O(n**2), but the list never gets over a few tens of entries, and so
 * doing it with a more sophisticated structure wouldn't be worthwhile. */
void add_blockmask(unsigned short newmask) {
   BLOCKING_MASK *tmp=NULL,*tmp2;
   
   /* skip past all the entries less than new */
   while (masks && (masks->mask<newmask)) {
      tmp2=masks->next;
      masks->next=tmp;
      tmp=masks;
      masks=tmp2;
   }
   /* add new, if appropriate */
   if ((!masks) || (masks->mask!=newmask)) {
      tmp2=(BLOCKING_MASK *)malloc(sizeof(BLOCKING_MASK));
      if (!tmp2) {
	 puts("ERROR - out of memory (blocking mask entry)");
	 exit(1);
      }
      tmp2->next=masks;
      masks=tmp2;
      tmp2->mask=newmask;
   }
   /* replace the skipped entries */
   while (tmp) {
      tmp2=tmp->next;
      tmp->next=masks;
      masks=tmp;
      tmp=tmp2;
   }
}

/* print a key of the masks currently on the list */
void print_blockmask_key(void) {
   BLOCKING_MASK *tmp;
   unsigned short tm;
   int print_head,bit;
   
   for (tmp=masks;tmp;tmp=tmp->next) {
      printf("%04X: ",tmp->mask);
      print_head=6;
      tm=tmp->mask;
      bit=0;
      while (tm>0) {
	 if (tm&1) {
	    if (print_head+strlen(category[bit])>71) {
	       printf("\n       ");
	       print_head=7;
	    } else {
	       putchar(' ');
	       print_head++;
	    }
	    printf("%s",category[bit]);
	    print_head+=strlen(category[bit]);
	    if (tm&~1) {
	       putchar(',');
	       print_head++;
	    }
	 }
	 bit++;
	 tm>>=1;
      }
      putchar('\n');
   }
}

/* print a pretty IP address, with reverse lookup if we're allowed */
void print_ip(unsigned long ip) {
   IPHASH_ENT *tmp;
   char *name;
   
   /* check if it's already in the table */
   for (tmp=iphash[ip%IPHASH_SIZE];
	tmp && (tmp->ip!=ip);
	tmp=tmp->next);
   if (tmp) /* if so, just print that */
     printf("%s",tmp->name);
#ifdef REVERSE_DNS
   /* if we're allowed a reverse lookup, take it */
   else if (time(NULL)<start_time+TIME_LIMIT) {
      struct hostent *he;
      
      he=gethostbyaddr((char *)&ip,4,AF_INET);
      tmp=(IPHASH_ENT *)malloc(sizeof(IPHASH_ENT));
      name=(char *)malloc(he?strlen(he->h_name)+1:16);
      if (!tmp || !name) {
	 puts("ERROR - out of memory (IP hash ent)");
	 exit(1);
      }
      tmp->next=iphash[ip%IPHASH_SIZE];
      iphash[ip%IPHASH_SIZE]=tmp;
      tmp->ip=ip;
      tmp->name=name;
      
      if (he)
	strcpy(name,he->h_name);
      else
	sprintf(name,"%ld.%ld.%ld.%ld",
		ip&0xFF,(ip>>8)&0xFF,(ip>>16)&0xFF,ip>>24);
      printf("%s",name);
   }
#endif
   else /* finally, we just print it out numerically */
     printf("%ld.%ld.%ld.%ld",ip&0xFF,(ip>>8)&0xFF,(ip>>16)&0xFF,ip>>24);
}

/* reverse a hash and print the results */
void print_revhash(unsigned long hash) {
   DICTHASH_ENT *tmp;
   char *neww,freeb[6],plaintext[12];
   unsigned score,bestscore=(unsigned)-1;
   int length,i,bflip;
   
   /* check if it's already in the table */
   for (tmp=dicthash[hash%DICTHASH_SIZE];
	tmp && (tmp->hash!=hash);
	tmp=tmp->next);
   if (!tmp) {
      puts("ERROR - reversing unseen hash (should never happen)");
      exit(1);
   }
   
   if (!tmp->word) { /* if no word, attempt reversal */
      neww=(char *)malloc(12);
      if (neww==NULL) {
	 puts("ERROR - out of memory (new word)\n");
	 exit(1);
      }
      tmp->word=neww;
      strcpy(neww,"?UNKNOWN?");
      
      for (length=1;length<=REVERSE_CRC_LENGTH;length++) {
	 for (i=0;i<6;i++) freeb[i]=0;
	 
	 do {
	    /* reverse the hash, and see if that worked */
	    reverse_crc(hash,length,freeb,plaintext);
	    if (hash==forward_crc(plaintext,length)) {
	       /* compute the score for this guess */
	       score=0;
	       for (i=0;i<length;i++)
		 score+=cscore[plaintext[i]-(plaintext[i]>64?64:32)];
	       if (plaintext[0]=='~') /* ~ at start is ignored */
		 score-=cscore['~'-64];
	       
	       /* if this is an improvement, use it */
	       if (score<bestscore) {
		  bestscore=score;
		  memcpy(neww,plaintext,length);
		  neww[length]='\0';
	       }
	    }
	    
	    for (bflip=0;GETBIT(freeb,bflip);bflip++) FLIPBIT(freeb,bflip);
	    FLIPBIT(freeb,bflip);
	 } while (bflip<freebits[length-1]);
      }
   }
   
   /* now print whatever word we found */
   printf("%s",tmp->word);
}

/***************************************************************************/
/* Main functional blocks */

/* Load and decrypt cyber.not */
void load_cyber_not(char *filename) {
   FILE *datafile;

   datafile=fopen(filename,"rb");
   if (!datafile) {
      puts("ERROR - can't open cyber.not");
      exit(1);
   }
   if ((fseek(datafile,0,SEEK_END)<0)
       || ((cyber_not_size=ftell(datafile))<0)
       || (fseek(datafile,0,SEEK_SET)<0)) {
      puts("ERROR - can't reposition in cyber.not");
      exit(1);
   }
   cyber_not=(char *)malloc(cyber_not_size);
   if ((!cyber_not)
       || (fread(cyber_not,1,cyber_not_size,datafile)<cyber_not_size)) {
      puts("ERROR - can't read cyber.not");
   }
   fclose(datafile);
   cpcrypt4(cyber_not,cyber_not_size);
}

/* Initialize the dictionary hash table with the hashes in cyber.not */
void find_hashes_to_reverse(void) {
   long table1_start,table1_end;
   long i,hcnt=0;
   unsigned long hash;
   short mask;
   char length;
   DICTHASH_ENT *tmp;
   
   /* find Table 1 in cyber.not */
   memcpy(&table1_start,cyber_not+0x0010,4); /* Table 1 offset */
   memcpy(&table1_end,cyber_not+0x0014,4); /* Table 1 length */
   table1_end+=table1_start;
   table1_start+=2; /* "SD" marker */
   table1_end-=2; /* "ED" marker */

   /* initialize our hash table to empty */
   dicthash=(DICTHASH_ENT **)malloc(DICTHASH_SIZE*sizeof(DICTHASH_ENT *));
   if (!dicthash) {
      puts("ERROR - can't allocate dictionary hash");
      exit(1);
   }
   for (i=0;i<DICTHASH_SIZE;i++) dicthash[i]=NULL;
   
   /* step through the table, looking for hashes */
   for (i=table1_start;i<table1_end;) {
      i+=4; /* skip IP address */
      memcpy(&mask,cyber_not+i,2); /* category mask */
      i+=2; /* skip over mask */
      
      if (mask==0) { /* we have hash records */
	 for (;(length=cyber_not[i]);) { /* yes, this should be assignment */
	    i+=3; /* skip length and mask */
	    for (length-=3;length>0;length-=4) {
	       memcpy(&hash,cyber_not+i,4);
	       i+=4;
	       for (tmp=dicthash[hash%DICTHASH_SIZE];
		    tmp && (tmp->hash!=hash);
		    tmp=tmp->next); /* does this hash exist already? */
	       if (!tmp) { /* if not, add it */
		  tmp=(DICTHASH_ENT *)malloc(sizeof(DICTHASH_ENT));
		  if (!tmp) {
		     puts("ERROR - out of memory");
		     exit(1);
		  }
		  tmp->next=dicthash[hash%DICTHASH_SIZE];
		  dicthash[hash%DICTHASH_SIZE]=tmp;
		  tmp->hash=hash;
		  tmp->word=NULL;
		  hcnt++;
	       }
	    }
	 }
	 i++; /* skip terminating length */
      }
   }
   printf("Scanning cyber.not, found %ld unique hash values\n",hcnt);
}

/* load a dictionary file */
void load_dictionary(char *filename) {
   FILE *datafile;
   char textline[LINELEN+8],x;
   int length,newwords=0,i;

   datafile=fopen(filename,"rt");
   if (!datafile) {
      /* not an error!  the dictionary need not exist */
      printf("Dictionary file %s missing.\n",filename);
      return;
   }

   /* First pass: word, ~word, word.htm, word.html */
   textline[0]='~';
   while (!feof(datafile) && fgets(textline+1,LINELEN,datafile)) {
      if (strchr(textline+1,'#') || strchr(textline+1,' '))
	continue;
      length=strlen(textline+1)-1;
      textline[length+1]='\0';
      for (i=1;i<=length;i++) textline[i]=tolower(textline[i]);
      strcat(textline+1,".html");
      newwords+=guess_word(textline+1,length);   /* word */
      newwords+=guess_word(textline,length+1);   /* ~word */
      newwords+=guess_word(textline+1,length+4); /* word.htm */
      newwords+=guess_word(textline+1,length+5); /* word.html */
   }
   
   /* Second pass: Xword, ~Xword, wordX, ~wordX, xwordx */
   fseek(datafile,0,SEEK_SET);
   while (!feof(datafile) && fgets(textline+2,LINELEN,datafile)) {
      if (strchr(textline+2,'#') || strchr(textline+2,' '))
	continue;
      length=strlen(textline+2)-1;
      for (i=2;i<=length+1;i++) textline[i]=tolower(textline[i]);
      for (x='a';x<='z';x++) {
	 textline[1]=x;
	 textline[length+2]=x;
	 newwords+=guess_word(textline+1,length+1); /* Xword */
	 newwords+=guess_word(textline,length+2); /* ~Xword */
	 newwords+=guess_word(textline+2,length+1); /* wordX */
	 if (x=='x')
	   newwords+=guess_word(textline+1,length+2); /* xwordx  */
	 textline[1]='~';
	 newwords+=guess_word(textline+1,length+2); /* ~wordX */
      }
      for (x='0';x<='9';x++) {
	 textline[1]=x;
	 textline[length+2]=x;
	 newwords+=guess_word(textline+1,length+1); /* Xword */
	 newwords+=guess_word(textline,length+2); /* ~Xword */
	 newwords+=guess_word(textline+2,length+1); /* wordX */
	 textline[1]='~';
	 newwords+=guess_word(textline+1,length+2); /* ~wordX */
      }
   }
   
   /* Third pass: .htm and .html variants of second pass */
   fseek(datafile,0,SEEK_SET);
   while (!feof(datafile) && fgets(textline+2,LINELEN,datafile)) {
      if (strchr(textline+2,'#') || strchr(textline+2,' '))
	continue;
      length=strlen(textline+2)-1;
      for (i=2;i<=length+1;i++) textline[i]=tolower(textline[i]);
      for (x='a';x<='z';x++) {
	 textline[1]=x;
	 textline[length+2]=x;
	 textline[length+3]='\0';
	 strcat(textline+2,".html");
	 newwords+=guess_word(textline+2,length+5); /* wordX.htm */
	 newwords+=guess_word(textline+2,length+6); /* wordX.html */
	 textline[length+2]='\0';
	 strcat(textline+2,".html");
	 newwords+=guess_word(textline+1,length+5); /* Xword.htm */
	 newwords+=guess_word(textline+1,length+6); /* Xword.html */
      }
      for (x='0';x<='9';x++) {
	 textline[1]=x;
	 textline[length+2]=x;
	 textline[length+3]='\0';
	 strcat(textline+2,".html");
	 newwords+=guess_word(textline+2,length+5); /* wordX.htm */
	 newwords+=guess_word(textline+2,length+6); /* wordX.html */
	 textline[length+2]='\0';
	 strcat(textline+2,".html");
	 newwords+=guess_word(textline+1,length+5); /* Xword.htm */
	 newwords+=guess_word(textline+1,length+6); /* Xword.html */
      }
   }
   
   fclose(datafile);
   printf("Found %d new words in %s\n",newwords,filename);
}

/* save a dictionary file */
void save_dictionary(char *filename) {
   FILE *datafile;
   long i;
   DICTHASH_ENT *tmp;

   /* open */
   datafile=fopen(filename,"wt");
   if (!datafile) {
      /* this error is not fatal */
      printf("ERROR - cannot write dictionary %s (non-fatal)\n",filename);
      return;
   }
   
   /* write */
   for (i=0;i<DICTHASH_SIZE;i++)
     for (tmp=dicthash[i];tmp;tmp=tmp->next)
       if (tmp->word)
	 fprintf(datafile,"%s\n",tmp->word);

   /* close */
   fclose(datafile);
}

void dump_newsgroup_blocks(void) {
   long table3_start,table3_end;
   long i;
   short mask;
   char length;
   
   puts("*** NEWSGROUP BLOCKS ***\n");   
   
   /* find Table 3 in cyber.not */
   memcpy(&table3_start,cyber_not+0x0024,4); /* Table 3 offset */
   memcpy(&table3_end,cyber_not+0x0028,4); /* Table 3 length */
   table3_end+=table3_start;
   table3_start+=2; /* "SD" marker */
   table3_end-=2; /* "ED" marker */
   
   /* get ready to print a new blocking mask key */
   clear_blockmask_key();

   /* step through the table, looking for newsgroups */
   for (i=table3_start;i<table3_end;) {
      length=cyber_not[i];
      i+=1; /* skip length byte */
      memcpy(&mask,cyber_not+i,2); /* category mask */
      i+=2; /* skip over mask */
      add_blockmask(mask);
      
      printf("%04X ",mask);
      for (length-=3;length>0;length--) {
	 putchar(cyber_not[i]);
	 i++;
      }
      putchar('\n');
   }

   /* print the key */
   putchar('\n');
   print_blockmask_key();
   putchar('\n');
}

/* Load a file of reverse-lookup hints.  Just initializes the table if
 * the filename parameter is null. */
void load_iphints(char *filename) {
   FILE *datafile;
   char name[LINELEN],*ntmp;
   int i,ipa,ipb,ipc,ipd,count=0;
   unsigned long ip;
   IPHASH_ENT *tmp;

   /* initialize our hash table to empty */
   iphash=(IPHASH_ENT **)malloc(IPHASH_SIZE*sizeof(IPHASH_ENT *));
   if (!iphash) {
      puts("ERROR - can't allocate IP hash");
      exit(1);
   }
   for (i=0;i<IPHASH_SIZE;i++) iphash[i]=NULL;
   if (filename==NULL) return;
   
   datafile=fopen(filename,"rt");
   if (!datafile) {
      /* not an error!  the hints file need not exist */
      printf("IP address file %s missing.\n",filename);
      return;
   }

   while (!feof(datafile)) {
      fscanf(datafile,"%d.%d.%d.%d\t%s\n",&ipa,&ipb,&ipc,&ipd,name);
      ip=((long)ipa)+((long)ipb<<8)+((long)ipc<<16)+((long)ipd<<24);
      tmp=(IPHASH_ENT *)malloc(sizeof(IPHASH_ENT));
      ntmp=(char *)malloc(strlen(name)+1);
      if ((!tmp) || (!ntmp)) {
	 puts("ERROR - out of memory (IP hash entry)");
	 exit(1);
      }
      tmp->next=iphash[ip%IPHASH_SIZE];
      iphash[ip%IPHASH_SIZE]=tmp;
      tmp->ip=ip;
      tmp->name=ntmp;
      strcpy(ntmp,name);
      count++;
   }
   
   fclose(datafile);
   printf("Found %d IP addresses in %s\n",count,filename);
}

/* save reverse-lookup hints file */
void save_iphints(char *filename) {
   FILE *datafile;
   long i;
   IPHASH_ENT *tmp;

   /* open */
   datafile=fopen(filename,"wt");
   if (!datafile) {
      /* this error is not fatal */
      printf("ERROR - cannot write IP hints %s (non-fatal)\n",filename);
      return;
   }
   
   /* write */
   for (i=0;i<IPHASH_SIZE;i++)
     for (tmp=iphash[i];tmp;tmp=tmp->next)
       fprintf(datafile,"%ld.%ld.%ld.%ld\t%s\n",
	       tmp->ip&0xFF,(tmp->ip>>8)&0xFF,(tmp->ip>>16)&0xFF,tmp->ip>>24,
	       tmp->name);

   /* close */
   fclose(datafile);
}

/* OK, this is the good part */
void dump_web_blocks(void) {
   long table1_start,table1_end,table2_start,table2_end;
   long i,j;
   unsigned long ip,hash;
   short mask;
   char length;
   
   /* find Table 1 in cyber.not */
   memcpy(&table1_start,cyber_not+0x0010,4); /* Table 1 offset */
   memcpy(&table1_end,cyber_not+0x0014,4); /* Table 1 length */
   table1_end+=table1_start;
   table1_start+=2; /* "SD" marker */
   table1_end-=2; /* "ED" marker */

   /* find Table 2 in cyber.not */
   memcpy(&table2_start,cyber_not+0x001A,4); /* Table 2 offset */
   memcpy(&table2_end,cyber_not+0x001E,4); /* Table 2 length */
   table2_end+=table2_start;
   table2_start+=2; /* "SD" marker */
   table2_end-=2; /* "ED" marker */

   /* step through the table 1, printing out the blocks*/
   for (i=table1_start;i<table1_end;) {
      puts("************************************"
	   "************************************");
      clear_blockmask_key();
      
      memcpy(&ip,cyber_not+i,4);
      i+=4; /* skip IP address */
      memcpy(&mask,cyber_not+i,2); /* category mask */
      i+=2; /* skip over mask */
      
      /* print the IP address and synonyms */
      putchar(' ');
      putchar(' ');
      print_ip(ip);
      putchar('\n');
      for (j=table2_start;j<table2_end;) {
	 memcpy(&hash,cyber_not+j,4);
	 j+=4;
	 length=cyber_not[j++];
	 if (hash==ip) {
	    for (;length>0;length--) {
	       memcpy(&hash,cyber_not+j,4);
	       j+=4;
	       putchar('=');
	       putchar(' ');
	       print_ip(hash);
	       putchar('\n');
	    }
	 } else
	   j+=(length*4);
      }
      
      /* print the individual blocks */
      if (mask==0) {
	 putchar('\n');
	 for (;(length=cyber_not[i]);) { /* yes, this should be assignment */
	    i++; /* skip length */
	    memcpy(&mask,cyber_not+i,2); /* category mask */
	    i+=2; /* skip over mask */
	    add_blockmask(mask);
	    printf("%04X http://%ld.%ld.%ld.%ld/",mask,
		   ip&0xFF,(ip>>8)&0xFF,(ip>>16)&0xFF,ip>>24);
	    for (length-=3;length>0;length-=4) {
	       memcpy(&hash,cyber_not+i,4);
	       i+=4;
	       print_revhash(hash);
	       putchar('/');
	    }
	    putchar('\n');
	 }
	 i++; /* skip terminating length */
	 putchar('\n');
      } else { /* block on entire site */
	 add_blockmask(mask);
	 puts("ENTIRE SITE:");
      }
      
      print_blockmask_key();
   }
}

/**************************************************************************/
/* Main program */

int main(int argc,char **argv) {
   puts("cndecode - Cyber Patrol 4 cyber.not decoder");
   puts("By Matthew Skala\n");
   
#ifdef REVERSE_DNS
   start_time=time(NULL);
#endif
   
   if (argc<2) {
      puts("ERROR - no cyber.not file specified");
      return 1;
   }
   load_cyber_not(argv[1]);
   find_hashes_to_reverse();

   if (argc>=4) /* supplemental */
     load_dictionary(argv[3]);
   if (argc>=3) /* main */
     load_dictionary(argv[2]);
   if (argc>=5)
     load_iphints(argv[4]);
   else
     load_iphints(NULL);
   
   dump_newsgroup_blocks();
   dump_web_blocks();
   
   if (argc>=4)
     save_dictionary(argv[3]);
   if (argc>=5)
     save_iphints(argv[4]);
   
   return 0;
}
