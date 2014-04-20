#ifndef play_h
#define play_h


typedef struct 
{
	float x;
	float y;
	float dx; 
	float dy;
	float step;
	int score;
} player_t;


class Player {
private:
	player_t player;

	static const int width;
	static const int height;

	// ctr
	Player() {}
	Player(const Player&) = delete;
	// operator=
	Player& operator=(const Player&) = delete;

protected:
	void StartInit( player_t* player) 
	{
		player->x = width/2;
		player-> y = height - height/10;
		player-> dx = width/6.83; //=150; 
		player-> dy = height/25.6; //=30;
		player-> step = width/341.3; //=3;
		player -> score = 0;
	}
public:
	static Player* Instance() 
	{
		static Player player;
		return &player;
	}
	void Start() { StartInit(&player); }
	player_t* getPlayer() { return &player; }
};

class Enemy {
private:
	player_t enemy;

	static const int width;
	static const int height;

	// ctr
	Enemy() {}
	Enemy(const Enemy&) = delete;
	// operator=
	Enemy& operator=(const Enemy&) = delete;

protected:
	void StartInit( player_t* player) 
	{
		player->x = width/2;
		player-> y = height/10;
		player-> dx = width/6.83; //=150; 
		player-> dy = height/25.6; //=30;
		player-> step = width/512; 
		player -> score = 0;
	}
public:
	static Enemy* Instance() 
	{
		static Enemy enemy;
		return &enemy;
	}
	void Start() { StartInit(&enemy); }
	player_t* getEnemy() { return &enemy; }
};


#endif
