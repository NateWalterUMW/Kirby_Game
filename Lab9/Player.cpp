//Nathan Walter
//CPSC 440
//Lab 9

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Player.h"

player::~player()
{
	for (int i = 0; i < 5; i++)		//deconstruct all 5 player bitmaps
	{
		al_destroy_bitmap(image[i]);
	}
}
player::player(int HEIGHT)
{
	//load each kirby bitmap into the array
	image[0] = al_load_bitmap("Kirby0.png");
	image[1] = al_load_bitmap("Kirby1.png");
	image[2] = al_load_bitmap("Kirby2.png");
	image[3] = al_load_bitmap("Kirby3.png");
	image[4] = al_load_bitmap("Kirby4.png");

	x = 20;
	y = HEIGHT / 2;
	lives = 5;
	ghostsShot = 0;
	speed = 7;
	boundx = al_get_bitmap_width(image[0]);
	boundy = al_get_bitmap_height(image[0]);
	score = 0;
}
void player::DrawPlayer()
{
	//switch statement to decide which player bitmap to display depending on lives left
	switch (lives)
	{
	case 5:
		al_draw_bitmap(image[0], x, y, 0);
		break;
	case 4:
		al_draw_bitmap(image[1], x, y, 0);
		break;
	case 3:
		al_draw_bitmap(image[2], x, y, 0);
		break;
	case 2:
		al_draw_bitmap(image[3], x, y, 0);
		break;
	case 1:
		al_draw_bitmap(image[4], x, y, 0);
		break;
	case 0:
		al_draw_bitmap(image[4], x, y, 0);
	}
}
void player::MoveUp()
{
	y -= speed;
	if (y < -4)		//was y < 0, I changed it to make kirby more flush with the top bound
		y = -4;
}
void player::MoveDown(int HEIGHT)
{
	y += speed;
	if (y > HEIGHT - boundy + 3)	//fixed collision detection with the bottom bound
		y = HEIGHT - boundy + 3;
}
void player::MoveLeft()
{
	x -= speed;
	if (x < -5)		//was x < 0, I changed it to make kirby more flush with the left bound
		x = -5;
}
void player::MoveRight()
{
	x += speed;
	if (x > 300)
		x = 300;
}
