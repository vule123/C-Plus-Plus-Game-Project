#include "Actor.h"
#include "GameWorld.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : GraphObject(imageID, startX, startY, startD), alive(true)
{
	m_world = w;
	setVisible(true);
}

Player::Player(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : Actor(w, imageID, startX, startY, startD), 
hitPts(20), ammo(20)
{
	setVisible(true);
}

void Player::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		int ch;
		StudentWorld* wo = nullptr;
		int newX = 0, newY = 0;
		if (getWorld()->getKey(ch))
		{
			int newX = 0, newY = 0;
			switch (ch)
			{
			case KEY_PRESS_LEFT:
				setDirection(left);
				newX = getX() - 1;
				newY = getY();
				if (getWorld()->isBoulder(newX, newY))
					getWorld()->getBoulderAt(newX, newY)->isPushed(left);
				wo = getWorld();
				if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isKleptoBot(newX, newY)
					&& !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isHole(newX, newY))
					moveTo(newX, newY);
				break;
			case KEY_PRESS_RIGHT:
				setDirection(right);
				newX = getX() + 1;
				newY = getY();
				if (getWorld()->isBoulder(newX, newY))
					getWorld()->getBoulderAt(newX, newY)->isPushed(right);
				wo = getWorld();
				if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isKleptoBot(newX, newY)
					&& !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isHole(newX, newY))
					moveTo(newX, newY);
				break;
			case KEY_PRESS_UP:
				setDirection(up);
				newX = getX();
				newY = getY() + 1;
				if (getWorld()->isBoulder(newX, newY))
					getWorld()->getBoulderAt(newX, newY)->isPushed(up);
				wo = getWorld();
				if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isKleptoBot(newX, newY)
					&& !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isHole(newX, newY))
					moveTo(newX, newY);
				break;
			case KEY_PRESS_DOWN:
				setDirection(down);
				newX = getX();
				newY = getY() - 1;
				if (getWorld()->isBoulder(newX, newY))
					getWorld()->getBoulderAt(newX, newY)->isPushed(down);
				wo = getWorld();
				if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isKleptoBot(newX, newY)
					&& !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isHole(newX, newY))
					moveTo(newX, newY);
				break;
			case KEY_PRESS_SPACE:
				if (getWorld()->getPlayer()->getAmmo() > 0)
				{
					getWorld()->createABullet(getX(), getY(), getDirection(), true);
					getWorld()->playSound(SOUND_PLAYER_FIRE);
					getWorld()->getPlayer()->ammo--;
				}
				break;
			case KEY_PRESS_ESCAPE:
				setDead();
				break;
			}

		}
	}
}

void Player::isAttacked()
{
	list<Bullet*> b = getWorld()->getBullet();
	list<Bullet*>::iterator li = b.begin();

	StudentWorld* wo = getWorld();
	while (li != b.end())
	{
		if (getX() == (*li)->getX() && getY() == (*li)->getY() && !(*li)->isFromPlayer())
		{
			hitPts -= 2;
			if (hitPts > 0)
				getWorld()->playSound(SOUND_PLAYER_IMPACT);
			(*li)->setDead();
			break;
		}
		li++;
	}
	if (hitPts <= 0)
	{
		setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
	}
}

Wall::Wall(StudentWorld* w, int imageID, int startX, int startY) : Actor(w, imageID, startX, startY)
{
	setVisible(true);
}

Bullet::Bullet(StudentWorld* w, int imageID, int startX, int startY, bool fPlayer, Direction startD) : Actor(w, imageID, startX, startY, startD),
fromPlayer(fPlayer), stop(true)
{
	setVisible(true);
}

