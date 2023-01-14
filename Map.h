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
#ifndef MAP_H
#define MAP_H
#include<SDL2/SDL.h>

#include "Target.h"

#include<string>
#include<list>

typedef struct {
  int y;
  Target* target;
} target_sort_t;

class Map {
public:
  Map(SDL_Renderer* renderer, int bg_path, int fg_path, int mask_path, int scope_path);
  void addTarget(int target_type);
  void think();
  void render();
  int shoot();
  void scroll(int x, int y);
  void scopeEnable(bool enable);
  void scopeView(SDL_Point view);
protected:
  SDL_Renderer* m_renderer;
  SDL_Surface* m_bg_surface;
  SDL_Texture* m_bg_texture;
  SDL_Surface* m_fg_surface;
  SDL_Texture* m_fg_texture;
  SDL_Surface* m_mask;
  SDL_Surface* m_scope_surface;
  SDL_Texture* m_scope_texture;

  SDL_Point m_offset;
  SDL_Point m_scope_offset;
  bool m_scope;

  std::list<Target*> m_targets;
  std::list<target_sort_t> m_render_list;
};

#endif // MAP_H
