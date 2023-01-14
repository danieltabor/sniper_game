/*
 * Copyright (c) 2022, Daniel Tabor
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "Map.h"
#include "config.h"
#include "resources.h"

#include<SDL2/SDL_image.h>

#include<iostream>



Map::Map(SDL_Renderer* renderer, int bg_path, int fg_path, int mask_path, int scope_path) {
  m_renderer = renderer;
  if( (m_bg_surface = IMG_Load_RW(getres(bg_path),1)) == 0 ) {
    std::cerr << "Error Loading \"" << bg_path << "\": " << SDL_GetError() << std::endl;
    SDL_Quit();
  }
  if( (m_bg_texture = SDL_CreateTextureFromSurface(m_renderer,m_bg_surface)) == 0 ) {
    std::cerr << "Error Loading \"" << bg_path << "\": " << SDL_GetError() << std::endl;
    SDL_Quit();
  }
  if( (m_fg_surface = IMG_Load_RW(getres(fg_path),1)) == 0 ) {
    std::cerr << "Error Loading \"" << bg_path << "\": " << SDL_GetError() << std::endl;
    SDL_Quit();
  }
  if( (m_fg_texture = SDL_CreateTextureFromSurface(m_renderer,m_fg_surface)) == 0 ) {
    std::cerr << "Error Loading \"" << bg_path << "\": " << SDL_GetError() << std::endl;
    SDL_Quit();
  }
  if( (m_mask = IMG_Load_RW(getres(mask_path),1)) == 0 ) {
    std::cerr << "Error Loading \"" << mask_path << "\": " << SDL_GetError() << std::endl;
    SDL_Quit();
  }
  if( (m_scope_surface = IMG_Load_RW(getres(scope_path),1)) == 0 ) {
    std::cerr << "Error Loading \"" << scope_path << "\": " << SDL_GetError() << std::endl;
    SDL_Quit();
  }
  if( (m_scope_texture = SDL_CreateTextureFromSurface(m_renderer,m_scope_surface)) == 0 ) {
    std::cerr << "Error Loading \"" << scope_path << "\": " << SDL_GetError() << std::endl;
    SDL_Quit();
  }

  m_offset.x = 0;
  m_offset.y = 0;
  m_scope = false;
}

void Map::addTarget(int target_type) {
  Target* target = new Target(m_renderer, m_mask, target_type);
  m_targets.push_back(target);
}

void Map::think() {
  std::list<Target*>::iterator it;
  for( it=m_targets.begin(); it!=m_targets.end(); it++ ) {
    (*it)->move();
    //(*it)->debug_print();
  }
}

void Map::render() {
  std::list<Target*>::iterator it;
  SDL_Rect src;
  SDL_Rect dst;
  SDL_Rect offset_scale;
  SDL_Rect cull;

  std::list<target_sort_t>::iterator rit;
  target_sort_t current;

  if( m_scope ) {
    offset_scale.x = m_offset.x+m_scope_offset.x-(m_scope_offset.x/ZOOMFACTOR);
    offset_scale.y = m_offset.y+m_scope_offset.y-(m_scope_offset.y/ZOOMFACTOR);
    offset_scale.w = ZOOMFACTOR;
    offset_scale.h = ZOOMFACTOR;

    src.x = m_offset.x+m_scope_offset.x - m_scope_surface->w/2;
    src.y = m_offset.y+m_scope_offset.y - m_scope_surface->h/2;
    src.w = m_scope_surface->w;
    src.h = m_scope_surface->h;

    dst.x = m_scope_offset.x - m_scope_surface->w*ZOOMFACTOR/2;
    dst.y = m_scope_offset.y - m_scope_surface->h*ZOOMFACTOR/2;
    dst.w = m_scope_surface->w*ZOOMFACTOR;
    dst.h = m_scope_surface->h*ZOOMFACTOR;

    cull.x = dst.x+16*ZOOMFACTOR;
    cull.y = dst.y+24*ZOOMFACTOR;
    cull.w = 25*ZOOMFACTOR;
    cull.h = 25*ZOOMFACTOR;

  }
  else {
    offset_scale.x = m_offset.x;
    offset_scale.y = m_offset.y;
    offset_scale.w = 1;
    offset_scale.h = 1;
    cull.x = 0;
    cull.y = 0;
    cull.w = WINWIDTH;
    cull.h = WINHEIGHT;
    src.x = m_offset.x;
    src.y = m_offset.y;
    src.w = WINWIDTH;
    src.h = WINHEIGHT;
    dst.x = 0;
    dst.y = 0;
    dst.w = WINWIDTH;
    dst.h = WINHEIGHT;
  }

  m_render_list.clear();
  for( it=m_targets.begin(); it!=m_targets.end(); it++ ) {
    current.y = (*it)->prerender(offset_scale,cull);
    if( current.y >= 0 ) {
      current.target = (*it);
      if( current.target->dead() ) {
        m_render_list.push_front(current);
      }
      else {
        for( rit=m_render_list.begin();
             rit!=m_render_list.end();
             rit++ ) {
          if( ! rit->target->dead() &&
              rit->y >= current.y ) {
            m_render_list.insert(rit,current);
            break;
          }
        }
        if( rit == m_render_list.end() ) {
          m_render_list.push_back(current);
        }
      }
    }
  }


  SDL_RenderClear(m_renderer);
  SDL_RenderCopy(m_renderer,m_bg_texture,&src,&dst);
  for( rit=m_render_list.begin();
       rit!=m_render_list.end();
       rit++ ) {
    rit->target->render();
  }
  SDL_RenderCopy(m_renderer,m_fg_texture,&src,&dst);
  if( m_scope ) {
    SDL_RenderCopy(m_renderer,m_scope_texture,0,&dst);
  }
}

int Map::shoot() {
  SDL_Point sight;
  std::list<target_sort_t>::reverse_iterator rit;
  int target_type = -1;
  if( m_scope ) {
    sight.x = m_offset.x+m_scope_offset.x;
    sight.y = m_offset.y+m_scope_offset.y;
    for( rit = m_render_list.rbegin();
         rit != m_render_list.rend();
         rit++ ) {
      if( rit->target->shoot(sight) ) {
        target_type = rit->target->type();
        break;
      }
    }
  }
  return target_type;
}

void Map::scroll(int x, int y) {
  m_offset.x = m_offset.x + x;
  if( m_offset.x < 0 ) { m_offset.x = 0; }
  else if( m_offset.x + WINWIDTH > m_bg_surface->w ) { m_offset.x = m_bg_surface->w-WINWIDTH; }
  m_offset.y = m_offset.y + y;
  if( m_offset.y < 0 ) { m_offset.y = 0; }
  else if( m_offset.y + WINHEIGHT > m_bg_surface->h ) { m_offset.y = m_bg_surface->h-WINHEIGHT; }
}

void Map::scopeEnable(bool enable) {
  m_scope = enable;
}

void Map::scopeView(SDL_Point view) {
  m_scope_offset = view;
}
