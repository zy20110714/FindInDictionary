#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 500 // 单行的最大字符数
#define MAX_LINE 200000 // 输入的最大行数

char dict[MAX_LINE][MAX_LEN] = {{0}}; // 保存有序字典
char text[MAX_LINE][MAX_LEN] = {{0}}; // 保存测试样例单词
char output[MAX_LINE][MAX_LEN] = {{0}};// 保存测试样例中不在字典的单词
int blockkey[704] = {0};//开辟缓存来存储每个区块的起始编号
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
  int textkey = 0;
  int keybegin = 0;
  int keyend = 0;
  int keynext = 0;
  unsigned int i = 0;
  unsigned int j = 0;
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
  // 若单词只有一个字母或者有多个字母但第二个字母为单引号时，则认为key值为x1
  if(str1[1] == 0 || str1[1] == '\'')
  {
    textkey = (str1[0] - 'a' + 1) * 26;
  }
  // 多个字母且第二个字母不为单引号时则key值为x1 * 26 + x2（x1为单词中第一个小写字母对应的数字，如a对应1；x2对应第二个小写字母）
  else
  {
    textkey = (str1[0] - 'a' + 1) * 26 + (str1[1] - 'a' + 1);
  }
  str1[i] = '\0';
  char flag = 0; // 是否在字典里面的标志
  if((blockkey[textkey] == 0) && (textkey > 26))
  {
    flag = 0;
    return flag;
  }
  else
  {
    keybegin = blockkey[textkey]; // 确定开始进行查找时的单词序号
  }
  keynext = textkey + 1;
  while(blockkey[keynext] == 0)
  {
    keynext++;
  }
  keyend = blockkey[keynext]; // 确定结束查找时的单词序号
  int ii = 0;
  for (ii = keybegin; ii < keyend; ii++) // 在此区间进行查找
  {
    char str2[MAX_LEN] = {0};
    for (j = 0; j < strlen(dict[ii]); j++)
    {
      str2[j] = dict[ii][j];
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
  unsigned int wordlen = 0;
  int count = 0;
  int dictkey = 0;
  // 打开文件
  if ((fp = fopen(fn, "r")) == NULL)
  {
    printf("open readDict file error!\n");
    return -1;
  }
  while(1)
  {
    // 读取一行到buf，每次调用文件指针fp会自动后移一行
    if(fgets(dict[count], MAX_LEN, fp) == NULL) // 文件读取结束则跳出循环
    {
      break;
    }
    wordlen = strlen(dict[count]);
    dict[count][wordlen - 1] = 0;//为方便计算key值，单词末尾暂时赋值数字0
    if(dict[count][1] == 0 || dict[count][1] == '\'')//若单词只有一个字母或者有多个字母但第二个字母为单引号时，则认为key值为x1
    {
      dictkey = (dict[count][0] - 'a' + 1) * 26;
    }
    else//多个字母且第二个字母不为单引号时则key值为x1 * 26 + x2（x1为单词中第一个小写字母对应的数字，如a对应1；x2对应第二个小写字母）
    {
      dictkey = (dict[count][0] - 'a' + 1) * 26 + (dict[count][1] - 'a' + 1);
    }
    if(blockkey[dictkey] == 0)//当缓存区当前key值所指缓存没有记录单词序号时，才进行记录
    {
      blockkey[dictkey] = count;
    }
    dict[count][wordlen - 1] = '\0'; // 消除末尾的换行符
    count ++;
  }
  blockkey[703] = count - 1;
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
    printf("open readtext file error!\n");
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
      // 某些用例存在带引号的英文对话，如'How are you?' 为防止将单词识别为'how  故加以判断
      if(row[i] == '\'' && isInWord == 0)
      {
        continue;
      }
      else if(isalpha(row[i]) != 0 || row[i] == '\'')
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
        ; // 无操作
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
  char textFilename[100] = "birdman.txt";
  num_t = readtext(textFilename);
  isWrongWord();
  printf("Words Misspelled: %d\n", num_o); // 输出统计结果
  system("PAUSE");
  return 0;
}
