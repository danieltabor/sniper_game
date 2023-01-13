#ifndef __RES_H__
#define __RES_H__

#include<SDL2/SDL.h>
#include<stdint.h>

typedef struct {
  unsigned int len;
  uint8_t *data;
} resource_t;

typedef struct {
  unsigned int offset;
  resource_t *res;
} res_data_t;


#ifndef __RES_C__
extern SDL_RWops* getres(int index);
#endif

#endif //__RES_H__
