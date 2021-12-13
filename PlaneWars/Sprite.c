#include "Sprite.h"

SDL_Keycode keys[5];
#define keys_add(keycode) \
for (int i = 0; i < 5; i++)	\
{\
	if(keys[i] == keycode){break;}  \
	if(keys[i] == SDLK_UNKNOWN)\
		{keys[i] = keycode;break;}\
}
#define keys_remove(keycode)\
for (int i = 0; i < 5; i++)\
{\
	if(keys[i] == keycode)\
		{keys[i] = SDLK_UNKNOWN;break;}\
}
bool keys_contains(SDL_KeyCode keycode)
{
	for (int i = 0; i < 5; i++)
	{
		if (keycode == keys[i])
		{
			return true;
		}
	}
	return false;
}

void sprite_init(Sprite* spr, int x, int y, SDL_Texture* texture)
{
	spr->x = x;
	spr->y = y;
	spr->img = texture;
}

void sprite_draw(Sprite* spr, SDL_Renderer* renderer)
{
	SDL_Rect rect = { spr->x,spr->y};
	SDL_QueryTexture(spr->img, NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(renderer, spr->img, NULL, &rect);
}

/* @Gamer*/
void gamer_init(Gamer* gamer, int x, int y)
{
	sprite_init(&gamer->_super, x, y,NULL);
	gamer->isDie = false;
	gamer->frame = 0;
}

void gamer_draw(Gamer* gamer, SDL_Renderer* renderer, SDL_Texture* texture)
{
	SDL_Rect rect = { gamer->_super.x,gamer->_super.y };
	if (SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h))
	{
		SDL_Log("queryTexture error %s\n", SDL_GetError());
	}

	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void gamer_move(Gamer* gamer, SDL_KeyboardEvent* ev)
{
	switch (ev->type)
	{
	case SDL_KEYDOWN:
		keys_add(ev->keysym.sym);
		break;
	case SDL_KEYUP:
		keys_remove(ev->keysym.sym);
		break;
	}
	static Uint32 beg = 0;
	Uint32 end = SDL_GetTicks();
	if (end - beg >= 5)
	{
		for (int i = 0; i < 5; i++)
		{
			if (keys[i] == SDLK_UP)
			{
				--gamer->_super.y;
			}
			if (keys[i] == (SDLK_DOWN))
			{
				++gamer->_super.y;
			}
			if (keys[i] == (SDLK_LEFT))
			{
				--gamer->_super.x;
			}
			if (keys[i] == (SDLK_RIGHT))
			{
				++gamer->_super.x;
			}
		}
		beg = end;
	}
	
}
