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
	Scheduler scheduler;
	
	Engine(int screenWidth, int screenHeight);
	~Engine();
	void update();
	void render(bool changeGameStatus = true);
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
	Actor *chooseFromList(TCODList<Actor*> &list,const char *title); //takes any list of actors and allows the player to choose an element from it
	void getAllActorsInRadius(TCODList<Actor *> &targets, int x, int y, float radius = 0.0, TargetingSystemBase::TargetType targetType = TargetingSystemBase::NO_TYPE, bool casterHostility = false) const; //this will add to the passed list all living actors that are within (radius) of the specified tile (x,y), and can discriminate based on TargetType if need be 
	//for the above function, a radius of 0 may add all living actors in the level
	//for the above function, if using it for Spells, you can tell it what sort of TargetType to look for (all creatures, enemy creatures, or friendly creatures), as well as a bool "hostility" param, which in the case of spells should be the hostility of the caster
	bool findNearbyOpenTile(int *x, int *y); //returns the coords of an open tile that is adjacent (or equal) to the set of coordinates passed to the function;	returns the coords in the form x*(mapWidth+1)+y
	
};

extern Engine engine;