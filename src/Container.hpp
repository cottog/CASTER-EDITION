class Container : public Persistent {
public:
	int size; //max number of actors. 0=unlimited
	int wallet; //how much currency you have
	TCODList<Actor *> inventory; 
	//the following actor pointers are armor slots
	Actor *head;  
	Actor *chest;
	Actor *legs;
	Actor *feet;
	Actor *hand1;
	Actor *hand2;
	Actor *gloves;
	Actor *ranged;
	
	Container(int size);
	~Container();
	bool add (Actor *actor);
	void remove(Actor *actor);
	void load(TCODZip &zip);
	void save(TCODZip &zip);
	void sendToBegin(Actor *actor);
};