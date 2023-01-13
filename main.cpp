#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "config.h"
#include "Target.h"
#include "Map.h"
#include "resources.h"

#include <iostream>
#include <string>

#include <stdlib.h>
#include <time.h>
#include <stdio.h>


char translate_input(SDL_Keysym in) {
  static char key_table[128] = {
    /*  0*/ 0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 32*/ ' ',0,0,0,0,0,0,
    /* 39*/ '\'',0,0,0,0,
    /* 44*/ ',','-','.','/','0','1','2','3','4','5','6','7','8','9',0,';',0,'=',0,0,0,
    /* 65*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 91*/ '[','\\',']',0,0,'`',
    /* 97*/ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    /*123*/ 0,0,0,0,0};
  static char shift_table[128] = {
    /*  0*/ 0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 32*/ ' ',0,0,0,0,0,0,
    /* 39*/ '\"',0,0,0,0,
    /* 44*/ '<','_','>','?',')','!','@','#','$','%','^','&','*','(',0,':',0,'+',0,0,0,
    /* 65*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* 91*/ '{','|' ,'}',0,0,'~',
    /* 97*/ 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    /*123*/ 0,0,0,0,0};
  if( in.sym >= 128 ) {
    return 0;
  }
  else if( in.mod & (KMOD_SHIFT | KMOD_CAPS) ) {
    return shift_table[in.sym];
  }
  else {
    return key_table[in.sym];
  }
}


