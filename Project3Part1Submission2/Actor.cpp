#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomethingBasic()
{
	int vert_speed = getVerticalSpeed() - getRacer()->getVerticalSpeed();
	int horiz_speed = getHorizontalSpeed();
	int new_y = getY() + vert_speed;
	int new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);
	if (new_x < 0 || new_y < 0 || new_x > VIEW_WIDTH || new_y > VIEW_HEIGHT)
	{
		nowDead();
		return;
	}
}

void OilSlick::doSomething()
{
	doSomethingBasic();
	if (overlaps(this, getRacer()))
	{
		play(SOUND_OIL_SLICK);
		getRacer()->spin();
	}
}


void Actor::play(const int sound)
{ 
	getWorld()->playSound(sound); 
}

bool Actor::overlaps(Actor* A, Actor* B)
{
	double delta_x = abs(A->getX() - B->getX());
	double delta_y = abs(A->getY() - B->getY());
	double radius_sum = A->getRadius() + B->getRadius();
	if (delta_x < radius_sum * .25 && delta_y < radius_sum * .6)
		return true;
	else
		return false;
}

void GhostRacer::doSomething()
{
	int keyStroke = -1;
	if (isAlive() == false)
		return; // GhostRacer is dead and none of the following steps should be performed
	if(getX() <= LEFT_EDGE) //step 2, left road boundary
	{
		if(getDirection() > 90)
			takeDamage(10);
		setDirection(82);
		play(SOUND_VEHICLE_CRASH);
	}
	else if(getX() >= RIGHT_EDGE) //step 3, right road boundary
	{
		if(getDirection() < 90)
			takeDamage(10);
		setDirection(98);
		play(SOUND_VEHICLE_CRASH);

	}
	else if (getWorld()->getKey(keyStroke)) //step 4
	{
		if (keyStroke == KEY_PRESS_LEFT && getDirection() < 114)
			setDirection(getDirection() + 8);
		else if (keyStroke == KEY_PRESS_RIGHT && getDirection() > 66)
			setDirection(getDirection() - 8);
		else if (keyStroke == KEY_PRESS_UP && getVerticalSpeed() < 5)
			setVerticalSpeed(getVerticalSpeed() + 1);
		else if (keyStroke == KEY_PRESS_DOWN && getVerticalSpeed() > -1)
			setVerticalSpeed(getVerticalSpeed() - 1);
	}
	//step 5 is below
	double max_shift_per_tick = 4.0;
	int direction = getDirection();
	double delta_x = cos(direction * PI / 180.0) * max_shift_per_tick;
	double cur_x = getX();
	double cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);
		
}

void GhostRacer::spin()
{
	int degreesSpun = randInt(5, 20);
	bool clockWise = randInt(0, 1);
	if (clockWise)
	{
		if (-1 * degreesSpun + getDirection() < 60)
			setDirection(60);
		else
			setDirection(-1 * degreesSpun + getDirection());
	}
	else
	{
		if (degreesSpun + getDirection() > 120)
			setDirection(120);
		else
			setDirection(degreesSpun + getDirection());
	}
}


void MovingActor::takeDamage(int damage)
{
	hp -= damage;
	if (hp <= 0)
		nowDead();
}
/*
void ZomPed::doSomething()
{
}

void HumPed::doSomething()
{
}*/

void HealingGoodie::doSomething()
{
	doSomethingBasic();
	if (overlaps(this, getRacer()))
	{
		getRacer()->heal(10); //heal 10 hp
		nowDead();
		play(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(250);
	}

	//IMPLEMENT LATER, DO NOT FORGET
	/*if (overlaps(this, holyWaterProjectile))
	{
		nowDead();
	}*/
}

void HolyWaterGoodie::doSomething()
{
	doSomethingBasic();
	if (overlaps(this, getRacer()))
	{
		getRacer()->holyWaterPickup(10);//increases racer's holy water charges by 10
		nowDead();
		play(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
	}
	
	//IMPLEMENT LATER, DO NOT FORGET
	/*if (overlaps(this, holyWaterProjectile))
	{
		nowDead();
	}*/
}

void SoulGoodie::doSomething()
{
	doSomethingBasic();
	if (overlaps(this, getRacer()))
	{
		getWorld()->savedASoul();
		nowDead();
		play(SOUND_GOT_SOUL);
		getWorld()->increaseScore(100);
	}
	setDirection(getDirection() - 10); //10 degrees clockwise would be 10 degrees less
}
