#ifndef LANG_H
#define LANG_H

void addlang(char *lang, void (*fnc)(void), char *ver );
void lang_begin_block( void (*fnc)()  );
void lang_begin_eqn( void (*fnc)()  );
void lang_begin_file( void (*fnc)()  );
void lang_begin_func( char* (*fnc)() );
void lang_declare( void (*fnc)()  );
void lang_end_eqn( void (*fnc)()  );
void lang_end_file( void (*fnc)()  );
void lang_end_func( char* (*fnc)() );
void lang_show_symbol( char* (*fnc)() );
void set_language(char*);
void setup_languages(void);

#endif /* LANG_H */
