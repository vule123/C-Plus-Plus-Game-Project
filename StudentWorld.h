#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

	virtual int init()
	{
		string levelFile = "level";
		if (getLevel() == 0)
			levelNum = "00";
		else if (getLevel() == 1)
			levelNum = "01";
		else if (getLevel() == 2)
			levelNum = "02";
		else if (getLevel() == 3)
			levelNum = "03";
		else 
			levelNum = "04";
		levelFile += levelNum;
		levelFile += ".dat";
		loadALevel(levelFile);
		m_bonus = 1000;
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		  // This code is here merely to allow the game to build, run, and terminate after hitting enter a few times 
		if (m_bonus > 0)
			m_bonus--;
		setDisplayText();

		list<Actor*>::iterator li = actors.begin();
		for (; li != actors.end(); li++)
		{
			(*li)->doSomething();
			(*li)->isAttacked();
		}

		li = actors.begin();
		while (li != actors.end())
		{
			Player* p = dynamic_cast<Player*>(*li);
			if (p != nullptr && !p->isAlive())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (!(*li)->isAlive())
			{
				delete (*li);
				li = actors.erase(li);
			}
			else
				li++;
		}
		
		if (!anyJewels())
		{
			setLevelFinished();
			if (getPlayer()->getX() == getExit()->getX() && getPlayer()->getY() == getExit()->getY())
			{
				increaseScore(2000 + m_bonus);
				playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
		list<Actor*>::iterator li = actors.begin();
		while (li != actors.end())
		{
			delete (*li);
			li = actors.erase(li);
		}
		actors.clear();
		while (!m_jewel.empty())
			m_jewel.pop_front();
	}

	int loadALevel(string level);
	void setDisplayText();
	void createABullet(int x, int y, GraphObject::Direction dir, bool fPlayer = false);
	void createAKleptoBot(int x, int y, RobotFactory::ProductType t);
	void createAnAmmo(int x, int y);
	void createARestoreHealth(int x, int y);
	void createAnExtraLife(int x, int y);
	
	Player* getPlayer() { return m_player; }
	list<Boulder*> getBoulder() { return m_boulder; }
	list<SnarlBot*> getSnarlBot() { return m_snarlbot; }
	list<RegularKleptoBot*> getRegularKleptoBot() { return m_rkleptobot; }
	list<AngryKleptoBot*> getAngryKleptoBot() { return m_akleptobot; }
	list<Bullet*> getBullet() { return m_bullet; }
	list<Jewel*> getJewel() { return m_jewel; }
	list<Goodie*> getGoodie() { return m_goodie; }
	Exit* getExit() { return m_exit; }
	list<Actor*> getActors() { return actors; }
	Actor* getAnActorAtLocation(int x, int y);
	int getLevelNum();
	
	bool isBoulder(int x, int y);
	bool isWall(int x, int y);
	bool isSnarlBot(int x, int y);
	bool isKleptoBot(int x, int y);
	bool isAngryKleptoBot(int x, int y);
	bool isFactory(int x, int y);
	bool isGoodie(int x, int y);
	bool isJewel(int x, int y);
	bool isHole(int x, int y);
	bool isAmmo(int x, int y);
	bool isRestoreHealth(int x, int y);
	bool isExtraLife(int x, int y);
	bool isPlayer(int x, int y);
	Boulder* getBoulderAt(int x, int y);

	void decJewels() { m_jewel.pop_front(); }
	bool anyJewels() { return m_jewel.size() > 0; }
	void setLevelFinished();

	~StudentWorld()
	{
		list<Actor*>::iterator li = actors.begin();
		while (li != actors.end())
		{
			li = actors.erase(li);
		}
	}

private:
	string levelNum;
	Player* m_player;
	list<Boulder*> m_boulder;
	list<SnarlBot*> m_snarlbot;
	list<Bullet*> m_bullet;
	list<Jewel*> m_jewel;
	list<RegularKleptoBot*> m_rkleptobot;
	list<AngryKleptoBot*> m_akleptobot;
	list<Goodie*> m_goodie;
	Exit* m_exit;
	int m_bonus;
	list<Actor*> actors;
};

#endif // STUDENTWORLD_H_
