class Engine{
public:
	enum GameStatus{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT
	} gameStatus;
	
	TCODConsole *mapcon;
	TCODList<Actor *> actors;
	Actor *player;
	Actor *stairs;
	Map *map;
	int fovRadius;
	int screenWidth;
	int screenHeight;
	int mapWidth;
	int mapHeight;
	int level;
	Gui *gui;
	TCOD_key_t lastKey;
	
	Engine(int screenWidth, int screenHeight);
	~Engine();
	void update();
	void render();
	void sendToBack(Actor *actor);
	Actor *getClosestMonster(int x, int y, float range) const;
	bool pickATile(int *x, int *y, float maxRange = 0.0f, float AOE = 0.0f);
	Actor *getActor(int x, int y) const;
	void init();
	void term();
	void load(bool pause = false);
	void save();
	void nextLevel();
	float distance(int x1, int x2, int y1, int y2);  //returns the distance between two cells
	const char* chooseSaveFile(TCODList<const char *> savepaths);   //returns the path to the save file that should be loaded
	void fullscreen(); //make it fullscreen if it isn't, or make it smaller if it is 
	Actor *chooseFromList(TCODList<Actor*> list,const char *title);
};

extern Engine engine;