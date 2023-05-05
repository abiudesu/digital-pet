#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>

#define NUM_TARGETS 3

SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;

typedef struct {
	SDL_Rect dest;
	int is_dragging;
	int tartype;
	int disappear;
}Targets;
//disappear为1需要消失0不需要
//丢完垃圾进入下个场景1为不进0为进,count用于计算是否每个垃圾都消失
int Enternextscene;
int count;

Targets targets[NUM_TARGETS];

void event_loop();
void event_loop2();

SDL_Surface* Room_Surface = NULL;
SDL_Texture* Room_Texture = NULL;
SDL_Rect Room_Rect;
SDL_Rect RB_Rect;
SDL_Surface* Banana_Surface = NULL;
SDL_Texture* Banana_Texture = NULL;
SDL_Surface* Paper_Surface = NULL;
SDL_Texture* Paper_Texture = NULL;

SDL_Surface* BG_Surface = NULL;
SDL_Texture* BG_Texture = NULL;
SDL_Surface* Rabbit_Surface = NULL;
SDL_Texture* Rabbit_Texture = NULL;
SDL_Surface* Carrot_Surface = NULL;
SDL_Texture* Carrot_Texture = NULL;
SDL_Rect src1;
SDL_Rect src2;
SDL_Rect put1;
SDL_Rect put2;
int walkspeed;
int bg_x;

int SDL_main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	Window = SDL_CreateWindow("Digital Pet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 710, 710, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	Enternextscene = 1;

	//写入不同垃圾的位置
	srand(time(NULL));
	int tar;
	for (tar = 0; tar < NUM_TARGETS; tar++) {
		targets[tar].dest.x = rand() % 401;
		targets[tar].dest.y= 500 + rand() % 181;
		targets[tar].dest.w = 60;
		targets[tar].dest.h = 60;
		targets[tar].is_dragging = 0;
		targets[tar].tartype = rand() % 2 + 1;
		targets[tar].disappear = 0;
	}

	//加载垃圾和背景和垃圾桶
	Room_Rect.x = 0;
	Room_Rect.y = 0;
	Room_Rect.w = 710;
	Room_Rect.h = 710;
	RB_Rect.x = 580;
	RB_Rect.y = 570;
	RB_Rect.w = 115;
	RB_Rect.h = 140;
	Room_Surface = IMG_Load("room1.png");
	Room_Texture = SDL_CreateTextureFromSurface(Renderer, Room_Surface);
	Banana_Surface = IMG_Load("banana.png");
	Banana_Texture = SDL_CreateTextureFromSurface(Renderer, Banana_Surface);
	Paper_Surface = IMG_Load("paper.png");
	Paper_Texture = SDL_CreateTextureFromSurface(Renderer, Paper_Surface);
	event_loop();
	//释放内存
	SDL_FreeSurface(Room_Surface);
	SDL_FreeSurface(Banana_Surface);
	SDL_FreeSurface(Paper_Surface);
	SDL_DestroyTexture(Room_Texture);
	SDL_DestroyTexture(Banana_Texture);
	SDL_DestroyTexture(Paper_Texture);
	SDL_RenderClear(Renderer);
	//进入场景2
	//加载资源,人物，背景，萝卜
	BG_Surface = IMG_Load("grass.png");
	BG_Texture = SDL_CreateTextureFromSurface(Renderer, BG_Surface);
	Rabbit_Surface = IMG_Load("walk1.png");
	Rabbit_Texture = SDL_CreateTextureFromSurface(Renderer, Rabbit_Surface);
	Carrot_Surface = IMG_Load("carrot.png");
	Carrot_Texture = SDL_CreateTextureFromSurface(Renderer, Carrot_Surface);
	//加载萝卜
	event_loop2();
	
	

	return 0;
}

