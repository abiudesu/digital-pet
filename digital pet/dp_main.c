#include<stdio.h>
#include<stdlib.h>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>

void QUIT();
void LOADMain();
void LOADTitle();
void LOADRoom();
void LOADRubbish();
void DrawRubbish();
void RectShapeonMouseEvent(SDL_Event* event, int i);

int scene;

int rtype[2];
int rDragenable = 0;
SDL_FPoint rDragStartPoint;
SDL_FPoint rDragStartMousePoint;
SDL_FPoint currentMousePoint;
SDL_FRect Rubbishrect[2];

SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;

SDL_Surface* MainSurface = NULL;
SDL_Texture* MainTexture = NULL;
SDL_Rect MainRect;

SDL_Surface* StartSurface = NULL;
SDL_Texture* StartTexture = NULL;
SDL_Rect StartRect;

SDL_Surface* Rubbish_Surface = NULL;
SDL_Texture* Rubbish_Texture = NULL;
SDL_Rect Rubbish_Rect;

TTF_Font* TitleFont = NULL;
SDL_Surface* TitleSurface = NULL;
SDL_Texture* TitleTexture = NULL;
SDL_Rect TitleRect;
SDL_Color Titlecolor = { 109,56,50,255 };


int SDL_main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Event MainEvent;
	Window = SDL_CreateWindow("Digital Pet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 710, 710, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	int scene = 1;

	LOADMain();
	LOADTitle();
	//SDL_Delay(5000);

	SDL_FreeSurface(MainSurface);
	SDL_FreeSurface(TitleSurface);
	SDL_DestroyTexture(MainTexture);
	SDL_DestroyTexture(TitleTexture);
	TTF_CloseFont(TitleFont);
	SDL_RenderClear(Renderer);
	LOADRoom();
	LOADRubbish();
	SDL_RenderPresent(Renderer);

	while (1) {
		long Begin = SDL_GetTicks();
		if (scene == 1) {
			while (SDL_PollEvent(&MainEvent)) {
				switch (MainEvent.type) {
					int i;
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					for (i = 0; i < 2; i++) {
						RectShapeonMouseEvent(&MainEvent,i);
					}
					break;
				case SDL_QUIT:
					QUIT();
					break;
				default:
					break;
				}
			}
			long End = SDL_GetTicks();
			long Cost = End - Begin;
			long Frame = 1000 / 60;
			long Delay = Frame - Cost;
			if (Delay > 0) {
				SDL_Delay(Delay);
			}
		}
	}



	return 0;
}

void LOADMain() {
	MainSurface = IMG_Load("R.jpg");
	MainTexture = SDL_CreateTextureFromSurface(Renderer, MainSurface);
	MainRect.x = 0;
	MainRect.y = 0;
	MainRect.w = 710;
	MainRect.h = 710;
	SDL_RenderClear(Renderer);
	SDL_RenderCopy(Renderer, MainTexture, NULL, &MainRect);
	SDL_RenderPresent(Renderer);
}

void LOADTitle() {
	TitleFont = TTF_OpenFont("OPlusSans3-Bold.ttf", 50);
	TitleSurface = TTF_RenderUTF8_Blended(TitleFont, "Digital Pet", Titlecolor);
	TitleTexture = SDL_CreateTextureFromSurface(Renderer, TitleSurface);
	TitleRect.x = 160;
	TitleRect.y = 18;
	TitleRect.w = 370;
	TitleRect.h = 96;
	SDL_RenderCopy(Renderer, TitleTexture, NULL, &TitleRect);
	SDL_RenderPresent(Renderer);
}

