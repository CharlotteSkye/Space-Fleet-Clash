#pragma once

#include "FVector.h"

class CEnemyBoid 
{
	CSpriteList *m_boidEnemies;


public:
	CEnemyBoid(Sint16 x, Sint16 y, Uint32 time, CSpriteList& boids, CSpriteList &pObstacle, CSprite& playerShip, CSpriteList& m_shots);
	~CEnemyBoid(void);


};