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
	Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1, double verticalSpeed1, double horizontalSpeed1, GhostRacer* GhostRacer1, bool affectedByProjectile1 = false)
		:GraphObject(imageID, startX, startY, dir, size, depth), alive(startAlive), collisionAvoidance(collisionAvoid), world(StudentWorld1), verticalSpeed(verticalSpeed1), horizontalSpeed(horizontalSpeed1), racer(GhostRacer1), affectedByProjectile(affectedByProjectile1) //takes in inputs to initialize base class and member variables
	{ } //what should this initialize and take input for?
	virtual void doSomething() = 0; //this is the doSomething() function which makes the object do whatever it needs to each tick, but this depends on what the type of the object is, so this must be virtual
	void doSomethingBasic(); //this is the default moving algorithm for nearly all derived classes, this is used in only some (most) derived classes, so I just call it when its needed as part of doSomething() so no need to be virtual, I don't call it in classes that use a different moving method
	bool isAlive() { return alive; } //this just checks if something is alive or not, this is the same for every single class so doesn't need to be virtual
	void nowDead() { alive = false; } //makes the object dead, same for all derived classes so no need for virtual
	bool isCollisionAvoidanceWorthy() { return collisionAvoidance; } // returns if the object isCollisionAvoidanceWorthy, which is the same for all derived classes so no need for virtual
	StudentWorld* getWorld() { return world; } // returns the base world, which again is the same for all derived classes so no need for virtual
	void play(const int sound); //this is just easier syntax for the playSound(int input), which is the same for all classes so no need for virtual
	double getVerticalSpeed() { return verticalSpeed; } //returns the vertical speed, which again is the same for all derived classes so no need for virtual
	double setVerticalSpeed(double newSpeed) { verticalSpeed = newSpeed; return verticalSpeed; } //changes then returns the vertical speed, which again is the same for all derived classes so no need for virtual
	double getHorizontalSpeed() { return horizontalSpeed; } //returns the horizontal speed, which again is the same for all derived classes so no need for virtual
	double setHorizontalSpeed(double newSpeed) { horizontalSpeed = newSpeed; return horizontalSpeed; } //changes then returns the horizontal speed, which again is the same for all derived classes so no need for virtual
	GhostRacer* getRacer() { return racer; } //returns the Ghostracer pointer, which again is the same for all derived classes so no need for virtual
	bool isAffectedByProjectile() { return affectedByProjectile; } ////returns the vertical speed, which again is the same for all derived classes so no need for virtual
	virtual void holyWaterHit(){  }// the default is that this does absolutely nothing (as many derived classes are completely unaffected), but some derived classes are affected, so this depends on the class, so this is virtual
private:
	bool collisionAvoidance;
	bool alive;
	StudentWorld* world;
	double verticalSpeed; 
	double horizontalSpeed;
	GhostRacer* racer;
	bool affectedByProjectile;
};

class BorderLine : public Actor
{//Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1)
public:
	BorderLine(int imageID, double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Actor(imageID, startX, startY, 0, 2.0, 2, true, false, StudentWorld1, -4, 0, GhostRacer1, false) //takes in inputs to initialize base class, constant inputs are the ones that the spec says that this type of object always has
	{}
	virtual void doSomething() { doSomethingBasic(); } //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
private:
};

class OilSlick : public Actor
{//Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1)
public:
	OilSlick(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Actor(IID_OIL_SLICK, startX, startY, 0, randInt(2,5), 1, true, false, StudentWorld1, -4, 0, GhostRacer1, false)//takes in inputs to initialize base class and member variables
	{}
	virtual void doSomething(); //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
private:
};

class MovingActor : public Actor
{//	Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1, int verticalSpeed1, int horizontalSpeed1, GhostRacer* GhostRacer1)
public:
	MovingActor(int imageID, double startX, double startY, int dir, double size, bool collisionAvoid, int hitpoints,  StudentWorld* StudentWorld1, int verticalSpeed1, int horizontalSpeed1, GhostRacer* GhostRacer1, bool affectedByProjectile)
		:Actor(imageID, startX, startY, dir, size, 0, true, collisionAvoid, StudentWorld1, verticalSpeed1, horizontalSpeed1, GhostRacer1, affectedByProjectile), hp(hitpoints), movementPlanDistance(0) //all Movingactors have depth 0, all actors start alive, movementPlandistance starts at 0
	{}
	void takeDamage(int damage); //all MovingActors are able to take damage (which is why this function is in the MovingActor class), so this function is not virtual because this just decrements the HP by the inputted amount, and then calls nowDead() if the HP is 0 or less. This process is the same for all derived classes, so no need for virtual
	int getHP() { return hp; } //returns HP, which is something that all MovingActors have (which is why this function is in the MovingActor class), so this function is not virtual because this process of getting the variable is the same for all derived classes
	int setHP(int newVal) { hp = newVal; return newVal;} //sets and returns HP, which is something that all MovingActors have (which is why this function is in the MovingActor class), so this function is not virtual because this process of setting the variable is the same for all derived classes
	
