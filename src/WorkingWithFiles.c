#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct data_time {
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_hour;
  int tm_min;
  int tm_sec;
} tm;

void errors(int, struct tm *now);
void read_file(char *file_name, char **buffer, int *counter_line, int *err,
               struct tm *now);
void print_to_terminal(char **buffer, int counter_line);
void change_file(char *file_name, char *string, int *err, struct tm *now);
void rewrite_file(char *file_name, char **buffer, int counter_line, int *err,
                  int sdvig, struct tm *now);
int shift_to_num(char *shift, int *err, struct tm *now);
int razryad(int, int);
void shifr_Cesary(char **buffer, int i, int j, int sdvig);
void print_data(struct tm *now);

int main(int argc, char *argv[]) {
  time_t mytime = time(NULL);  // текущее дата и время
  struct tm *now = localtime(&mytime);
  print_data(now);
  printf("start programm\n");

  int err = 0;  // наличие ошибок при запуске или исполнении программы

  char *file_name = malloc(sizeof(char) * 1000);
  if (file_name == NULL) {
    err = 1;
    errors(err, now);
  }

  if (argc == 1) {
    print_data(now);
    printf("err input arguments\n");
    err = 2;
    errors(err, now);
  }

  char **buffer = malloc(sizeof(char *)*1000);

  for (int i = 0; i < 1000; i++) {
    buffer[i] = malloc(sizeof(char)*1000);
    if (buffer[i] == NULL) {
      err = 3;
      errors(err, now);
    }
  }

  if (buffer == NULL) {
    err = 4;
    errors(err, now);
  }

  int counter_line = 0;

  /*
  ***МЕНЮ УПРАВЛЕНИЯ***
  При запуске скомпилированного файла указать номер пункта и соответсвующую
  информацию

  1 <имя файла>  - команда для чтения файла, записи содержимого в буфер и вывод
                   содержимого в терминал, если фай1ла с таким именем не
  существует или он пустой, то ошибка 2 <строка>     - команда для добавления
  строки в конце файла., имя которого указано в предыдущем пункте меню, если
  вызова не было, то ошибка 3 <сдвиг>      - команда шифрования файла, при
  помощи ключа (сдвиг на любое количество позиций по таблице ASCII)

  -1             - команда выхода из программы
  */
  for (int i = 1; i < argc; i++) {
    switch (*argv[i]) {
      case '1':
        if (strlen(argv[i]) == 1 && i + 1 < argc) {
          strcpy(file_name, argv[++i]);
          read_file(file_name, buffer, &counter_line, &err, now);
          // print_to_terminal(buffer, counter_line);
        } else {
          err = 5;
          errors(err, now);
          break;
        }
        break;

      case '2':
        if (strlen(argv[i]) == 1 && i + 1 < argc) {
          char *string = malloc(sizeof(char) * 1000);
          if (string == NULL) {
            err = 6;
            errors(err, now);
          }
          strcpy(string, argv[++i]);
          if (err == 0) {
            change_file(file_name, string, &err, now);
            free(string);
            read_file(file_name, buffer, &counter_line, &err, now);
            // print_to_terminal(buffer, counter_line);
          } else {
            err = 7;
            errors(err, now);
          }
        } else {
          err = 8;
          errors(err, now);
        }
        break;

      case '3':
        if (strlen(argv[i]) == 1 && i + 1 < argc) {
          if (argv[i] && !err) {
            char *shift = malloc(sizeof(char) * 1000);
            if (shift == NULL) {
              err = 17;
              errors(err, now);
            }
            strcpy(shift, argv[++i]);
            int sdvig = shift_to_num(shift, &err, now);
            free(shift);
            if (!err)
              rewrite_file(file_name, buffer, counter_line, &err, sdvig, now);
            print_to_terminal(buffer, counter_line);

          } else {
            err = 9;
            errors(err, now);
          }
        } else {
          err = 10;
          errors(err, now);
        }
        break;

      case '-':
        if (argv[i][1] == '1' && strlen(argv[i]) == 2) {
          free(file_name);
          print_data(now);
          printf("stop programm\n");
          return 0;
        } else {
          err = 11;
          errors(err, now);
        }
        break;

      default:
        err = 12;
        errors(err, now);
        break;
    }
  }

  for (int i = 0; i < counter_line; i++)
    free(buffer[i]);  // Освобождаем каждую строку буфера

  free(buffer);  // Освобождаем буфер
  free(file_name);
  return 0;
}

