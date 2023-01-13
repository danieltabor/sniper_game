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
