//Nathan Walter
//CPSC 440
//Lab 9

#ifndef PLAYERH
#define PLAYERH
class player
{
public:
	player(int HEIGHT);
	~player();
	void DrawPlayer();
	void MoveUp();
	void MoveDown(int HEIGHT);
	void MoveLeft();
	void MoveRight();
	int getBoundX() { return boundx; }
	int getBoundY() { return boundy; }
	int getX() { return x; }
	int getY() { return y; }
	void removeLife() { lives--; }
	int getLives() { return lives; }	//I added this function to get player lives
	int getGhostsShot() { return ghostsShot; }	//I added this function to get number of ghosts shot
	void setGhostsShot(int num) { ghostsShot++; }	//I added this function to increment the number of ghosts shot with each arrow-ghost collosion
private:
	int x;
	int y;
	int lives;
	int ghostsShot;
	int speed;
	int boundx;
	int boundy;
	int score;
	ALLEGRO_BITMAP* image[5];	//I changed this into an array to hold all 5 kirby bitmaps
};
#endif
