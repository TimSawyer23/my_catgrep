#include "s21_grep.h"

int main(int argc, char **argv) {
  flags flag = {0};
  flag.patterns = NULL;  // Инициализация указателя
  int result = 0;
  // Обработка аргументов командной строки
  result = (argc >= 2) ? handle_flags(argc, argv, &flag) : 1;
  // Установка паттернов
  if (result == 0) result = set_pattern(argv, &flag);
  // Обработка файлов
  if (result == 0) result = process_files(argc, argv, &flag);
  // Освобождение ресурсов
  free_resources(&flag);
  // Печатаем ошибки в зависимости от подаваемого кода ошибки
  ft_printErrors(result, &flag);
  return 0;
}

void ft_printErrors(int result, flags *flag) {
  switch (result) {
    case 1:
      fprintf(stderr, "Usage: s21_grep[OPTIONS]...PATTERNS[FILE]...\n");
      break;
    case 2:
      fprintf(stderr, "Ошибка выделения памяти\n");
      break;
    case 3:
      fprintf(stderr, "grep: patterns are not set\n");
      break;
    case 4:
      fprintf(stderr, "grep: %s: syntax error near unexpected\n",
              flag->patterns);
      break;
    default:
      break;
  }
}

// Обработка аргументов командной строки
int handle_flags(int argc, char **argv, flags *flag) {
  int result = 0;
  int rez = 0;
  while ((rez = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (rez) {
      case 'e':
        flag->e = 1;
        add_pattern(flag, optarg);
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag_f(optarg, flag);
        break;
      case 'o':
        flag->o = 1;
        break;
      case '?':
        if (flag->s != 1) {
          result = 1;
        }
    }
  }

  // Установка взаимоисключающих флагов
  if (flag->l) flag->c = flag->o = flag->n = 0;
  if (flag->c) flag->o = flag->n = 0;
  if (flag->v && flag->o) flag->err = 1;

  flag->myoptind = optind;
  return result;
}

// Добавляем первый встреченный аргумент, не являющийся опцией, в паттерны, при
// условии, что у нас нет и не будет других паттернов
int set_pattern(char **argv, flags *flag) {
  int result = 0;
  if (flag->e != 1 && flag->f != 1) {
    result = add_pattern(flag, argv[flag->myoptind]);
    flag->myoptind++;
  }
  return result;
}

// Обработка файлов, в которых ищем регулярные выражения
int process_files(int argc, char **argv, flags *flag) {
  int result = 0;
  flag->count_files = argc - flag->myoptind;
  while (flag->myoptind < argc) {
    result = grep(argv[flag->myoptind], flag);
    flag->myoptind++;
  }
  return result;
}

// Освобождение ресурсов
void free_resources(flags *flag) {
  if (flag->patterns) free(flag->patterns);
}

// Добавляем паттерн в массив
int add_pattern(flags *flag, char *pattern) {
  int result = 0;
  size_t new_size =
      flag->patterns_size + strlen(pattern) + 2;  // +2 для "|" и '\0'

  // Увеличиваем буфер, если необходимо
  if (new_size > flag->patterns_size) {
    char *new_patterns = realloc(flag->patterns, new_size);
    if (!new_patterns) result = 2;
    flag->patterns = new_patterns;
    flag->patterns_size = new_size;
  }

  // Добавляем новый паттерн
  if (result == 0) {
    if (flag->isfirst && (flag->f || flag->e)) {
      strcat(flag->patterns, "|");
    } else {
      flag->isfirst++;
      flag->patterns[0] = '\0';  // Очищаем буфер, если это первый паттерн
    }
    strcat(flag->patterns, pattern);
  }
  return result;
}

int grep(char *namefile, flags *flag) {
  int regoptions = regopt(REG_NEWLINE, flag);  // для хранения флагов
  int result = 0;
  FILE *file;
  file = fopen(namefile, "r");
  char *line = NULL;
  size_t buffer = 0;
  if (file == NULL) {
    if (flag->s != 1)
      fprintf(stderr, "s21_grep: %s: Нет такого файла или каталога\n",
              namefile);
  } else {
    int readgl = 0;
    int matches = 0;  // счетчик совпадений
    int count_lines = 0;
    while ((readgl = getline(&line, &buffer, file)) != -1) {
      count_lines++;
      if (line[readgl - 1] == '\n') line[readgl - 1] = '\0';
      if (flag->o == 1 && flag->err != 1)
        result = flag_o(line, flag, regoptions, count_lines, namefile);
      else {
        int regstat = result = comp_exec(flag, line, regoptions);
        if (regstat == 1) {
          matches++;
          if (flag->l != 1) {
            print_line(flag, namefile, line, count_lines);
          } else {
            printf("%s\n", namefile);
            break;
          }
        }
      }
    }
    if (result < 2) {
      if (flag->c == 1 && flag->count_files > 1 && flag->h != 1)
        printf("%s:%d\n", namefile, matches);
      else {
        if (flag->c == 1) printf("%d\n", matches);
      }
      result = 0;
    }
  }
  if (line) free(line);
  if (file) fclose(file);
  return result;
}

void print_line(flags *flag, char *namefile, char *line, int count_lines) {
  if (flag->count_files > 1 && flag->h != 1 && flag->c != 1 && flag->err != 1) {
    printf("%s:", namefile);
  }
  if (flag->n == 1 && flag->c != 1 && flag->err != 1)
    printf("%d:", count_lines);
  if (flag->c != 1 && flag->err != 1) printf("%s\n", line);
}

int flag_f(char *namefile, flags *flag) {
  FILE *file;
  file = fopen(namefile, "r");
  char *line = NULL;
  size_t buffer = 0;
  if (file == NULL) {
    if (flag->s != 1)
      fprintf(stderr, "grep: %s: Нет такого файла или каталога\n", namefile);
  } else {
    int result = 0;
    while ((result = getline(&line, &buffer, file)) != -1) {
      if (line[result - 1] == '\n') line[result - 1] = '\0';
      add_pattern(flag, line);
      flag->f = 1;
    }
    if (line) free(line);
    if (file) fclose(file);
  }
  return 0;
}

int regopt(int regoptions, flags *flag) {
  int opt = regoptions;
  if (flag->e == 1 || flag->f == 1) {
    opt = opt | REG_EXTENDED;
  }
  if (flag->i) {
    opt = opt | REG_ICASE;
  }
  return opt;
}

int comp_exec(flags *flag, char *line, int regoptions) {
  int result = 0;
  regex_t regex;  // Для хранения скомпилированного выражения regex
  // Проверяем что flag->patterns не равен NULL
  if (!flag->patterns) result = 3;
  // Компилируем регулярное выражение
  if (regcomp(&regex, flag->patterns, regoptions) != 0) result = 4;
  // Сопоставляем строку с регулярным выражением
  if (result == 0) {
    result = !regexec(&regex, line, 0, NULL, 0);
    regfree(&regex);
    // Инвертировать результат, если флаг v установлен
    if (flag->v == 1) result = !result;
  }
  return result;
}

int flag_o(char *line, flags *flag, int regoptions, int count_lines,
           char *namefile) {
  int result = 0;
  char *s = line;  // присваиваем указателю строку
  regex_t regex;  // будет содержать регулярные выражения
  regmatch_t pmatch[1];  // начало вхождения (so) и конец вхождения (eo)
  if (regcomp(&regex, flag->patterns, regoptions)) result = 3;
  if (result == 0) {
    for (int i = 0;; i++) {
      if (regexec(&regex, s, 1, pmatch, 0)) break;
      int len = pmatch[0].rm_eo -
                pmatch[0].rm_so;  // колво символов которые будут выводиться
      if (flag->count_files > 1 && flag->h != 1 && flag->c != 1) {
        printf("%s:", namefile);
      }
      if (flag->n == 1) {
        printf("%d:%.*s\n", count_lines, len,
               s + pmatch[0].rm_so);  // строка выводится с начала
      } else {
        printf("%.*s\n", len,
               s + pmatch[0].rm_so);  // входа (len колво символов)
      }
      s += pmatch[0].rm_eo;  // строке присваивается конец паттерна
    }
  }
  regfree(&regex);
  return result;
}