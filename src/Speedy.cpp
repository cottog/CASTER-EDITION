#include "main.hpp"


Speedy::Speedy(float maxSpeed) : speed(maxSpeed),maxSpeed(maxSpeed){
}

void Speedy::save(TCODZip &zip){
	zip.putFloat(speed);
	zip.putFloat(maxSpeed);
}

void Speedy::load(TCODZip &zip){
	speed = zip.getFloat();
	maxSpeed = zip.getFloat();
}