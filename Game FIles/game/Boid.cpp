#include "stdafx.h"
#include "Boid.h"

CBoid::CBoid(Sint16 x, Sint16 y, Uint32 time, char* pFileBitmap, CSpriteList &boids, 
	CSpriteList &pEnemies, CSprite &playerShip, CSprite &enemyBase , CSpriteList& pShots)
	: CSprite(x, y, 25, 25, time)
{
	//all pointers & variables passed into the constructor are initialized
	m_pBoids = &boids;
	m_pEnemies = &pEnemies;
	m_pPlayer =  &playerShip;
	m_enemyBase = &enemyBase;
	m_shots = &pShots;
	atks = 0;

	lastTime = 0;

	LoadImage(pFileBitmap, CColor::White());

	bad = false;
	state = NONE;
	ChangeState(IDLE);
	health = 100;

	SetImage(pFileBitmap);
	SetSize(25, 25);
}


CBoid::CBoid(Sint16 x, Sint16 y, Uint32 time, char* pFileBitmap, CSpriteList& boids,
	CSpriteList &pEnemies, CSprite &playerShip, CSprite &enemyBase, CSpriteList& pShots, bool AmIbad)
	: CSprite(x, y, 25, 25, time)
{
	//all pointers & variables passed into the constructor are initialized
	m_pBoids = &boids;
	m_pEnemies = &pEnemies;
	m_pPlayer = &playerShip;
	m_shots = &pShots;
	m_enemyBase = &enemyBase;
	atks = 0;
	bad = AmIbad;

	lastTime = 0;

	LoadImage(pFileBitmap, CColor::White());

	state = NONE;
	ChangeState(IDLE);
	health = 100;

	SetImage(pFileBitmap);
	SetSize(25,25);

}


CBoid::~CBoid()
{
}