void Bullet::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		if (stop)
		{
			stop = false;
			return;
		}
		StudentWorld* wo = getWorld();
		Direction d = getDirection();
		int newX = 0, newY = 0;
		if (!wo->isBoulder(getX(), getY()) && !wo->isFactory(getX(), getY()) && !wo->isGoodie(getX(), getY()) && !wo->isKleptoBot(getX(), getY())
			&& !wo->isSnarlBot(getX(), getY()) && !wo->isWall(getX(), getY()) && !wo->isHole(getX(), getY()) && !wo->isPlayer(getX(), getY())
			&& !wo->isAngryKleptoBot(getX(), getY()))
		{
			switch (d)
			{
			case left:
				newX = getX() - 1;
				newY = getY();
				moveTo(newX, newY);
				if (wo->isBoulder(getX(), getY()) || wo->isFactory(getX(), getY()) || wo->isGoodie(getX(), getY()) || wo->isKleptoBot(getX(), getY())
					|| wo->isSnarlBot(getX(), getY()) || wo->isWall(getX(), getY()) || wo->isHole(getX(), getY()) || wo->isPlayer(getX(), getY())
					|| wo->isAngryKleptoBot(getX(), getY()))
					setVisible(false);
				break;
			case right:
				newX = getX() + 1;
				newY = getY();
				moveTo(newX, newY);
				if (wo->isBoulder(getX(), getY()) || wo->isFactory(getX(), getY()) || wo->isGoodie(getX(), getY()) || wo->isKleptoBot(getX(), getY())
					|| wo->isSnarlBot(getX(), getY()) || wo->isWall(getX(), getY()) || wo->isHole(getX(), getY()) || wo->isPlayer(getX(), getY())
					|| wo->isAngryKleptoBot(getX(), getY()))
					setVisible(false);
				break;
			case up:
				newX = getX();
				newY = getY() + 1;
				moveTo(newX, newY);
				if (wo->isBoulder(getX(), getY()) || wo->isFactory(getX(), getY()) || wo->isGoodie(getX(), getY()) || wo->isKleptoBot(getX(), getY())
					|| wo->isSnarlBot(getX(), getY()) || wo->isWall(getX(), getY()) || wo->isHole(getX(), getY()) || wo->isPlayer(getX(), getY())
					|| wo->isAngryKleptoBot(getX(), getY()))
					setVisible(false);
				break;
			case down:
				newX = getX();
				newY = getY() - 1;
				moveTo(newX, newY);
				if (wo->isBoulder(getX(), getY()) || wo->isFactory(getX(), getY()) || wo->isGoodie(getX(), getY()) || wo->isKleptoBot(getX(), getY())
					|| wo->isSnarlBot(getX(), getY()) || wo->isWall(getX(), getY()) || wo->isHole(getX(), getY()) || wo->isPlayer(getX(), getY())
					|| wo->isAngryKleptoBot(getX(), getY()))
					setVisible(false);
				break;
			}
		}
		else
			setDead();
	}
}

Boulder::Boulder(StudentWorld* w, int imageID, int startX, int startY) : Actor(w, imageID, startX, startY), hitPts(10)
{
	setVisible(true);
}

void Boulder::isAttacked()
{
	list<Bullet*> b = getWorld()->getBullet();
	list<Bullet*>::iterator li = b.begin();
	while (li != b.end())
	{
		if (getX() == (*li)->getX() && getY() == (*li)->getY())
		{
			hitPts -= 2;
			break;
		}
		li++;
	}
	if (hitPts <= 0)
		setDead();
}

void Boulder::isPushed(const Direction& dir)
{
	if (!isAlive())
		return;
	else
	{
		int newX = 0, newY = 0;
		StudentWorld* wo = nullptr;
		switch (dir)
		{
		case left:
			newX = getX() - 1;
			newY = getY();
			wo = getWorld();
			if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
				&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY))
				moveTo(newX, newY);
			break;
		case right:
			newX = getX() + 1;
			newY = getY();
			wo = getWorld();
			if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
				&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY))
				moveTo(newX, newY);
			break;
		case up:
			newX = getX();
			newY = getY() + 1;
			wo = getWorld();
			if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
				&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY))
				moveTo(newX, newY);
			break;
		case down:
			newX = getX();
			newY = getY() - 1;
			wo = getWorld();
			if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
				&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY))
				moveTo(newX, newY);
			break;
		}
	}
}

