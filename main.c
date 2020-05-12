#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/*Прототипы функций*/
void free_matrix(int **, int **, int **, int **, int);
void multiplication_matrix(int **, int **, int **, int);
void multiplication_matrix_trans(int **, int **, int **, int);
void matrix_transponation(int **, int );
void print_matrix(int **, int , int );

/*Константы*/
const int maxSizeString = 25;
const int pwdSizeMax = 1024;

int main(int argc, char const *argv[])
{
  /*Блок переменных*/
  FILE *matrixs = NULL;
  char *err_wd = NULL;
  char namePWD[maxSizeString];
  char nameFile[maxSizeString];
  char pwd[pwdSizeMax];
  char string[255];
  int **matrixOne = NULL;
  int **matrixTwo = NULL;
  int **matrixResOne = NULL;
  int **matrixResTwo = NULL;
  int sizeMatrix;
  int counter;
  int printer;
  int i, j;
  clock_t timeAlgoOne;
  clock_t timeAlgoTwo;
  /*Определяем путь от корня до исполняемого файла*/
  /*Файлы с примерами должны лежать в отдельной директории, в формате .txt.
  В файле первой строкой должен идти размер квадратной матрицы, пустая строка,
  первая матрица с разделитем между числами в виде пробела, пустая матрица,
  вторая матрица с разделитем между числами в виде пробела.*/
  printer = 0;
  err_wd = getcwd(pwd, pwdSizeMax);
	if(err_wd == NULL)
	{
		puts("Ошибка определения пути!");
		exit(-1);
	}
  sizeMatrix = 0;

  /*Определяем имя необходимого нам тестового файла.*/
  puts("Введите имя файла:");
  fgets(nameFile, maxSizeString, stdin);
  if(printer != 1)
  {
    puts("Вывод матриц отключен!");
    puts("Для отображения матриц перекомпелируйте файл с переменной printer = 1");
  }
  /*Пытаемся открыть файл с полученным именем находящемся в директории "examp-
  les".*/
  strncat(pwd, "/examples/", maxSizeString);
  strncat(pwd, nameFile, maxSizeString);
  pwd[strlen(pwd)-1] = '\0';
  matrixs = fopen(pwd, "r");
  if(matrixs == NULL)
  {
    printf("Файл с таким названием не найден!\n");
    exit(-1);
  }

  /*Считываем размер матрицы*/
  fscanf(matrixs, "%d", &sizeMatrix);
  /*Резервирование памяти под 3 матрицы - 2-ух полученных из файла и 1-ой полу-
  ченной во время умножения матриц*/
  matrixOne = (int **)malloc(sizeMatrix*sizeof(int *));
  matrixTwo = (int **)malloc(sizeMatrix*sizeof(int *));
  matrixResOne = (int **)malloc(sizeMatrix*sizeof(int *));
  matrixResTwo = (int **)malloc(sizeMatrix*sizeof(int *));
  for (counter = 0; counter < sizeMatrix; counter++)
  {
    matrixOne[counter] = (int *)malloc(sizeMatrix*sizeof(int));
    matrixTwo[counter] = (int *)malloc(sizeMatrix*sizeof(int));
    matrixResOne[counter] = (int *)malloc(sizeMatrix*sizeof(int));
    matrixResTwo[counter] = (int *)malloc(sizeMatrix*sizeof(int));
  }

  /*Считываем данные из файла*/
  for(i = 0; i < sizeMatrix; i++)
  {
    for(j = 0; j < sizeMatrix; j++)
    {
      fscanf(matrixs, "%d", &matrixOne[i][j]);
    }
  }
  if(printer == 1)
    printf("Первая матрица\n");
  print_matrix(matrixOne, sizeMatrix, printer);
  if(printer == 1)
    printf("\nВторая матрица\n");
  for(i = 0; i < sizeMatrix; i++)
  {
    for(j = 0; j < sizeMatrix; j++)
    {
      fscanf(matrixs, "%d", &matrixTwo[i][j]);
    }
  }
  print_matrix(matrixTwo, sizeMatrix, printer);

  /*Закрываем открытый файл и производим умножение матриц и находим время работы
  функций.*/
  fclose(matrixs);

  timeAlgoOne = clock();
  multiplication_matrix(matrixOne, matrixTwo, matrixResOne, sizeMatrix);
  timeAlgoOne = clock() - timeAlgoOne;
  if(printer == 1)
    printf("\nПроизведение матриц\n");
  print_matrix(matrixResOne, sizeMatrix, printer);
  printf("\nВремя работы алгоритма №1: %f\n", (double)timeAlgoOne/CLOCKS_PER_SEC);

  timeAlgoTwo = clock();
  multiplication_matrix_trans(matrixOne, matrixTwo, matrixResTwo, sizeMatrix);
  timeAlgoTwo = clock() - timeAlgoTwo;
  if(printer == 1)
    printf("\nПроизведение матриц\n");
  print_matrix(matrixResTwo, sizeMatrix, printer);
  printf("\nВремя работы алгоритма №2: %f\n", (double)timeAlgoTwo/CLOCKS_PER_SEC);
  /*Высвобождение памяти*/
  free_matrix(matrixOne, matrixTwo, matrixResOne, matrixResTwo, sizeMatrix);
}

