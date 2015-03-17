class Actor : public Persistent {
public:
	int ID;   //used to identify similar actors, helps stacking
	int x, y; //position on map
	int ch; //ascii code
	TCODColor col; //color for character
	bool blocks; //can we walk on this actor?
	Attacker *attacker; //does this actor deal damage?
	Destructible *destructible; //can this actor be destroyed?
	Ai *ai; //does this actor update itself?
	Pickable *pickable; //can this actor be picked up and used?
	Container *container; //can this actor contain other actors?
	TCODList<Aura *> auras;
	
	Actor(int x, int y, int ch, const char *name, const TCODColor &col);
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