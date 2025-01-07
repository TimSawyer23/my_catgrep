#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int result = 0;
  struct flags flagi = {0};  // Инициализация структуры
  flagi.linecount = 1;
  char **filesNames =
      malloc((argc - 1) * sizeof(char *));  // Выделение памяти для имен файлов
  if (filesNames == NULL) {
    free(filesNames);
    result = 1;
  } else {
    if (result == 0) ft_readFlags(argc, argv, &flagi);
    if (result == 0) result = ft_readFilesNames(argc, argv, filesNames);
    if (result == 0) result = ft_readFiles(filesNames, &flagi);
    cleanup(&flagi, filesNames);
  }
  ft_printError(result);
  return 0;
}

int ft_readFiles(char **filesNames, struct flags *flagi) {
  int result = 0;
  if (flagi->fileNamesCount > 0) {
    if (flagi->error == 0) {
      flagi->lastch = '\n';
      for (int i = 0; i < flagi->fileNamesCount; i++) {
        ft_cat(filesNames[i], flagi);
      }
    } else {
      result = 3;
    }
  } else {
    result = 2;
  }
  return result;
}

void cleanup(struct flags *flagi, char **filesNames) {
  for (int i = 0; i < flagi->fileNamesCount; i++) {
    free(filesNames[i]);
  }
  free(filesNames);
}

void ft_printError(int rcode) {
  switch (rcode) {
    case 1:
      printf("Память пыталась, но не смогла выделиться.");
      break;
    case 2:
      printf("Отсутствуют файлы.");
      break;
    case 3:
      printf("Встретился неизвестный пиратский флаг.");
      break;
    case 4:
      printf("Файл пытался, но не смог открыться.");
      break;
    default:
      break;
  }
}

int ft_strcmp(char *s1, char *s2) {
  int i = 0;
  while ((s1[i] == s2[i]) && s1[i] && s2[i]) i++;
  return (s1[i] - s2[i]);
}

void handleLongFlag(char *flag, struct flags *flagi) {
  if (ft_strcmp(flag, "--number") == 0) {
    flagi->n = 1;
  } else if (ft_strcmp(flag, "--number-nonblank") == 0) {
    flagi->b = 1;
    flagi->n = 1;
  } else if (ft_strcmp(flag, "--show-ends") == 0) {
    flagi->e = 1;
  } else if (ft_strcmp(flag, "--squeeze-blank") == 0) {
    flagi->s = 1;
  } else if (ft_strcmp(flag, "--show-tabs") == 0) {
    flagi->t = 1;
  } else if (ft_strcmp(flag, "--show-nonprinting") == 0) {
    flagi->v = 1;
  } else if (ft_strcmp(flag, "--") != 0) {
    flagi->error = 1;
  }
}

void handleShortFlag(char flag, struct flags *flagi) {
  switch (flag) {
    case 'b':
      flagi->b = 1;
      flagi->n = 1;
      break;
    case 'e':
      flagi->e = 1;
      flagi->v = 1;
      break;
    case 's':
      flagi->s = 1;
      break;
    case 't':
      flagi->t = 1;
      flagi->v = 1;
      break;
    case 'n':
      flagi->n = 1;
      break;
    case 'v':
      flagi->v = 1;
      break;
    case 'E':
      flagi->e = 1;
      break;
    case 'T':
      flagi->t = 1;
      break;
    default:
      flagi->error = 1;
      break;
  }
}

void handleArgument(char *arg, struct flags *flagi) {
  if (arg[0] == '-') {
    if (arg[1] == '-') {
      handleLongFlag(arg, flagi);
    } else {
      for (int j = 1; arg[j]; j++) {
        handleShortFlag(arg[j], flagi);
      }
    }
  } else {
    flagi->fileNamesCount += 1;  // Считаем только имена файлов
  }
}

void ft_readFlags(int argc, char *argv[], struct flags *flagi) {
  for (int i = 1; i < argc; i++) {
    handleArgument(argv[i], flagi);
  }
}

int ft_readFilesNames(int argc, char *argv[], char **names) {
  int index = 0;
  int result = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-' && result == 0) {
      names[index] = strdup(argv[i]);
      if (names[index] == NULL) {
        fprintf(stderr, "Память пыталась, но не смогла выделиться\n");
        result = 5;
        break;
      }
      index++;
    }
  }
  return result;
}

int ft_cat(char *filename, struct flags *flagi) {
  int result = 0;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    result = 4;
    perror("fopen");
  };
  if (result == 0) ft_handleflags(flagi, file);
  if (file) fclose(file);
  return result;
}

void ft_handleflags(struct flags *flagi, FILE *file) {
  char ch;
  int ncount = 0;
  for (char last = flagi->lastch; (ch = fgetc(file)) != EOF; last = ch) {
    flagi->lastch = ch;
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
        printf("%6d\t", flagi->linecount);
        flagi->linecount++;
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
      if (!isascii(ch)) {
        if (ch != '\t') {
          printf("M-");
          ch = ch - 128;
        }
      }
      if (iscntrl(ch)) {
        if (ch == 127) {  // если символ DEL то печатает ^?
          putchar('^');
          putchar('?');
        } else if (ch == '\t') {
          putchar(ch);
        } else {
          putchar('^');
          putchar(ch + 64);
        }
        continue;
      }
    }
    putchar(ch);
  }
}