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
