#ifndef S21_CAT_H
#define S21_CAT_H
#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int b;
  int e;
  int s;
  int t;
  int n;
  int v;
  int error;
  int fileNamesCount;
  int linecount;
  char lastch;
};

void ft_readFlags(int argc, char *argv[], struct flags *flagi);
int ft_readFilesNames(int argc, char *argv[], char **names);
int ft_readFiles(char **filesNames, struct flags *flagi);
int ft_cat(char *filename, struct flags *flagi);
void cleanup(struct flags *flagi, char **filesNames);
void ft_handleflags(struct flags *flagi, FILE *file);
void ft_printError(int rcode);
int ft_strcmp(char *s1, char *s2);

#endif