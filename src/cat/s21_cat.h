#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  bool b_flag;
  bool e_flag;
  bool v_flag;
  bool n_flag;
  bool s_flag;
  bool t_flag;
} Flags;

int process_flags(int argc, char *argv[], Flags *flags);
void process_inputs(Flags flags, int optind, int argc, char *argv[]);
void process_input(Flags flags, FILE *input);
bool is_printable(char c);

#endif