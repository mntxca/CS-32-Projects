#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Actor::doSomethingBasic()
{
	double vert_speed = getVerticalSpeed() - getRacer()->getVerticalSpeed();
	double horiz_speed = getHorizontalSpeed();
	double new_y = getY() + vert_speed;
	double new_x = getX() + horiz_speed;
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
	if (getWorld()->overlaps(this, getRacer()))
	{
		play(SOUND_OIL_SLICK);
		getRacer()->spin();
	}
}


void Actor::play(const int sound)
{ 
	getWorld()->playSound(sound); 
}

/*bool Actor::overlaps(Actor* A, Actor* B)
{
	double delta_x = abs(A->getX() - B->getX());
	double delta_y = abs(A->getY() - B->getY());
	double radius_sum = A->getRadius() + B->getRadius();
	if (delta_x < radius_sum * .25 && delta_y < radius_sum * .6)
		return true;
	else
		return false;
}*/

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
		else if (keyStroke == KEY_PRESS_SPACE && holyWaterCount > 0)
		{
			double delta_x = (cos(getDirection() * PI / 180.0)) * SPRITE_HEIGHT;
			double delta_y = (sin(getDirection() * PI / 180.0)) * SPRITE_HEIGHT;
			getWorld()->addProjectile(getX() + delta_x, getY() + delta_y, getDirection());
			play(SOUND_PLAYER_SPRAY);
			holyWaterDecrement();
		}
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