SnarlBot::SnarlBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : Actor(w, imageID, startX, startY, startD), 
hitPts(10), time(0), shooting(false)
{
	setVisible(true);
}

void SnarlBot::doSomething()
{
	if ((getDirection() == right || getDirection() == left) && getY() != getWorld()->getPlayer()->getY())
		stopShooting();
	if ((getDirection() == up || getDirection() == down) && getX() != getWorld()->getPlayer()->getX())
		stopShooting();
	if (!isAlive())
		return;
	else 
	{
		Direction dir = getDirection();
		StudentWorld* wo = getWorld();
		int newX = 0, newY = 0;
		int ticks = (28 - getWorld()->getLevel()) / 4;
		if (ticks < 3)
			ticks = 3;

		if (time % ticks == 0)
		{
			time = 0;
			switch (dir)
			{
			case right:
				newX = getX() + 1;
				newY = getY();
	
				if (!isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
					&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY)
					&& !wo->isHole(newX, newY) && !wo->isPlayer(newX, newY))
					moveTo(newX, newY);
				else if (!isShooting() && !wo->isPlayer(newX, newY))
				{
					setDirection(left);
				}
				else {}
				break;
			case left:
				newX = getX() - 1;
				newY = getY();
				
				if (!isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
					&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY)
					&& !wo->isHole(newX, newY) && !wo->isPlayer(newX, newY))
					moveTo(newX, newY);
				else if (!isShooting() && !wo->isPlayer(newX, newY))
					setDirection(right);
				else {}
				break;
			case up:
				newX = getX();
				newY = getY() + 1;

				if (!isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
					&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY)
					&& !wo->isHole(newX, newY) && !wo->isPlayer(newX, newY))
					moveTo(newX, newY);
				else if (!isShooting() && !wo->isPlayer(newX, newY))
				{
					setDirection(down);
				}
				else {}
				break;
			case down:
				newX = getX();
				newY = getY() - 1;

				if (!isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) && !wo->isGoodie(newX, newY) && !wo->isJewel(newX, newY)
					&& !wo->isKleptoBot(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) && !wo->isAmmo(newX, newY)
					&& !wo->isHole(newX, newY) && !wo->isPlayer(newX, newY))
					moveTo(newX, newY);
				else if (!isShooting() && !wo->isPlayer(newX, newY))
				{
					setDirection(up);
				}
				else {}
				break;
			}

			int y = getY();
			if (y == getWorld()->getPlayer()->getY())
			{
				wo = getWorld();
				bool hit = false;
				int begin = 0, end = 0, dis = 0;
				switch (dir)
				{
				case left:
					begin = getX();
					end = getWorld()->getPlayer()->getX();
					dis = end - begin;
					if (dis < 0)
					{
						for (int i = 1; i < (dis * (-1)); i++)
						{
							
							if (wo->isBoulder(begin - i, y) || wo->isFactory(begin - i, y) || wo->isGoodie(begin - i, y) || wo->isJewel(begin - i, y)
								|| wo->isKleptoBot(begin - i, y) || wo->isSnarlBot(begin - i, y) || wo->isWall(begin - i, y) || wo->isAmmo(begin - i, y)
								|| wo->isHole(begin - i, y))
								hit = true;
						}
						if (!hit)
						{
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}

					break;
				case right:
					begin = getX();
					end = getWorld()->getPlayer()->getX();
					dis = end - begin;
					wo = getWorld();
					if (dis > 0)
					{
						for (int i = 1; i < end - begin; i++)
						{
							
							if (wo->isBoulder(begin + i, y) || wo->isFactory(begin + i, y) || wo->isGoodie(begin + i, y) || wo->isJewel(begin + i, y)
								|| wo->isKleptoBot(begin + i, y) || wo->isSnarlBot(begin + i, y) || wo->isWall(begin + i, y) || wo->isAmmo(begin + i, y)
								|| wo->isHole(begin + i, y))
								hit = true;

						}
						
						if (!hit)
						{
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}
					break;
				}
			}

			int x = getX();
			if (x == getWorld()->getPlayer()->getX())
			{
				wo = getWorld();
				bool hit = false;
				int begin = 0, end = 0, dis = 0;
				switch (dir)
				{
				case down:
					begin = getY();
					end = getWorld()->getPlayer()->getY();
					dis = end - begin;
					
					if (dis < 0)
					{
						for (int i = 1; i < (dis * (-1)); i++)
						{
							
							if (wo->isBoulder(x, begin - i) || wo->isFactory(x, begin - i) || wo->isGoodie(x, begin - i) || wo->isJewel(x, begin - i)
								|| wo->isKleptoBot(x, begin - i) || wo->isSnarlBot(x, begin - i) || wo->isWall(x, begin - i) || wo->isAmmo(x, begin - i)
								|| wo->isHole(x, begin - i))
								hit = true;
						}
						if (!hit)
						{
							
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}
					

					break;
				case up:
					begin = getY();
					end = getWorld()->getPlayer()->getY();
					dis = end - begin;
					wo = getWorld();
					if (dis > 0)
					{
						for (int i = 1; i < end - begin; i++)
						{
							
							if (wo->isBoulder(x, begin + i) || wo->isFactory(x, begin + i) || wo->isGoodie(x, begin + i) || wo->isJewel(x, begin + i)
								|| wo->isKleptoBot(x, begin + i) || wo->isSnarlBot(x, begin + i) || wo->isWall(x, begin + i) || wo->isAmmo(x, begin + i)
								|| wo->isHole(x, begin + i))
								hit = true;

						}
						
						if (!hit)
						{
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}
					break;
				}
			}
		}
		time++;
	}
}

void SnarlBot::isAttacked()
{
	list<Bullet*> b = getWorld()->getBullet();
	list<Bullet*>::iterator li = b.begin();
	
	StudentWorld* wo = getWorld();
	while (li != b.end())
	{
		if (getX() == (*li)->getX() && getY() == (*li)->getY() && (*li)->isFromPlayer())
		{
			hitPts -= 2;
			if (hitPts > 0)
				getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		}
		li++;
	}
	if (hitPts <= 0)
	{
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(100);
	}
}

HorizSnarlBot::HorizSnarlBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : SnarlBot(w, imageID, startX, startY, startD)
{
	setVisible(true);
}

VertSnarlBot::VertSnarlBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : SnarlBot(w, imageID, startX, startY, startD)
{
	setVisible(true);
}

KleptoBot::KleptoBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : Actor(w, imageID, startX, startY, startD), time(0), find(false)
{
	resetDistanceMoved();
	setVisible(true);
}

void KleptoBot::pickDirection()
{
	int d = rand() % 4;
	
	if (!canMove(left) && !canMove(right) && !canMove(up) && !canMove(down))
	{
		if (d == 0)
			setDirection(left);
		else if (d == 1)
			setDirection(right);
		else if (d == 2)
			setDirection(up);
		else if (d == 3)
			setDirection(down);
	}
	else
	{
		if (!find)
		{
			find = true;
			if (d == 0) setDirection(left);
			else if (d == 1) setDirection(right);
			else if (d == 2) setDirection(up);
			else if (d == 3) setDirection(down);
			return;
		}
		int a[4] = { -1, -1, -1, -1 };
		while (true)
		{
			if ((d == 0 && canMove(left)))
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
				find = false;
				break;
			}
			else if (d == 1 && canMove(right))
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
				find = false;
				break;
			}
			else if (d == 2 && canMove(up))
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
				find = false;
				break;
			}
			else if (d == 3 && canMove(down))
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
				find = false;
				break;
			}
			else
			{
				if(d == 0) a[0] = 0;
				if (d == 1) a[1] = 1;
				if (d == 2) a[2] = 2;
				if (d == 3) a[3] = 3;
				d = rand() % 4;
				for (int j = 0; j < 4; j++)
				{
					if (d == a[j])
					{
						d = rand() % 4;
						j = -1;
					}
				}
			}
		}
	}
}