void CBoid::OnUpdate(Uint32 time, Uint32 deltaTime)
{
	//determines radius that the boid will check to determined if an enemy is within their sight
	//player enemies have a slightly larger sight range than allies
	double spotEnemy;
	if (bad) spotEnemy = 140;
	if (!bad) spotEnemy = 120;

	//random values to tell the boid when they need to change direction randomely
	int turn = rand() % 360;
	int run = rand() & 270;

	//after approximately 1000 frames of fleeing or 2000 frames for chasing
	//stop fleeing/chasing
	stopFlee = 1000;
	stopChase = 2000;
	t = time;

	fleeTime = t - lastTime;
	chaseTime = t - lastTime;
	
	//if statements to make sure that the timers stay at the max if they're ever above
	if (fleeTime >= stopFlee) fleeTime = stopFlee;
	if (chaseTime >= stopChase) chaseTime = stopChase;


	
	//determines the distance of all enemies of the boid, and if 1 is within spot enemy radius
	//The boid will set that enemy's position as the enemyPosition variable
	float enemyDistance = 0;
	for each (CSprite * pSprite in *m_pEnemies)
	{
		enemyDistance = Distance(pSprite->GetPosition(), GetPosition());
		if (enemyDistance < spotEnemy)
		{
			enemyPosition = pSprite->GetPosition(); 
			break; 
		}

	}

	// State-dependent actions
	switch (state)
	{
	case IDLE:

		//if during idle, and below a certain health level, boid will go to either
		//enemystation or playerposition to heal back up to a certain amount
		if (!bad)
		{

			if (health <= 60 && Distance(m_pPlayer->GetPosition(), GetPosition()) > 50)
			{
				SetSpeed(GetSpeed() + 3);
				SetDirection(m_pPlayer->GetPosition() - GetPosition());
			}

			if (Distance(m_pPlayer->GetPosition(), GetPosition()) < 80)
			{
				SetHealth(GetHealth() + 2.0f);
				SetVelocity(0, 0);
			}
		}
		if (bad)
		{

			if (health < 40 && Distance(m_enemyBase->GetPosition(), GetPosition()) > 50)
			{
				SetSpeed(GetSpeed() + 1);
				SetDirection(m_enemyBase->GetPosition() - GetPosition());
			}

			if ((Distance(m_enemyBase->GetPosition(), GetPosition()) < 50))
			{
				SetHealth(GetHealth() + 0.5f);
				SetVelocity(0, 0);
			}
			
		}

		break;
	case PATROL:	
		// take a random turn at a random frame, on average once every 60 frames
		SetSpeed(GetSpeed() - 2);
		if (turn == 0)
		{
			SetDirection(rand() % 360);
		}

		//makes sure the boid doesn't leave the patrol radius, so boids aren't all across the screen when patrolling 
		if ((GetX() < patrolPosition.m_x - 50 || GetX() > patrolPosition.m_x + 50) 
			|| GetY() < patrolPosition.m_y - 50 || GetY() > patrolPosition.m_y + 50)
		{
			SetDirection(patrolPosition - GetPosition());

		}
		break;

	case CHASE:
		//during chase, allied boids speed up faster than enemy boids
		//to make up for how enemy boids have a large range and sight radius
		if (!bad)
			SetSpeed(GetSpeed() + 2);
		if (bad)
			SetSpeed(GetSpeed() + 0.2);

		//Go towards enemy that has been spotted
		SetDirection(enemyPosition - GetPosition());
		break;
	case ATTACK:

		//keeps pointing towards the enemy position, and shoots 2 lasers
		SetDirection(enemyPosition - GetPosition());
		shoot(GetVelocity(), time, deltaTime);
		break;
	case FLEE:
		//if allied to the player, boid will speed up quicker when fleeing
		if (!bad)
			SetSpeed(GetSpeed() + 2);
		if (bad)
			SetSpeed(GetSpeed() + 0.2);
		//checks enemy sprites within sight radius, and at a random frame
		//Boid will move away in a slightly random position from the enemy sprite found
		for each (CSprite * pSprite in *m_pEnemies)
		{
			if (Distance(pSprite->GetPosition(), GetPosition()) < spotEnemy && run == 0)
			{
				SetDirection((enemyPosition - GetPosition() + CVector((rand() % 45), (rand() % 45))) * -1);
			}
			//SetDirection((enemyPosition - GetPosition()) * -1);
		}
		break;
	case COMMAND:
		//Sets the direction to the attraction point
		SetDirection(c_attraction - GetPosition());

		break;

	case DIE:
		//RIP boid
		break;
	}

	

	// if in motion, rotate so that to follow the direction of the motion; otherwise reset rotation
	if (GetSpeed() > 0)
		SetRotation(GetDirection() - 90);

	// Transitions
	int count;
	switch (state)
	{
	case IDLE:

		//if health is above 60, Start Patrolling
		//Unles there's an enemy within sight radius 
		//Or the boid has been commanded to go somewhere
		if (health >= 60)
		{
			ChangeState(PATROL);

			if (enemyDistance < spotEnemy) ChangeState(CHASE);

			if (IsAttraction()) ChangeState(COMMAND);
		}

		if (health < 1) ChangeState(DIE);
		
		break;
	case PATROL:
		
		//during Patrol, if an enemy is within sight radius, then start chasing the enemy
		if (enemyDistance < spotEnemy) ChangeState(CHASE);

		//if health is too low, change state to IDLE so the boid can go back and heal
		if (health <= 40) ChangeState(IDLE);

		//The boid will only be commanded if it's health is above a certain amount
		if (IsAttraction() && health > 40) ChangeState(COMMAND);
		
		if (health < 1) ChangeState(DIE);
		break;
	case CHASE:
		
		//while chasing, if the enemy is too far out of range
		//or boid has too low of health
		//change state to IDLE
		if (enemyDistance > spotEnemy || health < 40) ChangeState(IDLE);

		//if boid has been chasing for too long
		//run away, so that the boid doesn't run too far into enemy territory
		if (chaseTime >= stopChase) ChangeState(FLEE);

		//if enemy being chased is within half the distance of sight radius
		//then ATTACK the enemy
		if (enemyDistance < spotEnemy / 2) ChangeState(ATTACK);
		
		if (health < 1) ChangeState(DIE);
		break;
	case ATTACK:

		//if the boid has attacked more than twice, they flee away
		if (atks >= 2)
		{
			fighting = false;
			ChangeState(FLEE);
		}

		if (health < 1) ChangeState(DIE);
		break;

	case FLEE:
		
		//if the boid has been fleeing for a certain amount of time, stop fleeing
		if (fleeTime >= stopFlee) ChangeState(IDLE);

		if (health < 1) ChangeState(DIE);

		break;
	case COMMAND:
		//if boid gets too close to their attraction postition, they will start patrolling around it
		if (Distance(c_attraction, GetPosition()) < 50)
		{
			SetAttraction();
			ChangeState(PATROL);
		}

		//if while going to a location, an enemy is spotted, then the boid will chase them
		if (enemyDistance < spotEnemy) ChangeState(CHASE);

		//if health is ever below a certain amount, IDLE so they can heal back up
		if (health <= 40) ChangeState(IDLE);

		if (health < 1) ChangeState(DIE);
		break;

	case DIE:
		//RIP boid
		break;
	}

	//radius of the boid for purposes of swarm intelligence
	double r = 15;

	//Vectors added up to determine the end result of where the boid must go
	CFVector sep = separate(r);
	CFVector ali = align(r * 2);
	CFVector coh = cohesion(r * 2);
	sep *= 1.5;
	ali *= 0.0;
	coh *= 0.0;
	
	CFVector vec = sep + ali + coh;

	//if the boid is ever fighting, then they ignore swarm rules for a second
	if (fighting)
	{
		vec = CFVector(0,0);
	}

	// Generic behaviour: bounce from the screen margins
	if ((GetX() < 32 || GetX() > 988) || (GetY() < 32 || GetY() > 720))
	{
		SetDirection(CVector(500, 300) - GetPosition());
	}

	//Applies swarm intelligence vectors to the current direction the boid is going
	SetDirection(GetXVelocity() + vec.GetX(), GetYVelocity() + vec.GetY());

	
	//safeguards boids so they don't go above or below maximum and minimum speed values
	if (GetSpeed() > fastSpeed)
		SetSpeed(fastSpeed);

	else if (GetSpeed() < slowSpeed)
		SetSpeed(slowSpeed);

	CSprite::OnUpdate(time, deltaTime);

	//Gets rid of boid if they have died, RIP
	m_pBoids->remove_if(deleted);

}