void title(SDL_Renderer* renderer) {
  SDL_Surface* title_surface;
  SDL_Texture* title_texture;
  SDL_Surface* howto_surface;
  SDL_Texture* howto_texture;
  Mix_Music *bgm;
  SDL_Event event;
  int stage = 0;
  Uint32 start;

  if( (title_surface = IMG_Load_RW(getres(TITLE_JPG),1)) == 0 ) {
    std::cerr << "Error Loading title: \"" << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
  if( (title_texture = SDL_CreateTextureFromSurface(renderer,title_surface)) == 0 ) {
    std::cerr << "Error Loading title: \"" << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
  if( (howto_surface = IMG_Load_RW(getres(HOWTO_JPG),1)) == 0 ) {
    std::cerr << "Error Loading howto: \"" << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
  if( (howto_texture = SDL_CreateTextureFromSurface(renderer,howto_surface)) == 0 ) {
    std::cerr << "Error Loading howto: \"" << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
  if( (bgm = Mix_LoadMUS_RW(getres(TITLE_OGG),1)) == 0 ) {
    std::cerr << "Error Loading title music: \": " << SDL_GetError() << std::endl;
    SDL_Quit();\
    exit(1);
  }

  Mix_PlayMusic(bgm,-1);
  start = SDL_GetTicks();
  for(;;) {
      while( SDL_PollEvent(&event) ) {
        if( event.type == SDL_QUIT ) {
          SDL_Quit();
          exit(0);
        }
        else if( SDL_GetTicks() > start+MINTITLE*1000 ) {
          if( event.type == SDL_MOUSEBUTTONDOWN ) {
            stage++;
            start = SDL_GetTicks();
          }
          else if( event.type == SDL_KEYDOWN ) {
             stage++;
             start = SDL_GetTicks();
          }
        }
      }
    if( stage > 1 ) {
      break;
    }
    SDL_RenderClear(renderer);
    if( stage == 0 ) {
      SDL_RenderCopy(renderer,title_texture,0,0);
    }
    else if( stage == 1 ) {
      SDL_RenderCopy(renderer,howto_texture,0,0);
    }
    SDL_RenderPresent(renderer);
  }

  Mix_FreeMusic(bgm);
  SDL_DestroyTexture(title_texture);
  SDL_FreeSurface(title_surface);
  SDL_DestroyTexture(howto_texture);
  SDL_FreeSurface(howto_surface);
  SDL_Delay(MAXDELAY);
}

void render_notebook(SDL_Renderer* renderer, TTF_Font* font, Target* target, int score) {
  static SDL_Surface* surface=0;
  static SDL_Texture* texture=0;
  static SDL_Surface* scribble_surface=0;
  static SDL_Texture* scribble_texture=0;
  static SDL_Rect scribbledst;
  static SDL_Rect targetdst;
  static SDL_Rect scoredst;
  char score_str[10];

  if( texture == 0 ) {
    if( (surface = IMG_Load_RW(getres(NOTEBOOK_PNG),1)) == 0 ) {
      std::cerr << "Error Loading notebook: \"" << SDL_GetError() << std::endl;
      SDL_Quit();
      exit(1);
    }
    if( (texture = SDL_CreateTextureFromSurface(renderer,surface)) == 0 ) {
      std::cerr << "Error Loading notebook: \"" << SDL_GetError() << std::endl;
      SDL_Quit();
      exit(1);
    }


    if( (scribble_surface = IMG_Load_RW(getres(SCRIBBLE_PNG),1)) == 0 ) {
      std::cerr << "Error Loading scribble: \"" << SDL_GetError() << std::endl;
      SDL_Quit();
      exit(1);
    }
    if( (scribble_texture = SDL_CreateTextureFromSurface(renderer,scribble_surface)) == 0 ) {
      std::cerr << "Error Loading scribble: \"" << SDL_GetError() << std::endl;
      SDL_Quit();
      exit(1);
    }
    scribbledst.x = 337*WINWIDTH/640;
    scribbledst.y = 54*WINHEIGHT/480;
    scribbledst.w = scribble_surface->w*WINWIDTH/640;
    scribbledst.h = scribble_surface->h*WINHEIGHT/480;

    targetdst.x = 100*WINWIDTH/640;
    targetdst.y = 115*WINHEIGHT/480;
    targetdst.w = 128*WINWIDTH/640;
    targetdst.h = 192*WINHEIGHT/480;
    scoredst.x = 135*WINWIDTH/640;
    scoredst.y = 370*WINHEIGHT/480;
  }

  snprintf(score_str,5,"%d",score);
  SDL_Color fg = {0,0,0,255};
  SDL_Surface* text_surface = TTF_RenderText_Blended(font, score_str, fg);
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer,text_surface);
  scoredst.w = text_surface->w;
  scoredst.h = text_surface->h;

  SDL_RenderCopy(renderer,texture,0,0);
  target->force_render(targetdst);
  SDL_RenderCopy(renderer,text_texture,0,&scoredst);
  SDL_RenderCopy(renderer,scribble_texture,0,&scribbledst);

  SDL_DestroyTexture(text_texture);
  SDL_FreeSurface(text_surface);
}

void render_points(SDL_Renderer* renderer, TTF_Font* font, int last_points) {
  SDL_Rect dst;
  char score_str[10];

  if( last_points > 0 ) {
    snprintf(score_str,5,"+%d",last_points);
  }
  else {
    snprintf(score_str,5,"%d",last_points);
  }
  SDL_Color fg = {255,0,0,255};
  SDL_Surface* text_surface = TTF_RenderText_Blended(font,score_str,fg);
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer,text_surface);
  dst.w = text_surface->w;
  dst.h = text_surface->h;

  dst.x = 0;
  dst.y = 0;
  SDL_RenderCopy(renderer,text_texture,0,&dst);

  dst.x = WINWIDTH-text_surface->w;
  dst.y = 0;
  SDL_RenderCopy(renderer,text_texture,0,&dst);

  dst.x = 0;
  dst.y = WINHEIGHT-text_surface->h;
  SDL_RenderCopy(renderer,text_texture,0,&dst);

  dst.x = WINWIDTH-text_surface->w;
  dst.y = WINHEIGHT-text_surface->h;
  SDL_RenderCopy(renderer,text_texture,0,&dst);

  SDL_DestroyTexture(text_texture);
  SDL_FreeSurface(text_surface);
}

int main(int argc, char** argv) {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Event event;
  Map *map;
  Target* my_target;
  char input;
  int score = 0;
  int i;
  Uint32 ticks;
  Uint32 delay;
  Uint32 reload_ticks = 0;
  int last_points = true;

  bool scrolling = false;
  bool scope = false;
  bool notebook = true;
  SDL_Point mouse_pos;

  TTF_Font* font;
  Mix_Music *bgm;
  Mix_Chunk *shot;
  Mix_Chunk *good_hit;
  Mix_Chunk *bad_hit;

  if( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
    std::cerr << "Could not init SDL: " << SDL_GetError() << std::endl;
    return 1;
  }
  if( IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG) == 0 ) {
    std::cerr << "Could not init IMG: " << SDL_GetError() << std::endl;
    return 1;
  }
  if( Mix_OpenAudio(44100, AUDIO_S16, 2, 4096) != 0 ) {
    std::cerr << "Could not init Mix: " << SDL_GetError() << std::endl;
    return 1;
  }
  if( TTF_Init() != 0 ) {
    std::cerr << "Could not init TTF: " << SDL_GetError() << std::endl;
    return 1;
  }
  if( (font = TTF_OpenFontRW(getres(NOTEBOOK_TTF),1,36)) == 0 ) {
    std::cerr << "Error Loading font: \"" << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  if( (bgm = Mix_LoadMUS_RW(getres(BGM_OGG),1)) == 0 ) {
    std::cerr << "Could not load bgm: " << SDL_GetError() << std::endl;
    return 1;
  }
  if( (shot = Mix_LoadWAV_RW(getres(SHOT_OGG),1)) == 0 ) {
    std::cerr << "Could not load sound: " << SDL_GetError() << std::endl;
    return 1;
  }
  if( (good_hit = Mix_LoadWAV_RW(getres(GOOD_OGG),1)) == 0 ) {
    std::cerr << "Could not load sound: " << SDL_GetError() << std::endl;
    return 1;
  }
  if( (bad_hit = Mix_LoadWAV_RW(getres(BAD_OGG),1)) == 0 ) {
    std::cerr << "Could not load sound: " << SDL_GetError() << std::endl;
    return 1;
  }

  window = SDL_CreateWindow("Sniper",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WINWIDTH,WINHEIGHT,0);
  renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);;

  srand(time(0));
  InitTargets(renderer, TARGET_0_PNG);
  map = new Map(renderer,
                MAP_BG_PNG, MAP_FG_PNG,
                MAP_MASK_PNG,SCOPE_PNG);
  for( i=0; i<TARGETCOUNT; i++ ) {
    map->addTarget(i%TARGETTYPECOUNT);
  }
  my_target = new Target(renderer, (SDL_Surface*)0, rand()%TARGETTYPECOUNT);

  title(renderer);


  Mix_PlayMusic(bgm,-1);
  for(;;) {
    ticks = SDL_GetTicks();
    while( SDL_PollEvent(&event) ) {
      if( event.type == SDL_QUIT ) {
        goto done;
      }
      else if( event.button.type == SDL_MOUSEBUTTONDOWN ) {
        if( event.button.button == SDL_BUTTON_LEFT ) {
          if( ! scope ) {
            scrolling = true;
          }
          else if( ticks > reload_ticks ) {
            Mix_PlayChannel(-1, shot, 0);
            i = map->shoot();
            reload_ticks = ticks+RELOADTIME*1000;
            if( i < 0 ) {
              last_points=MISPOINTS;
            }
            else {
              if( i == my_target->type() ) {
                Mix_PlayChannel(-1, good_hit, 0);
                last_points=HITPOINTS;
              }
              else {
                Mix_PlayChannel(-1, bad_hit, 0);
                last_points=WRGPOINTS;
              }
            }
            score=score+last_points;
          }
        }
        else if( event.button.button == SDL_BUTTON_RIGHT ) {
          scrolling = false;
          scope = true;
          map->scopeEnable(scope);
          map->scopeView(mouse_pos);
        }
      }
      else if( event.button.type == SDL_MOUSEBUTTONUP ) {
        if( event.button.button == SDL_BUTTON_LEFT ) {
          if( ! scope ) {
            scrolling = false;
          }
        }
        if( event.button.button == SDL_BUTTON_RIGHT ) {
          scrolling = false;
          scope = false;
          map->scopeEnable(scope);
          map->scopeView(mouse_pos);
        }
      }
      else if( event.type == SDL_MOUSEMOTION ) {
        if( scrolling ) {
          map->scroll(mouse_pos.x-event.motion.x, mouse_pos.y-event.motion.y);
        }
        mouse_pos.x = event.motion.x;
        mouse_pos.y = event.motion.y;
        if( scope ) {
          map->scopeView(mouse_pos);
        }

      }
      else if( event.type == SDL_KEYDOWN ) {
        if( event.key.keysym.sym == SDLK_TAB ) {
          if( !event.key.repeat ) {
            notebook = !notebook;
          }
        }
      }
    }

    map->think();
    map->render();
    if( notebook ) {
      render_notebook(renderer,font,my_target,score);
    }
    if( reload_ticks && ticks < reload_ticks ) {
      render_points(renderer,font,last_points);
    }
    SDL_RenderPresent(renderer);

    delay = MAXDELAY-(SDL_GetTicks()-ticks);
    if( delay > MAXDELAY ) { delay=MAXDELAY; }
    SDL_Delay( delay );
  }

  done:
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

