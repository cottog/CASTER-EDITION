class Actor : public Persistent {
public:
	int ID;   //used to identify similar actors, helps stacking
	int x, y; //position on map
	int ch; //ascii code
	int trueCh;	//the actual ascii code, in case they need to get rezzed
	TCODColor col; //color for character
	TCODColor trueCol;	//the actual color for the character, in case they need to get rezzed
	bool hostile; //is it hostile to the player?
	bool blocks; //can we walk on this actor?
	int xpLevel; //what level is this Actor?
	Attacker *attacker; //does this actor deal damage?
	Destructible *destructible; //can this actor be destroyed?
	Ai *ai; //does this actor update itself?
	Pickable *pickable; //can this actor be picked up and used?
	Container *container; //can this actor contain other actors?
	Caster *caster; //can this actor cast spells?
	Speedy *speedy;
	TCODList<Aura *> auras;

	
	Actor(int x, int y, int ch, const char *name, const TCODColor &col, bool hostile = true, int xpLevel = 1);
	Actor(const Actor &actor);	//copy constructor; doesn't work for Pickable actors, as I dont expect to use it for that
	~Actor();
	void update();
	void render() const;
	float getDistance(int cx, int cy) const;
	void load(TCODZip &zip);
	void save(TCODZip &zip);
	void updateAuras();
	const char* getName(bool real = false); //real == true only returns the actor's real name, where as == false may return corpseName
	void setName(const char* name);
	bool isVisible();
protected:
	const char *name; //the actor's name
};