void CBoid::ChangeState(STATE newState)
{
	if (newState == state)
		return;		// No state change

	state = newState;
	switch (state)
	{
	case IDLE:
		//reset attacks and set speed to average
		SetSpeed(standardSpeed);
		atks = 0;
		break;
	case PATROL:
		//sets patrol position, resets attacks, and moves boid in random direction
		patrolPosition = GetPosition();
		SetDirection((float)(rand() % 360));
		atks = 0;
		break;
	case CHASE:
		//sets chase direction, sets speed to average, and starts the chase timer sequence
		SetDirection(enemyPosition - GetPosition());
		SetSpeed(standardSpeed);
		lastTime = t;
		break;
	case ATTACK:
		fighting = true;
		break;
	case FLEE:
		//Sets a slightly random direction the boid will go away from the nearest enemy, sets flee timer sequence
		SetDirection((enemyPosition - GetPosition() + CVector((rand() % 45), (rand() % 45))) * -1);
		lastTime = t;
		break;
	case COMMAND:
		//Sets speed to standard, direction to attraction position, resets attacks
		SetDirection(c_attraction - GetPosition());
		SetSpeed(standardSpeed);
		atks = 0;
		break;
	case DIE:
		//Kill boid :( RIP
		SetVelocity(0, 0);
		Delete();

		break;
	}
}

CFVector CBoid::separate(double radius)
{
	CFVector vec;
	int count = 0;

	//checks if boid is too close to another boid and if so, they move away
	for each (CBoid *pBoid in *m_pBoids)
		if (pBoid != this && BoidDistance(pBoid) <= radius && BoidDistance(pBoid) > 0)
		{
			vec += CFVector(GetPosition() - pBoid->GetPosition()).SetLength(100);
			count++;
		}
	//as above, but for enemy boids
	for each (CBoid * pBoid in *m_pEnemies)
		if (pBoid != this && BoidDistance(pBoid) <= radius && BoidDistance(pBoid) > 0)
		{
			vec += CFVector(GetPosition() - pBoid->GetPosition()).SetLength(100);
			count++;
		}
	//if allied to the player, treat them as if they were another boid if they get too close
	if (!bad)
	{
		if (Distance(m_pPlayer->GetPosition(), GetPosition()) <= radius+40 && Distance(m_pPlayer->GetPosition(), GetPosition()) > 0)
		{
			vec += CFVector(GetPosition() - m_pPlayer->GetPosition()).SetLength(100);
			count++;
		}
	}
	if (count == 0)
		return CFVector(0, 0);
	else
		return vec / count;
}

CFVector CBoid::align(double radius)
{
	CFVector vec;
	int count = 0;
	//if distance between boid and another ally boid is less than r, then align their destinations
	for each (CBoid *pBoid in *m_pBoids)
		if (pBoid != this && BoidDistance(pBoid) <= radius && BoidDistance(pBoid) > 0)
		{
			vec += CFVector(pBoid->GetXVelocity(), pBoid->GetYVelocity());
			count++;
		}

	if (count == 0)
		return CFVector(0, 0);
	else
		return vec / count;
}

CFVector CBoid::cohesion(double radius)
{
	CFVector vec;
	int count = 0;
	//if boid is close enough to another boid, then they will seek them out to stay in a group
	for each (CBoid *pBoid in *m_pBoids)
		if (pBoid != this && BoidDistance(pBoid) <= radius && BoidDistance(pBoid) > 0)
		{
			vec += pBoid->GetPosition();
			count++;
		}

	if (count == 0)
		return CFVector(0, 0);
	else
		return vec / count - GetPosition();
}


void CBoid::shoot(CVector target, Uint32 time, Uint32 deltaTime)
{
	if (atks < 2)
	{
		// dynamically creating a new shot sprite
		CSprite* newShot = new CSprite(GetX(), GetY()+7, 2, 10, "shot.bmp", CColor::White(), time);
		// set direction and speed the new shot sprite
		newShot->SetDirection(target + CVector((rand() % 50) - 26, (rand() % 50) - 26));
		newShot->SetRotation(newShot->GetDirection());
		newShot->SetSpeed(200 + GetSpeed());
		newShot->Die(700);

		m_shots->push_back(newShot);
		atks++;
	}
}