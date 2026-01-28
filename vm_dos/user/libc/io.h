#ifndef vm_dos_IO_H
#define vm_dos_IO_H
int write(const char *s);
int read(char *buf, int len);
unsigned long ticks(void);
#endif