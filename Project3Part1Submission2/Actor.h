#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//#1 of part 1
#include "StudentWorld.h"
class GhostRacer;
class StudentWorld;
class Actor : public GraphObject //#1 p2: derived from GraphObject class
{
public:
	Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1, int verticalSpeed1, int horizontalSpeed1, GhostRacer* GhostRacer1)
		:GraphObject(imageID, startX, startY, dir, size, depth), alive(startAlive), collisionAvoidance(collisionAvoid), world(StudentWorld1), verticalSpeed(verticalSpeed1), horizontalSpeed(horizontalSpeed1), racer(GhostRacer1)
	{ } //what should this initialize and take input for?
	virtual void doSomething() = 0;
	void doSomethingBasic();
	bool isAlive() { return alive; }
	void nowDead() { alive = false; } //makes the object dead
	bool isCollisionAvoidanceWorthy() { return collisionAvoidance; }
	StudentWorld* getWorld() { return world; } //how do I implement this function?
	void play(const int sound);
	int getVerticalSpeed() { return verticalSpeed; }
	int setVerticalSpeed(int newSpeed) { verticalSpeed = newSpeed; return verticalSpeed; } //NOTE, THERE IS NO ERROR CHECKING FOR setting SPEED with this function
	int getHorizontalSpeed() { return horizontalSpeed; }
	int setHorizontalSpeed(int newSpeed) { horizontalSpeed = newSpeed; return horizontalSpeed; } //NOTE, THERE IS NO ERROR CHECKING FOR setting SPEED with this function
	GhostRacer* getRacer() { return racer; }
	bool overlaps(Actor* A, Actor* B);
private:
	bool collisionAvoidance;
	bool alive;
	StudentWorld* world;
	int verticalSpeed; 
	int horizontalSpeed;
	GhostRacer* racer;
};

class BorderLine : public Actor
{//Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1)
public:
	BorderLine(int imageID, double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Actor(imageID, startX, startY, 0, 2.0, 2, true, false, StudentWorld1, -4, 0, GhostRacer1)
	{}
	virtual void doSomething() { doSomethingBasic(); }
private:
};

class OilSlick : public Actor
{//Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1)
public:
	OilSlick(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Actor(IID_OIL_SLICK, startX, startY, 0, randInt(2,5), 1, true, false, StudentWorld1, -4, 0, GhostRacer1)
	{}
	virtual void doSomething();
private:
};

class MovingActor : public Actor
{
public:
	MovingActor(int imageID, double startX, double startY, int dir, double size, bool collisionAvoid, int hitpoints,  StudentWorld* StudentWorld1, int verticalSpeed1, int horizontalSpeed1, GhostRacer* GhostRacer1)
		:Actor(imageID, startX, startY, dir, size, 0, true, collisionAvoid, StudentWorld1, verticalSpeed1, horizontalSpeed1, GhostRacer1), hp(hitpoints) //all actors have depth 0, all actors start alive??? check both
	{}
	void takeDamage(int damage);
	int getHP() { return hp; }
	int setHP(int newVal) { hp = newVal; return newVal;} //no Error Checking here, be sure to check! (Racer can't go above 100 hp)
private:
	int hp;
	
};

class GhostRacer : public MovingActor
{
public:
	GhostRacer(StudentWorld* StudentWorld1)
		:MovingActor(IID_GHOST_RACER, 128, 32, up, 4.0, true, 100, StudentWorld1, 0, 0, this), holyWaterCount(10)
	{}
	virtual void doSomething();
	void spin();
	int getHolyWaterValue() { return holyWaterCount; }
	int holyWaterPickup(int num) { holyWaterCount += num; return holyWaterCount; }
	int holyWaterDecrement() { if (holyWaterCount > 0) holyWaterCount--; else return -1; }
	void heal(int amount) { if (getHP() + amount > 100) setHP(100); else setHP(getHP() + amount); }
private:
	int holyWaterCount;
};
/*
class Pedestrian : public MovingActor 
{//int imageID, double startX, double startY, int dir, double size, bool collisionAvoid, int hitpoints,  StudentWorld* StudentWorld1
public:
	Pedestrian(int imageID, double startX, double startY, double size, StudentWorld* StudentWorld1)
		:MovingActor(imageID, startX, startY, 0, size, true, 2, StudentWorld1), movementPlanDistance(0)
	{}
private:
	double movementPlanDistance;
};
*/
class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX, double startY, double size, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Actor(imageID, startX, startY, 0, size, 2, true, false, StudentWorld1, -4, 0, GhostRacer1)
	{}
private:
	
};

class HealingGoodie : public Goodie
{
public:
	HealingGoodie(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Goodie(IID_HEAL_GOODIE, startX, startY, 1.0, StudentWorld1, GhostRacer1)
	{}
	virtual void doSomething();
private:

};

class HolyWaterGoodie : public Goodie
{
public:
	HolyWaterGoodie(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Goodie(IID_HOLY_WATER_GOODIE, startX, startY, 2.0, StudentWorld1, GhostRacer1)
	{}
	virtual void doSomething();
private:

};

class SoulGoodie : public Goodie
{
public:
	SoulGoodie(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Goodie(IID_SOUL_GOODIE, startX, startY, 4.0, StudentWorld1, GhostRacer1)
	{}
	virtual void doSomething();
private:

};


/*
class ZomPed : public Pedestrian
{//Pedestrian(int imageID, double startX, double startY, double size, StudentWorld* StudentWorld1)
public:
	ZomPed(double startX, double startY, StudentWorld* StudentWorld1)
		:Pedestrian(IID_ZOMBIE_PED, startX, startY, 3.0, StudentWorld1), ticksUntilNextGrunt(0)
	{}
	virtual void doSomething();
private:
	int ticksUntilNextGrunt;

};

class HumPed : public Pedestrian
{
public:
	HumPed(double startX, double startY, StudentWorld* StudentWorld1)
		:Pedestrian(IID_HUMAN_PED, startX, startY, 2.0, StudentWorld1)
	{}
	virtual void doSomething();
private:


};

class ZomCab : public MovingActor
{
public:
	virtual void doSomething();
private:


};
*/


#endif // ACTOR_H_