bool KleptoBot::canMove(Direction d)
{
	int newX = 0, newY = 0;
	StudentWorld* wo = getWorld();
	switch (d)
	{
	case left:
		newX = getX() - 1;
		newY = getY();
		if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) 
			&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
			&& !wo->isHole(newX, newY))
			return true;
		break;
	case right:
		newX = getX() + 1;
		newY = getY();
		if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) 
			&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
			&& !wo->isHole(newX, newY))
			return true;
		break;
	case down:
		newX = getX();
		newY = getY() - 1;
		if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
			&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
			&& !wo->isHole(newX, newY))
			return true;
		break;
	case up:
		newX = getX();
		newY = getY() + 1;
		if (!wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY) 
			&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
			&& !wo->isHole(newX, newY))
			return true;
		break;
	}
	return false;
}

RegularKleptoBot::RegularKleptoBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : KleptoBot(w, imageID, startX, startY, startD),
hitPts(5), stolen(nullptr), typeStolen(0)
{
	setVisible(true);
}

void RegularKleptoBot::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		StudentWorld* wo = getWorld();
		list<Goodie*> goodie = getWorld()->getGoodie();
		list<Goodie*>::iterator li = goodie.begin();
		while (li != goodie.end())
		{
			if (getX() == (*li)->getX() && getY() == (*li)->getY())
			{
				if (rand() % 10 == 1 && stolen == nullptr && (*li)->isVisible())
				{
					(*li)->setVisible(false);
					getWorld()->playSound(SOUND_ROBOT_MUNCH);
					stolen = *li;
					int a = (*li)->getX();
					int b = (*li)->getY();
					if (wo->isAmmo(a, b)) typeStolen = 1;
					if (wo->isRestoreHealth(a, b)) typeStolen = 2;
					if (wo->isExtraLife(a, b)) typeStolen = 3;
					(*li)->setDead();
					return;
				}
			}
			li++;
		}

		Direction dir = getDirection();
		int newX = 0, newY = 0;
		int ticks = (28 - getWorld()->getLevel()) / 4;
		if (ticks < 3)
			ticks = 3;

		if (getTime() % ticks == 0)
		{
			switch (dir)
			{
			case right:
				newX = getX() + 1;
				newY = getY();
				
				if (getDistanceMoved() < getDistance() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) 
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			case left:
				newX = getX() - 1;
				newY = getY();
				
				if (getDistanceMoved() < getDistance() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) 
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			case up:
				newX = getX();
				newY = getY() + 1;
				
				if (getDistanceMoved() < getDistance() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			case down:
				newX = getX();
				newY = getY() - 1;
				
				if (getDistanceMoved() < getDistance() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY) 
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			}
			
		}
		incTime();
	}
}