void errors(int num_errors, struct tm *now) {
  if (num_errors) {
    char *comment = " ";
    switch (num_errors) {
      case 1:
      case 3:
      case 4:
      case 6:
      case 17:
      case 19:
        comment = "memory";
        break;
      case 2:
      case 5:
      case 8:
      case 10:
      case 11:
      case 12:
      case 18:
      case 20:
        comment = "err input parametrs";
        break;
      case 7:
      case 9:
        comment = "before err";
        break;
      case 13:
      case 14:
      case 15:
      case 16:
        comment = "don't have file or file empty";
        break;
      default:
        comment = "another err";
        break;
    }
    print_data(now);
    printf("n/a, err=%d (%s)\n", num_errors, comment);
  }
}

void read_file(char *file_name, char **buffer, int *counter_line, int *err,
               struct tm *now) {
  FILE *file = fopen(file_name, "r");
  print_data(now);
  printf("open file \"%s\" for reading...\n", file_name);
  if (file != NULL) {
    while (fgets(buffer[*counter_line], 1000, file)) {
      (*counter_line)++;
    }
    if (*counter_line == 0) {
      print_data(now);
      printf("file \"%s\" is empty\n", file_name);
      *err = 13;
      errors(*err, now);
    }
    print_data(now);
    printf("close file \"%s\"\n", file_name);
    fclose(file);
  } else {
    print_data(now);
    printf("No such file \"%s\" in directory\n", file_name);
    *err = 14;
    errors(*err, now);
  }
}

void print_to_terminal(char **buffer, int counter_line) {
  for (int i = 0; i < counter_line; i++) printf("%s", buffer[i]);
}

void change_file(char *file_name, char *string, int *err, struct tm *now) {
  FILE *file = fopen(file_name, "a");
  print_data(now);
  printf("open file \"%s\" for add string...\n", file_name);
  if (file != NULL) {
    print_data(now);
    printf("add string \"%s\" in end of file \"%s\"\n", string, file_name);
    fputs("\n", file);
    fputs(string, file);
    print_data(now);
    printf("close file \"%s\"\n", file_name);
    fclose(file);
  } else {
    print_data(now);
    printf("No such file \"%s\" in directory\n", file_name);
    *err = 15;
    errors(*err, now);
  }
}

void rewrite_file(char *file_name, char **buffer, int counter_line, int *err,
                  int sdvig, struct tm *now) {
  FILE *file = fopen(file_name, "w");
  print_data(now);
  printf("open file \"%s\" for rewriting(coding), key=%d\n", file_name, sdvig);
  if (file != NULL) {
    for (int i = 0; i < counter_line; i++) {
      int column = (int)strlen(buffer[i]);
      for (int j = 0; j < column; j++) {
        shifr_Cesary(buffer, i, j, sdvig);
        fputc(buffer[i][j], file);
      }
    }
    print_data(now);
    printf("close file \"%s\"\n", file_name);
    fclose(file);
  } else {
    print_data(now);
    printf("No such file \"%s\" in directory\n", file_name);
    *err = 16;
    errors(*err, now);
  }
}

int razryad(int i, int len) {
  if (i == len - 1) return 1;
  return 10 * razryad(i + 1, len);
}

int shift_to_num(char *shift, int *err, struct tm *now) {
  if (shift == NULL) {
    *err = 18;
    errors(*err, now);
  }

  int len = strlen(shift);
  int num = 0;
  int minus = 1;
  int start_index = 0;

  if (shift[0] == '-') {
    minus = -1;
    start_index = 1;
  }

  for (int i = start_index; i < len; i++) {
    if (shift[i] >= '0' && shift[i] <= '9') {
      num += (shift[i] - '0') * razryad(i - start_index, len - start_index);
    } else {
      *err = 18;
      errors(*err, now);
      break;
    }
  }

  num *= minus;
  return num % 128;
}

void shifr_Cesary(char **buffer, int i, int j, int sdvig) {
  if (buffer[i][j] + sdvig >= 0 && buffer[i][j] + sdvig < 128)
    buffer[i][j] += sdvig;
  if (buffer[i][j] + sdvig > 127) buffer[i][j] = (buffer[i][j] + sdvig) % 128;
  if (buffer[i][j] + sdvig < 0) buffer[i][j] = 128 - buffer[i][j] - sdvig;
}

void print_data(struct tm *now) {
  printf("%02d.%02d.%04d %02d:%02d:%02d\t", now->tm_mday, now->tm_mon + 1,
         now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec);
}
