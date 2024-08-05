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
  int lineNumber = 1;
  int slashn = 0;
  int lastslashn = 0;
  int firstLine = 1;

  int i = 0;

  char *allch;
  char *buffer;

  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    perror("fopen");
    return;
  }

  while ((ch = fgetc(file)) != EOF) {
    if(i = 0) {
      allch = malloc(1 * sizeof(char) + 1);
      allch[i] = ch;
      allch[i+1] = '\0';
      printf("%c", allch[i]);
    }
    buffer = strdup(allch);
    free(allch);
    allch = malloc(strlen(buffer) * sizeof(char) + 2);
    strcpy(allch, buffer);
    allch[i] = ch;
    allch[i+1] = '\0';
    i++;
    free(buffer);

    printf("%s", allch);
    
    
    
    
    
    
    
    /* if (flagi->s && ch == '\n') {
      slashn++;
      if (slashn >= 1) {
        lastslashn = 1;
        continue;
      }
    } else {
      if (lastslashn) putchar('\n');
      lastslashn = 0;
      slashn = 0;
    }

    if (flagi->e && ch == '\n') {
      putchar('$');
    }

    if (flagi->b && ch == '\n') {
      lineNumber++;
    }

    if (flagi->n && firstLine) {
      printf("%6d	 ", lineNumber++);
      firstLine = 0;
    } else if (flagi->n && ch == '\n') {
      putchar(ch);
      printf("%6d	 ", lineNumber++);
      continue;
    }

    if (flagi->t && ch == '\t') {
      printf("^I");
      continue;
    }

    if (flagi->v) {
      if (!isprint(ch) && ch != '\n' && ch != '\t') {
        printf("M-^%c", ch ^ 0x40);
        continue;
      }
    }
    putchar(ch); */
  }

  fclose(file);
}