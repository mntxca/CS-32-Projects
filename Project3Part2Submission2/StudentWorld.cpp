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
: GameWorld(assetPath), souls2Save(0), yCoordinateOfLastWhiteBorderLine(0), bonusCount(5000), death(false)
{
    vector<Actor*> actors;
    iter = actors.begin();
}

int StudentWorld::init()
{
    souls2Save = getLevel() * 2 + 5; //this varies per level
    bonusCount = 5000; //reset the bonusCount to 5000
    racer = new GhostRacer(this);
    death = false;
    const int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    
    for (int j = 0; j <= N - 1; j++)
    {
        //left yellow BorderLines
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, j * SPRITE_HEIGHT, this, racer));
        //right yellow BorderLines
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j * SPRITE_HEIGHT, this, racer));
    }
    //middle white dashed lane divider BorderLines
    for (int j = 0; j <= M - 1; j++)
    {
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3, j * (4 * SPRITE_HEIGHT), this, racer));
        
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, j * (4 * SPRITE_HEIGHT), this, racer));
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
    for (int i = 0; i < actors.size(); i++)
        actors[i]->doSomething();
    if (racer->isAlive() == false)
    {
        return fatality(); //fatality() returns GWSTATUS_PLAYER_DIED;
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
            actors[i] = actors[actors.size() - 1]; //store the last interesting pointer in vector into position i
            actors.pop_back();
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
        //right yellow BorderLines
        actors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, new_border_y, this, racer));
    }
    //for the white dashed borderLines
    if (delta_y >= 4 * SPRITE_HEIGHT)
    {
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH / 3, new_border_y, this, racer));
        actors.push_back(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH / 3, new_border_y, this, racer));
        yCoordinateOfLastWhiteBorderLine = new_border_y;
    }
    //add the OilSlicks randomly
    addOilSlicksRandomly();
    addGoodiesRandomly();
    addPedsRandomly();
    addZomCabsRandomly();

    scoreShow();
    bonusCountDec(); //this should be after scoreShow, so if you somehow finish on the first tick, you get all 5000 points, not 4999
    if (death)
        return GWSTATUS_PLAYER_DIED;
    else
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
    souls2Save = 0;
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

