//Nathan Walter
//CPSC 440
//Lab 9

#include "player.h"
#include "ghost.h"

class Arrow
{
public:
	Arrow();
	~Arrow();
	void DrawArrow();
	void FireArrow(player& Player);
	void UpdateArrow(int WIDTH);
	void CollideArrow(player& Player, ghost ghosts[], int cSize, int ghostsShot);
private:
	int x;
	int y;
	bool live;
	int speed;
	ALLEGRO_BITMAP* image;
};


