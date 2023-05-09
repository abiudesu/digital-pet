#include<stdio.h>
#include<stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#define NUM_TARGETS 3

SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;

typedef struct {
	SDL_Rect dest;
	int is_dragging;
	int tartype;
	int disappear; /* 1 - need exit
					  0 - continue run */
}Targets;

int enterNextScene;
int count;

Targets targets[NUM_TARGETS];

typedef struct {
	SDL_Rect dest;
	int walktype;
}Walkanime;
int frameanime;
int walkstep;
int fcarrot;
int walkspeed;
int carrotframe;
carrotframe = 0;

Walkanime object[6];

void event_loop();
void event_loop2();

SDL_Surface* Main_Surface = NULL;
SDL_Texture* Main_Texture = NULL;
SDL_Rect Main_Rect;

TTF_Font* TitleFont = NULL;
SDL_Surface* TitleSurface = NULL;
SDL_Texture* TitleTexture = NULL;
SDL_Rect TitleRect;
SDL_Color Titlecolor = { 109,56,50,255 };
SDL_Surface* TitleSurface1 = NULL;
SDL_Texture* TitleTexture1 = NULL;
SDL_Rect TitleRect1;
SDL_Surface* TitleSurface2 = NULL;
SDL_Texture* TitleTexture2 = NULL;
SDL_Rect TitleRect2;
SDL_Surface* TitleSurface3 = NULL;
SDL_Texture* TitleTexture3 = NULL;
SDL_Rect TitleRect3;

SDL_Surface* Room_Surface = NULL;
SDL_Texture* Room_Texture = NULL;
SDL_Rect Room_Rect;
SDL_Rect RB_Rect;
SDL_Surface* Banana_Surface = NULL;
SDL_Texture* Banana_Texture = NULL;
SDL_Surface* Paper_Surface = NULL;
SDL_Texture* Paper_Texture = NULL;
SDL_Surface* End_Surface = NULL;
SDL_Texture* End_Texture = NULL;
SDL_Rect End_Rect;

