#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void startGame();
struct GAME_DATA* initializeGame();
void buildString(char * base, int flags[]);

struct GAME_DATA {
  char * correctWord;
  int alphabets[26];
  int tryCount;
};

int main(void) {
  printf("Hello, World");
  struct GAME_DATA* gameData = initializeGame();
  startGame();
}

void startGame() {
  printf("Hello, Game");
}

struct GAME_DATA* initializeGame() {
  srand((unsigned int)time(NULL));
  struct GAME_DATA* gameData = (struct GAME_DATA*)malloc(sizeof(struct GAME_DATA));
  gameData->alphabets = {0};
  gameData->correctWord = "spreadsheet";
  gameData->tryCount = 0;
  return gameData;
}