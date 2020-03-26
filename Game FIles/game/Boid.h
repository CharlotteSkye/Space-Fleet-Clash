#pragma once

#include "FVector.h"

class CBoid : public CSprite
{
	//States to control Boid's decision making and processes
	enum STATE { NONE, IDLE, PATROL, CHASE, ATTACK, FLEE, DIE, COMMAND};
	STATE state;

	//Vector of the position of the enemy if found
	CVector enemyPosition;
	//Vector recorded when patrolling starts
	CVector patrolPosition;

	float health;

	//timers to know when to stop fleeing or chasing
	Uint32 t;
	float fleeTime;
	float chaseTime;
	float lastTime;
	float stopFlee;
	float stopChase;

	//maximum, minimum, and average speed values
	float fastSpeed = 120;
	float slowSpeed = 50;
	float standardSpeed = 75;

	//ally boids, enemy boids, and shot lists
	CSpriteList *m_pBoids;
	CSpriteList *m_pEnemies;
	CSpriteList *m_shots;

	//pointers to the playership and enemy base
	CSprite* m_pPlayer;
	CSprite* m_enemyBase;
	
	//Vector to move towards if commanded
	CVector c_attraction;

	//bool to determine if the boid is attracted to something
	bool c_bAttraction;

	//bool to determine if the boid is attacking
	bool fighting;

	//bool to know if the boid is on the opposing team to the player
	bool bad;

	//number of attacks that have been shot
	int atks;

public:
	//Constructor for the player's boids
	CBoid(Sint16 x, Sint16 y, Uint32 time, char* pFileBitmap,
		CSpriteList &boids, CSpriteList &pObstacles, CSprite &playerShip, CSprite &enemyBase, CSpriteList &m_shots);
	
	//Constructor for the enemies boids
	CBoid(Sint16 x, Sint16 y, Uint32 time, char* pFileBitmap,
		CSpriteList &boids, CSpriteList &pObstacles, CSprite& playerShip, CSprite &enemyBase, CSpriteList& m_shots, bool bad);

	~CBoid(void);

	void SetAttraction(CVector attr)		{ c_bAttraction = true; c_attraction = attr; }
	void SetAttraction()					{ c_bAttraction = false; }
	bool IsAttraction()					{ return c_bAttraction; }
	CVector GetAttraction()				{ return c_attraction; }
	

	double BoidDistance(CBoid *pBoid)		{ return GetPosition().Distance(pBoid->GetPosition()); }

	//swarm intelligence code for boids
	CFVector separate(double radius);
	CFVector align(double radius);
	CFVector cohesion(double radius);

	bool GetFight() { return fighting; }

	float GetHealth() { return health; }

	void SetHealth(float newHealth) {health = newHealth;}

	void ChangeState(STATE newState);
	STATE GetState() { return state; }
	

	void shoot(CVector target, Uint32 time, Uint32 deltaTime);

	virtual void OnUpdate(Uint32 time, Uint32 deltaTime);
};
