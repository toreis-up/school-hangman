#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <termio.h>

#define ALPHABET_COUNT 26
#define MAX_TRY 5

struct GAME_DATA
{
  char *correctWord;
  int alphabets[ALPHABET_COUNT];
  int tryCount;
};

void StartGame();
struct GAME_DATA *initializeGame();
int MainLoop(struct GAME_DATA *gameData);
char *LoadWord();
int *CopyIntArray(int *src, int length);
bool IsAlreadyUsed(int *flags, char ch);
bool IsContain(char *str, char ch);
bool IsAllHit(char * str, int *flags);
char *GetUsedChars(int *flags);
char GetChar(void);
char *BuildString(char *base, int *flags);
int Length(char *string);

int main(void)
{
  StartGame();
}

void StartGame()
{
  struct GAME_DATA *gameData = initializeGame();
  while(true) {
    bool exit = MainLoop(gameData);
    if (exit == 1) {
      break;
    }
  }
}

char *LoadWord() {
  FILE *fp;

  fp = fopen("toeic1500_utf.dat", "r");

  if (fp == NULL) {
    printf("File is corrupt.\n");
    fclose(fp);
    return NULL;
  }

  fseek(fp, 0L, SEEK_END);
  int length = ftell(fp);
  
  int position = rand() % length;
  char myStr[200];

  fseek(fp, position, SEEK_SET);
  fread(myStr, 200, 1,fp);

  printf("%s\n", myStr);

  char *word = (char *)malloc(sizeof(char) * 100);
  bool inLine = false;
  bool inWord = false;
  int c = 0;

  for (int i = 0; i < 100; i++) {
    if (!inLine) {
      if (myStr[i] == '\n') {
        inLine = true;
      }
    }
    else {
      if (!inWord) {
        if (myStr[i] == ' ') {
          inWord = true;
        }
      }
      else {
        if (myStr[i] == ' ') {
          word[c] = '\0';
          break;
        }
        else {
          word[c] = myStr[i];
          c++;
        }
      }
    }
  }
  if (fp == NULL)
    fclose(fp);
  return word;
}

char *GetUsedChars(int *flags) {
  char * str = (char *)malloc(sizeof(char) * ALPHABET_COUNT);
  int c = 0;

  for (int i = 0; i < ALPHABET_COUNT; i++) {
    if (flags[i] == 1) {
      str[c] = 'a' + i;
      c++;
    }
  }

  return str;
}

int MainLoop(struct GAME_DATA *gameData)
{
  char *baseString = gameData->correctWord;
  int *alphabetFlags = gameData->alphabets;
  char *str = BuildString(baseString, alphabetFlags);
  char *usedChars = GetUsedChars(alphabetFlags);
  printf("単語: %s\n", str);
  printf("使われた文字: %s\n", usedChars);
  printf("試行回数: %d / %d\n", gameData->tryCount, MAX_TRY);
  char input = GetChar();

  bool isAlreadyUsed = IsAlreadyUsed(alphabetFlags, input);
  if (isAlreadyUsed)
  {
    printf("This word is already used.\n");
    return 0;
  }
  alphabetFlags[input - 'a'] = 1;

  bool isAllHit = IsAllHit(baseString, alphabetFlags);
  
  if (isAllHit) {
    printf("Correct!!\n\n");
    return 1;
  }

  bool isNotContain = !IsContain(baseString, input);
  if (isNotContain)
  {
    printf(":(\n");

    gameData->tryCount += 1;

    if (gameData->tryCount >= MAX_TRY) {
      printf("%d times missed!!\n", MAX_TRY, gameData->tryCount);
      return 1;
    }
  }

  printf("\n");
  return 0;
}

bool IsAllHit(char * str, int * flags) {
  for (int i = 0; i < Length(str); i++) {
    if (flags[str[i] - 'a'] != 1) {
      return false;
    }
  }
  return true;
}

struct GAME_DATA *initializeGame()
{
  srand((unsigned int)time(NULL));
  struct GAME_DATA *gameData = (struct GAME_DATA *)malloc(sizeof(struct GAME_DATA));

  memset(gameData->alphabets, 0, ALPHABET_COUNT);
  gameData->correctWord = LoadWord();
  gameData->tryCount = 0;
  return gameData;
}

char *BuildString(char *base, int *flags)
{
  char *builtString = (char *)malloc(sizeof(char) * Length(base));
  for (int i = 0; i < Length(base); i++)
  {
    if (flags[base[i] - 'a'] == 1)
    {
      builtString[i] = base[i];
    }
    else
    {
      builtString[i] = '_';
    }
  }
  return builtString;
}

int Length(char *string)
{
  int count = 0;
  while (true)
  {
    if (string[count] == '\0')
    {
      break;
    }
    count++;
  }

  return count;
}

bool IsContain(char *str, char ch)
{
  bool contain = false;
  for (int i = 0; i < Length(str); i++)
  {
    if (str[i] == ch)
    {
      contain = true;
      break;
    }
  }
  return contain;
}

bool IsAlreadyUsed(int *flags, char ch)
{
  int alphabetIndex = ch - 'a';
  if (flags[alphabetIndex] == 1)
  {
    return true;
  }
  return false;
}

int *CopyIntArray(int *src, int length)
{
  int *dup = (int *)malloc(sizeof(int) * length);
  for (int i = 0; i < length; i++)
  {
    dup[i] = src[i];
  }
  return dup;
}

// COPIED CODE
char GetChar(void)
{
  struct termio old_term, new_term;

  char c;

  /* 現在の設定を得る */
  ioctl(0, TCGETA, &old_term);

  /* 設定のコピーをつくる */
  new_term = old_term;

  /* 入力文字のエコーを抑止する場合 */
  new_term.c_lflag &= ~(ICANON | ECHO);

  /* エコーは止めない場合 */
  // new_term.c_lflag &= ~(ICANON);

  /* 新しい設定を反映する */
  ioctl(0, TCSETAW, &new_term);

  /* 1 文字入力 */
  c = getchar();

  /* 古い設定に戻す */
  ioctl(0, TCSETAW, &old_term);

  return (c);
}
