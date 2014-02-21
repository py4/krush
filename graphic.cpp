#include "graphic.h"
#include <iostream>
using namespace std;

void apply_surface(int x, int y,SDL_Surface*& source,SDL_Surface*& destination)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(source,NULL,destination,&offset);
	
}

bool is_valid_click(Board board, SDL_Event event)
{
	int x = event.button.x;
	int y = event.button.y;

	if((y / 50) > board.row_count)
		return false;
	if((x / 50) > board.column_count)
		return false;
	if(x < 0)
		return false;
	if(y < 0)
		return false;

	return true;
}

void select_object(Board board, SDL_Event event, SDL_Surface*& screen,vector <Object>& selected_objects)
{
	if(!is_valid_click(board,event))
	{
		cout << "not a valid click!" << endl;
		return;
	}
	
	int x = event.button.x;
	int y = event.button.y;
	cout << "selected:  " << endl;
	cout << y / 50 << " " << x / 50 << endl;
	apply_surface((x / 50) * 50, (y / 50) * 50,board.selected_object,screen);
	SDL_Flip(screen);
	selected_objects.push_back(board.objects[y/50][x/50]);
}

void unselect_object(Board board, SDL_Surface*& screen,Object object)
{
		SDL_Rect rect;
   	rect.x = object.j * 50;
		rect.y = object.i * 50;
		rect.h = 50;
		rect.w = 50;
		SDL_FillRect(screen,&rect,0x000000);
		apply_surface(rect.x,rect.y,object.image,screen);
		SDL_Flip(screen);
}


bool init_screen(SDL_Surface*& screen, Board& board)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		cout << "coudln't init" << endl;
		return false;
	}
	screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
	if(screen == NULL)
	{
		cout << "set video mode failed:  " << endl;
		cout << SDL_GetError() << endl;
		return false;
	}

	SDL_WM_SetCaption("Krush",NULL);
	cout << "[init_scree] is null:  " << (screen == NULL) << endl;
	SDL_Surface* blue = load_image("img/b.bmp");
	SDL_Surface* red = load_image("img/r.bmp");
	SDL_Surface* green = load_image("img/g.bmp");
	SDL_Surface* orange = load_image("img/o.bmp");
	SDL_Surface* yellow = load_image("img/y.bmp");
	SDL_Surface* dark = load_image("img/dark.bmp");
	board.selected_object = dark;
	
	for(int i = 0; i < board.row_count ; i++)
		for(int j = 0; j < board.column_count; j++)
		{
			switch(board.objects[i][j].color)
			{
			case 'b':
				board.objects[i][j].image = blue;
				break;
			case 'r':
				board.objects[i][j].image = red;
				break;
			case 'g':
				board.objects[i][j].image = green;
				break;
			case 'o':
				cout << "[o colors]:  " << endl;
				cout << i << " " << j << endl;
				board.objects[i][j].image = orange;
				break;
			case 'y':
				board.objects[i][j].image = yellow;
				break;
		  }

			apply_surface(j*50,i*50,board.objects[i][j].image,screen);
	 }

	SDL_Flip(screen);


	

	return true;
}

SDL_Surface* load_image(string file_name)
{
	SDL_Surface* temp_image = NULL;
	SDL_Surface* image = NULL;
	temp_image = IMG_Load(file_name.c_str());
	if(temp_image != NULL)
	{
		image = SDL_DisplayFormat(temp_image);
		SDL_SetColorKey(image,SDL_SRCCOLORKEY,0xffffff);
		SDL_FreeSurface(temp_image);
	}
	else
	{
		cout << "IMG_load failed: " << endl;
		cout << SDL_GetError() << endl;
	}
	return image;
}



