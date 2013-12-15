/* Define if your raw sockets have arguments in host order as in BSD.  */
#undef BSD_RAWSOCK_ORDER

/* Define if your system knows about struct sockaddr_storage */
#undef HAVE_SOCKADDR_STORAGE

/* Define to int if your system does not know about sa_family_t */
#undef sa_family_t

/* Define to int if your system does not know about socklen_t */
#undef socklen_t

/* Define to `unsigned long long' if <sys/types.h> doesn't define.  */
#undef u_int64_t

/* Define to `unsigned int' if <sys/types.h> doesn't define.  */
#undef u_int32_t

/* Define to `unsigned short' if <sys/types.h> doesn't define.  */
#undef u_int16_t

/* Define to `unsigned char' if <sys/types.h> doesn't define.  */
#undef u_int8_t

/* Undefine if <netdb.h> contains this, otherwise define to 1 */
#undef NI_NUMERICHOST

/* Undefine if <netdb.h> contains this, otherwise define to 256 */
#undef NI_MAXHOST

/* Undefine if <netdb.h> contains this, otherwise define to 32 */
#undef NI_MAXSERV

@BOTTOM@

/* Take care of getaddrinfo */
#undef HAVE_STRUCT_ADDRINFO
#ifndef HAVE_STRUCT_ADDRINFO
#include "getaddrinfo.h"
#endif /* !HAVE_STRUCT_ADDRINFO */

#ifndef HAVE_GETADDRINFO
int getaddrinfo(const char *hostname, const char *servname, 
                const struct addrinfo *hints, struct addrinfo **res);
#endif /* !HAVE_GETADDRINFO */

/* Define if timeradd is defined in <sys/time.h> */
#undef HAVE_TIMERADD
#ifndef HAVE_TIMERADD
#define timeradd(tvp, uvp, vvp)                                         \
        do {                                                            \
                (vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;          \
                (vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;       \
                if ((vvp)->tv_usec >= 1000000) {                        \
                        (vvp)->tv_sec++;                                \
                        (vvp)->tv_usec -= 1000000;                      \
                }                                                       \
        } while (0)
#define	timersub(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)
#endif /* !HAVE_TIMERADD */

/* Define if fd_mask is defined in <sys/select.h> */
#undef HAVE_FDMASK_IN_SELECT

/* Define if you have the <db.h> header file.  */
#undef HAVE_DB_H

/* Define if you have the <db_185.h> header file.  */
#undef HAVE_DB_185_H

/* Define if timeradd is defined in <sys/time.h> */
#undef HAVE_TIMERADD
#ifndef HAVE_TIMERADD
#undef timercmp
#define	timercmp(tvp, uvp, cmp)						\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	    ((tvp)->tv_usec cmp (uvp)->tv_usec) :			\
	    ((tvp)->tv_sec cmp (uvp)->tv_sec))
#define timeradd(tvp, uvp, vvp)                                         \
        do {                                                            \
                (vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;          \
                (vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;       \
                if ((vvp)->tv_usec >= 1000000) {                        \
                        (vvp)->tv_sec++;                                \
                        (vvp)->tv_usec -= 1000000;                      \
                }                                                       \
        } while (0)
#define	timersub(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)
#endif /* !HAVE_TIMERADD */

/* Define if TAILQ_FOREACH is defined in <sys/queue.h> */
#undef HAVE_TAILQFOREACH
#ifndef HAVE_TAILQFOREACH
#define	TAILQ_FIRST(head)		((head)->tqh_first)
#define	TAILQ_END(head)			NULL
#define	TAILQ_NEXT(elm, field)		((elm)->field.tqe_next)
#define TAILQ_LAST(head, headname)					\
	(*(((struct headname *)((head)->tqh_last))->tqh_last))
#define TAILQ_FOREACH(var, head, field)					\
	for((var) = TAILQ_FIRST(head);					\
	    (var) != TAILQ_END(head);					\
	    (var) = TAILQ_NEXT(var, field))
#define	TAILQ_INSERT_BEFORE(listelm, elm, field) do {			\
	(elm)->field.tqe_prev = (listelm)->field.tqe_prev;		\
	(elm)->field.tqe_next = (listelm);				\
	*(listelm)->field.tqe_prev = (elm);				\
	(listelm)->field.tqe_prev = &(elm)->field.tqe_next;		\
} while (0)
#endif /* !HAVE_TAILQFOREACH */

/* Define if LIST_FIRST is defined in <sys/queue.h> */
#undef HAVE_LISTFIRST
#ifndef HAVE_LISTFIRST
#define	LIST_FIRST(head)		((head)->lh_first)
#define	LIST_END(head)			NULL
#define	LIST_EMPTY(head)		(LIST_FIRST(head) == LIST_END(head))
#define	LIST_NEXT(elm, field)		((elm)->field.le_next)
#endif /* !HAVE_LISTFIRST */

/* Prototypes for missing functions */
#ifndef HAVE_STRLCAT
size_t	 strlcat(char *, const char *, size_t);
#endif

#ifndef HAVE_STRLCPY
size_t	 strlcpy(char *, const char *, size_t);
#endif

#ifndef HAVE_STRSEP
char	*strsep(char **, const char *);
#endif

#ifndef HAVE_BASENAME
char	*basename(const char *);
char	*dirname(const char *);
#endif

#define timeout_add(ev, tv)		event_add(ev, tv)
#define timeout_set(ev, cb, arg)	event_set(ev, -1, 0, cb, arg)
#define timeout_del(ev)			event_del(ev)
#define timeout_pending(ev, tv)		event_pending(ev, EV_TIMEOUT, tv)
#define timeout_initialized(ev)		((ev)->ev_flags & EVLIST_INIT)
