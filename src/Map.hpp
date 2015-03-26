struct Tile {
    bool explored;	//have we walked through this tile?
    bool blocked;	//can we walk through this tile?
    Tile() : explored(false),blocked(false) {}
};
 
class Map : public Persistent {
public :
    int width,height;
    Tile *tiles; //no longer protected as it will be used to determine if a 
 
    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
	bool isInFov(int x, int y) const;
	bool isExplored(int x, int y) const;
	bool canWalk(int x, int y) const;
	void computeFov();
	void exploreTile(int x, int y); //this should typically be used for detection or magic mapping spells
 	void render() const;
	void init(bool withActors);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
protected :
    
	TCODMap *map;
	long seed;
	TCODRandom *banana;
    friend class BspListener;
 
    void dig(int x1, int y1, int x2, int y2);
    void createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors);
	void addMonster(int x, int y);
	void addItem(int x, int y);
};