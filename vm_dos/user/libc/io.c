#include "std.h"
extern int ksys_write(const char *);
extern int ksys_read(char *, int);
extern unsigned long ksys_ticks(void);
int write(const char *s) { return ksys_write(s); }
int read(char *buf, int len) { return ksys_read(buf, len); }
unsigned long ticks(void) { return ksys_ticks(); }