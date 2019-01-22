//
// @file : snake.c
// @author : 2012104208 Jongmin Yun
// @datetime : 2019-01-22 23:28
// @description : 
// 
// To make application, do commend like this.
// >> make snake
// or
// >> make all
//
// Keyboard usage ::
//    /- Up
//    W  /- Right
// A  S  D
// \  \- Down
//  \- Left 
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

#include "SetTicker.h"
#include "HelperMacro.h"
#include "HelperGlobal.h"
#include "HelperCurses.h"

int gFps = 60;
int gTickDelay = 32;
const char* sStrBlank  = " ";
const char* sStrPlayer = "O";
const char* sStrApple  = "+";

// @struct Area
// @brief Store information for specified area.
struct Area { int x, y, w, h; };
// @brief The region of console.
struct Area gWindowArea = {0, 0, 0, 0};

// @brief Input callback for SIGIO.
void OnInput(int);
// @brief Callback for `ctrl+C`.
// when signal is interrupted, restore previous setting of terminal console,
// and exit program.  
// iSig argument will not be used.
void CallbackSIGINT(int iSig);
// @brief Callback for alarm, display screen each given delay time.
// iSig will not be used.
void CbGameLoop(int iSig);
// @brief Setup program's properties.
void SetupProgram();
// @brief Shutdown program's properties.
void ShutdownProgram();
// @brief Check [x, y] is out of bound of [x, y, w, h] of iArea.
// If collided, return 1 or 0.
int IsOutOfBoundary(const struct Area* iArea, int x, int y);

// @struct Point2D
struct Point2D { int x, y; };
// @brief Check point2d is same.
BOOL Point2D_IsSame(struct Point2D* iLhs, struct Point2D* iRhs)
{
  if (iLhs->x == iRhs->x && iLhs->y == iRhs->y) { return TRUE; }
  else { return FALSE; }
}
// @brief Move add.
void Point2D_Move(struct Point2D* iPoint, int x, int y)
{
  iPoint->x += x;
  iPoint->y += y;
}

