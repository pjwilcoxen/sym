#ifndef OPTIONS_H
#define OPTIONS_H

int get_line_length();

int is_alpha_elements();
int is_eqn_lvalue();
int is_eqn_normalized();
int is_eqn_scalar();
int is_eqn_set();
int is_eqn_vector();
int is_explicit_time();
int is_reserved_word(char*);
int is_sum_scalar();
int is_sum_set();
int is_sum_vector();

int set_alpha_elements();
int set_eqn_lvalue();
int set_eqn_normalized();
int set_eqn_scalar();
int set_eqn_vector();
int set_explicit_time();
int set_line_length(int);
int set_reserved_word(char*);
int set_sum_scalar();
int set_sum_vector();

#endif /* OPTIONS_H */