	//These three following functions are like peas in a pod. They all deal with getting, decrementing, and setting new movementPlanDist, which is used in all the derived classes of MovingActor (which is why these functions are in the MovingActor class)
	//The process that these three work with are the same no matter what derived class it is, which is why none of these three are virtual
	int getMovementPlanDist() { return movementPlanDistance; }
	int decMovementPlanDist() { movementPlanDistance--; return movementPlanDistance; }
	void setMovementPlanDist(int newPlan) { movementPlanDistance = newPlan; }
	
	//This below is used for multiple derived classes of MovingActor(which is why this function is in the MovingActor class), so this function is not virtual because this function is just not called in derived classes that do things differently
	void newMovementPlan();
private:
	int hp;
	int movementPlanDistance;
};

class GhostRacer : public MovingActor
{
public:
	GhostRacer(StudentWorld* StudentWorld1)
		:MovingActor(IID_GHOST_RACER, 128, 32, up, 4.0, true, 100, StudentWorld1, 0, 0, this, false), holyWaterCount(10) //takes in inputs to initialize base class and member variables, including speed components, position, up direction, 100 hitpoints, CAW and a few others
	{}
	virtual void doSomething(); //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
	void spin(); //this function spins the direction of the GhostRacer when driving over an oilslick, and this is part of the GhostRacer class because this function changes the direction of the GhostRacer object. This is not virtual because there is no equivalent spinning of any other class type
	
	//These next three functions go hand in hand. The three functions return the holyWaterCount, which measures the amount of sprays left, or add the number of picked up holyWater sprays, or decrement the holyWaterCount when a spraying projectile is released
	//Only the GhostRacer can shoot and pick up holy water, which is why these three functions are in this class. These are not virtual because there is only one class that can actually use the holy water
	int getHolyWaterValue() { return holyWaterCount; } 
	int holyWaterPickup(int num) { holyWaterCount += num; return holyWaterCount; }
	void holyWaterDecrement() { if (holyWaterCount > 0) holyWaterCount--; }
	