SDL_Surface* BG_Surface = NULL;
SDL_Texture* BG_Texture = NULL;
SDL_Surface* Walk2_Surface = NULL;
SDL_Texture* Walk2_Texture = NULL;
SDL_Surface* Walk1_Surface = NULL;
SDL_Texture* Walk1_Texture = NULL;
SDL_Surface* Walk3_Surface = NULL;
SDL_Texture* Walk3_Texture = NULL;
SDL_Surface* Walk4_Surface = NULL;
SDL_Texture* Walk4_Texture = NULL;
SDL_Surface* Walk5_Surface = NULL;
SDL_Texture* Walk5_Texture = NULL;
SDL_Surface* Stand_Surface = NULL;
SDL_Texture* Stand_Texture = NULL;
SDL_Surface* Carrot_Surface = NULL;
SDL_Texture* Carrot_Texture = NULL;
SDL_Rect Carrot_Rect;
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
	enterNextScene = 1;
	fcarrot = 0;

	// init welcome scene resources
	Main_Surface = IMG_Load("R.jpg");
	Main_Texture = SDL_CreateTextureFromSurface(Renderer, Main_Surface);
	Main_Rect.x = 0;
	Main_Rect.y = 0;
	Main_Rect.w = 710;
	Main_Rect.h = 710;

	TitleFont = TTF_OpenFont("OPlusSans3-Bold.ttf", 50);
	TitleSurface = TTF_RenderUTF8_Blended(TitleFont, "Digital Pet", Titlecolor);
	TitleTexture = SDL_CreateTextureFromSurface(Renderer, TitleSurface);
	TitleRect.x = 160;
	TitleRect.y = 18;
	TitleRect.w = 370;
	TitleRect.h = 96;

	// draw welcome scene
	SDL_RenderClear(Renderer);
	SDL_RenderCopy(Renderer, Main_Texture, NULL, &Main_Rect);
	SDL_RenderCopy(Renderer, TitleTexture, NULL, &TitleRect);
	SDL_RenderPresent(Renderer);
	SDL_Delay(5000);


	// init rubbish
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

	// load background and rubbish bucket
	TitleSurface1 = TTF_RenderUTF8_Blended(TitleFont, "Throw the garbage into the trash can", Titlecolor);
	TitleTexture1 = SDL_CreateTextureFromSurface(Renderer, TitleSurface1);
	TitleRect1.x = 20;
	TitleRect1.y = 20;
	TitleRect1.w = 650;
	TitleRect1.h = 80;
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

	// release memory
	SDL_FreeSurface(Room_Surface);
	SDL_FreeSurface(Banana_Surface);
	SDL_FreeSurface(Paper_Surface);
	SDL_DestroyTexture(Room_Texture);
	SDL_DestroyTexture(Banana_Texture);
	SDL_DestroyTexture(Paper_Texture);
	SDL_FreeSurface(TitleSurface);
	SDL_FreeSurface(TitleSurface1);
	SDL_DestroyTexture(TitleTexture);
	SDL_DestroyTexture(TitleTexture1);
	SDL_RenderClear(Renderer);

	// enter scene 2
	// load animation
	for (int i = 0; i < 6; i++) {
		object[i].dest.x = 297;
		object[i].dest.y = 413;
		object[i].dest.w = 114;
		object[i].dest.h = 204;
		object[i].walktype = 0;
	}

	// load resources
	TitleSurface2 = TTF_RenderUTF8_Blended(TitleFont, "Press right button and find the carrot", Titlecolor);
	TitleTexture2 = SDL_CreateTextureFromSurface(Renderer, TitleSurface2);
	TitleRect2.x = 20;
	TitleRect2.y = 20;
	TitleRect2.w = 650;
	TitleRect2.h = 80;
	BG_Surface = IMG_Load("background.png");
	BG_Texture = SDL_CreateTextureFromSurface(Renderer, BG_Surface);
	Walk1_Surface = IMG_Load("walk1.png");
	Walk1_Texture = SDL_CreateTextureFromSurface(Renderer, Walk1_Surface);
	Walk2_Surface = IMG_Load("walk2.png");
	Walk2_Texture = SDL_CreateTextureFromSurface(Renderer, Walk2_Surface);
	Walk3_Surface = IMG_Load("walk3.png");
	Walk3_Texture = SDL_CreateTextureFromSurface(Renderer, Walk3_Surface);
	Walk4_Surface = IMG_Load("walk4.png");
	Walk4_Texture = SDL_CreateTextureFromSurface(Renderer, Walk4_Surface);
	Walk5_Surface = IMG_Load("walk5.png");
	Walk5_Texture = SDL_CreateTextureFromSurface(Renderer, Walk5_Surface);
	Stand_Surface = IMG_Load("stand.png");
	Stand_Texture = SDL_CreateTextureFromSurface(Renderer, Stand_Surface);
	Carrot_Surface = IMG_Load("carrot.png");
	Carrot_Texture = SDL_CreateTextureFromSurface(Renderer, Carrot_Surface);
	Carrot_Rect.x = 710;
	Carrot_Rect.y = 413;
	Carrot_Rect.w = 80;
	Carrot_Rect.h = 150;

	// load carrot
	event_loop2();

	// release resources
	SDL_FreeSurface(TitleSurface2);
	SDL_FreeSurface(BG_Surface);
	SDL_FreeSurface(Stand_Surface);
	SDL_FreeSurface(Walk1_Surface);
	SDL_FreeSurface(Walk2_Surface);
	SDL_FreeSurface(Walk3_Surface);
	SDL_FreeSurface(Walk4_Surface);
	SDL_FreeSurface(Walk5_Surface);
	SDL_DestroyTexture(TitleTexture2);
	SDL_DestroyTexture(BG_Texture);
	SDL_DestroyTexture(Stand_Texture);
	SDL_DestroyTexture(Walk1_Texture);
	SDL_DestroyTexture(Walk2_Texture);
	SDL_DestroyTexture(Walk3_Texture);
	SDL_DestroyTexture(Walk4_Texture);
	SDL_DestroyTexture(Walk5_Texture);
	SDL_FreeSurface(Carrot_Surface);
	SDL_DestroyTexture(Carrot_Texture);

	// resource load when game ending
	TitleSurface3 = TTF_RenderUTF8_Blended(TitleFont, "Thanks for playing", Titlecolor);
	TitleTexture3 = SDL_CreateTextureFromSurface(Renderer, TitleSurface3);
	TitleRect3.x = 20;
	TitleRect3.y = 20;
	TitleRect3.w = 650;
	TitleRect3.h = 80;
	End_Surface = IMG_Load("OIP.jpg");
	End_Texture = SDL_CreateTextureFromSurface(Renderer, End_Surface);
	End_Rect.x = 0;
	End_Rect.y = 0;
	End_Rect.w = 710;
	End_Rect.h = 710;
	SDL_RenderClear(Renderer);
	SDL_RenderCopy(Renderer, End_Texture, NULL, &End_Rect);
	SDL_RenderCopy(Renderer, TitleTexture3, NULL, &TitleRect3);
	SDL_RenderPresent(Renderer);
	SDL_Delay(3000);

	// release memory
	SDL_FreeSurface(TitleSurface3);
	SDL_FreeSurface(End_Surface);
	SDL_DestroyTexture(TitleTexture3);
	SDL_DestroyTexture(End_Texture);
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	return 0;
}

