#include "game.h"

Game::Game():FramePS(60), ScreenW(640), ScreenH(480), LevelW(1680), LevelH(1050)
{
 TICK_INTERVAL = 1000 / FramePS;
}

Game::~Game()
{
 SDL_Quit();
}

void Game::Init(char* title)
{
 SDL_WM_SetCaption(title, NULL);
 SDL_ShowCursor(SDL_DISABLE);
}

void Game::Process()
{
 SDL_Event event;
 SDLKey k;
 int mousex, mousey;
 int vel = 5, xvel = 0, yvel = 0;


 Uint8* keystate;
 bool running = true;

 SDL_Rect camera = {200, 200, ScreenW, ScreenH};
 int cameraX = camera.x, cameraY = camera.y, cameraH = camera.h, cameraW = camera.w;

 Graphics bg;
 bg.SetSource(camera);
 bg.LoadImage("Images\\candles.jpg");

 enum {RIGHT, DOWN, LEFT, UP};

 Graphics arrows[4];
 char *arrowPath[] = {"Images\\rightarrow.png", "Images\\downarrow.png", "Images\\leftarrow.png", "Images\\uparrow.png"};
 int posx[] = {ScreenW - 20, ScreenW / 2 - 10, 0, ScreenW / 2 - 13};
 int posy[] = {ScreenH / 2 - 13, ScreenH - 26, ScreenH / 2 - 13, 0};
 short i;
 for(i = 0; i < 4; i++)
 {
    arrows[i].LoadImage(arrowPath[i]);
    arrows[i].MakeTransparent(0, 0, 0);
    arrows[i].SetXY(posx[i], posy[i]);
 }

 Graphics cursor;
 cursor.LoadImage("Images\\cursor.png");
 cursor.MakeTransparent(255, 0, 0);

 nextTime = SDL_GetTicks() + TICK_INTERVAL;
 while(running)
 {
    bg.DrawClip();
    cursor.Draw();
    if(SDL_PollEvent(&event))
    {
       switch(event.type)
        {
         case SDL_MOUSEMOTION:
            mousex = event.motion.x;
            mousey = event.motion.y;

            cursor.SetXY(mousex, mousey);

            if(mousex + 1 >= ScreenW)
                xvel = vel;
            else if(mousex <= 0)
                xvel = -vel;
            else
                xvel = 0;

            if(mousey + 1 >= ScreenH)
                yvel = vel;
            else if(mousey <= 0)
                yvel = -vel;
            else
                yvel = 0;
         break;

         ///scrolling using keyboard
         case SDL_KEYDOWN:
            k=event.key.keysym.sym;
            if(k == SDLK_DOWN)
                yvel = vel;
            else if(k == SDLK_UP)
                yvel = -vel;
            else if(k == SDLK_RIGHT)
                xvel = vel;
            else if(k == SDLK_LEFT)
                xvel = -vel;
         break;

         case SDL_KEYUP:
            k=event.key.keysym.sym;
            if(k == SDLK_DOWN || k == SDLK_UP)
                yvel = 0;
            else if(k == SDLK_RIGHT || k == SDLK_LEFT)
                xvel = 0;
         break;

         case SDL_QUIT:
          running = false;
          continue;
         break;
        }
     }


    if(!(xvel == 0 && yvel == 0))
    {
         cameraX += xvel;
         cameraY += yvel;

         if(cameraX <= 0)
            cameraX = 0;
         else if(cameraX + cameraW >= LevelW)
            cameraX = LevelW - cameraW;

         if(cameraY <= 0)
            cameraY = 0;
         else if(cameraY + cameraH >= LevelH)
            cameraY = LevelH - cameraH;

         camera.x = cameraX;
         camera.y = cameraY;

         bg.SetSource(camera);

         //showing the arrows
         if(xvel > 0) //moving right
            arrows[RIGHT].Draw();
         else if(xvel < 0)
            arrows[LEFT].Draw();

         if(yvel > 0) //moving down
            arrows[DOWN].Draw();
         else if(yvel < 0)
            arrows[UP].Draw();
     }

    window.Flip();

    SDL_PumpEvents();
    keystate = SDL_GetKeyState(NULL);
    if(keystate[SDLK_ESCAPE])
    {
       running = false;
       continue;
    }

    SDL_Delay(TimeLeft());
    nextTime += TICK_INTERVAL;
 }

}

Uint32 Game::TimeLeft()
{
  Uint32 now;

  now = SDL_GetTicks();
  if(nextTime <= now)
      return 0;
  else
      return nextTime - now;
}
