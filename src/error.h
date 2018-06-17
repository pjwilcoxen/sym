#ifndef ERROR_H
#define ERROR_H

int  error_count();
void error_back(char*,...);
void error_front(char*,...);
void error_more(char*,...);
void fatal_error(char*,char*);
void show_error(char*,char*,char*);
void validate(void*,int,char*);

#define FAULT(astr) show_error("Fatal Internal Fault","%s",astr)

#endif /* ERROR_H */