// @struct PlayerBody
// @brief each player body.
struct PlayerBody
{
  struct Point2D     mPos;
  struct PlayerBody* mNext;
};
// @enum EMovement
// @brief Specifies player's movement direction.
enum EMovement { EMove_Left = 0, EMove_Up = 1, EMove_Right = 2, EMove_Down = 3 };
int movement[4][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

// @struct PlayerController
// @brief Player controller.
struct PlayerController 
{
  struct PlayerBody* mBodyHeader; 
  enum EMovement     movementStatus;
  struct Point2D     mRemoveDrawPos;
} gPlayer = {NULL, EMove_Right}; 
// @brief Move player following `gPlayer.movementStatus`.
void PlayerController_Move()
{
  if (gPlayer.mBodyHeader == NULL) { return; }

  struct Point2D header;
  struct Point2D tail;
  
  // Move head first.
  int* movementAmnt = movement[gPlayer.movementStatus];

  header = gPlayer.mBodyHeader->mPos;
  gPlayer.mRemoveDrawPos.x = header.x;
  gPlayer.mRemoveDrawPos.y = header.y;
  Point2D_Move(&gPlayer.mBodyHeader->mPos, movementAmnt[0], movementAmnt[1]);

  // And move tails.
  struct PlayerBody* ptrBody = gPlayer.mBodyHeader->mNext;
  while (ptrBody != NULL)
  {
    tail = ptrBody->mPos;
    gPlayer.mRemoveDrawPos.x = tail.x;
    gPlayer.mRemoveDrawPos.y = tail.y;

    struct Point2D moveAmount = {header.x - tail.x, header.y - tail.y};
    header = ptrBody->mPos;

    // Move tail
    Point2D_Move(&ptrBody->mPos, moveAmount.x, moveAmount.y);
    ptrBody = ptrBody->mNext;
  };
};
// @brief Create body. If not exist any body, just create body with center point.
void PlayerController_CreateBody()
{
  if (gPlayer.mBodyHeader == NULL)
  { // If body-header is not exist at all, just create body with center point.
    struct Point2D pos = {
        (gWindowArea.w + gWindowArea.x * 2) / 2,
        (gWindowArea.h + gWindowArea.y * 2) / 2
    };
    gPlayer.mBodyHeader = (struct PlayerBody*)malloc(sizeof(struct PlayerBody));
    gPlayer.mBodyHeader->mNext = NULL;
    gPlayer.mBodyHeader->mPos  = pos;
  }
  else
  { // If body-header is exist, traverse until tail, and create new body.
    struct PlayerBody* ptrBody = gPlayer.mBodyHeader;

    while (ptrBody != NULL)
    {
      struct PlayerBody* deleteinput = ptrBody;
      ptrBody = deleteinput->mNext;
    };
  }
}
// @brief Clear player controller's bodies.
void PlayerController_Clear()
{
  struct PlayerBody* ptrBody = gPlayer.mBodyHeader;
  while (ptrBody != NULL)
  {
    struct PlayerBody* deleteinput = ptrBody;
    ptrBody = deleteinput->mNext;

    free(deleteinput);
  };
  
  gPlayer.mBodyHeader = NULL;
}
// @brief Check collision.
BOOL PlayerController_CheckCollision(struct Point2D* iPoint)
{
  struct PlayerBody* ptrBody = gPlayer.mBodyHeader;
  while (ptrBody != NULL)
  {
    if (Point2D_IsSame(&ptrBody->mPos, iPoint) == TRUE) { return TRUE; }
    ptrBody = ptrBody->mNext;
  }; 
  return FALSE;
}
// @brief Check oob.
BOOL PlayerController_CheckOOB()
{
  if (gPlayer.mBodyHeader == NULL) { return FALSE; }
  else
  {
    struct PlayerBody* ptrBody = gPlayer.mBodyHeader;
    while (ptrBody != NULL)
    {
      if (IsOutOfBoundary(
          &gWindowArea, 
          ptrBody->mPos.x, ptrBody->mPos.y) == TRUE) { return TRUE; }

      ptrBody = ptrBody->mNext;
    }; 
  }
  return FALSE;
}
void PlayerController_DrawBodies()
{
  struct PlayerBody* ptrBody = gPlayer.mBodyHeader;
  while (ptrBody != NULL)
  {
    curse.SetCursor(ptrBody->mPos.x, ptrBody->mPos.y);
    curse.Write(sStrPlayer);

    ptrBody = ptrBody->mNext;
  };  

  curse.SetCursor(gPlayer.mRemoveDrawPos.x, gPlayer.mRemoveDrawPos.y);
  curse.Write(sStrBlank);
}

// @enum GameStatus
// @brief Game status of snake game.
enum GameStatus
{
  EGame_Start, EGame_Playing, EGame_Result, EGame_Shutdown,
} gGameStatus;

// @struct KeyInput
// @brief Stores key input value from AIO.
struct KeyInput 
{ 
  unsigned char    mKey; 
  struct KeyInput* mNextKey;
};
// @struct KeyInputList
// @brief input list of key on each frame to process.
struct KeyInputList
{
  struct KeyInput* mInputList;
} gKeyInputList = {NULL};

// @brief Clear key inputted key list.
void KeyInputList_Clear()
{
  struct KeyInput* ptrinput = gKeyInputList.mInputList;
  while (ptrinput != NULL)
  {
    struct KeyInput* deleteinput = ptrinput;
    ptrinput = deleteinput->mNextKey;
    free(deleteinput);
  };

  gKeyInputList.mInputList = NULL;
};
// @brief Check inputed key is already exist in list.
BOOL KeyInputList_IsKeyAlreadyExist(unsigned char iKey)
{
  struct KeyInput* ptrInput = gKeyInputList.mInputList;
  while (ptrInput != NULL)
  {
    if (ptrInput->mKey == iKey) { return TRUE; }
    ptrInput = ptrInput->mNextKey;
  };

  return FALSE;
}
// @brief Insert key into KeyInputList.
void KeyInputList_InsertKey(unsigned char iKey)
{
  if (gKeyInputList.mInputList == NULL)
  {
    gKeyInputList.mInputList = (struct KeyInput*)malloc(sizeof(struct KeyInput));
    gKeyInputList.mInputList->mKey = iKey;
    gKeyInputList.mInputList->mNextKey = NULL;
    return;
  }
  else
  {
    struct KeyInput* ptrInput = gKeyInputList.mInputList;
    while (ptrInput != NULL)
    {
      if (ptrInput->mNextKey == NULL)
      { // If next memory space is empty, create new key item.
        ptrInput->mNextKey = (struct KeyInput*)malloc(sizeof(struct KeyInput));
        ptrInput->mNextKey->mKey = iKey;
        ptrInput->mNextKey->mNextKey = NULL;
      }
      else { ptrInput = ptrInput->mNextKey; }
    }; 
  }
};

// @struct Apple
// @brief Apple instance.
struct Apple
{
  struct Point2D mPos;
} gApple;
// @struct Realign position of apple with given iArea.
void Apple_AlignPosition(const struct Area* iArea)
{
  static BOOL isInitialized = FALSE;
  if (isInitialized == FALSE)
  {
    srand(time(0));
    isInitialized = TRUE;
  }

  const int x = iArea->x;
  const int y = iArea->y;
  const int w = iArea->w - 1;
  const int h = iArea->h;

  gApple.mPos.x = rand() % w + x; 
  gApple.mPos.y = rand() % h + y; 
};
// @brief Clear apple drawing position and realign apple.
void Apple_Reposition()
{
  curse.SetCursor(gApple.mPos.x, gApple.mPos.y);
  curse.Write(sStrBlank);
  Apple_AlignPosition(&gWindowArea);
}
// @brief Draw apple.
void Apple_Draw()
{
  curse.SetCursor(gApple.mPos.x, gApple.mPos.y);
  curse.Write(sStrApple);
}

// @struct GlobalStatus
// @brief Global status of each game.
struct GlobalStatus
{
  unsigned mScore;
} gGlobalStatus = {0};
// @brief Add score.
void GlobalStatus_AddScore(unsigned iScore)
{
  gGlobalStatus.mScore += iScore;
};

//!
//! Main function
//!

int main(int argc, char** argv)
{
	NOTUSE(argc); NOTUSE(argv);
  SetupProgram();

  while (gGameStatus != EGame_Shutdown)
  {
    pause();
  }

  ShutdownProgram();

  printf("\n");
  for (int i = 0; i < 80; ++i) { printf("-"); } printf("\n");
  printf("Game End. Score : %u\n", gGlobalStatus.mScore);
  for (int i = 0; i < 80; ++i) { printf("-"); } printf("\n");

	return 0;
}

void SetupProgram()
{
	// Set callback of each signal.
	global.BindSIGINT(CallbackSIGINT);
	global.BindSIGALRM(CbGameLoop);
  global.BindSIGIO(OnInput);

	// Store setting, and initialize curses. & initialize screen size.
	// and, set delay for display function.
	global.SetTcProperty(1);
  global.SetEcho(0);
  global.SetCanonical(0);
  global.SetAsyncIO(1);
	global.GetScreenSize(&gWindowArea.w, &gWindowArea.h);

	// Set initial parameter.
  gGameStatus = EGame_Start;
  PlayerController_CreateBody();
  Apple_AlignPosition(&gWindowArea);

	// Initialize curses
	curse.Initialize();
	curse.Clear();
  // Set game loop delay.
	SetTicker(gTickDelay);
}

void ShutdownProgram()
{
	// Release curses instance and restore stored properties.
  PlayerController_Clear();
	curse.SwapBuffer();
	curse.Release();
	global.SetTcProperty(0);
}

void CallbackSIGINT(int iSig)
{
	NOTUSE(iSig);
	global.SetTcProperty(0);
	exit(1);
}

void EGame_Playing_Input();
void EGame_Playing_Update();
void EGame_Playing_Draw();

void CbGameLoop(int iSig)
{
	NOTUSE(iSig);
	signal(SIGALRM, SIG_IGN);

  switch (gGameStatus)
  {
  case EGame_Start: 
  { 
    gGameStatus = EGame_Playing; 
    signal(SIGALRM, CbGameLoop);
  } break;
  case EGame_Playing:
  {
    EGame_Playing_Input();
    KeyInputList_Clear();

    EGame_Playing_Update();

    EGame_Playing_Draw();
    curse.SwapBuffer();
    signal(SIGALRM, CbGameLoop);
  } break;
  case EGame_Result: { } break;
  case EGame_Shutdown:
  {
    SetTicker(0);
  } break;
  default: break;
  }
}

void EGame_Playing_Input()
{
  if (KeyInputList_IsKeyAlreadyExist('q') == TRUE)
  {
    gGameStatus = EGame_Shutdown; return;
  }
  else if (KeyInputList_IsKeyAlreadyExist('a') == TRUE)
  {
    gPlayer.movementStatus = EMove_Left; return;
  }
  else if (KeyInputList_IsKeyAlreadyExist('w') == TRUE)
  {
    gPlayer.movementStatus = EMove_Up; return;
  }
  else if (KeyInputList_IsKeyAlreadyExist('d') == TRUE)
  {
    gPlayer.movementStatus = EMove_Right; return;
  }
  else if (KeyInputList_IsKeyAlreadyExist('s') == TRUE)
  {
    gPlayer.movementStatus = EMove_Down; return;
  }
}

void EGame_Playing_Update()
{
  // Move snake
  PlayerController_Move();
  
  // Check apple is collided with snake.
  
  // Check snake is collided with snake.
  if (PlayerController_CheckOOB() == TRUE)
  {
    gGameStatus = EGame_Shutdown;
    return;
  }
}

void EGame_Playing_Draw()
{
  PlayerController_DrawBodies();
}

int IsOutOfBoundary(const struct Area* iArea, int x, int y)
{
  M_ASSERT(iArea != NULL, "Must be valid iArea");
  const int w = iArea->w + iArea->x;
  const int h = iArea->h + iArea->y;

	// width - 1 is needed to avoiding sticking out from right to left 0 col.
	if (x < iArea->x || x >= w-1 || y < iArea->y || y >= h) { return 1; }
	else { return 0; }
}

void OnInput(int iSig)
{
  NOTUSE(iSig);
  int c = getchar(); tolower(c);

  // Check inputted key is already in list.
  BOOL flag = KeyInputList_IsKeyAlreadyExist((unsigned char)c);
  // If not, insert key with new inputted key `c`.
  if (flag == FALSE)
  {
    KeyInputList_InsertKey((unsigned char)c);
  }
}