void RegularKleptoBot::isAttacked()
{
	list<Bullet*> b = getWorld()->getBullet();
	list<Bullet*>::iterator li = b.begin();

	StudentWorld* wo = getWorld();
	while (li != b.end())
	{
		if (getX() == (*li)->getX() && getY() == (*li)->getY() && (*li)->isFromPlayer())
		{
			hitPts -= 2;
			if (hitPts > 0)
				getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		}
		li++;
	}
	if (hitPts <= 0)
	{
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(10);
		if (stolen != nullptr)
		{
			if (typeStolen == 1)
			{
				getWorld()->createAnAmmo(getX(), getY());
				stolen = nullptr;
			}
			else if (typeStolen == 2)
			{
				getWorld()->createARestoreHealth(getX(), getY());
				stolen = nullptr;
			}
			else if (typeStolen == 3)
			{
				getWorld()->createAnExtraLife(getX(), getY());
				stolen = nullptr;
			}
		}
	}
}

AngryKleptoBot::AngryKleptoBot(StudentWorld* w, int imageID, int startX, int startY, Direction startD) : KleptoBot(w, imageID, startX, startY, startD),
hitPts(8), shooting(false), stolen(nullptr), typeStolen(0)
{
	setVisible(true);
}

void AngryKleptoBot::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		StudentWorld* wo = getWorld();
		list<Goodie*> goodie = getWorld()->getGoodie();
		list<Goodie*>::iterator li = goodie.begin();
		while (li != goodie.end())
		{
			if (getX() == (*li)->getX() && getY() == (*li)->getY())
			{
				if (rand() % 10 == 1 && stolen == nullptr && (*li)->isVisible())
				{
					(*li)->setVisible(false);
					getWorld()->playSound(SOUND_ROBOT_MUNCH);
					stolen = *li;
					int a = (*li)->getX();
					int b = (*li)->getY();
					if (wo->isAmmo(a, b)) typeStolen = 1;
					if (wo->isRestoreHealth(a, b)) typeStolen = 2;
					if (wo->isExtraLife(a, b)) typeStolen = 3;
					return;
				}
			}
			li++;
		}
		if ((getDirection() == right || getDirection() == left) && getY() != getWorld()->getPlayer()->getY())
			stopShooting();
		if ((getDirection() == up || getDirection() == down) && getX() != getWorld()->getPlayer()->getX())
			stopShooting();

		Direction dir = getDirection();
		int newX = 0, newY = 0;
		int ticks = (28 - getWorld()->getLevel()) / 4;
		if (ticks < 3)
			ticks = 3;

		if (getTime() % ticks == 0)
		{
			resetTime();
			switch (dir)
			{
			case right:
				newX = getX() + 1;
				newY = getY();
				
				if (getDistanceMoved() < getDistance() && !isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else if (wo->isPlayer(newX, newY))
				{
					setShooting();
					getWorld()->createABullet(getX(), getY(), dir);
					getWorld()->playSound(SOUND_ENEMY_FIRE);
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			case left:
				newX = getX() - 1;
				newY = getY();
				
				if (getDistanceMoved() < getDistance() && !isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else if (wo->isPlayer(newX, newY))
				{
					setShooting();
					getWorld()->createABullet(getX(), getY(), dir);
					getWorld()->playSound(SOUND_ENEMY_FIRE);
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			case up:
				newX = getX();
				newY = getY() + 1;
				
				if (getDistanceMoved() < getDistance() && !isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else if (wo->isPlayer(newX, newY))
				{
					setShooting();
					getWorld()->createABullet(getX(), getY(), dir);
					getWorld()->playSound(SOUND_ENEMY_FIRE);
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			case down:
				newX = getX();
				newY = getY() - 1;
				
				if (getDistanceMoved() < getDistance() && !isShooting() && !wo->isBoulder(newX, newY) && !wo->isFactory(newX, newY)
					&& !wo->isPlayer(newX, newY) && !wo->isSnarlBot(newX, newY) && !wo->isWall(newX, newY)
					&& !wo->isHole(newX, newY))
				{
					moveTo(newX, newY);
					incDistanceMoved();
				}
				else if (wo->isPlayer(newX, newY))
				{
					setShooting();
					getWorld()->createABullet(getX(), getY(), dir);
					getWorld()->playSound(SOUND_ENEMY_FIRE);
				}
				else
				{
					pickDirection();
					setDistance();
					resetDistanceMoved();
				}
				break;
			}
			int y = getY();
			if (y == getWorld()->getPlayer()->getY())
			{
				wo = getWorld();
				bool hit = false;
				int begin = 0, end = 0, dis = 0;
				switch (dir)
				{
				case left:
					begin = getX();
					end = getWorld()->getPlayer()->getX();
					dis = end - begin;
					if (dis < 0)
					{
						for (int i = 1; i < (dis * (-1)); i++)
						{
						
							if (wo->isBoulder(begin - i, y) || wo->isFactory(begin - i, y) || wo->isGoodie(begin - i, y) || wo->isJewel(begin - i, y)
								|| wo->isKleptoBot(begin - i, y) || wo->isSnarlBot(begin - i, y) || wo->isWall(begin - i, y) || wo->isAmmo(begin - i, y)
								|| wo->isHole(begin - i, y))
								hit = true;
						
						}
						if (!hit)
						{
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}

					break;
				case right:
					begin = getX();
					end = getWorld()->getPlayer()->getX();
					dis = end - begin;
					wo = getWorld();
					if (dis > 0)
					{
						for (int i = 1; i < end - begin; i++)
						{
							if (wo->isBoulder(begin + i, y) || wo->isFactory(begin + i, y) || wo->isGoodie(begin + i, y) || wo->isJewel(begin + i, y)
								|| wo->isKleptoBot(begin + i, y) || wo->isSnarlBot(begin + i, y) || wo->isWall(begin + i, y) || wo->isAmmo(begin + i, y)
								|| wo->isHole(begin + i, y))
								hit = true;
						
						}
						
						if (!hit)
						{
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}
					break;
				}
			}

			int x = getX();
			if (x == getWorld()->getPlayer()->getX())
			{
				wo = getWorld();
				bool hit = false;
				int begin = 0, end = 0, dis = 0;
				switch (dir)
				{
				case down:
					begin = getY();
					end = getWorld()->getPlayer()->getY();
					dis = end - begin;
					
					if (dis < 0)
					{
						for (int i = 1; i < (dis * (-1)); i++)
						{
							
							if (wo->isBoulder(x, begin - i) || wo->isFactory(x, begin - i) || wo->isGoodie(x, begin - i) || wo->isJewel(x, begin - i)
								|| wo->isKleptoBot(x, begin - i) || wo->isSnarlBot(x, begin - i) || wo->isWall(x, begin - i) || wo->isAmmo(x, begin - i)
								|| wo->isHole(x, begin - i))
								hit = true;
						}
						if (!hit)
						{
							
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}

					break;
				case up:
					begin = getY();
					end = getWorld()->getPlayer()->getY();
					dis = end - begin;
					wo = getWorld();
					if (dis > 0)
					{
						for (int i = 1; i < end - begin; i++)
						{
							
							if (wo->isBoulder(x, begin + i) || wo->isFactory(x, begin + i) || wo->isGoodie(x, begin + i) || wo->isJewel(x, begin + i)
								|| wo->isKleptoBot(x, begin + i) || wo->isSnarlBot(x, begin + i) || wo->isWall(x, begin + i) || wo->isAmmo(x, begin + i)
								|| wo->isHole(x, begin + i))
								hit = true;

						}
						
						if (!hit)
						{
							setShooting();
							getWorld()->createABullet(getX(), getY(), dir);
							getWorld()->playSound(SOUND_ENEMY_FIRE);
						}
					}
					break;
				}
			}
		}
		incTime();
	}
}

void AngryKleptoBot::isAttacked()
{
	list<Bullet*> b = getWorld()->getBullet();
	list<Bullet*>::iterator li = b.begin();

	StudentWorld* wo = getWorld();
	while (li != b.end())
	{
		if (getX() == (*li)->getX() && getY() == (*li)->getY() && (*li)->isFromPlayer())
		{
			hitPts -= 2;
			if (hitPts > 0)
				getWorld()->playSound(SOUND_ROBOT_IMPACT);
			break;
		}
		li++;
	}
	if (hitPts <= 0)
	{
		setDead();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(20);
		if (stolen != nullptr)
		{
			if (typeStolen == 1)
			{
				getWorld()->createAnAmmo(getX(), getY());
				stolen = nullptr;
			}
			else if (typeStolen == 2)
			{
				getWorld()->createARestoreHealth(getX(), getY());
				stolen = nullptr;
			}
			else if (typeStolen == 3)
			{
				getWorld()->createAnExtraLife(getX(), getY());
				stolen = nullptr;
			}
		}
	}
}

Hole::Hole(StudentWorld* w, int imageID, int startX, int startY) : Actor(w, imageID, startX, startY)
{
	setVisible(true);
}

void Hole::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		list<Boulder*> b = getWorld()->getBoulder();
		list<Boulder*>::iterator li = b.begin();
		while (li != b.end())
		{
			if (getX() == (*li)->getX() && getY() == (*li)->getY())
			{
				getWorld()->getBoulderAt(getX(), getY())->setDead();
				setDead();
			}
			li++;
		}
	}
}

Jewel::Jewel(StudentWorld* w, int imageID, int startX, int startY) : Actor(w, imageID, startX, startY)
{
	setVisible(true);
}

void Jewel::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		Player* p = getWorld()->getPlayer();
		if (getX() == p->getX() && getY() == p->getY())
		{
			getWorld()->increaseScore(50);
			setDead();
			getWorld()->decJewels();
			getWorld()->playSound(SOUND_GOT_GOODIE);
		}
	}
}

Goodie::Goodie(StudentWorld* w, int imageID, int startX, int startY) : Actor(w, imageID, startX, startY)
{
	setVisible(true);
}

RestoreHealth::RestoreHealth(StudentWorld* w, int imageID, int startX, int startY) : Goodie(w, imageID, startX, startY)
{
	setVisible(true);
}

void RestoreHealth::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		Player* p = getWorld()->getPlayer();
		if (isVisible() && getX() == p->getX() && getY() == p->getY())
		{
			getWorld()->increaseScore(500);
			setDead();
			getWorld()->getPlayer()->fullHealth();
			getWorld()->playSound(SOUND_GOT_GOODIE);
		}
	}
}

ExtraLife::ExtraLife(StudentWorld* w, int imageID, int startX, int startY) : Goodie(w, imageID, startX, startY)
{
	setVisible(true);
}

void ExtraLife::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		Player* p = getWorld()->getPlayer();
		if (isVisible() && getX() == p->getX() && getY() == p->getY())
		{
			getWorld()->increaseScore(1000);
			setDead();
			getWorld()->incLives();
			getWorld()->playSound(SOUND_GOT_GOODIE);
		}
	}
}

