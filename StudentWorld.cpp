#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::loadALevel(string level)
{
	string curLevel = level;
	Level lev("Assets");
	Level::LoadResult result = lev.loadLevel(curLevel);
	if (result == Level::load_fail_file_not_found ||
		result == Level::load_fail_bad_format)
		return -1;
	
	for (int x = 0; x < VIEW_WIDTH; x++)
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			Level::MazeEntry item = lev.getContentsOf(x, y);
			Boulder* b = nullptr;
			SnarlBot* s = nullptr;
			Jewel* j = nullptr;
			RestoreHealth* r = nullptr;
			ExtraLife* e = nullptr;
			Ammo* a = nullptr;
			switch (item)
			{
			case Level::player:
				m_player = new Player(this, IID_PLAYER, x, y);
				actors.push_back(m_player);
				break;
			case Level::wall:
				actors.push_back(new Wall(this, IID_WALL, x, y));
				break;
			case Level::boulder:
				b = new Boulder(this, IID_BOULDER, x, y);
				m_boulder.push_front(b);
				actors.push_front(b);
				break;
			case Level::horiz_snarlbot:
				s = new HorizSnarlBot(this, IID_SNARLBOT, x, y);
				m_snarlbot.push_front(s);
				actors.push_front(s);
				break;
			case Level::vert_snarlbot:
				s = new VertSnarlBot(this, IID_SNARLBOT, x, y);
				m_snarlbot.push_front(s);
				actors.push_front(s);
				break;
			case Level::hole:
				actors.push_back(new Hole(this, IID_HOLE, x, y));
				break;
			case Level::jewel:
				j = new Jewel(this, IID_JEWEL, x, y);
				m_jewel.push_back(j);
				actors.push_back(j);
				break;
			case Level::restore_health:
				r = new RestoreHealth(this, IID_RESTORE_HEALTH, x, y);
				m_goodie.push_back(r);
				actors.push_back(r);
				break;
			case Level::extra_life:
				e = new ExtraLife(this, IID_EXTRA_LIFE, x, y);
				m_goodie.push_back(e);
				actors.push_back(e);
				break;
			case Level::ammo:
				a = new Ammo(this, IID_AMMO, x, y);
				m_goodie.push_back(a);
				actors.push_back(a);
				break;
			case Level::kleptobot_factory:
				actors.push_back(new RobotFactory(this, IID_ROBOT_FACTORY, x, y, RobotFactory::REGULAR));
				break;
			case Level::angry_kleptobot_factory:
				actors.push_back(new RobotFactory(this, IID_ROBOT_FACTORY, x, y, RobotFactory::ANGRY));
				break;
			case Level::exit:
				m_exit = new Exit(this, IID_EXIT, x, y);
				actors.push_back(m_exit);
				break;
			}
		}
	}
	return 0;
}

void StudentWorld::createABullet(int x, int y, GraphObject::Direction dir, bool fPlayer)
{
	Bullet* bullet = nullptr;
	switch (dir)
	{
	case GraphObject::left:
		bullet = new Bullet(this, IID_BULLET, x - 1, y, fPlayer, dir);
		break;
	case GraphObject::right:
		bullet = new Bullet(this, IID_BULLET, x + 1, y, fPlayer, dir);
		break;
	case GraphObject::up:
		bullet = new Bullet(this, IID_BULLET, x, y + 1, fPlayer, dir);
		break;
	case GraphObject::down:
		bullet = new Bullet(this, IID_BULLET, x, y - 1, fPlayer, dir);
		break;
	}
	m_bullet.push_back(bullet);
	actors.push_back(bullet);
}

void StudentWorld::createAKleptoBot(int x, int y, RobotFactory::ProductType t)
{
	if (t == RobotFactory::ProductType::REGULAR)
	{
		RegularKleptoBot* rBot = new RegularKleptoBot(this, IID_KLEPTOBOT, x, y, GraphObject::Direction::right);
		m_rkleptobot.push_front(rBot);
		actors.push_front(rBot);
	}
	if (t == RobotFactory::ProductType::ANGRY)
	{
		AngryKleptoBot* aBot = new AngryKleptoBot(this, IID_ANGRY_KLEPTOBOT, x, y, GraphObject::Direction::right);
		m_akleptobot.push_front(aBot);
		actors.push_front(aBot);
	}
}

