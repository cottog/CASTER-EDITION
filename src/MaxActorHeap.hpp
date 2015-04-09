#include <vector>

class MaxActorHeap{
private:
	std::vector<Actor *> _vector;
	
	void BubbleDown(int index);
	void BubbleUp(int index);
	void Heapify();

public:
	MaxActorHeap();
	~MaxActorHeap();

	void clear();
	void push(Actor *newActor);
	Actor *pop();
	Actor *peek(int i = 0); //return the actor located at this index without removing it
	bool isEmpty(){ return _vector.empty();}
	int size() {return _vector.size();}
};