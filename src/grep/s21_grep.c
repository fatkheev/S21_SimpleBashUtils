#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char patterns[MAX_PATTERNS][MAX_LINE_LENGTH];
  char *filenames[MAX_PATTERNS];
  int pattern_count = 0;
  Flags flags = {false, false, false, false, false, false, false};

  int name_count =
      parser(argc, argv, patterns, &pattern_count, filenames, &flags);

  open_and_search_in_file(filenames, name_count, patterns, pattern_count,
                          flags);
  return 0;
}

int parser(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
           int *pattern_count, char *filenames[], Flags *flags) {
  if (argc < 3) {
    printf(
        "Usage: %s [-i] [-v] [-c] [-e] [-l] [-n] [-h] [-s] <pattern> <file>\n",
        argv[0]);
    exit(-1);
  }

  parse_options(argc, argv, patterns, pattern_count, flags);

  if (*pattern_count == 0 && optind < argc) {
    strcpy(patterns[*pattern_count], argv[optind]);
    (*pattern_count)++;
    optind++;
  }

  int name_count = argc - optind;

  for (int i = 0; optind < argc; i++) {
    filenames[i] = argv[optind++];
  }

  return name_count;
}

void parse_options(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
                   int *pattern_count, Flags *flags) {
  int option_index = 0;
  int c;

  while ((c = getopt_long(argc, argv, "ive:clnhs", long_options,
                          &option_index)) != -1) {
    switch (c) {
      case 'i':
        flags->ignore_registr_i = true;
        break;
      case 'v':
        flags->invert_search_v = true;
        break;
      case 'c':
        flags->count_match_c = true;
        break;
      case 'e':
        strcpy(patterns[(*pattern_count)++], optarg);
        break;
      case 'l':
        flags->only_names_l = true;
        break;
      case 'n':
        flags->print_line_numbers_n = true;
        break;
      case 'h':
        flags->no_name_file_h = true;
        break;
      case 's':
        flags->ignor_errors_s = true;
        break;
      default:
        printf(
            "Usage: %s [-i] [-v] [-c] [-l] [-n] [-h] [-s] [-e pattern] "
            "<file>\n",
            argv[0]);
        exit(-1);
    }
  }
}

void open_and_search_in_file(char *filenames[], int name_count,
                             char patterns[][MAX_LINE_LENGTH],
                             int pattern_count, Flags flags) {
  for (int j = 0; j < name_count; j++) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    int evrica = 0;
    int line_number = 0;

    FILE *file;

    if (filenames[j] != NULL) {
      file = fopen(filenames[j], "r");
      if (file == NULL) {
        if (flags.ignor_errors_s == false) {
          printf("grep: %s: No such file or directory\n", filenames[j]);
        }
        continue;
      }
    }

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
      line_number++;
      if (line[strlen(line) - 1] != '\n') {
        strcat(line, "\n");
      }
      for (int i = 0; i < pattern_count; i++) {
        int found = find_pattern(line, patterns[i], flags.ignore_registr_i);
        int is_match = is_search_match(flags.invert_search_v, found);

        if (is_match == 1) {
          evrica = 1;
        }

        if (is_match == 1 && flags.count_match_c) {
          count++;
          continue;
        }

        if (flags.only_names_l) continue;

        if (is_match == 1) {
          if (flags.no_name_file_h) {
            if (flags.print_line_numbers_n)
              printf("%d:%s", line_number, line);
            else
              printf("%s", line);
          } else if (name_count < 2) {
            if (flags.print_line_numbers_n)
              printf("%d:%s", line_number, line);
            else
              printf("%s", line);
          } else {
            if (flags.print_line_numbers_n)
              printf("%s:%d:%s", filenames[j], line_number, line);
            else
              printf("%s:%s", filenames[j], line);
          }
          break;
        }
      }
    }

    if (flags.count_match_c) {
      if (name_count > 1) {
        printf("%s:%d\n", filenames[j], count);
      } else {
        printf("%d\n", count);
      }
    }

    if (flags.only_names_l && evrica == 1) {
      printf("%s\n", filenames[j]);
    }

    if (file != stdin) {
      fclose(file);
    }
  }
}

int find_pattern(char *line, char *pattern, bool ignore_registr_i) {
  regex_t regex;

  int regcomp_result;
  int res_nomatch;

  if (ignore_registr_i == true) {
    regcomp_result = regcomp(&regex, pattern, REG_ICASE);
  } else {
    regcomp_result = regcomp(&regex, pattern, REG_EXTENDED);
  }

  if (regcomp_result == 1) {
    return 0;
  }

  int regexec_result = regexec(&regex, line, 0, NULL, 0);
  regfree(&regex);

  if (regexec_result != 1) {
    res_nomatch = 1;
  } else if (regexec_result == REG_NOMATCH) {
    res_nomatch = 0;
  } else {
    res_nomatch = 0;
  }
  return res_nomatch;
}

int is_search_match(bool invert_search_v, int found) {
  if (invert_search_v) {
    found = !found;
  }
  return found;
}