void StudentWorld::createAnAmmo(int x, int y)
{
	Ammo* a = new Ammo(this, IID_AMMO, x, y);
	m_goodie.push_back(a);
	actors.push_back(a);
}

void StudentWorld::createARestoreHealth(int x, int y)
{
	RestoreHealth* a = new RestoreHealth(this, IID_RESTORE_HEALTH, x, y);
	m_goodie.push_back(a);
	actors.push_back(a);
}

void StudentWorld::createAnExtraLife(int x, int y)
{
	ExtraLife* a = new ExtraLife(this, IID_EXTRA_LIFE, x, y);
	m_goodie.push_back(a);
	actors.push_back(a);
}

string toString(int a, int wide, char c)
{
	ostringstream oss;
	oss.fill(c);
	oss << setw(wide) << a;
	return oss.str();
}

string formatString(string score, string level, string lives, string health, string ammo, string bonus)
{
	return "Score: " + score + "  Level: " + level + "  Lives: " + lives + "  Health: " +
		health + "%" + "  Ammo: " + ammo + "  Bonus: " + bonus;
}

void StudentWorld::setDisplayText()
{
	string score = toString(GameWorld::getScore(), 7, '0');
	string lives = toString(GameWorld::getLives(), 2, ' ');
	string health = toString(m_player->getHealth() * 100 / 20, 3, ' ');
	string ammo = toString(m_player->getAmmo(), 3, ' ');
	string bonus = toString(m_bonus, 4, ' ');
	string status = formatString(score, levelNum, lives, health, ammo, bonus);
	GameWorld::setGameStatText(status);
}

Actor* StudentWorld::getAnActorAtLocation(int x, int y)
{
	list<Actor*>::iterator li = actors.begin();
	while (li != actors.end())
	{
		if ((*li)->getX() == x && (*li)->getY() == y)
			return *li;
		li++;
	}
	return nullptr;
}

bool StudentWorld::isWall(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	Wall* b = dynamic_cast<Wall*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isBoulder(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	Boulder* b = dynamic_cast<Boulder*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isHole(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	Hole* b = dynamic_cast<Hole*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isJewel(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	Jewel* b = dynamic_cast<Jewel*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isGoodie(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	Goodie* b = dynamic_cast<Goodie*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isSnarlBot(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	SnarlBot* b = dynamic_cast<SnarlBot*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isKleptoBot(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	KleptoBot* b = dynamic_cast<KleptoBot*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isAngryKleptoBot(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	AngryKleptoBot* b = dynamic_cast<AngryKleptoBot*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isFactory(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	RobotFactory* b = dynamic_cast<RobotFactory*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isAmmo(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	Ammo* b = dynamic_cast<Ammo*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isRestoreHealth(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	RestoreHealth* b = dynamic_cast<RestoreHealth*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isExtraLife(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	ExtraLife* b = dynamic_cast<ExtraLife*>(a);
	if (b != nullptr)
		return true;
	return false;
}

bool StudentWorld::isPlayer(int x, int y)
{
	Actor* a = getAnActorAtLocation(x, y);
	Player* b = dynamic_cast<Player*>(a);
	if (b != nullptr)
		return true;
	return false;
}

Boulder* StudentWorld::getBoulderAt(int x, int y)
{
	list<Boulder*>::iterator li = m_boulder.begin();
	while (li != m_boulder.end())
	{
		if ((*li)->getX() == x && (*li)->getY() == y)
			return *li;
		li++;
	}
	return nullptr;
}

void StudentWorld::setLevelFinished()
{
	if (!m_exit->isVisible())
	{
		playSound(SOUND_REVEAL_EXIT);
		m_exit->isRevealed();
	}
}