void HealingGoodie::doSomething()
{
	doSomethingBasic();
	if (getWorld()->overlaps(this, getRacer()))
	{
		getRacer()->heal(10); //heal 10 hp
		nowDead();
		play(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(250);
	}

	//IMPLEMENT LATER, DO NOT FORGET
	/*if (getWorld()->overlaps(this, holyWaterProjectile))
	{
		nowDead();
	}*/
}

void HolyWaterGoodie::doSomething()
{
	doSomethingBasic();
	if (getWorld()->overlaps(this, getRacer()))
	{
		getRacer()->holyWaterPickup(10);//increases racer's holy water charges by 10
		nowDead();
		play(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
	}
	
	//IMPLEMENT LATER, DO NOT FORGET
	/*if (getWorld()->overlaps(this, holyWaterProjectile))
	{
		nowDead();
	}*/
}

void SoulGoodie::doSomething()
{
	doSomethingBasic();
	if (getWorld()->overlaps(this, getRacer()))
	{
		getWorld()->savedASoul();
		nowDead();
		play(SOUND_GOT_SOUL);
		getWorld()->increaseScore(100);
	}
	setDirection(getDirection() - 10); //10 degrees clockwise would be 10 degrees less
}

void ZomPed::doSomething()
{
	if (isAlive() == false)
		return;
	if (getWorld()->overlaps(this, getRacer()))
	{
		getRacer()->takeDamage(5);
		gotHurt();
		return;
	}
	if (getX() - getRacer()->getX() <= 30 && getX() - getRacer()->getX() >= -30 && getY() > getRacer()->getX())
	{
		setDirection(270);
		if (getX() - getRacer()->getX() < 0)
			setHorizontalSpeed(1);
		else if (getX() - getRacer()->getX() > 0)
			setHorizontalSpeed(-1);
		else
			setHorizontalSpeed(0);
		decTicksUntilNextGrunt();
		if (getTicksUntilNextGrunt() <= 0)
		{
			play(SOUND_ZOMBIE_ATTACK);
			setTicksUntilNextGrunt(20);
		}
	}
	doSomethingBasic();
	if (getMovementPlanDist() > 0)
	{
		decMovementPlanDist();
		return;
	}
	else
		newMovementPlan();
}

void ZomPed::gotHurt()
{
	takeDamage(1); //function automatically calls nowDead if necessary
	if (getWorld()->overlaps(this, getRacer()))
		takeDamage(1); //and it zomPed should now have 0 hp, so is dead
	if (isAlive() == false)
	{
		play(SOUND_PED_DIE);
		if (getWorld()->overlaps(this, getRacer()) == false)
			getWorld()->addHealingGoodieRandomly(getX(), getY());
		getWorld()->increaseScore(150);
	}
	else
		play(SOUND_PED_HURT);
}

void HumPed::doSomething()
{
	if (isAlive() == false)
		return;
	if (getWorld()->overlaps(this, getRacer()))
		getWorld()->fatality();
	doSomethingBasic();
	decMovementPlanDist();
	if (getMovementPlanDist() > 0)
		return;
	else
		newMovementPlan();
}

void HumPed::holyWaterHit()
{
	setHorizontalSpeed(-1 * getHorizontalSpeed());
	if (getDirection() == 180)
		setDirection(0);
	else
		setDirection(180);
	play(SOUND_PED_HURT);
}

void ZomCab::doSomething()
{
	//Step 1, checked
	if (isAlive() == false)
		return;
	//Step 2, checked
	if (getWorld()->overlaps(this, getRacer()))
	{
		if (hasDamagedGhostRacer)
			;//skip to step 3
		else
		{
			play(SOUND_VEHICLE_CRASH);
			getRacer()->takeDamage(20);
			if (getX() - getRacer()->getX() <= 0) //if ZomCab is to the left or on same x as the racer
			{
				setHorizontalSpeed(-5);
				setDirection(120 + randInt(0, 20));
			}
			else //if ZomCab is to the right of the racer
			{
				setHorizontalSpeed(5);
				setDirection(60 - randInt(0, 20));
			}
			hasDamagedGhostRacer = true;
		}
	}
	doSomethingBasic(); //Step 3, checked
	//Step 4
	/*int minInFront = 96;
	int minBehind = 96;
	int result = getWorld()->checkIfObjectInFrontOrBehindLane(this, minInFront, minBehind);*/
	
	if (getVerticalSpeed() > getRacer()->getVerticalSpeed()) //Step 4
		{
			if (getWorld()->minInFront(this))//&& (result == 1 || result == 3))
				{
					setVerticalSpeed(getVerticalSpeed() - 0.5);
					return;
				}
		}
	if (getVerticalSpeed() <= getRacer()->getVerticalSpeed()) //Step 5
		{
			if (getWorld()->minBehind(this))// && (result == 1 || result == 3))
				{
					setVerticalSpeed(getVerticalSpeed() + 0.5);
					return;
				}
		}
	decMovementPlanDist(); //step 6
	if (getMovementPlanDist() > 0) //step 7
		return;
	else //step 8
	{
     	setMovementPlanDist(randInt(4, 32));
		//setVerticalSpeed(getVerticalSpeed() + randInt(-10, 10));
		setVerticalSpeed(getVerticalSpeed() + randInt(-2, 2));
	}
}

void ZomCab::holyWaterHit()
{
	takeDamage(1); //function automatically calls nowDead if necessary
	if (isAlive() == false)
	{
		play(SOUND_VEHICLE_DIE);
		getWorld()->addOilSlickFromCabDeath(getX(), getY());
		getWorld()->increaseScore(200);
		return;
	}
	else
		play(SOUND_VEHICLE_HURT);
}

void MovingActor::newMovementPlan()
{
	int speed = randInt(1, 6);
	if (speed <= 3)
		speed -= 4;
	else
		speed -= 3;
	setHorizontalSpeed(speed);
	setMovementPlanDist(randInt(4, 32));
	if (getHorizontalSpeed() < 0)
		setDirection(180);
	else
		setDirection(0);
}

void Projectile::doSomething()
{
	//Step 1
	if (isAlive() == false)
		return;
	//Step 2
	Actor* activated = nullptr;
	activated = getWorld()->checkActivation(this);
	if (activated != nullptr)
	{
		activated->holyWaterHit();
		nowDead();
		return;
	}
	//Step 3
	moveForward(SPRITE_HEIGHT);
	//Step 4
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT)
	{
		nowDead();
		return;
	}
	//Step 5
	tickMoved();
}