void event_loop() {
	while (enterNextScene) {
		SDL_Event MainEvent;
		long Begin = SDL_GetTicks();
		if (SDL_PollEvent(&MainEvent)) {
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
			//���Ʊ���������
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
			SDL_RenderCopy(Renderer, TitleTexture1, NULL, &TitleRect1);
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
				enterNextScene = 0;			}
			else {
				count = 0;
			}
		}
	}
}
void event_loop2() {
	srand(time(NULL));
	int carrotapprear;
	int outscene = 1;
	carrotapprear = 250 + rand() % 101;
	walkspeed = 1;
	bg_x = 0;
	frameanime = 0;
	walkstep = 0;
	int i = 0;
	while (outscene) {
		SDL_Event MainEvent;
		Uint32 Begin = SDL_GetTicks;
		SDL_Rect src1 = { bg_x, 0, BG_Surface->w, BG_Surface->h };          // ����ͼƬ����1
		SDL_Rect src2 = { 0, 0, BG_Surface->w, BG_Surface->h };               // ����ͼƬ����2
		SDL_Rect put1 = { 0, 0, BG_Surface->w-bg_x, BG_Surface->h };      // ��ʾ����1
		SDL_Rect put2 = { BG_Surface->w-bg_x, 0, BG_Surface->w, BG_Surface->h }; // ��ʾ����2

		if (SDL_PollEvent(&MainEvent)){
			if (MainEvent.type == SDL_QUIT) {
				SDL_Quit();
				break;
			}
			//if (MainEvent.key.keysym.sym == SDLK_LEFT) {
			//	bg_x = bg_x - walkspeed;
			//	frameanime = frameanime + 1;
			//	printf("%d", bg_x);
			//}
			if (MainEvent.key.keysym.sym == SDLK_RIGHT) {
				bg_x = bg_x + walkspeed;
				frameanime = frameanime + 1;
				carrotframe = carrotframe + 1;
				walkstep = walkstep + 1;
				if (fcarrot == 1) {
					Carrot_Rect.x = Carrot_Rect.x - 1;
				}
			}
		}
		if (bg_x > 710) {
			bg_x = 0;
		}
		if (frameanime == 10) {
			frameanime = 0;
			i = i + 1;
		}
		if (i > 5) {
			i = 0;
		}
		if (walkstep == carrotapprear) {
			fcarrot = 1;
		}
		if (Carrot_Rect.x < object[i].dest.x + object[i].dest.w) {
			outscene = 0;
		}

		// start render scene and charactors
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, BG_Texture, &src1, &put1);
		SDL_RenderCopy(Renderer, BG_Texture, &src2, &put2);
		if (fcarrot) {
			SDL_RenderCopy(Renderer, Carrot_Texture, NULL, &Carrot_Rect);
		}
		switch (i) {
			case 0:SDL_RenderCopy(Renderer, Stand_Texture, NULL, &object[i].dest);
				break;
			case 1:SDL_RenderCopy(Renderer, Walk1_Texture, NULL, &object[i].dest);
				break;
			case 2:SDL_RenderCopy(Renderer, Walk2_Texture, NULL, &object[i].dest);
				break;
			case 3:SDL_RenderCopy(Renderer, Walk3_Texture, NULL, &object[i].dest);
				break;
			case 4:SDL_RenderCopy(Renderer, Walk4_Texture, NULL, &object[i].dest);
				break;
			case 5:SDL_RenderCopy(Renderer, Walk5_Texture, NULL, &object[i].dest);
				break;
			default:
				break;
		}
		SDL_RenderCopy(Renderer, TitleTexture2, NULL, &TitleRect2);
		SDL_RenderPresent(Renderer);

		// control fps
		long End = SDL_GetTicks();
		long Cost = End - Begin;
		long Frame = 1000 / 60;
		long Delay = Frame - Cost;
		if (Delay > 0) {
			SDL_Delay(Delay);
		}
	}
}