Ammo::Ammo(StudentWorld* w, int imageID, int startX, int startY) : Goodie(w, imageID, startX, startY)
{
	setVisible(true);
}

void Ammo::doSomething()
{
	if (!isAlive())
		return;
	else
	{
		Player* p = getWorld()->getPlayer();
		if (isVisible() && getX() == p->getX() && getY() == p->getY())
		{
			getWorld()->increaseScore(100);
			setDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->getPlayer()->addAmmo(20);
		}
	}
}

RobotFactory::RobotFactory(StudentWorld* w, int imageID, int startX, int startY, ProductType type) : Actor(w, imageID, startX, startY), m_type(type)
{
	setVisible(true);
}

void RobotFactory::doSomething()
{
	int count = 0;
	for (int i = 1; i < 4; i++)
	{
		if (getX() - i >= 0 && getWorld()->isKleptoBot(getX() - i, getY()))
			count++;
		if (getX() + i <= VIEW_WIDTH && getWorld()->isKleptoBot(getX() + i, getY()))
			count++;
		if (getY() - i >= 0 && getWorld()->isKleptoBot(getX(), getY() - i))
			count++;
		if (getY() + i <= VIEW_HEIGHT && getWorld()->isKleptoBot(getX(), getY() + i))
			count++;
	}
	if (count < 3 && !getWorld()->isKleptoBot(getX(), getY()))
	{
		if (rand() % 50 == 1)
		{
			getWorld()->createAKleptoBot(getX(), getY(), getProductType());
			getWorld()->playSound(SOUND_ROBOT_BORN);
		}
	}
}

Exit::Exit(StudentWorld* w, int imageID, int startX, int startY) : Actor(w, imageID, startX, startY)
{
	setVisible(false);
}

void Exit::isRevealed()
{
	setVisible(true);
	getWorld()->playSound(SOUND_REVEAL_EXIT);
}