int StudentWorld::fatality()
{
    
    //do not delete racer here, as that is done in the cleanUp() function
    decLives(); //this is not called automatically by the framework, must be called by me
    //cleanUp(); is automatically called by the "framework" provided
    death = true;
    playSound(SOUND_PLAYER_DIE);
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::addHealingGoodieRandomly(double x_pos, double y_pos)
{
    if (randInt(1, 5) == 1)
    {
        actors.push_back(new HealingGoodie(x_pos, y_pos, this, racer));
    }
}

void StudentWorld::addOilSlickFromCabDeath(double x_pos, double y_pos)
{
    if (randInt(1, 5) == 1)
    {
        actors.push_back(new OilSlick(x_pos, y_pos, this, racer));
    }
}

void StudentWorld::addProjectile(double x_pos, double y_pos, int direction)
{
    actors.push_back(new Projectile(x_pos, y_pos, this, racer, direction));
}

bool StudentWorld::overlaps(Actor* A, Actor* B)
{
    double delta_x = abs(A->getX() - B->getX());
    double delta_y = abs(A->getY() - B->getY());
    double radius_sum = A->getRadius() + B->getRadius();
    if (delta_x < radius_sum * .25 && delta_y < radius_sum * .6)
        return true;
    else
        return false;
}

Actor* StudentWorld::checkActivation(Projectile* P)
{
    for (int i = 0; i < actors.size(); i++)
    {
        if (actors[i]->isAffectedByProjectile() && actors[i] != P)
        {
            if (overlaps(P, actors[i]))
                return actors[i];
        }
    }
    
    return nullptr;
}

bool StudentWorld::minInFront(ZomCab* ZCab)
{
    for (int i = 0; i < actors.size() + 1; i++)
    {
        Actor* a;
        if (i == actors.size())
            a = racer;
        else
            a = actors[i];
        if (ZCab != a && sameLane(ZCab, a) && a->isCollisionAvoidanceWorthy())
        {
            if((ZCab->getY() < a->getY()) && (a->getY() - ZCab->getY() < 96))
                return true;
        }
    }
    return false;
}

bool StudentWorld::minBehind(ZomCab* ZCab)
{
    Actor* a;
    for (int i = 0; i < actors.size(); i++)
    {
            a = actors[i];
        if (ZCab != a && sameLane(ZCab, a) && a->isCollisionAvoidanceWorthy())
        {
            if ((ZCab->getY() > a->getY()) && (ZCab->getY() - a->getY() < 96))
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::sameLane(ZomCab* ZCab, Actor* actor2) 
{
    //check if in first lane
    if (ZCab->getX() >= LEFT_EDGE && ZCab->getX() < LEFT_EDGE + ROAD_WIDTH / 3)
    {
        if (actor2->getX() >= LEFT_EDGE && actor2->getX() < LEFT_EDGE + ROAD_WIDTH / 3)
            return true;
        else
            return false;
    }
    //check if in second lane
    else if (ZCab->getX() >= LEFT_EDGE + ROAD_WIDTH / 3 && ZCab->getX() < RIGHT_EDGE - ROAD_WIDTH / 3)
    {
        if (actor2->getX() >= LEFT_EDGE + ROAD_WIDTH / 3 && actor2->getX() < RIGHT_EDGE - ROAD_WIDTH / 3)
            return true;
        else
            return false;
    }
    //check if in third lane
    else if (ZCab->getX() >= RIGHT_EDGE - ROAD_WIDTH / 3 && ZCab->getX() <= RIGHT_EDGE)
    {
        if (actor2->getX() >= RIGHT_EDGE - ROAD_WIDTH / 3 && actor2->getX() <= RIGHT_EDGE)
            return true;
        else 
            return false;
    }
    else //somehow the cab is off of the road, I'll just return false here
        return false;
}

int StudentWorld::laneNumber(Actor* actor2)
{
    if (actor2->getX() >= LEFT_EDGE && actor2->getX() < LEFT_EDGE + ROAD_WIDTH / 3)
        return 1; //left lane
    else if (actor2->getX() >= LEFT_EDGE + ROAD_WIDTH / 3 && actor2->getX() < RIGHT_EDGE - ROAD_WIDTH / 3)
        return 2; //middle lane
    else if (actor2->getX() >= RIGHT_EDGE - ROAD_WIDTH / 3 && actor2->getX() < RIGHT_EDGE) //according to spec, don't include right edge
        return 3; //right lane
    else
        return -1; //not on a lane
}

void StudentWorld::addOilSlicksRandomly()
{
    double ChanceOilSlick = max(150 - getLevel() * 10, 40);
    int random = randInt(0, ChanceOilSlick);
    if (random == 0)
    {
        int randomXValue = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new OilSlick(randomXValue, VIEW_HEIGHT, this, racer));
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
    }

    //Lost Soul Goodies
    double ChanceLostSoul = 100;
    if (randInt(0, ChanceLostSoul) == 0)
    {
        int randomXValue = randInt(LEFT_EDGE, RIGHT_EDGE);
        actors.push_back(new SoulGoodie(randomXValue, VIEW_HEIGHT, this, racer));
    }
    //Healing Goodies spawn from deaths of zombies, so won't be any spawning in this function
}

void StudentWorld::addPedsRandomly()
{
    //Human Peds
    double chanceHumanPed = max(200 - getLevel() * 10, 30);
    if (randInt(0, chanceHumanPed) == 0)
    {
        int randomXValue = randInt(0, VIEW_WIDTH);
        actors.push_back(new HumPed(randomXValue, VIEW_HEIGHT, this, racer));
    }
    //Zombie Peds
    double chanceZombiePed = max(100 - getLevel() * 10, 20);
    if (randInt(0, chanceZombiePed) == 0)
    {
        int randomXValue = randInt(0, VIEW_WIDTH);
        actors.push_back(new ZomPed(randomXValue, VIEW_HEIGHT, this, racer));
    }

}

void StudentWorld::addZomCabsRandomly()
{
    //step 1
    double chanceVehicle = max(100 - getLevel() * 10, 20);
    if (randInt(0, chanceVehicle) == 0)//step 2, 3
    {
        //step 1
        



        int leftLanefromTop = -1;
        int leftLanefromBottom = -1;
        int middleLanefromTop = -1;
        int middleLanefromBottom = -1;
        int rightLanefromTop = -1;
        int rightLanefromBottom = -1;
        
        findCollisionWorthyActors(leftLanefromTop, leftLanefromBottom, middleLanefromTop, middleLanefromBottom, rightLanefromTop, rightLanefromBottom);
        addZomCab(leftLanefromTop, leftLanefromBottom, middleLanefromTop, middleLanefromBottom, rightLanefromTop, rightLanefromBottom);
    }
}

bool StudentWorld::findCollisionWorthyActors(int &leftLanefromTop, int &leftLanefromBottom, int &middleLanefromTop, int &middleLanefromBottom, int &rightLanefromTop, int &rightLanefromBottom)
{
    
    for (int i = 0; i < actors.size() + 1; i++) //getting rid of the plus one makes cabs spawn and crash into ghostRacer
    {
        int y_pos = -1;
        bool found = false;
        int lane = 0;
        if (i == actors.size())
        {
            found = true;
            lane = laneNumber(racer);
            y_pos = racer->getY();
        }
        else 
        {
            if (actors[i]->isCollisionAvoidanceWorthy())
            {
                y_pos = actors[i]->getY();
                found = true;
                lane = laneNumber(actors[i]);
            }
            
        }
            //left lane is first
            if (found && lane == 1)
            {
                if (leftLanefromBottom == -1 || y_pos < leftLanefromBottom)
                    leftLanefromBottom = y_pos;
                if (leftLanefromTop == -1 || ((VIEW_HEIGHT - y_pos) < leftLanefromTop))
                    leftLanefromTop = VIEW_HEIGHT - y_pos;
            }
            //middle lane is second
            if (found && lane == 2)
            {
                if (middleLanefromBottom == -1 || y_pos < middleLanefromBottom)
                    middleLanefromBottom = y_pos;
                if (middleLanefromTop == -1 || ((VIEW_HEIGHT - y_pos) < middleLanefromTop))
                    middleLanefromTop = VIEW_HEIGHT - y_pos;
            }
            //right lane is third
            if (found && lane == 3)
            {
                if (rightLanefromBottom == -1 || y_pos < rightLanefromBottom)
                    rightLanefromBottom = y_pos;
                if (rightLanefromTop == -1 || ((VIEW_HEIGHT - y_pos) < rightLanefromTop))
                    rightLanefromTop = VIEW_HEIGHT - y_pos;
            }
        
    }
    return false;
}

bool StudentWorld::addZomCab(const int leftLanefromTop, const int leftLanefromBottom, const int middleLanefromTop, const int middleLanefromBottom, const int rightLanefromTop, const int rightLanefromBottom)
{
    int arr[] = { 1, 1, 1 };
    arr[0] = randInt(1, 3);
    do {
        arr[1] = randInt(1, 3);
    } while (arr[0] == arr[1]);
    do {
        arr[2] = randInt(1, 3);
    } while ((arr[0] == arr[1]) || (arr[1] == arr[2]) || (arr[0] == arr[2]));

    for (int i = 0; i <= 2; i++) //will run three times, once per each lane, break/return if you add a zomCab though
    {
        //find value for currentLane
        int cur_lane = arr[i];
        //now do all the checks
        if (cur_lane == 1)
        {
            if (leftLanefromBottom == -1 || leftLanefromBottom > (VIEW_HEIGHT / 3))
            {
                actors.push_back(new ZomCab(leftLaneCenterX, SPRITE_HEIGHT / 2, racer->getVerticalSpeed() + randInt(2,4), this, racer));
                return true;
            }
            else if (leftLanefromTop == -1 || leftLanefromTop < (2 * VIEW_HEIGHT / 3))
            {
                actors.push_back(new ZomCab(leftLaneCenterX, VIEW_HEIGHT - SPRITE_HEIGHT / 2, racer->getVerticalSpeed() - randInt(2, 4), this, racer));
                return true;
            }
        }
        else if (cur_lane == 2)
        {
            if (middleLanefromBottom == -1 || middleLanefromBottom > (VIEW_HEIGHT / 3))
            {
                actors.push_back(new ZomCab(middleLaneCenterX, SPRITE_HEIGHT / 2, racer->getVerticalSpeed() + randInt(2, 4), this, racer));
                return true;
            }
            else if (middleLanefromTop == -1 || middleLanefromTop < (2 * VIEW_HEIGHT / 3))
            {
                actors.push_back(new ZomCab(middleLaneCenterX, VIEW_HEIGHT - SPRITE_HEIGHT / 2, racer->getVerticalSpeed() - randInt(2, 4), this, racer));
                return true;
            }
        }
        else if (cur_lane == 3)
        {
            if (rightLanefromBottom == -1 || rightLanefromBottom > (VIEW_HEIGHT / 3))
            {
                actors.push_back(new ZomCab(rightLaneCenterX, SPRITE_HEIGHT / 2, racer->getVerticalSpeed() + randInt(2, 4), this, racer));
                return true;
            }
            else if (rightLanefromTop == -1 || rightLanefromTop < (2 * VIEW_HEIGHT / 3))
            {
                actors.push_back(new ZomCab(rightLaneCenterX, VIEW_HEIGHT - SPRITE_HEIGHT / 2, racer->getVerticalSpeed() - randInt(2, 4), this, racer));
                return true;
            }
        }
        else
            return false; //can't add to lane that doesn't even exist
    }
    return false;
}


