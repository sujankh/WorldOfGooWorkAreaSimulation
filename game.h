#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>

#include "screen.h"
extern Screen window;

#include "graphics.h"

class Game
{
 private:
   const int FramePS;
   const int ScreenW, ScreenH, LevelW, LevelH;
   int TICK_INTERVAL;
   Uint32 nextTime;

   /*UTILITIES*/
   Uint32 TimeLeft();

 public:
   Game();
   ~Game();
   void Init(char* title);
   void Process();
};
#endif
