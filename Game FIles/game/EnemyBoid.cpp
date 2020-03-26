#include "stdafx.h"
#include "EnemyBoid.h"


CEnemyBoid::CEnemyBoid(Sint16 x, Sint16 y, Uint32 time, CSpriteList& boids, CSpriteList& pEnemies, CSprite& playerShip, CSpriteList& pShots)
{
	m_boidEnemies = &pEnemies;
}

CEnemyBoid::~CEnemyBoid()
{
}
