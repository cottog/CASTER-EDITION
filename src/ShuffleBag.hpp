template <class type_T>
class ShuffleBag{	//a shufflebag is a class I am implementing to add some filtered randomness
public:
	ShuffleBag();	//constructor
	~ShuffleBag();	//destructor; deletes the TCODList
	void add(type_T item);	//adds an item to the TCODList
	type_T retrieve();	//retrieves a random element from the TCODList, removing it from the TCODList in the process
	bool isEmpty();		//returns true if the bag is empty, false if not
protected:
	TCODList<type_T> bag;
};

template <class type_T>
ShuffleBag<type_T>::ShuffleBag(){
}

template <class type_T>
ShuffleBag<type_T>::~ShuffleBag(){
}

template <class type_T>
void ShuffleBag<type_T>::add(type_T item){
	bag.push(item);
}

template <class type_T>
type_T ShuffleBag<type_T>::retrieve(){
	TCODRandom *rng = TCODRandom::getInstance();
	int index = rng->getInt(0,bag.size()-1);
	type_T item = bag.get(index);
	bag.remove(index);
	return item;
}

template <class type_T>
bool ShuffleBag<type_T>::isEmpty(){
	return bag.isEmpty();
}




template <class type_T>
class ShuffleBag<type_T*>{	//specialize the shufflebag so it can clearAndDelete() a TCODList of pointers
public:
	ShuffleBag();	//constructor
	~ShuffleBag();	//destructor; deletes the TCODList
	void add(type_T* item);	//adds an item to the TCODList
	type_T* retrieve();	//retrieves a random element from the TCODList, removing it from the TCODList in the process
	bool isEmpty();		//returns true if the bag is empty, false if not
protected:
	TCODList<type_T> bag;
};



template <class type_T>
ShuffleBag<type_T*>::ShuffleBag(){
}

template <class type_T>
ShuffleBag<type_T*>::~ShuffleBag(){
	bag.clearAndDelete();
}

template <class type_T>
void ShuffleBag<type_T*>::add(type_T* item){
	bag.push(item);
}

template <class type_T>
type_T* ShuffleBag<type_T*>::retrieve(){
	TCODRandom *rng = TCODRandom::getInstance();
	int index = rng->getInt(0,bag.size()-1);
	type_T* item = bag.get(index);
	bag.remove(index);
	return item;
}

template <class type_T>
bool ShuffleBag<type_T*>::isEmpty(){
	return bag.isEmpty();
}