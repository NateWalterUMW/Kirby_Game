//Nathan Walter
//CPSC 440
//Lab 9

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include "player.h"
#include "ghost.h"
#include "Arrow.h"

using namespace std;

void getMessage(player& player, int playerLives, int ghostsShot, int sec, const int FPS);

int main(void)
{
	cout << "\nWelcome to the Kirby game!\n\n";
	cout << "Shoot the ghosts to avoid getting hit. If a ghost gets you, you take damage.\n\n";
	cout << "Left Arrow: move left\n";
	cout << "Right Arrow: move right\n";
	cout << "Up Arrow: move up\n";
	cout << "Down Arrow: move down\n";
	cout << "Spacebar: fire arrow\n";
	cout << "Escape: exit the game";

	const int WIDTH = 800;
	const int HEIGHT = 400;
	const int NUM_ArrowS = 5;
	const int NUM_ghostS = 10;
	int ms = 0;	//time in milliseconds
	enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
	bool keys[5] = { false, false, false, false, false };

	//primitive variable
	bool done = false;
	bool redraw = true;
	const int FPS = 60;

	//Allegro variables
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;

	//Initialization Functions
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	//initialize allegro addons
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	//object variables
	player myPlayer(HEIGHT);
	Arrow Arrows[NUM_ArrowS];
	ghost ghosts[NUM_ghostS];

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	srand(time(NULL));

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_start_timer(timer);
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			redraw = true;
			if (keys[UP])
				myPlayer.MoveUp();
			if (keys[DOWN])
				myPlayer.MoveDown(HEIGHT);
			if (keys[LEFT])
				myPlayer.MoveLeft();
			if (keys[RIGHT])
				myPlayer.MoveRight();

			for (int i = 0; i < NUM_ArrowS; i++)
				Arrows[i].UpdateArrow(WIDTH);
			for (int i = 0; i < NUM_ghostS; i++)
				ghosts[i].Startghost(WIDTH, HEIGHT);
			for (int i = 0; i < NUM_ghostS; i++)
				ghosts[i].Updateghost();
			for (int i = 0; i < NUM_ArrowS; i++)
				Arrows[i].CollideArrow(myPlayer, ghosts, NUM_ghostS, myPlayer.getGhostsShot());
			for (int i = 0; i < NUM_ghostS; i++)
				ghosts[i].Collideghost(myPlayer);

			getMessage(myPlayer, myPlayer.getLives(), myPlayer.getGhostsShot(), ms, FPS);		//display current player lives, ghosts killed on each timer event
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				for (int i = 0; i < NUM_ArrowS; i++)
					Arrows[i].FireArrow(myPlayer);
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			myPlayer.DrawPlayer();
			for (int i = 0; i < NUM_ArrowS; i++)
				Arrows[i].DrawArrow();
			for (int i = 0; i < NUM_ghostS; i++)
				ghosts[i].Drawghost();

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		if (myPlayer.getLives() == 0)	//if player is out of lives, then gameover
		{
			al_set_target_bitmap(al_get_backbuffer(display));
			al_clear_to_color(al_map_rgb(0, 0, 0));
			done = true;
		}
		ms++;
	}
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_rest(5.0);	//pause the screen for 5 seconds upon gameover
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);						//destroy our display object
	system("Pause");
	return 0;
}
/*This function is responsible for printing the player lives, ghosts killed and gameover stats to the screen.
@param int playerLives, int ghostsKilled
@return void*/
void getMessage(player& player, int playerLives, int ghostsShot, int sec, const int FPS)
{
	ALLEGRO_FONT* font18 = al_load_ttf_font("GROBOLD.ttf", 18, 0);
	ALLEGRO_FONT* font24 = al_load_ttf_font("GROBOLD.ttf", 24, 0);

	al_draw_text(font18, al_map_rgb(255, 255, 255), 8, 8, ALLEGRO_ALIGN_LEFT, "Kirby");

	//switch statement to decide how many lives to display on the screen
	switch (playerLives)
	{
	case 5:
		//al_draw_text(font18, al_map_rgb(0, 255, 0), 8, 8, ALLEGRO_ALIGN_LEFT, "I I I I I");		//these are remants from when I was using text to represent health, then I switched
		al_draw_filled_rectangle(58, 8, 178, 23, al_map_rgb(0, 255, 0));								//to using primitives to display a graphical health bar instead
		al_draw_filled_rectangle(58, 20, 178, 23, al_map_rgb(0, 255, 0));
		break;
	case 4:
		//al_draw_text(font18, al_map_rgb(0, 255, 0), 8, 8, ALLEGRO_ALIGN_LEFT, "I I I I");
		al_draw_filled_rectangle(58, 8, 178, 23, al_map_rgb(0, 30, 0));
		al_draw_filled_rectangle(58, 8, 153, 23, al_map_rgb(0, 255, 0));
		al_draw_filled_rectangle(58, 20, 178, 23, al_map_rgb(0, 255, 0));	//was 103
		break;
	case 3:
		//al_draw_text(font18, al_map_rgb(255, 255, 0), 8, 8, ALLEGRO_ALIGN_LEFT, "I I I");
		al_draw_filled_rectangle(58, 8, 178, 23, al_map_rgb(30, 30, 0));
		al_draw_filled_rectangle(58, 8, 128, 23, al_map_rgb(255, 255, 0));
		al_draw_filled_rectangle(58, 20, 178, 23, al_map_rgb(255, 255, 0));	//was 78
		break;
	case 2:
		//al_draw_text(font18, al_map_rgb(255, 255, 0), 8, 8, ALLEGRO_ALIGN_LEFT, "I I");
		al_draw_filled_rectangle(58, 8, 178, 23, al_map_rgb(30, 30, 0));
		al_draw_filled_rectangle(58, 8, 103, 23, al_map_rgb(255, 255, 0));
		al_draw_filled_rectangle(58, 20, 178, 23, al_map_rgb(255, 255, 0));	//was 53
		break;
	case 1:
		//al_draw_text(font18, al_map_rgb(255, 0, 0), 8, 8, ALLEGRO_ALIGN_LEFT, "I");
		al_draw_filled_rectangle(58, 8, 178, 23, al_map_rgb(30, 0, 0));
		al_draw_filled_rectangle(58, 8, 78, 23, al_map_rgb(255, 0, 0));
		al_draw_filled_rectangle(58, 20, 178, 23, al_map_rgb(255, 0, 0));		//was 28
		break;
	case 0:
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(font18, al_map_rgb(255, 0, 0), 8, 8, ALLEGRO_ALIGN_LEFT, "Out of lives");
		al_draw_text(font24, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "Gameover!");
		al_draw_textf(font18, al_map_rgb(255, 255, 255), 400, 230, ALLEGRO_ALIGN_CENTER, "(You shot %d ghosts in %d seconds)", ghostsShot, sec / FPS);
		break;
	}
	al_draw_textf(font18, al_map_rgb(255, 255, 255), 8, 33, ALLEGRO_ALIGN_LEFT, "Score: %d", ghostsShot);	//print number of ghosts shot
	al_draw_textf(font18, al_map_rgb(255, 255, 255), 792, 8, ALLEGRO_ALIGN_RIGHT, "%d s", sec / FPS);	//print seconds elapsed

	//display motivational messages based on how many ghost the player has shot
	if (ghostsShot >= 10 && ghostsShot < 20)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 300, 125, ALLEGRO_ALIGN_LEFT, "You can do better");
	}
	else if (ghostsShot >= 20 && ghostsShot < 30)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 25, 200, ALLEGRO_ALIGN_LEFT, "Not bad");
	}
	else if (ghostsShot >= 30 && ghostsShot < 40)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 650, 75, ALLEGRO_ALIGN_LEFT, "You're OK");
	}
	else if (ghostsShot >= 40 && ghostsShot < 50)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 500, 300, ALLEGRO_ALIGN_LEFT, "Nice job!");
	}
	else if (ghostsShot >= 50 && ghostsShot < 60)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 100, 50, ALLEGRO_ALIGN_LEFT, "You're pretty good!");
	}
	else if (ghostsShot >= 60 && ghostsShot < 70)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 175, 275, ALLEGRO_ALIGN_LEFT, "You rock!");
	}
	else if (ghostsShot >= 70 && ghostsShot < 80)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 450, 110, ALLEGRO_ALIGN_LEFT, "Amazing!!");
	}
	else if (ghostsShot >= 80 && ghostsShot < 90)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 180, 280, ALLEGRO_ALIGN_LEFT, "Increible!!");
	}
	else if (ghostsShot >= 90 && ghostsShot < 100)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 80, 350, ALLEGRO_ALIGN_LEFT, "You're awesome!!");
	}
	else if (ghostsShot >= 100)
	{
		al_draw_text(font18, al_map_rgb(255, 255, 255), 600, 80, ALLEGRO_ALIGN_LEFT, "Legendary!!!");
	}
	al_destroy_font(font18);
	al_destroy_font(font24);
}