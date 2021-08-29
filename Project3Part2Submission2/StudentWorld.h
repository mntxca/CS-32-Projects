#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include <vector>
#include "Actor.h"
#include <sstream>
#include <iomanip>
using namespace std;

class GhostRacer;
class Actor;
class ZomCab;
class Projectile;


const double PI = 3.141592653589793238463;
const int N = VIEW_HEIGHT / SPRITE_HEIGHT;
const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
const int leftLaneCenterX = ROAD_CENTER - ROAD_WIDTH / 3;
const int middleLaneCenterX = ROAD_CENTER;
const int rightLaneCenterX = ROAD_CENTER + ROAD_WIDTH / 3;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init(); //this setups the road and makes the game ready to play by adding the borderlines and racer
    virtual int move(); //this handles each tick and makes sure every object has a chance to move, and also deletes any dead actors every tick
    virtual void cleanUp(); //this deletes all the dynamically allocated objects when you die or the game is quit
    ~StudentWorld() { cleanUp(); } //this helps clean up all the dynamically allocated objects at the end of the game
    int savedASoul() { souls2Save--; return souls2Save; } //decrements the souls2Save variable, this is in the StudentWorld class because the StudentWorld class is the only class I wrote which is capable of keeping track number of objects of all types of classes, including the soul goodies
    int fatality(); //this is to let the StudentWorld class know that the racer has died, and the "round" of the game must be ended, and the framework then can automatically call cleanUp(), this is in the StudentWorld class because you need a way to return GWSTATUS_PLAYER_DIED from the move() function if the player dies if, for example, a HumPed is hit
    
    //These three functions are similar, as they are functions that allow the Actors from Actor.h and Actor.cpp to add new objects to the game either randomly when the Actor dies or when the player wants to shoot the cannon. The first function adds HealingGoodies randomly after a ZomPed is destroyed, and the second adds an oil slick randomly after a ZomCab is destroyed, and the third creates a Projectile after the space bar is pressed if there is enough holy water left
    //These three functions are forced to be part of the StudentWorld class because this is the class where the vector of Actor objects lives, and thus all objects must be added somewhere through StudentWorld 
    void addHealingGoodieRandomly(double x_pos, double y_pos); //(DO NOT PUT IN THE move() function, only to be called from Actor.cpp)
    void addOilSlickFromCabDeath(double x_pos, double y_pos); //(DO NOT PUT IN THE move() function, only to be called from Actor.cpp)
    void addProjectile(double x_pos, double y_pos, int direction); //(DO NOT PUT IN THE move() function, only to be called from Actor.cpp)
    

    bool overlaps(Actor* A, Actor* B); //This function checks if two Actors overlap by comparing their x and y coordinates and "radiuses". This must be part of the StudentWorld class because this class needs to use this function in the checkActivation(Projectile* P) function (which is part of the StudentWorld class), which goes through all the Actors in the vector and finds the first that overlaps with itself (the Projectile)
    Actor* checkActivation(Projectile* P); //This function goes through all the Actors in the vector and finds the first that overlaps with itself (the Projectile), excluding itself. This must be in the StudentWorld class because only a part of the StudentWorld class can traverse the private vector of Actors in the StudentWorld class
    
    //These two functions find if the closest collisionAvoidanceWorthy Actors are within 96 px of the given ZomCab (the minInFront includes the GhostRacer, and the minBehind excludes the GhostRacer). These both must be in StudentWorld because these functions go through all the Actors in the vector to see which are the closest, and the private vector of the StudentWorld class can only be accessed through the StudentWorld class
    bool minInFront(ZomCab* ZCab);
    bool minBehind(ZomCab* ZCab);
private:
    void scoreShow();
    int bonusCountDec() { if (getBonusCount() > 0) bonusCount--; return bonusCount; }
    int getSouls2Save() { return souls2Save; }
    int getBonusCount() { return bonusCount; }
    void addOilSlicksRandomly();
    void addGoodiesRandomly();
    void addPedsRandomly();
    void addZomCabsRandomly();
    bool findCollisionWorthyActors(int& leftLanefromTop, int& leftLanefromBottom, int& middleLanefromTop, int& middleLanefromBottom, int& rightLanefromTop, int& rightLanefromBottom);
    bool addZomCab(const int leftLanefromTop, const int leftLanefromBottom, const int middleLanefromTop, const int middleLanefromBottom, const int rightLanefromTop, const int rightLanefromBottom);
    bool sameLane(ZomCab* ZCab, Actor* actor2);
    int laneNumber(Actor* actor1);
    vector<Actor*> actors;
    GhostRacer* racer;
    int souls2Save;
    int bonusCount;
    bool death;
    double yCoordinateOfLastWhiteBorderLine;
    vector<Actor*>::iterator iter;
};

#endif // STUDENTWORLD_H_
