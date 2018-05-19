#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 500 // 单行的最大字符数
#define MAX_LINE 200000 // 输入的最大行数

char dict[MAX_LINE][MAX_LEN] = {{0}}; // 保存有序字典
char text[MAX_LINE][MAX_LEN] = {{0}}; // 保存测试样例单词
char output[MAX_LINE][MAX_LEN] = {{0}};// 保存测试样例中不在字典的单词
int num_d = 0; // 字典中单词数
int num_t = 0; // 测试样例中单词数
int num_o = 0; // 测试样例中不在字典的单词数

/**************************************
 * 不在字典里面返回0，在字典里则返回1
 *************************************/
char check(char * str)
{
  // 将实参转换成全小写单词
  char str1[MAX_LEN] = {0};
  unsigned int i = 0;
  for (i = 0; i < strlen(str); i++)
  {
    if (str[i] >= 'A' && str[i] <= 'Z')
    {
      str1[i] = str[i] + ('a' - 'A');
    }
    else
    {
      str1[i] = str[i];
    }
  }
  str1[i] = '\0';
  char flag = 0; // 是否在字典里面的标志
  char str2[MAX_LEN] = {0}; // 将字典中的某一单词转换成全小写单词
  for (int i = 0; i < num_d; i++)
  {
    unsigned int j = 0;
    for (j = 0; j < strlen(dict[i]); j++)
    {
      if (dict[i][j] >= 'A' && dict[i][j] <= 'Z')
      {
        str2[j] = dict[i][j] + ('a' - 'A');
      }
      else
      {
        str2[j] = dict[i][j];
      }
    }
    str2[j] = '\0';
    // 比较这2个全小写单词
    if (strcmp(str1, str2) == 0)
    {
      flag = 1;
      break;
    }
    else
    {
      flag = 0;
    }
  }
  return flag;
}

/**************************************
 * 输出并保存不在字典中的单词
 *************************************/
void isWrongWord()
{
  for (int i = 0; i < num_t; i++)
  {
    if( !check(text[i]) )
    {
      printf("%s\n", text[i]);
      strcpy(output[num_o], text[i]);
      num_o++;
    }
  }
  return;
}

/**************************************
 * 读取字典
 *************************************/
int readDict(char *fn)
{
  FILE *fp;
  int count = 0;
  // 打开文件
  if ((fp = fopen(fn, "r")) == NULL)
  {
    printf("open file error!\n");
    return -1;
  }
  while(1)
  {
    // 读取一行到buf，每次调用文件指针fp会自动后移一行
    if(fgets(dict[count], MAX_LEN, fp) == NULL) // 文件读取结束则跳出循环
    {
      break;
    }
    dict[count][strlen(dict[count]) - 1] = '\0'; // 消除末尾的换行符
    count ++;
  }
  return count;
}

/**************************************
 * 读取测试样例
 *************************************/
int readtext(char *fn)
{
  FILE *fp;
  int count = 0;
  // 打开文件
  if ((fp = fopen(fn, "r")) == NULL)
  {
    printf("open file error!\n");
    return -1;
  }
  char row[MAX_LEN]; // 临时存储1行内容
  while(1)
  {
    // 当gets返回NULL时代表遇到EOF,结束输入
    if(fgets(row, MAX_LEN, fp) == NULL)
    {
      break;
    }
    unsigned int isInWord = 0; // 当前字符是否是在一个单词当中
    for (unsigned int i = 0; i < strlen(row); i++)
    {
      if(isalpha(row[i]) != 0 || row[i] == '\'')
      {
        text[count][isInWord++] = row[i];
      }
      else if(isInWord > 0)
      {
        count ++;
        isInWord = 0;
      }
      else
      {
        ;
      }
    }
  }
  return count;
}

/**************************************
 * 主函数
 *************************************/
int main()
{
  char dictionaryFilename[100] = "dictionary.txt";
  num_d = readDict(dictionaryFilename);
  char textFilename[100] = "carroll.txt";
  num_t = readtext(textFilename);
  isWrongWord();
  printf("Words Misspelled: %d\n", num_o); // 输出统计结果
}
