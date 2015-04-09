class Scheduler{
public:
	MinActorHeap heap;

	Scheduler();
	~Scheduler();
	void run();
	void add(Actor *actor);
	void addFromList(TCODList<Actor *> list); //this does not destroy the list
	void clear();
};