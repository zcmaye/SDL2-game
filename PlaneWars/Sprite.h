#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<stdbool.h>





/* @Sprite 精灵类*/
typedef struct
{
	int x;
	int y;
	SDL_Texture* img;
}Sprite;

void sprite_init(Sprite* spr, int x, int y, SDL_Texture* texture);
void sprite_draw(Sprite* spr,SDL_Renderer*renderer);

/* @Gamer 玩家*/
typedef struct
{
	Sprite _super;
	bool isDie;
	int frame;		//当前绘制的帧
}Gamer;

void gamer_init(Gamer* gamer, int x, int y);
void gamer_draw(Gamer* gamer, SDL_Renderer* renderer, SDL_Texture* texture);
void gamer_move(Gamer* gamer, SDL_KeyboardEvent* ev);