void event_loop() {
	while (Enternextscene) {
		SDL_Event MainEvent;
		long Begin = SDL_GetTicks();
		if (SDL_PollEvent(&MainEvent)) {
			if (MainEvent.key.keysym.sym == SDLK_LEFT) {
				printf("%d", 1);
			}
			if (MainEvent.type == SDL_QUIT) {
				SDL_Quit();
				break;
			}
			else if (MainEvent.type == SDL_MOUSEBUTTONDOWN) {
				int x = MainEvent.button.x;
				int y = MainEvent.button.y;
				for (int i = 0; i < NUM_TARGETS; i++) {
					if (x >= targets[i].dest.x && x <= targets[i].dest.x + targets[i].dest.w && y >= targets[i].dest.y && y <= targets[i].dest.y + targets[i].dest.h) {
						targets[i].is_dragging = 1;
					}
				}
			}
			else if (MainEvent.type == SDL_MOUSEBUTTONUP) {
				for (int i = 0; i < NUM_TARGETS; i++) {
					targets[i].is_dragging = 0;
					if (targets[i].dest.x > RB_Rect.x && targets[i].dest.x<RB_Rect.x + RB_Rect.w && targets[i].dest.y>RB_Rect.y && targets[i].dest.y < RB_Rect.y + RB_Rect.h) {
						targets[i].disappear = 1;
					}
				}
			}
			else if (MainEvent.type == SDL_MOUSEMOTION) {
				int dx = MainEvent.motion.xrel;
				int dy = MainEvent.motion.yrel;
				for (int i = 0; i < NUM_TARGETS; i++) {
					if (targets[i].is_dragging) {
						targets[i].dest.x += dx;
						targets[i].dest.y += dy;
					}
				}
			}
			SDL_RenderClear(Renderer);
			//绘制背景和垃圾
			SDL_RenderCopy(Renderer, Room_Texture, NULL, &Room_Rect);
			for (int i = 0; i < NUM_TARGETS; i++) {
				if (targets[i].disappear==0) {
					if (targets[i].tartype == 1) {
						SDL_RenderCopy(Renderer, Banana_Texture, NULL, &targets[i].dest);
					}
					else {
						SDL_RenderCopy(Renderer, Paper_Texture, NULL, &targets[i].dest);
					}
				}
			}
			SDL_RenderPresent(Renderer);
			long End = SDL_GetTicks();
			long Cost = End - Begin;
			long Frame = 1000 / 60;
			long Delay = Frame - Cost;
			if (Delay > 0) {
				SDL_Delay(Delay);
			}
			for (int i = 0; i < NUM_TARGETS; i++) {
				count = count + targets[i].disappear;
			}
			if (count == 3) {
				Enternextscene = 0;			}
			else {
				count = 0;
			}
		}
	}
}
void event_loop2() {
	walkspeed = 10;
	bg_x = 0;
	while (1) {
		SDL_Event MainEvent;
		long Begin = SDL_GetTicks;
		SDL_Rect src1 = { bg_x, 0, 710, 710 };          // 捕获图片区域1
		SDL_Rect src2 = { 0, 0, bg_x, 710 };               // 捕获图片区域2
		SDL_Rect put1 = { 0, 0, 710 - bg_x, 710 };      // 显示区域1
		SDL_Rect put2 = { 710-bg_x, 0, 710, 710 }; // 显示区域2

		if (SDL_PollEvent(&MainEvent)){
			if (MainEvent.type == SDL_QUIT) {
				SDL_Quit();
				return;
			}
			if (MainEvent.key.keysym.sym == SDLK_LEFT) {
				bg_x = bg_x - walkspeed;
				printf("%d", bg_x);
			}
			if (MainEvent.key.keysym.sym == SDLK_RIGHT) {
				bg_x = bg_x + walkspeed;
				printf("%d", bg_x);
			}
		}
		if (bg_x > 710) {
			bg_x = 0;
		}
		if (bg_x < 0) {
			bg_x = 710;
		}
		//这里开始写绘制场地和人物
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, BG_Texture, &src1, &put1);
		SDL_RenderCopy(Renderer, BG_Texture, &src2, &put2);
		SDL_RenderPresent(Renderer);
	}
}