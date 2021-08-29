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


const double PI = 3.141592653589793238463;
const int N = VIEW_HEIGHT / SPRITE_HEIGHT;
const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld() { cleanUp(); }
    void scoreShow();
    int savedASoul() { souls2Save--; return souls2Save; }
    int getSouls2Save() { return souls2Save; }
    int bonusCountDec() { if (getBonusCount() > 0) bonusCount--; return bonusCount; }
    int getBonusCount() { return bonusCount; }
private:
    void addOilSlicksRandomly();
    void addGoodiesRandomly();
    std::vector<Actor*> actors;
    GhostRacer* racer;
    int souls2Save;
    int bonusCount;
    int numActorsNotIncludingRacer;
    double yCoordinateOfLastWhiteBorderLine;
    vector<Actor*>::iterator iter;
};

#endif // STUDENTWORLD_H_
