/*
 * varargs.h for Z8002 (nonsegmented)
 *
 * All arguments are pushed on the stack as 16-bit words (right-to-left).
 * Longs and doubles occupy multiple words.  The frame pointer (r14) points
 * to the saved FP; arguments start at 4(r14).
 *
 * Usage:
 *   #include <varargs.h>
 *   int printf(va_alist) va_dcl {
 *       va_list ap;
 *       char *fmt;
 *       va_start(ap);
 *       fmt = va_arg(ap, char *);
 *       ...
 *       va_end(ap);
 *   }
 */

typedef char *va_list;

#define va_dcl int va_alist;
#define va_start(ap) ap = (char *)&va_alist
#define va_end(ap)
#define va_arg(ap, type) ((type *)(ap += sizeof(type)))[-1]