/*Тела прототипов.*/

/*Функция освобождает память из под 4 массивов матриц. Функция необходима для
освобождения памяти в штатной и внештатной ситуациях, и для сокращения кода.
Входные параметры:
  int **one, **two, **resOne, **resTwo - указатели на матрицы
  int size - размеры массивов
Выходные параметры:
  отсутсвуют*/
void free_matrix(int **one, int **two, int **resOne, int **resTwo, int size)
{
  int counter;
  for (counter = 0; counter < size; counter++)
  {
    free(one[counter]);
    free(two[counter]);
    free(resOne[counter]);
    free(resTwo[counter]);
  }
  free(one);
  free(two);
  free(resOne);
  free(resTwo);
}

/*Функция находит произведение двух матриц.
Входные параметры:
  int **one, **two - указатели на массивы с матрицами
  int **res - указатель на массив в который будет записан результат умножения
  int size - размеры массивов
Выходные переменные:
  отсутсвуют
  int **res - неявный (изменяет в функции данные из main)*/
void multiplication_matrix(int **one, int **two, int **res, int size)
{
  int i, j, k;

  for (i = 0; i < size; i++)
  {
    for(j = 0; j < size; j++)
    {
      res[i][j] = 0;
      for(k = 0; k < size; k++)
      {
        res[i][j] += one[i][k]*two[k][j];
      }
    }
  }
}

/*Функция находит произведение двух матриц с использованием транспонирования.
Входные параметры:
  int **one, **two - указатели на массивы с матрицами
  int **res - указатель на массив в который будет записан результат умножения
  int size - размеры массивов
Выходные переменные:
  отсутсвуют
  int **res - неявный (изменяет в функции данные из main)*/
void multiplication_matrix_trans(int **one, int **two, int **res, int size)
{
  int i, j, k;

  matrix_transponation(two, size);
  for (i = 0; i < size; i++)
  {
    for(j = 0; j < size; j++)
    {
      res[i][j] = 0;
      for(k = 0; k < size; k++)
      {
        res[i][j] += one[i][k]*two[j][k];
      }
    }
  }
}

/*Функци тарнспонирования матрицы
Входные параметры:
  int **one - указатель на массив
  int size - размеры массива
Выходные данные:
отсутсвуют
int **matrix - неявный (изменяет в функции данные из main)*/
void matrix_transponation(int **matrix, int size)
{
  int i, j;
  int time_var;

  for (i = 0; i < size; i++)
  {
    for(j = i; j < size; j++)
    {
      time_var = matrix[i][j];
      matrix[i][j] = matrix[j][i];
      matrix[j][i] = time_var;
    }
  }
}

/*Функция отображает матрицы, если поднят специальный флаг
Входные параметры:
  int **matrix - указатель на массив
  int size - размеры массива
  int printer - флаг, указывающий на необходиость печати*/
void print_matrix(int **matrix, int size, int printer)
{
  int i, j;
  if(printer == 1)
  {
    for(i = 0; i < size; i++)
    {
      for(j = 0; j < size; j++)
      {
        printf("%d ", matrix[i][j]);
      }
      printf("\n");
    }
  }
}
