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
#ifndef TARGET_H
#define TARGET_H

#include<SDL2/SDL.h>

#include<string>

class Target {
public:
  Target(SDL_Renderer* renderer, SDL_Surface* mask, int target_type);
  void move();
  int  prerender(SDL_Rect offset_scale, SDL_Rect cull);
  void render();
  void force_render(SDL_Rect dst);
  bool shoot(SDL_Point sight);
  int type();
  bool dead();
  void debug_print();
protected:
  bool collision();

  int m_target_type;
  SDL_Renderer* m_renderer;
  SDL_Surface* m_mask;
  SDL_Point m_pos;
  SDL_Point m_dst;
  unsigned int m_dst_ticks;
  unsigned int m_move_ticks;
  int m_frame;
  int m_dir;
  SDL_Rect m_renderdst;
  bool m_dead;
};

void InitTargets(SDL_Renderer* renderer, int start);

#endif // TARGET_H
