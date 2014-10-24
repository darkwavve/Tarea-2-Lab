#include<SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <list>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*game_backgrounds, *pelotita_buena, *pelotita_mala,*game_over;
SDL_Rect rect_background, rect_pelotita;

void loopMenu();

void loopGame_Over()
{
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0) ;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_RETURN)
                    loopMenu();
            }
        }
        SDL_RenderCopy(renderer, game_over, NULL, &rect_background);
        SDL_RenderPresent(renderer);
    }
}

void loopJuego(int mode)
{
    list<float> pelotitas_x;
    pelotitas_x.push_back(0);
    pelotitas_x.push_back(90);
    pelotitas_x.push_back(180);
    pelotitas_x.push_back(270);
    float FPS = 0;
    int cont = 0;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }

            if(Event.type == SDL_MOUSEBUTTONDOWN)
            {
                int click_x,click_y;
                SDL_GetMouseState(&click_x,&click_y);
                cout<<click_x<<","<<click_y<<endl;

                for(list<float>::iterator i = pelotitas_x.begin();
                        i != pelotitas_x.end();
                        i++)
                {
                    if(click_x<66 + FPS &&click_x>0 + FPS && click_y<66 + *i&& click_y>0 + *i)
                    {
                        pelotitas_x.erase(i);
                        --i;
                        if(pelotitas_x.size() == 0)
                            loopGame_Over();
                        break;
                    }
                }
            }
        }

        SDL_RenderCopy(renderer, game_backgrounds, NULL, &rect_background);

    if(mode == 0)
    {
       if(cont == 0)
        {
        FPS += 1.5;
        rect_pelotita.x = FPS;
        }
        if(cont == 1)
        {
        FPS -= 1.5;
        rect_pelotita.x = FPS;
        }
        if(FPS >= rect_background.w-rect_pelotita.w)
            cont = 1;
        if(FPS <= rect_background.x)
            cont = 0;
    }

    if(mode == 1)
    {
       if(cont == 0)
        {
        FPS += 3;
        rect_pelotita.x = FPS;
        }
        if(cont == 1)
        {
        FPS -= 3;
        rect_pelotita.x = FPS;
        }
        if(FPS >= rect_background.w-rect_pelotita.w)
            cont = 1;
        if(FPS <= rect_background.x)
            cont = 0;
    }

        for(list<float>::iterator i = pelotitas_x.begin();
                i != pelotitas_x.end();
                i++)
        {
            rect_pelotita.y = (*i);
            SDL_RenderCopy(renderer, pelotita_mala, NULL, &rect_pelotita);
        }
        SDL_RenderPresent(renderer);
    }
}

void loopMenu()
{
    int mode = 0;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0) ;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_a)
                    mode = 0;
                if(Event.key.keysym.sym == SDLK_s)
                    mode = 1;
                if(Event.key.keysym.sym == SDLK_RETURN)
                    loopJuego(mode);
            }
        }
        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        SDL_RenderPresent(renderer);
    }
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Ball Bouncer", 100, 100, 700/*WIDTH*/, 350/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"assets/menu.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;

    pelotita_mala = IMG_LoadTexture (renderer,"assets/pelotita_mala.png");
    pelotita_buena = IMG_LoadTexture (renderer,"assets/pelotita_buena.png");
    game_backgrounds = IMG_LoadTexture(renderer,"assets/background.png");
    game_over = IMG_LoadTexture(renderer,"assets/game_over.png");

    SDL_QueryTexture(pelotita_buena,NULL,NULL,&rect_pelotita.w,&rect_pelotita.h);
    rect_pelotita.x = 0;
    rect_pelotita.y = 0;

    loopMenu();

    return 0;
}
