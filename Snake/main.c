#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include<SDL.h>
SDL_Renderer* winRender;
SDL_Window* window;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#define Random(low,hig) (rand()%(hig-low)+low)

#define MAXLEN  1000
enum Dir
{
	Up,
	Down,
	Left,
	Right
};

/* @食物*/
typedef struct
{
	int x;
	int y;
	bool isDie;
	SDL_Color color;
}Food;
Food food;
void food_init(Food* food, int x, int y, bool isDie)
{
	food->x = x;
	food->y = y;
	food->isDie = isDie;
	SDL_Color c = { 0,0,0,255 };
	food->color = c;
}
void food_randPosistion(Food* food)
{
	if (!food->isDie)
		return;
	food->x = Random(0, 64) * 10;
	food->y = Random(0, 48) * 10;
	SDL_Color c = { rand() % 256,rand() % 256,rand() % 256,255 };
	food->color = c;
	food->isDie = false;
}
void food_draw(Food* food)
{
	if (food->isDie)
		return;

	SDL_SetRenderDrawColor(winRender, food->color.r, food->color.g, food->color.b, 255);
	SDL_Rect rect = { food->x,food->y,10,10 };
	SDL_RenderFillRect(winRender, &rect);
}

/* @蛇*/
typedef struct 
{
	int length;		//蛇的长度
	enum Dir dir;
	SDL_Point pos[MAXLEN];	//蛇身体坐标
}Snake;

Snake snake;
void snake_init(Snake* snake, int len)
{
	snake->length = len;
	snake->dir = Right;
	for (int i = len; i > 0; i--)
	{
		SDL_Point point = {  i*10, 0 };
		snake->pos[i] = point;
	}
}
void snake_draw(Snake* snake)
{
	SDL_SetRenderDrawColor(winRender, 255, 0, 0, 0);
	for (int i = 0; i < snake->length; i++)
	{
		SDL_Rect rect = {snake->pos[i].x,snake->pos[i].y,10,10};
		SDL_RenderDrawRect(winRender, &rect);		
	}
}
void snake_move(Snake* snake)
{
	for (int i = snake->length - 1; i > 0; i--)//从最后一节蛇开始，每一节蛇都等于前一节蛇的上一次坐标
	{
		snake->pos[i].x = snake->pos[i - 1].x;
		snake->pos[i].y = snake->pos[i - 1].y;
	}
	switch (snake->dir)
	{
	case Up:
		snake->pos[0].y -= 10;
		if (snake->pos[0].y < 0)
		{
			snake->pos[0].y = SCREEN_HEIGHT;
		}
		break;
	case Down:
		snake->pos[0].y += 10;
		if (snake->pos[0].y > SCREEN_HEIGHT)
		{
			snake->pos[0].y = 0;
		}
		break;
	case Left:
		snake->pos[0].x -= 10;
		if (snake->pos[0].x <  0)
		{
			snake->pos[0].x = SCREEN_WIDTH;
		}
		break;
	case Right:
		snake->pos[0].x += 10;
		if (snake->pos[0].x > SCREEN_WIDTH)
		{
			snake->pos[0].x = 0;
		}
		
		break;
	}
}
void snake_autoMove(Snake* snake,int x,int y)
{
	for (int i = snake->length - 1; i > 0; i--)//从最后一节蛇开始，每一节蛇都等于前一节蛇的上一次坐标
	{
		snake->pos[i].x = snake->pos[i - 1].x;
		snake->pos[i].y = snake->pos[i - 1].y;
	}

	if (snake->pos[0].x != x)
	{
		if (snake->pos[0].x < x)
		{
			snake->pos[0].x += 10;
		}
		else if (snake->pos[0].x > x)
		{
			snake->pos[0].x -= 10;
		}
	}
	else
	{
		if (snake->pos[0].y < y)
		{
			snake->pos[0].y += 10;
		}
		else if (snake->pos[0].y > y)
		{
			snake->pos[0].y -= 10;
		}
	}
}

void snakeEatFood(Snake* snake, Food* food)
{
	if (snake->pos[0].x == food->x && snake->pos[0].y == food->y)
	{
		//为了防止吃到食物之后，蛇的新身体在左上角闪一下
		snake->pos[snake->length].x = -10;
		snake->pos[snake->length].x = -10;

		++snake->length;
		food->isDie = true;
	}
}

bool handingEvent();
void keyPressEvent(SDL_KeyboardEvent* ev);
Uint32 snakeMoveCall(Uint32 interval, void* param)
{
	snake_autoMove(&snake,food.x,food.y);
	//snake_move(&snake);
	food_randPosistion(&food);
	snakeEatFood(&snake, &food);
	return interval;
}
int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
	{
		SDL_Log("init failed~");
		return -1;
	}

	window =  SDL_CreateWindow(u8"贪吃蛇", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN); 
	winRender = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	srand((Uint32)time(NULL));
	

	snake_init(&snake,3);
	food_init(&food,-10,-10,true);
	
	SDL_TimerID snakeMoveTimerid = SDL_AddTimer(1, snakeMoveCall, NULL);
	
	bool isDone = false;
	while (!isDone)
	{
		Uint32 begTime = SDL_GetTicks();
		//设置背景颜色
		SDL_SetRenderDrawColor(winRender, 255, 255, 255, 255);
		SDL_RenderClear(winRender);
		SDL_SetRenderDrawColor(winRender, 200, 200, 200, 255);
		//绘制网格
		for (int i = 0; i < SCREEN_HEIGHT ; i++)	//横线
		{
			SDL_RenderDrawLine(winRender, 0, i * 10, SCREEN_WIDTH, i * 10);
		}
		for (int i = 0; i < SCREEN_WIDTH; i++)		//竖线
		{
			SDL_RenderDrawLine(winRender, i * 10, 0, i * 10, SCREEN_HEIGHT);
		}

		snake_draw(&snake);
		food_draw(&food);
		SDL_RenderPresent(winRender);
		SDL_Log("len:%d\n", snake.length);
		Uint32 remain = SDL_GetTicks() - begTime;	//剩余时间 60fps
		int delay = 1000 / 60 - remain;		
		if (delay > 0)
			SDL_Delay(delay);
		

		isDone = handingEvent();
	}
	SDL_RemoveTimer(snakeMoveTimerid);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

bool handingEvent()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
		case SDL_QUIT:
			return true;
			break;
		case SDL_KEYDOWN:
			keyPressEvent(&ev.key);
			break;
		}
	}
	return false;
}

void keyPressEvent(SDL_KeyboardEvent* ev)
{
	switch (ev->keysym.sym)
	{
	case  SDLK_UP:
		snake.dir = Up;
		break;
	case  SDLK_DOWN:
		snake.dir = Down;
		break;
	case  SDLK_LEFT:
		snake.dir = Left;
		break;
	case  SDLK_RIGHT:
		snake.dir = Right;
		break;
	}
}