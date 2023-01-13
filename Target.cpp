#include <Target.h>
#include "config.h"
#include "resources.h"

#include<SDL2/SDL_image.h>

#include <iostream>

#include <stdlib.h>
#include <stdio.h>

typedef struct {
  SDL_Surface* surface;
  SDL_Texture* texture;
} target_template_t;

static target_template_t g_target_templates[TARGETTYPECOUNT];
static target_template_t g_target_dead;

void InitTargets(SDL_Renderer* renderer, int start) {
  size_t i;
  int path;

  for( i=0, path=start; i<TARGETTYPECOUNT; i++, path++ ) {
    if( (g_target_templates[i].surface = IMG_Load_RW(getres(path),1)) == 0 ) {
      std::cerr << "Error Loading sprite \"" << i << "\": " << SDL_GetError() << std::endl;
      SDL_Quit();
      exit(1);
    }
    if( (g_target_templates[i].texture = SDL_CreateTextureFromSurface(renderer,g_target_templates[i].surface)) == 0 ) {
      std::cerr << "Error Loading sprite \"" << i << "\": " << SDL_GetError() << std::endl;
      SDL_Quit();
      exit(1);
    }
  }
  if( (g_target_dead.surface = IMG_Load_RW(getres(DEAD_PNG),1)) == 0 ) {
    std::cerr << "Error Loading dead sprite \": " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
  if( (g_target_dead.texture = SDL_CreateTextureFromSurface(renderer,g_target_dead.surface)) == 0 ) {
    std::cerr << "Error Loading dead sprite \": " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
}

Target::Target(SDL_Renderer* renderer, SDL_Surface* mask, int target_type) {
  m_renderer = renderer;
  m_mask = mask;
  m_target_type = target_type;
  m_frame = 0;
  m_dir = 0;
  m_move_ticks = 0;
  m_dst_ticks = 0;
  m_dead = false;
  if( mask ) {
    do {
      m_pos.x = rand() % m_mask->w;
      m_pos.y = rand() % m_mask->h;
    } while( collision() );
  }
  else {
    m_pos.x = 0;
    m_pos.y = 0;
  }
}

void Target::move() {
  SDL_Point last_pos;

  m_move_ticks++;
  if( m_dead ) {
    if( m_move_ticks >= m_dst_ticks ) {
      m_dead = false;
      m_move_ticks = 0;
      m_dst_ticks = 0;
      if( m_mask ) {
        do {
          m_pos.x = rand() % m_mask->w;
          m_pos.y = rand() % m_mask->h;
        } while( collision() );
      }
    }
  }

  if( ! m_dead && m_mask != 0) {

    if( m_dst_ticks == 0 || m_move_ticks == m_dst_ticks ) {
      m_dst.x = rand() % m_mask->w;
      m_dst.y = rand() % m_mask->h;
      m_dst_ticks = FRAMERATE * ( 5 + (rand() % 15) );
      m_move_ticks = 0;
    }

    last_pos.x = m_pos.x;
    last_pos.y = m_pos.y;
    m_pos.x = m_pos.x + (int)( (double)(m_dst.x - m_pos.x) / (double)(m_dst_ticks - m_move_ticks) );
    m_pos.y = m_pos.y + (int)( (double)(m_dst.y - m_pos.y) / (double)(m_dst_ticks - m_move_ticks) );
    if( collision() ) {
      m_pos.x = last_pos.x;
      m_pos.y = last_pos.y;
      m_dst_ticks = 0;
    }


    if( abs(last_pos.x - m_pos.x) > abs(last_pos.y - m_pos.y) ) {
      if( last_pos.x < m_pos.x ) {
        m_dir = 3;
      }
      else {
        m_dir = 2;
      }
    }
    else {
      if( last_pos.y < m_pos.y ) {
        m_dir = 0;
      }
      else {
        m_dir = 1;
      }
    }
  }
}

bool Target::shoot(SDL_Point sight) {
  if( m_dead ) {
    return false;
  }
  else if( m_pos.x+16 < sight.x ||
      m_pos.x > sight.x ||
      m_pos.y-24 > sight.y ||
      m_pos.y < sight.y ) {
    return false;
  }
  else {
    m_dead = true;
    m_move_ticks = 0;
    m_dst_ticks = RESPAWNTIME * FRAMERATE;
    return true;
  }
}

int Target::type() {
  return m_target_type;
}

bool Target::dead() {
  return m_dead;
}

int Target::prerender(SDL_Rect offset_scale, SDL_Rect cull) {
   m_renderdst.w = 16*offset_scale.w;
   m_renderdst.h = 24*offset_scale.h;
   m_renderdst.x = (m_pos.x-offset_scale.x)*offset_scale.w;
   m_renderdst.y = (m_pos.y-offset_scale.y-24)*offset_scale.h;

   if( m_renderdst.x+m_renderdst.w < cull.x ||
       m_renderdst.x > cull.x+cull.w ||
       m_renderdst.y+m_renderdst.h < cull.y ||
       m_renderdst.y > cull.y+cull.h ) {
     return -1;
   }
   return m_renderdst.y;
 }

void Target::render() {
  SDL_Rect src;
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  if( ! m_dead) {
    if( m_dir == 0 ) {
      src.x = 0;
      if( m_frame/TICKPERFRAME ) { flip = SDL_FLIP_HORIZONTAL; }
    }
    else if( m_dir == 1 ) {
      src.x = 16;
      if( m_frame/TICKPERFRAME ) { flip = SDL_FLIP_HORIZONTAL; }
    }
    else if( m_dir == 2 ) {
      src.x = 32 + (16*(m_frame/TICKPERFRAME));
    }
    else { //m_dir == 3
      src.x = 32 + (16*(m_frame/TICKPERFRAME));
      flip = SDL_FLIP_HORIZONTAL;
    }
    src.y = 0;
    src.w = 16;
    src.h = 24;
    m_frame = (m_frame+1)%(TICKPERFRAME*2);
    SDL_RenderCopyEx(m_renderer,g_target_templates[m_target_type].texture,&src,&m_renderdst,0,0,flip);
  }
  else {
    SDL_RenderCopy(m_renderer,g_target_dead.texture,0,&m_renderdst);
  }
}

void Target::force_render(SDL_Rect dst) {
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 16;
  src.h = 24;
  SDL_RenderCopy(m_renderer,g_target_templates[m_target_type].texture,&src,&dst);
}

void Target::debug_print() {
  std::cout << "X: " << m_pos.x << "  Y: " << m_pos.y << std::endl;
}

bool Target::collision() {
  Uint8 pointA = *(((Uint8*)m_mask->pixels) + (m_mask->pitch*m_pos.y) + (m_pos.x + 0));
  Uint8 pointB = *(((Uint8*)m_mask->pixels) + (m_mask->pitch*m_pos.y) + (m_pos.x + 15));
  if( pointA == 0 || pointB == 0 ) {
     return true;
  }
  else {
    return false;
  }
}
