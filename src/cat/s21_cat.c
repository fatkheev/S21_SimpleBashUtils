#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Flags flags = {false, false, false, false, false, false};
  int err = process_flags(argc, argv, &flags);

  if (err == 0) {
    process_inputs(flags, optind, argc, argv);
  }

  return 0;
}

int process_flags(int argc, char *argv[], Flags *flags) {
  int opt, res = 0;

  static struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "+bevnst", long_options, NULL)) != -1) {
    switch (opt) {
      case 'b':
        flags->b_flag = true;
        break;
      case 'e':
        flags->e_flag = true;
        break;
      case 'v':
        flags->v_flag = true;
        break;
      case 'n':
        flags->n_flag = true;
        break;
      case 's':
        flags->s_flag = true;
        break;
      case 't':
        flags->v_flag = true;
        flags->t_flag = true;
        break;
      default:
        printf("Неверный флаг!");
        res = 1;
    }
  }
  if (flags->n_flag == true && flags->b_flag == true) {
    flags->n_flag = false;
  }

  return res;
}

void process_inputs(Flags flags, int optind, int argc, char *argv[]) {
  if (optind == argc) {
    process_input(flags, stdin);
  } else {
    for (int i = optind; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      } else {
        process_input(flags, file);
        fclose(file);
      }
    }
  }
}

void process_input(Flags flags, FILE *input) {
  int line_number = 1;
  int c;
  bool new_line = true;
  bool previous_new_line = false;
  int cnt_line = 0;
  bool line_not_n = false;

  while ((c = fgetc(input)) != EOF) {
    if (c != '\n') {
      cnt_line = 0;
      line_not_n = true;
    }

    if (flags.s_flag == true && c == '\n' && previous_new_line == true) {
      cnt_line++;
      if (cnt_line > 1) {
        continue;
      }
    }

    if ((flags.b_flag == true && new_line == true && line_not_n == true) ||
        (flags.n_flag == true && new_line == true)) {
      printf("%6d\t", line_number++);
    }

    if (flags.t_flag && c == '\t') {
      printf("^I");
    } else if ((flags.v_flag == true || flags.e_flag == true) &&
               !is_printable(c) && c != '\n' && c != '\t') {
      if (c == 127) {
        printf("^?");
      } else if (c < 32) {
        printf("^%c", c + 64);
      } else if (c > 12 && c < 160) {
        printf("M-^%c", c - 64);
      } else {
        printf("%c", c);
      }
    } else {
      if (flags.e_flag && c == '\n') {
        putchar('$');
      }
      putchar(c);
    }

    if (c == '\n') {
      previous_new_line = true;
      new_line = true;
      line_not_n = false;
    } else {
      previous_new_line = false;
      new_line = false;
    }
  }
}

bool is_printable(char c) { return c >= 32 && c <= 126; }