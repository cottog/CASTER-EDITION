#include "main.hpp"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;
static const int MAX_ROOM_ITEMS = 2;


class BspListener : public ITCODBspCallback {
private :
    Map &map; // a map to dig
    int roomNum; // room number
    int lastx,lasty; // center of the last room
public :
    BspListener(Map &map) : map(map), roomNum(0) {}
    bool visitNode(TCODBsp *node, void *userData) {
    	if ( node->isLeaf() ) {    
    		int x,y,w,h;
			bool withActors = (bool)userData;
			// dig a room
			w=map.banana->getInt(ROOM_MIN_SIZE, node->w-2);
			h=map.banana->getInt(ROOM_MIN_SIZE, node->h-2);
			x=map.banana->getInt(node->x+1, node->x+node->w-w-1);
			y=map.banana->getInt(node->y+1, node->y+node->h-h-1);
			map.createRoom(roomNum == 0, x, y, x+w-1, y+h-1,withActors);
			if ( roomNum != 0 ) {
			    // dig a corridor from last room
			    map.dig(lastx,lasty,x+w/2,lasty);
			    map.dig(x+w/2,lasty,x+w/2,y+h/2);
			}
            lastx=x+w/2;
            lasty=y+h/2;
            roomNum++;
        }
        return true;
    }
};

Map::Map(int width, int height) : width(width),height(height) {
    seed = TCODRandom::getInstance()->getInt(0,0x7FFFFFFF);
}

Map::~Map() {
    delete [] tiles;
	delete map;
	delete banana;
}

void Map::init(bool withActors) {
	banana = new TCODRandom(seed,TCOD_RNG_CMWC);
	tiles = new Tile[width*height];
	map = new TCODMap(width, height);
	TCODBsp bsp(0,0,width,height);
	bsp.splitRecursive(banana,8,ROOM_MAX_SIZE,ROOM_MAX_SIZE,1.5f,1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener,(void *)withActors);
}

void Map::save(TCODZip &zip) {
	zip.putInt(seed);
	for (int i=0; i< width*height; i++) {
		zip.putInt(tiles[i].explored);
		zip.putInt(tiles[i].blocked);
	}
}

void Map::load(TCODZip &zip) {
	seed = zip.getInt();
	init(false);
	for (int i =0; i<width*height; i++) {
		tiles[i].explored = zip.getInt();
		tiles[i].blocked = zip.getInt();
	}
}

