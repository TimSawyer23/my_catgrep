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
  int isNotFlag;
  int fileNamesCount;
};

void ft_readFlags(int argc, char *argv[], struct flags *flagi);
void ft_readFilesNames(int argc, char *argv[], char **names,
                       struct flags *flagi);
void ft_readFile(char *filename, struct flags *flagi);

int main(int argc, char *argv[]) {
  struct flags flagi = {0};  // Инициализация структуры
  char **filesNames =
      malloc((argc - 1) * sizeof(char *));  // Выделение памяти для имен файлов

  if (filesNames == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }

  ft_readFlags(argc, argv, &flagi);
  ft_readFilesNames(argc, argv, filesNames, &flagi);

  if (flagi.fileNamesCount > 0) {
    for (int i = 0; i < flagi.fileNamesCount; i++) {
      ft_readFile(filesNames[i], &flagi);
    }
  }

  // Освобождение памяти
  for (int i = 0; i < flagi.fileNamesCount; i++) {
    free(filesNames[i]);
  }
  free(filesNames);

  return 0;
}

void ft_readFlags(int argc, char *argv[], struct flags *flagi) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j]; j++) {
        switch (argv[i][j]) {
          case 'b':
            flagi->b = 1;
            flagi->n = 1;
            break;
          case 'e':
            flagi->e = 1;
            break;
          case 's':
            flagi->s = 1;
            break;
          case 't':
            flagi->t = 1;
            break;
          case 'n':
            flagi->n = 1;
            break;
          case 'v':
            flagi->v = 1;
            break;
          default:
            flagi->isNotFlag = 1;
            break;
        }
      }
    } else {
      flagi->fileNamesCount += 1;  // Считаем только имена файлов
    }
  }
}

void ft_readFilesNames(int argc, char *argv[], char **names,
                       struct flags *flagi) {
  int index = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      names[index] = strdup(argv[i]);
      if (names[index] == NULL) {
        fprintf(stderr, "Память пыталась, но не смогла выделиться\n");
        exit(1);
      }
      index++;
    }
  }
}

void ft_readFile(char *filename, struct flags *flagi) {
  char ch;
  int ncount = 0;
  int lineCount = 1;
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    perror("fopen");
    return;
  }

  for (char last = '\n'; (ch = fgetc(file)) != EOF; last = ch) {
    if (last == '\n') {
      if (flagi->s) {
        if (ch == '\n') {
          if (ncount == 1) {
            continue;
          }
          ncount = 1;
        } else {
          ncount = 0;
        }
      }
      if (flagi->n && (!flagi->b || ch != '\n')) {
        printf("%6d\t", lineCount);
        lineCount++;
      }
    }
    if (ch == '\n') {
      if (flagi->e) {
        putchar('$');
      }
    } else if (ch == '\t' && flagi->t) {
      printf("^I");
      continue;
    } else if (flagi->v) {
      if (!isascii(ch) || !isprint(ch)) {
        printf("M-");
      }
      if (iscntrl(ch)) {
        putchar('^');
        if (ch == 127) {
          putchar('?');
        } else {
          putchar(ch + 64);
        }
        continue;
      }
    }
    putchar(ch);
  }
  fclose(file);
}