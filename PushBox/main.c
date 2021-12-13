#include<SDL.h>
#include<stdbool.h>
#include<stdio.h>
#define ROW 10
#define COL	10
enum
{
	SPACE,		//0 空地
	WALL,		//1 墙
	DEST,		//2 目的地
	BOX,		//3 箱子
	PLAYER		//4 玩家
};
int map[10][10] = 
{
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,1,1,1,0,0,0},
	{0,0,0,1,2,1,0,0,0},
	{0,0,0,1,3,1,1,1,1},
	{0,1,1,1,0,3,0,2,1},
	{0,1,2,3,4,0,1,1,1},
	{0,1,1,1,1,3,1,0,0},
	{0,0,0,0,1,2,1,0,0},
	{0,0,0,0,1,1,1,0,0},
	{0,0,0,0,0,0,0,0,0},
};

SDL_Surface* surface[7];
void loadResource()
{
	for (int i = 0; i < 7; i++)
	{
		char imgfile[20] = { 0 };
		sprintf(imgfile, "images/%d.bmp", i);
		surface[i] = SDL_LoadBMP(imgfile);
	}
}

bool isDone = false;
bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("init failed~\n");
		return false;
	}
	loadResource();

	return true;
}
void draw(SDL_Surface* winSurface)
{
	for (int i = 0; i < 10; i++)
	{
		for (int k = 0; k < 10; k++)
		{
			SDL_Rect rect = { k * 64,i * 64,64,64 };
			SDL_BlitSurface(surface[map[i][k]], NULL, winSurface, &rect);
		}
	}
}
void keyDownEvent(SDL_KeyboardEvent* ev)
{
	//找到玩家 下标
	int row = -1, col = -1;
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			//玩家的值是什么 PLAYER, PLAYER +DEST
			if (map[i][k] == PLAYER || map[i][k] == PLAYER + DEST)
			{
				row = i;
				col = k;
				goto end;
			}
		}
	}
end:;
	switch (ev->keysym.sym)
	{
	case SDLK_UP:
		//玩家的前面是空地或者目的地
		if (map[row - 1][col] == SPACE || map[row - 1][col] == DEST)
		{
			//移动玩家到目标位置
			map[row - 1][col] += PLAYER;
			//把玩家从原来的位置清除掉
			map[row][col] -= PLAYER;
		}
		//推箱子：玩家的前面是箱子，并且箱子前面是空地或者目的地
		else if (map[row - 1][col] == BOX)
		{
			if (map[row - 2][col] == SPACE || map[row - 2][col] == DEST)
			{
				map[row - 2][col] += BOX;  //把箱子移动到目标位置
				map[row - 1][col] -= BOX;	//吧箱子从原来的位置清除
				map[row - 1][col] += PLAYER;
				map[row][col] -= PLAYER;
			}
		}
		break;
	case SDLK_DOWN:
		//玩家的前面是空地或者目的地
		if (map[row + 1][col] == SPACE || map[row + 1][col] == DEST)
		{
			//移动玩家到目标位置
			map[row + 1][col] += PLAYER;
			//把玩家从原来的位置清除掉
			map[row][col] -= PLAYER;
		}
		//推箱子：玩家的前面是箱子，并且箱子前面是空地或者目的地
		else if (map[row + 1][col] == BOX)
		{
			if (map[row + 2][col] == SPACE || map[row + 2][col] == DEST)
			{
				map[row + 2][col] += BOX;  //把箱子移动到目标位置
				map[row + 1][col] -= BOX;	//吧箱子从原来的位置清除
				map[row + 1][col] += PLAYER;
				map[row][col] -= PLAYER;
			}
		}
		break;
	case SDLK_LEFT:
		//玩家的前面是空地或者目的地
		if (map[row][col - 1] == SPACE || map[row][col - 1] == DEST)
		{
			//移动玩家到目标位置
			map[row][col - 1] += PLAYER;
			//把玩家从原来的位置清除掉
			map[row][col] -= PLAYER;
		}
		//推箱子：玩家的前面是箱子，并且箱子前面是空地或者目的地
		else if (map[row][col - 1] == BOX)
		{
			if (map[row][col - 2] == SPACE || map[row][col - 2] == DEST)
			{
				map[row][col - 2] += BOX;  //把箱子移动到目标位置
				map[row][col - 1] -= BOX;	//吧箱子从原来的位置清除
				map[row][col - 1] += PLAYER;
				map[row][col] -= PLAYER;
			}
		}
		break;
	case SDLK_RIGHT:
		//玩家的前面是空地或者目的地
		if (map[row][col + 1] == SPACE || map[row][col + 1] == DEST)
		{
			//移动玩家到目标位置
			map[row][col + 1] += PLAYER;
			//把玩家从原来的位置清除掉
			map[row][col] -= PLAYER;
		}
		//推箱子：玩家的前面是箱子，并且箱子前面是空地或者目的地
		else if (map[row][col + 1] == BOX)
		{
			if (map[row][col + 2] == SPACE || map[row][col + 2] == DEST)
			{
				map[row][col + 2] += BOX;  //把箱子移动到目标位置
				map[row][col + 1] -= BOX;	//吧箱子从原来的位置清除
				map[row][col + 1] += PLAYER;
				map[row][col] -= PLAYER;
			}
		}
		break;
	default:
		break;
	}
}
void processEvent()
{
	static SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			isDone = true;
			break;
		case SDL_KEYDOWN:
			keyDownEvent(&ev.key);
			break;
		default:
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	if (!init())
	{
		return -1;
	}
	SDL_Window* window  = SDL_CreateWindow(u8"推箱子", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,640, 640, SDL_WINDOW_SHOWN);
	SDL_Surface* winSurface = SDL_GetWindowSurface(window);

	while (!isDone)
	{
		processEvent();
		draw(winSurface);
		SDL_UpdateWindowSurface(window);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}