void LOADRoom() {
	StartSurface = IMG_Load("room1.png");
	StartTexture = SDL_CreateTextureFromSurface(Renderer, StartSurface);
	StartRect.x = 0;
	StartRect.y = 0;
	StartRect.w = 710;
	StartRect.h = 710;
	SDL_RenderCopy(Renderer, StartTexture, NULL, &StartRect);
}
//随机生成两个垃圾分别把位置储存在rubbishrect中
void LOADRubbish() {
	srand(time(NULL));
	int bop, i;
	for (i = 0; i < 2; i++) {
		bop = 1 + (int)(10.0 * rand() / (RAND_MAX + 1.0));
		if (bop % 2 == 1) {
			Rubbish_Surface = IMG_Load("banana.png");
			Rubbish_Texture = SDL_CreateTextureFromSurface(Renderer, Rubbish_Surface);
			Rubbishrect[i].x = Rubbish_Rect.x = (int)(401 * rand() / (RAND_MAX + 1.0));
			Rubbishrect[i].y = Rubbish_Rect.y = 500 + (int)(181 * rand() / (RAND_MAX + 1.0));
			Rubbishrect[i].w = Rubbish_Rect.w = 60;
			Rubbishrect[i].h = Rubbish_Rect.h = 60;
			rtype[i] = 1;
			SDL_RenderCopy(Renderer, Rubbish_Texture, NULL, &Rubbish_Rect);
		}
		else {
			Rubbish_Surface = IMG_Load("paper.png");
			Rubbish_Texture = SDL_CreateTextureFromSurface(Renderer, Rubbish_Surface);
			Rubbishrect[i].x = Rubbish_Rect.x = (int)(401 * rand() / (RAND_MAX + 1.0));
			Rubbishrect[i].y = Rubbish_Rect.y = 500 + (int)(181 * rand() / (RAND_MAX + 1.0));
			Rubbishrect[i].w = Rubbish_Rect.w = 60;
			Rubbishrect[i].h = Rubbish_Rect.h = 60;
			rtype[i] = 2;
			SDL_RenderCopy(Renderer, Rubbish_Texture, NULL, &Rubbish_Rect);
		}
	}
}
//把两个垃圾和背景呈现在屏幕上
void DrawRubbish() {
	LOADRoom();
	int i;
	for (i = 0; i < 2; i++) {
		Rubbish_Rect.x = Rubbishrect[i].x;
		Rubbish_Rect.y = Rubbishrect[i].y;
		Rubbish_Rect.w = Rubbishrect[i].w;
		Rubbish_Rect.h = Rubbishrect[i].h;
		if (rtype[i] == 1) {
			Rubbish_Surface = IMG_Load("banana.png");
			Rubbish_Texture = SDL_CreateTextureFromSurface(Renderer, Rubbish_Surface);
		}
		else {
			Rubbish_Surface = IMG_Load("paper.png");
			Rubbish_Texture = SDL_CreateTextureFromSurface(Renderer, Rubbish_Surface);
		}
		SDL_RenderCopy(Renderer, Rubbish_Texture, NULL, &Rubbish_Rect);
	}
	SDL_RenderPresent(Renderer);
}
//b站陈云老师视频里将鼠标指令封装成函数，方便对多个对象执行
void RectShapeonMouseEvent(SDL_Event *event,int i) {
	switch (event->type) {
	case SDL_MOUSEMOTION:
		if (rDragenable == 1) {
			//for (i = 0; i < 2; i++) {
			Rubbishrect[i].x = rDragStartPoint.x + event->motion.x - rDragStartMousePoint.x;
			Rubbishrect[i].y = rDragStartPoint.y + event->motion.y - rDragStartMousePoint.y;
			DrawRubbish();
			//}		
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		currentMousePoint.x = event->button.x;
		currentMousePoint.y = event->button.y;
		if (SDL_PointInRect(&currentMousePoint, &Rubbishrect[i])) {
			rDragStartPoint.x = Rubbishrect[i].x;
			rDragStartPoint.y = Rubbishrect[i].y;
			rDragStartMousePoint.x = event->button.x;
			rDragStartMousePoint.y = event->button.y;
			rDragenable = 1;
			}
		break;
	case SDL_MOUSEBUTTONUP:
		rDragenable = 0;
		break;
	case SDL_QUIT:
		QUIT();
		break;
	default:
		break;
	}
}

void QUIT() {
	SDL_FreeSurface(StartSurface);
	SDL_FreeSurface(Rubbish_Surface);
	SDL_DestroyTexture(MainTexture);
	SDL_DestroyTexture(TitleTexture);
	SDL_DestroyTexture(StartTexture);
	SDL_DestroyTexture(Rubbish_Texture);
	TTF_CloseFont(TitleFont);
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();
}