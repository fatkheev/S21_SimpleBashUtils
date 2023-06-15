#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PATTERNS 1024

typedef struct {
  bool ignore_registr_i;
  bool invert_search_v;
  bool count_match_c;
  bool only_names_l;
  bool print_line_numbers_n;
  bool no_name_file_h;
  bool ignor_errors_s;
} Flags;

int parser(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
           int *pattern_count, char *filenames[], Flags *flags);
void parse_options(int argc, char *argv[], char patterns[][MAX_LINE_LENGTH],
                   int *pattern_count, Flags *flags);
void open_and_search_in_file(char *filenames[], int name_count,
                             char patterns[][MAX_LINE_LENGTH],
                             int pattern_count, Flags flags);
int find_pattern(char *line, char *pattern, bool ignore_registr_i);
int is_search_match(bool invert_search_v, int found);
void obrabotka(FILE *file, char *filename, char patterns[][MAX_LINE_LENGTH],
               int pattern_count, Flags flags, int name_count,
               char line[MAX_LINE_LENGTH], int count, int evrica,
               int line_number);

struct option long_options[] = {
    {"i", 0, 0, 'i'}, {"v", 0, 0, 'v'}, {"c", 0, 0, 'c'}, {"l", 0, 0, 'l'},
    {"n", 0, 0, 'n'}, {"h", 0, 0, 'h'}, {"s", 0, 0, 's'}, {0, 0, 0, 0}};