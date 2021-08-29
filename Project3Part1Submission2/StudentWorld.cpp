#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), souls2Save(0), numActorsNotIncludingRacer(0), yCoordinateOfLastWhiteBorderLine(0), bonusCount(5000)
{
    vector<Actor*> actors;
    iter = actors.begin();
}

int StudentWorld::init()
{
    souls2Save = getLevel() * 2 + 5; //this varies per level
    bonusCount = 5000; //reset the bonusCount to 5000
    racer = new GhostRacer(this);
    const int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    
    for (int j = 0; j <= N - 1; j++)
    {
        //left yellow BorderLines
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, j * SPRITE_HEIGHT, this, racer));
        numActorsNotIncludingRacer++;
        //right yellow BorderLines
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j * SPRITE_HEIGHT, this, racer));
        numActorsNotIncludingRacer++;
    }
    //middle white dashed lane divider BorderLines
    for (int j = 0; j <= M - 1; j++)
    {
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3, j * (4 * SPRITE_HEIGHT), this, racer));
        numActorsNotIncludingRacer++;
        
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, j * (4 * SPRITE_HEIGHT), this, racer));
        numActorsNotIncludingRacer++;
    }
    yCoordinateOfLastWhiteBorderLine = (M-1) * (4 * SPRITE_HEIGHT);
    //scoreShow();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    racer->doSomething();
    for (int i = 0; i < numActorsNotIncludingRacer; i++)
        actors[i]->doSomething();
    if (racer->isAlive() == false)
    {
        //do not delete racer here, as that is done in the cleanUp() function
        decLives(); //this is not called automatically by the framework, must be called by me
        //cleanUp(); is automatically called by the "framework" provided
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    else if (0 == souls2Save) //this is to check if the racer has saved the required number of souls for the current level (and 0 are left)
    {
        //award bonus points based on time (will do this later) DO THIS DO NOT FORGET
        increaseScore(bonusCount);
        return GWSTATUS_FINISHED_LEVEL;
    }
    //deletes all objects that died this tick
    for (int i = 0; i < actors.size();)
    {
        if (actors.at(i)->isAlive() == false)
        {
            delete actors[i]; //first delete the actor
            actors[i] = actors[numActorsNotIncludingRacer - 1]; //store the last interesting pointer in vector into position i
            actors.pop_back();
            numActorsNotIncludingRacer--;
            //no i++ because you don't want to move forward as you don't want to skip the new actors[i]
        }
        else
            i++; //move forward because no deleting at i
    }
    //Add new objects (e.g., a new goodie, oil slick, zombie or human ped, yellow or white road border line, or vehicle) as required for proper gameplay
    //have to add boundary lines, which are going away at the speed of the borderLine speed, so you have to add them back at the same speed
    yCoordinateOfLastWhiteBorderLine += -4.0 - racer->getVerticalSpeed();
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double delta_y = new_border_y - yCoordinateOfLastWhiteBorderLine;
    
    if (delta_y >= SPRITE_HEIGHT)
    {
        //left yellow BorderLines
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, new_border_y, this, racer));
        numActorsNotIncludingRacer++;
        //right yellow BorderLines
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, new_border_y, this, racer));
        numActorsNotIncludingRacer++;
    }
    //for the white dashed borderLines
    if (delta_y >= 4 * SPRITE_HEIGHT)
    {
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH / 3, new_border_y, this, racer));
        numActorsNotIncludingRacer++;

        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH / 3, new_border_y, this, racer));
        numActorsNotIncludingRacer++;
        yCoordinateOfLastWhiteBorderLine = new_border_y;
    }
    //add the OilSlicks randomly
    addOilSlicksRandomly();
    addGoodiesRandomly();

    scoreShow();
    bonusCountDec(); //this should be after scoreShow, so if you somehow finish on the first tick, you get all 5000 points, not 4999
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //go through the vector and delete everything, then delete the GhostRacer
    for (int i = actors.size() - 1; i >= 0; i--)
    {
            delete actors[i]; //first delete the actor at last position
            //one less actor now, so i-- happens
            actors.pop_back();
    }
    delete racer;
    numActorsNotIncludingRacer = 0;
    souls2Save = 0;
    numActorsNotIncludingRacer = 0;
    yCoordinateOfLastWhiteBorderLine = 0;
}

void StudentWorld::scoreShow()
{
    ostringstream oss;
    oss << "Score: " + to_string(getScore());
    oss << "  Lvl: " + to_string(getLevel());
    oss << "  Souls2Save: " + to_string(getSouls2Save());
    oss << "  Lives: " + to_string(getLives());
    oss << "  Health: " + to_string(racer->getHP());
    oss << "  Sprays: " + to_string(racer->getHolyWaterValue());
    oss << "  Bonus: " + to_string(getBonusCount());
    string s = oss.str();
    setGameStatText(s);
}

void StudentWorld::addOilSlicksRandomly()
{
    double ChanceOilSlick = max(150 - getLevel() * 10, 40);
    int random = randInt(0, ChanceOilSlick);
    if (random == 0)
    {
        int randomXValue = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new OilSlick(randomXValue, VIEW_HEIGHT, this, racer));
        numActorsNotIncludingRacer++;
    }
    //OilSlick(double startX, double startY, StudentWorld* StudentWorld1, GhostRacer* GhostRacer1)
}

void StudentWorld::addGoodiesRandomly()
{
    //Holy Water Refill Goodies
    double ChanceHolyWater = 100 + 10 * getLevel();
    if (randInt(0, ChanceHolyWater) == 0)
    {
        int randomXValue = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new HolyWaterGoodie(randomXValue, VIEW_HEIGHT, this, racer));
        numActorsNotIncludingRacer++;
    }

    //Lost Soul Goodies
    double ChanceLostSoul = 100;
    if (randInt(0, ChanceLostSoul) == 0)
    {
        int randomXValue = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new SoulGoodie(randomXValue, VIEW_HEIGHT, this, racer));
        numActorsNotIncludingRacer++;
    }
    //Healing Goodies spawn from deaths of zombies, so won't be any spawning in this function
}


