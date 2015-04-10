class Speedy : public Persistent{
public:	
	float speed;
	float maxSpeed;	

	Speedy(float maxSpeed);
	void save(TCODZip &zip);
	void load(TCODZip &zip);
};