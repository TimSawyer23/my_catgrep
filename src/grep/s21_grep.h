#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flag {
  int e, i, v, c, l, n, h, s, f, o;
  int isfirst;
  int err;
  int count_files;
  int myoptind;
  char *patterns;  // Динамический буфер для паттернов
  size_t patterns_size;  // Текущий размер буфера
} flags;

void print_line(flags *flag, char *namefile, char *line, int count_lines);
int flag_o(char *line, flags *flag, int regoptions, int count_lines,
           char *namefile);
int comp_exec(flags *flag, char *line, int regoptions);
int grep(char *namefile, flags *flag);
int add_pattern(flags *flag, char *pattern);
int regopt(int regoptions, flags *flag);
int flag_f(char *namefile, flags *flag);
int set_pattern(char **argv, flags *flag);
int process_files(int argc, char **argv, flags *flag);
int handle_flags(int argc, char **argv, flags *flag);
void initialize_flags(flags *flag);
void free_resources(flags *flag);
void ft_printErrors(int result, flags *flag);

#endif