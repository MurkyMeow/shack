#ifndef _IMPORTS_H_
#define _IMPORTS_H_

void console_log(int) __attribute__((__import_name__("console_log")));
void console_logf(float) __attribute__((__import_name__("console_log")));

#endif
