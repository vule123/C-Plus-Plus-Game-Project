#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <iostream>
#include <cstdlib>
#include <list>
using namespace std;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* w, int imageID, int startX, int startY, Direction startD = none);
	virtual ~Actor() {}
	StudentWorld* getWorld() { return m_world; }
	bool isAlive() { return alive; }
	void setDead() { alive = false; }
	virtual void isAttacked() = 0;
	virtual void doSomething() = 0;
private:
	StudentWorld* m_world;
	bool alive;
};

class Player : public Actor
{
public:
	Player(StudentWorld* w, int imageID, int startX, int startY, Direction startD = right);
	virtual ~Player() {}
	void isAttacked();
	void doSomething();
	int getHealth() { return hitPts > 0 ? hitPts : 0; }
	int getAmmo() { return ammo > 0 ? ammo : 0; }
	void addAmmo(int howMany) { ammo += howMany; }
	void fullHealth() { hitPts = 20; }
private:
	int hitPts;
	int ammo;
};

class SnarlBot : public Actor
{
public:
	SnarlBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD = right);
	virtual ~SnarlBot() {}
	bool isShooting() { return shooting; }
	void setShooting() { shooting = true; }
	void stopShooting() { shooting = false; }
	void isAttacked();
	void doSomething();
private:
	bool shooting;
	int hitPts;
	int time;
};

class HorizSnarlBot : public SnarlBot
{
public:
	HorizSnarlBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD = right);
	virtual ~HorizSnarlBot() {}
};

class VertSnarlBot : public SnarlBot
{
public:
	VertSnarlBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD = down);
	virtual ~VertSnarlBot() {}
};

class KleptoBot : public Actor
{
public:
	KleptoBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD = none);
	virtual ~KleptoBot() {}
	int getDistance() { return distanceBeforeTurning; }
	void setDistance() {
		int d = rand() % 6 + 1;
		distanceBeforeTurning = d; }
	void resetDistanceMoved() { distanceMoved = 0; }
	int getDistanceMoved() { return distanceMoved; }
	void incDistanceMoved() { distanceMoved++; }
	int getTime() { return time; }
	void resetTime() { time = 0; }
	void incTime() { time++; }
	void pickDirection();
	bool canMove(Direction d);

	virtual void isAttacked() { return; }
	virtual void doSomething() { return; }
private:
	int time;
	int distanceMoved;
	int distanceBeforeTurning;
	bool find;
};

class Goodie : public Actor
{
public:
	Goodie(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~Goodie() {}
	void isAttacked() { return; }
	virtual void doSomething() {}
};

class RegularKleptoBot : public KleptoBot
{
public:
	RegularKleptoBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD = none);
	virtual ~RegularKleptoBot() { delete stolen; }
	void isAttacked();
	void doSomething();
private:
	int hitPts;
	Goodie* stolen;
	int typeStolen;
};

class AngryKleptoBot : public KleptoBot
{
public:
	AngryKleptoBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD = none);
	virtual ~AngryKleptoBot() { delete stolen; }
	bool isShooting() { return shooting; }
	void setShooting() { shooting = true; }
	void stopShooting() { shooting = false; }
	void isAttacked();
	void doSomething();
private:
	int hitPts;
	bool shooting;
	Goodie* stolen;
	int typeStolen;
};

class Wall : public Actor
{
public:
	Wall(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~Wall() {}
	
	void isAttacked() { return; }
	void doSomething() { return; }
};

class Bullet : public Actor
{
public:
	Bullet(StudentWorld* w, int imageID, int startX, int startY, bool fPlayer, Direction startD = right);
	virtual ~Bullet() {}
	bool isFromPlayer() { return fromPlayer; }

	void isAttacked() { return; }
	void doSomething();
private:
	bool fromPlayer;
	bool stop;
};

class Boulder : public Actor
{
public:
	Boulder(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~Boulder() {}
	void isAttacked();
	void isPushed(const Direction& dir);
	void doSomething() { return; }
private:
	int hitPts;
};

class Hole : public Actor
{
public:
	Hole(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~Hole() {}
	void isAttacked() { return; }
	void doSomething();
};

class Jewel : public Actor
{
public:
	Jewel(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~Jewel() {}
	void isAttacked() { return; }
	void doSomething();
};

class RestoreHealth : public Goodie
{
public:
	RestoreHealth(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~RestoreHealth() {}
	void isAttacked() { return; }
	void doSomething();
};

class ExtraLife : public Goodie
{
public:
	ExtraLife(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~ExtraLife() {}
	void isAttacked() { return; }
	void doSomething();
};

class Ammo : public Goodie
{
public:
	Ammo(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~Ammo() {}
	void isAttacked() { return; }
	void doSomething();
};

class RobotFactory : public Actor
{
public:
	enum ProductType { REGULAR, ANGRY };
	RobotFactory(StudentWorld* w, int imageID, int startX, int startY, ProductType type);
	virtual ~RobotFactory() {}
	ProductType getProductType() { return m_type; }
	void isAttacked() { return; }
	void doSomething();
private:
	ProductType m_type;
};

class Exit : public Actor
{
public:
	Exit(StudentWorld* w, int imageID, int startX, int startY);
	virtual ~Exit() {}
	void isRevealed();
	void isAttacked() { return; }
	void doSomething() { return; }
};

#endif // ACTOR_H_