	//This function heals the GhostRacer by the healed amount, but to no more than 100 HP. Only the GhostRacer can heal, which is why this is in the GhostRacer class, and it is also not virtual for this reason (no need to be virtual if there are no more implementations of same function) 
	void heal(int amount) { if (getHP() + amount > 100) setHP(100); else setHP(getHP() + amount); }

private:
	int holyWaterCount;
};


class Goodie : public Actor
{
public:
	Goodie(int imageID, double startX, double startY, int dir, double size, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1, bool affectedByProjectile)
		:Actor(imageID, startX, startY, dir, size, 2, true, false, StudentWorld1, -4, 0, GhostRacer1, affectedByProjectile)//takes in inputs to initialize base class and member variables, including depth of 2 and speed components
	{}
private:
	
};

class HealingGoodie : public Goodie
{
public:
	HealingGoodie(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Goodie(IID_HEAL_GOODIE, startX, startY, 0, 1.0, StudentWorld1, GhostRacer1, true) //takes in inputs to initialize base class and member variables, like direction of 0 and size of 1.0, and affectedByProjectile of true
	{}
	virtual void doSomething(); //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
	virtual void holyWaterHit() { nowDead(); } //the virtual holyWaterHit() function is overrided in some classes I wrote because some classes must respond to holy water hitting them differently, while some do absolutely nothing, which is the default function in the Actor class so this must be virtual
private:

};

class HolyWaterGoodie : public Goodie
{
public:
	HolyWaterGoodie(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Goodie(IID_HOLY_WATER_GOODIE, startX, startY, 90, 2.0, StudentWorld1, GhostRacer1, true)
	{}
	virtual void doSomething();//the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
	virtual void holyWaterHit() { nowDead(); } //the virtual holyWaterHit() function is overrided in some classes I wrote because some classes must respond to holy water hitting them differently, while some do absolutely nothing, which is the default function in the Actor class so this must be virtual
private:

};

class SoulGoodie : public Goodie
{
public:
	SoulGoodie(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Goodie(IID_SOUL_GOODIE, startX, startY, 0, 4.0, StudentWorld1, GhostRacer1, false)
	{}
	virtual void doSomething();//the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
private:

};

class Pedestrian : public MovingActor
{//MovingActor(int imageID, double startX, double startY, int dir, double size, bool collisionAvoid, int hitpoints,  StudentWorld* StudentWorld1, int verticalSpeed1, int horizontalSpeed1, GhostRacer* GhostRacer1)
public:
	Pedestrian(int imageID, double startX, double startY, double size, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:MovingActor(imageID, startX, startY, 0, size, true, 2, StudentWorld1, -4 , 0, GhostRacer1, true), movementPlanDistance(0)
	{}
	
private:
	int movementPlanDistance;
};


class ZomPed : public Pedestrian
{//Pedestrian(int imageID, double startX, double startY, double size, StudentWorld* StudentWorld1)
public:
	ZomPed(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Pedestrian(IID_ZOMBIE_PED, startX, startY, 3.0, StudentWorld1, GhostRacer1), ticksUntilNextGrunt(0)
	{}
	virtual void doSomething(); //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
	
	//These next three functions go hand in hand. The three functions return the holyWaterCount, which measures the amount of sprays left, or add the number of picked up holyWater sprays, or decrement the holyWaterCount when a spraying projectile is released
	//Only the GhostRacer can shoot and pick up holy water, which is why these three functions are in this class. These are not virtual because there is only one class that can actually use the holy water
	int decTicksUntilNextGrunt() { if (ticksUntilNextGrunt > 0) ticksUntilNextGrunt--; else return -1; return ticksUntilNextGrunt; }
	int getTicksUntilNextGrunt() { return ticksUntilNextGrunt; }
	int setTicksUntilNextGrunt(int newTicks) { ticksUntilNextGrunt = newTicks; return newTicks; }

	virtual void holyWaterHit() { gotHurt(); } //the virtual holyWaterHit() function is overrided in some classes I wrote because some classes must respond to holy water hitting them differently, while some do absolutely nothing, which is the default function in the Actor class so this must be virtual
	
private:
	void gotHurt();
	int ticksUntilNextGrunt;

};

class HumPed : public Pedestrian
{
public:
	HumPed(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:Pedestrian(IID_HUMAN_PED, startX, startY, 2.0, StudentWorld1, GhostRacer1)
	{}
	virtual void doSomething(); //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
	virtual void holyWaterHit(); //the virtual holyWaterHit() function is overrided in some classes I wrote because some classes must respond to holy water hitting them differently, while some do absolutely nothing, which is the default function in the Actor class so this must be virtual
private:


};

class ZomCab : public MovingActor
{//int imageID, double startX, double startY, int dir, double size, bool collisionAvoid, int hitpoints,  StudentWorld* StudentWorld1, int verticalSpeed1, int horizontalSpeed1, GhostRacer* GhostRacer1
public:
	ZomCab(double startX, double startY, double verticalSpeed1, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
		:MovingActor(IID_ZOMBIE_CAB, startX, startY, 90, 4.0, true, 3, StudentWorld1, verticalSpeed1, 0, GhostRacer1, true), hasDamagedGhostRacer(false)
	{}
	virtual void doSomething(); //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
	virtual void holyWaterHit(); //the virtual holyWaterHit() function is overrided in some classes I wrote because some classes must respond to holy water hitting them differently, while some do absolutely nothing, which is the default function in the Actor class so this must be virtual
private:
	bool hasDamagedGhostRacer;
};

class Projectile : public Actor
{//Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth, bool startAlive, bool collisionAvoid, StudentWorld* StudentWorld1, int verticalSpeed1, int horizontalSpeed1, GhostRacer* GhostRacer1)
public:
	Projectile(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1, int direction)
		:Actor(IID_HOLY_WATER_PROJECTILE, startX, startY, direction, 1.0, 1, true, false, StudentWorld1, 0, 0, GhostRacer1, false), travelDistLeft(160)
	{}
	virtual void doSomething(); //the virtual doSomething() function is in every single class that I wrote because each class must doSomething() each tick, and each class must doSomething() different, so this must be virtual
	void tickMoved() {travelDistLeft -= SPRITE_HEIGHT; if (travelDistLeft <= 0) { nowDead(); }} //decrement by the sprite's height every tick, and once the travelDist is completed (when it is 0), then kill the sprite (to range limit it by 160 px). This is in the Projectile class because only projectiles have a range limit out of all classes, and for the same reason, there is no need for this to be virtual because there are no other implementations of this.

private:
	int travelDistLeft;
};

#endif // ACTOR_H_