void Map::dig(int x1, int y1, int x2, int y2) {
	//use these corners to save room coords, sizes, and room numbers in a struct
	//put the struct in a TCODList saved by the Map object
	//possibly add room number to the properties struct so room info can be looked up by room number
	//save the room number as a property so that a tile can be asked what room # its in

    if ( x2 < x1 ) {
        int tmp=x2;
        x2=x1;
        x1=tmp;
    }
    if ( y2 < y1 ) {
        int tmp=y2;
        y2=y1;
        y1=tmp;
    }
    for (int tilex=x1; tilex <= x2; tilex++) {
        for (int tiley=y1; tiley <= y2; tiley++) {
            map->setProperties(tilex,tiley,true,true);
            tiles[tilex+tiley*width].wall = false;
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors) {
    dig (x1,y1,x2,y2);
	if (!withActors) {
		return;
	}
    if ( first ) {
        // put the player in the first room
        engine.player->x=(x1+x2)/2;
        engine.player->y=(y1+y2)/2;
		Actor *helmet = new Actor(engine.player->x,engine.player->y,'[',"Helmet",TCODColor::darkerYellow);
		helmet->blocks = false;
		helmet->ID = 000010;
		TCODList<ItemBonus *> bonus;
		ItemBonus *DRBonus = new ItemBonus(ItemBonus::LIGHT,2);
		bonus.push(DRBonus);
		ItemReq *requirement = new ItemReq(ItemReq::NOREQ,0);
		helmet->pickable = new Equipment(Equipment::HEAD,bonus,requirement);
		engine.actors.push(helmet);
		
		Actor *knife = new Actor(engine.player->x,engine.player->y,'|',"Knife {2d6,H}",TCODColor::darkerRed);
		knife->blocks = false;
		knife->ID = 000011;
		TCODList<ItemBonus *> bonus2;
		ItemBonus *StrBonus = new ItemBonus(ItemBonus::STRENGTH,3);
		bonus2.push(StrBonus);
		ItemReq *req = new ItemReq(ItemReq::NOREQ,0);
		knife->pickable = new Weapon(2,6,3,15,Weapon::HEAVY,Equipment::HAND1,
			bonus2,req);
		engine.actors.push(knife);
		
		Actor *poison = new Actor(engine.player->x, engine.player->y,'!',"Bottle of poison",TCODColor::lightGreen);
		poison->blocks = false;
		poison->ID = 000012;
		poison->pickable = new Poison(1,10);
		engine.actors.push(poison);
    } else {
        TCODRandom *rng=TCODRandom::getInstance();
        int nbMonsters = rng->getInt(0,MAX_ROOM_MONSTERS);
		while (nbMonsters > 0) {
			int x = rng->getInt(x1,x2);
			int y = rng->getInt(y1,y2);
			if (canWalk(x,y)){
				addMonster(x,y);
				nbMonsters--;
			}
		}
		
		//add items
		int nbItems = rng->getInt(0,MAX_ROOM_ITEMS);
		while(nbItems >0) {
			int x = rng->getInt(x1,x2);
			int y = rng->getInt(y1,y2);
			if (canWalk(x,y)){
				addItem(x,y);
				nbItems--;
			}
		}
    }
	//set stairs position
	engine.stairs->x = (x1+x2)/2;
	engine.stairs->y = (y1+y2)/2;
}

void Map::addMonster(int x, int y){
	TCODRandom *rng = TCODRandom::getInstance();
	if (rng->getInt(0,100) < 80) {
		//create an orc
		Actor *orc = new Actor(x,y,'o',"orc",
			TCODColor::desaturatedGreen);
		orc->ID = 000002;
		orc->destructible = new MonsterDestructible(10,0,"dead orc",10);
		orc->attacker = new Attacker(3);
		orc->ai = new MonsterAi();
		orc->speedy = new Speedy(1);
		engine.actors.push(orc);
	} else {
		//create a troll
		Actor *troll = new Actor(x,y,'T',"troll",
			TCODColor::darkerGreen);
		troll->ID = 000003;
		troll->destructible = new MonsterDestructible(16,1,"troll carcass",15);
		troll->attacker = new Attacker(4);
		troll->ai = new MonsterAi();
		troll->speedy = new Speedy(1);
		engine.actors.push(troll);
	}
}

bool Map::isWall(int x, int y) const {
    return tiles[x+y*width].wall;
}
 
void Map::render() const {
    static const TCODColor darkWall(0,0,100);
    static const TCODColor darkGround(50,50,150);
	static const TCODColor lightWall(130,110,50);
	static const TCODColor lightGround(200,180,50);

	
	//this is for the "tiles" rendering
	/* for (int x=0; x < width; x++) {
	    for (int y=0; y < height; y++) {
			if (isInFov(x,y)){
				TCODConsole::root->setCharBackground( x,y,
	            isWall(x,y) ? lightWall : lightGround );
			} else if (isExplored(x,y)){
				TCODConsole::root->setCharBackground( x,y,
	            isWall(x,y) ? darkWall : darkGround );
			}
	    }
	} */
	
	for (int x=0; x < width; x++) {
	    for (int y=0; y < height; y++) {
			if (isInFov(x,y)){
				engine.mapcon->setCharForeground( x,y,TCODColor::white);
				if (isWall(x,y)) {
					engine.mapcon->setChar(x,y,'#');
				} else {
					engine.mapcon->setChar(x,y,'.');
				}
			} else if (isExplored(x,y)){
				engine.mapcon->setCharForeground( x,y,TCODColor::darkerGrey);
				if (isWall(x,y)) {
					engine.mapcon->setChar(x,y,'#');
				} else {
					engine.mapcon->setChar(x,y,'.');
				}
			}
	    }
	}
	
}

void Map::exploreTile(int x, int y) {
	if (x > width || y > height) {
		return;
	}
	tiles[x+y*width].explored = true;
	return;
}

bool Map::isExplored(int x, int y) const {
	return tiles[x+y*width].explored;
}

bool Map::isInFov(int x, int y) const {
	if (map->isInFov(x,y)){
		tiles[x+y*width].explored=true;
		return true;
	}
	return false;
}

bool Map::canWalk(int x, int y) const {
	if (x > width || y > height){	//(x,y) is outside of the map
		return false;
	}
	if (isWall(x,y)) {	//(x,y) is a wall
		return false;
	}
	if ( tiles[x+y*width].blocked ){	//another actor is there
		return false;
	}
	return true;
}

void Map::computeFov(){
	map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}

void Map::addItem(int x, int y) {
	TCODRandom *rng = TCODRandom::getInstance();
	int dice = rng->getInt(0,100);
	if (dice < 70) {
		//create a health potion
		Actor *healthPotion = new Actor(x,y,'!',"health potion",TCODColor::violet);
		healthPotion->ID = 000004;
		healthPotion->blocks = false;
		healthPotion->pickable = new Healer(12);
		engine.actors.push(healthPotion);
		engine.sendToBack(healthPotion);
	} else if (dice <70+10) {
		//create a scroll of lightning bolt
		Actor *scrollOfLightningBolt = new Actor(x,y,'?',"Scroll of LIghtning Bolt",
			TCODColor::lightYellow);
		scrollOfLightningBolt->ID = 000005;
		scrollOfLightningBolt->blocks = false;
		scrollOfLightningBolt->pickable = new LightningBolt(5,20);
		engine.actors.push(scrollOfLightningBolt);
		engine.sendToBack(scrollOfLightningBolt);
	} else if (dice < 70+10+10) {
		//create a scroll of fireball
		Actor *scrollOfFireball = new Actor(x,y,'?',"Scroll of Fireball",
			TCODColor::lighterOrange);
		scrollOfFireball->ID = 000006;
		scrollOfFireball->blocks = false;
		scrollOfFireball->pickable = new Fireball(8,3,12);
		engine.actors.push(scrollOfFireball);
		engine.sendToBack(scrollOfFireball);
	} else {
		//create a scroll of confusion
		Actor *scrollOfConfusion = new Actor(x,y,'?',"scroll of confusion",
			TCODColor::cyan);
		scrollOfConfusion->ID = 000007;
		scrollOfConfusion->blocks = false;
		scrollOfConfusion->pickable = new Confuser(10,3);
		engine.actors.push(scrollOfConfusion);
		engine.sendToBack(scrollOfConfusion);
	}
}