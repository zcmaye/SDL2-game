#include"Sprite.h"
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 760;
SDL_Window* window;
SDL_Renderer * winRender;

SDL_Texture* tex_bk;				//±³¾°Í¼
SDL_Texture* tex_player[2];			//Íæ¼ÒÕý³£Í¼
SDL_Texture* tex_playerBlowup[4];	//Íæ¼Ò±¬Õ¨Í¼
SDL_Texture* tex_bullet[2];			//×Óµ¯Í¼

void loadResource()
{
	tex_bk = IMG_LoadTexture(winRender, "Resources/img/background.png");
	tex_player[0] = IMG_LoadTexture(winRender, "Resources/img/hero1.png");
	tex_player[1] = IMG_LoadTexture(winRender, "Resources/img/hero2.png");
	
	char imgfile[50] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		SDL_snprintf(imgfile, 50, "Resources/img/hero_blowup_n%d.png", i + 1);
		tex_playerBlowup[i] = IMG_LoadTexture(winRender, imgfile);
	}

	tex_bullet[0] = IMG_LoadTexture(winRender, "Resources/img/bullet1.png");
	tex_bullet[1] = IMG_LoadTexture(winRender, "Resources/img/bullet2.png");

	SDL_Texture* t[2] = { tex_bk };
	&t[0];
}

Sprite sprbk;
Gamer gamer;
Uint32 updateData(Uint32 interval, void* param)
{
	Gamer* gamer = (Gamer*)param;
	gamer->frame = (gamer->frame + 1) % 2;
	return interval;
}

bool init();
void draw(SDL_Renderer* renderer);
bool handingEvent();
int main(int argc ,char*argv[])
{
	if (!init())
	{
		return -1;
	}
	loadResource();
	
	sprite_init(&sprbk, 0, 0, tex_bk);
	gamer_init(&gamer, 280, 650);

	SDL_AddTimer(300, updateData, &gamer);

	bool isDone = false;
	while (!isDone)
	{
		isDone = handingEvent();


		draw(winRender);
	}
	return 0;
}

bool handingEvent()
{
	static SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			return true;
			break;
		default:
			
			break;
		}
	}
	gamer_move(&gamer, &ev.key);
	return false;
}
bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1 || IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == -1)
	{
		return false;
	}

	window = SDL_CreateWindow("PlaneWars", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	winRender = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	return true;
}
void draw(SDL_Renderer* renderer)
{
	sprite_draw(&sprbk, winRender);
	gamer_draw(&gamer, renderer, tex_player[gamer.frame]);
	
	SDL_RenderPresent(winRender);
}