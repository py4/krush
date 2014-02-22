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

void select_object(Board& board, SDL_Event event,vector <Object>& selected_objects)
{
	if(!is_valid_click(board,event))
	{
		cout << "not a valid click!" << endl;
		return;
	}
	
	int x = event.button.x;
	int y = event.button.y;
//	cout << "selected:  " << endl;
//	cout << y / 50 << " " << x / 50 << endl;
	apply_surface((x / 50) * 50, (y / 50) * 50,board.selected_object,board.screen);
	SDL_Flip(board.screen);
	selected_objects.push_back(board.objects[y/50][x/50]);
}

void unselect_object(Board& board,Object object)
{
		SDL_Rect rect;
   	rect.x = object.j * 50;
		rect.y = object.i * 50;
		rect.h = 50;
		rect.w = 50;
		SDL_FillRect(board.screen,&rect,0x000000);
		apply_surface(rect.x,rect.y,object.image,board.screen);
		SDL_Flip(board.screen);
}

void dump_board_without(Board& board, Object obj1,Object obj2)
{
	SDL_Rect rect;
	rect.h = SCREEN_HEIGHT;
	rect.w = SCREEN_WIDTH;
	SDL_FillRect(board.screen,NULL,0x000000);
	for(int i = 0; i < board.row_count; i++)
		for(int j = 0; j < board.column_count; j++)
		{
			if(obj1.i == i and obj1.j == j)
				continue;
			if(obj2.i == i and obj2.j == j)
				continue;
			apply_surface(j*50,i*50,board.objects[i][j].image,board.screen);
			}

	SDL_Flip(board.screen); 
}

void dump_board_without(Board board,vector <Object> objects, SDL_Surface*& screen)
{
	SDL_Rect rect;
	rect.h = SCREEN_HEIGHT;
	rect.w = SCREEN_WIDTH;
	SDL_FillRect(screen,NULL,0x000000);
	bool check = false;
	for(int i = 0; i < board.row_count; i++)
		for(int j = 0; j < board.column_count; j++)
		{
			for(int k = 0; k < objects.size(); k++)
				if(objects[k].i == i and objects[k].j == j)
					check = true;
			if(check)
			{
				check = false;
				continue;
			}
			apply_surface(j*50,i*50,board.objects[i][j].image,screen);
	 }
	SDL_Flip(screen); 
}

void move_to(Board& board,Object& object, vector <Object> removed_objects,int dest_x, int dest_y)
{
	SDL_Rect rect;
	rect.x = object.j * 50;
	rect.y = object.i * 50;
	while((rect.x != dest_x) or (rect.y != dest_y))
	{
		dump_board_without(board,removed_objects,board.screen);
		if(rect.x < dest_x)
			rect.x += 1;
		else if(rect.x > dest_x)
			rect.x -= 1;

		if(rect.y < dest_y)
			rect.y += 1;
		else if(rect.y > dest_y)
			rect.y -= 1;
		SDL_BlitSurface(object.image,NULL,board.screen,&rect);
		SDL_Flip(board.screen);
		SDL_Delay(3);
	}
}

void rotate_in_graphic(Board& board,Object& obj1,Object& obj2)
{
	cout << "======= rotate in graphic =====" << endl;
	cout << "obj1:  " << obj1.i << " " << obj1.j << endl;
	cout << "obj2:  " << obj2.i << " " << obj2.j << endl;
	cout << endl;
	SDL_Rect rect1,rect2;
	rect1.x = obj1.j * 50;
	rect1.y = obj1.i * 50;
	rect2.x = obj2.j * 50;
	rect2.y = obj2.i* 50;
	bool checked = false;
	while(true)
	{
		if((rect1.x != obj2.j*50) or (rect1.y != obj2.i*50))
		{
			if(!checked)
			{
				dump_board_without(board,obj1,obj2);
				checked = true;
			}
			if(rect1.x < obj2.j*50)
				rect1.x += 1;
			else if(rect1.x > obj2.j*50)
				rect1.x -= 1;

			if(rect1.y < obj2.i*50)
				rect1.y += 1;
			else if(rect1.y > obj2.i*50)
				rect1.y -= 1;
			SDL_BlitSurface(obj1.image,NULL,board.screen,&rect1);
		}
		else
			break;
		
		if((rect2.x != obj1.j*50) or (rect2.y != obj1.i*50))
		{
			if(!checked)
			{
				dump_board_without(board,obj1,obj2);
				checked = true;
			}
			
			if(rect2.x < obj1.j*50)
				rect2.x += 1;
			else if(rect2.x > obj1.j*50)
				rect2.x -= 1;

			if(rect2.y < obj1.i*50)
				rect2.y += 1;
			else if(rect2.y > obj1.i*50)
				rect2.y -= 1;
			SDL_BlitSurface(obj2.image,NULL,board.screen,&rect2);
		}
		else
			break;

		checked = false;
		SDL_Flip(board.screen);
		SDL_Delay(3);
		
	}
}

void remove_object_from_screen(Board& board,Object object)
{
	SDL_Rect rect;
	rect.x = object.j*50;
	rect.y = object.i*50;
	rect.h = 59;
	rect.w = 59;
	SDL_FillRect(board.screen,&rect,0x000000);
	SDL_Flip(board.screen);
}

void reload_screen(Board& board)
{
	SDL_Rect rect;
	rect.h = SCREEN_HEIGHT;
	rect.w = SCREEN_WIDTH;
	SDL_FillRect(board.screen,NULL,0x000000);
	for(int i = 0; i < board.row_count; i++)
		for(int j = 0; j < board.column_count; j++)
			apply_surface(j*50,i*50,board.objects[i][j].image,board.screen);
	SDL_Flip(board.screen);
}


bool init_screen(Board& board)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		cout << "coudln't init" << endl;
		return false;
	}
	board.screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
	if(board.screen == NULL)
	{
		cout << "set video mode failed:  " << endl;
		cout << SDL_GetError() << endl;
		return false;
	}

	SDL_WM_SetCaption("Krush",NULL);
	SDL_Surface* blue = load_image("img/b.bmp");
	SDL_Surface* red = load_image("img/r.bmp");
	SDL_Surface* green = load_image("img/g.bmp");
	SDL_Surface* orange = load_image("img/o.bmp");
	SDL_Surface* yellow = load_image("img/y.bmp");
	SDL_Surface* dark = load_image("img/dark.bmp");
	board.images[0] = red;
	board.images[1] = blue;
	board.images[2] = green;
	board.images[3] = yellow;
	board.images[4] = orange;
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
				board.objects[i][j].image = orange;
				break;
			case 'y':
				board.objects[i][j].image = yellow;
				break;
		  }

			apply_surface(j*50,i*50,board.objects[i][j].image,board.screen);
	 }

	SDL_Flip(board.screen);
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




