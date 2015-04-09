class Scheduler{
public:
	MaxActorHeap heap;

	Scheduler();
	~Scheduler();
	void run();
	void add(Actor *actor);
	void addFromList(TCODList<Actor *> &list); //this does not destroy the list passed in
	void clear();
};