#include "stdafx.h"
#include "MyGame.h"
#include "Boid.h"

CMyGame::CMyGame(void) :
	playerShip(300, 300, 120, 120, "playerShip (2).png", CColor::White(), 0),
	enemyStation(600,500, 300, 300, "enemySpaceStation.bmp",CColor::White(), 0),
	m_background("stars.gif")
	
{
	// TODO: add initialisation here
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here


}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	//Win and lose conditions
	if (playerShip.GetHealth() <= 0)
	{
		GameOver();
	}
	if (enemyStation.GetHealth() <= 0)
	{
		GameOver();
	}


	Uint32 t = GetTime();

	//sets cooldown timer for lazer
	cooldown = t - lastTime;

	//enemy station constantly repairs itself over time slightly
	enemyStation.SetHealth(enemyStation.GetHealth() + 0.5);

	//if player has shot their lazer, then put it on cooldown
	if (lazer)
	{
		lastTime = t;
	}


	CVector pos = playerShip.GetPosition();

	//makes sure cooldown doesn't go over max amount
	if (cooldown >= 10000) cooldown = 10000;

	//Update code for all sprites
	for each (CBoid *pBoid in m_allyboids1)
		pBoid->Update(GetTime());

	for each (CBoid * pBoid in m_allyboids2)
		pBoid->Update(GetTime());

	for each (CBoid * pBoid in m_allyboids3)
		pBoid->Update(GetTime());

	for each (CBoid * pBoid in m_AllyshotList)
		pBoid->Update(GetTime());

	for each (CBoid * pBoid in m_EnemyshotList)
		pBoid->Update(GetTime());

	for each (CBoid * pBoid in m_enemyboids)
		pBoid->Update(GetTime());

	for each  (CSprite* pSprite in explosions) 
		pSprite->Update(GetTime());

	for each (CBoid * pBoid in m_enemyboids)
		pBoid->Update(GetTime());

	playerShip.Update(GetTime());
	enemyStation.Update(GetTime());

	//checks if any enemies have been hit by a shot from player boids
	//and lowers their health if they've been hit
	for each (CSprite * pShot in m_AllyshotList)
	{
		for each (CBoid * pBoid in m_enemyboids)
		{
			float r = 10;

			if (Distance(pBoid->GetPosition(), pShot->GetPosition()) < r)
			{
				pBoid->SetHealth(pBoid->GetHealth() - 20);
				pShot->Delete();
			}
		}

		float r = 100;

		if (Distance(enemyStation.GetPosition(), pShot->GetPosition()) < r)
		{
			enemyStation.SetHealth(enemyStation.GetHealth() - 10);
			pShot->Delete();
		}


	}

	//checks if enemy boids have been hit by lazer, if so they take massive damage
	for each (CSprite * pShot in explosions)
	{
		for each (CBoid * pBoid in m_enemyboids)
		{
			float r = 10;
			CRectangle hit;
			pShot->GetBoundingRect(hit);
			if (hit.Contains(pBoid->GetCenter()))
			{
				pBoid->SetHealth(pBoid->GetHealth() - 3);
			}
		}
	}

	//checks if any allies have been hit by a shot from enemy boids
	//and lowers their health if they've been hit
	for each (CSprite * pShot in m_EnemyshotList)
	{
		float r = 10;
		for each (CBoid * pBoid in m_allyboids1)
		{
			if (Distance(pBoid->GetPosition(), pShot->GetPosition()) < r)
			{
				pBoid->SetHealth(pBoid->GetHealth() - 20);
				pShot->Delete();
			}
		}

		for each (CBoid * pBoid in m_allyboids2)
		{
			if (Distance(pBoid->GetPosition(), pShot->GetPosition()) < r)
			{
				pBoid->SetHealth(pBoid->GetHealth() - 20);
				pShot->Delete();
			}
		}

		for each (CBoid * pBoid in m_allyboids3)
		{
			if (Distance(pBoid->GetPosition(), pShot->GetPosition()) < r)
			{
				pBoid->SetHealth(pBoid->GetHealth() - 20);
				pShot->Delete();
			}
		}

		if (Distance(playerShip.GetPosition(), pShot->GetPosition()) < 20)
		{
			playerShip.SetHealth(playerShip.GetHealth() - 5);
			pShot->Delete();
		}

	}

	//after a random amount of frames, an enemy boid will go towards the player ship
	for each (CBoid * pBoid in m_enemyboids)
	{
		int r = rand() % 1000;
		if (r == 0)
		{
			pBoid->SetAttraction(playerShip.GetPosition());
		}
	}

	//after a random amount of frames, an enemy or ally boid will be spawned 
	//at the position fo the player ship or below the enemy station
	int spawnShip = rand() % 3000;

	if (spawnShip < 3)
	{
		bool bad = true;
		CBoid* pBoid = new CBoid(enemyStation.GetX(), enemyStation.GetY() - 100, GetTime(), "SimplePixelSpaceships_Enemy.bmp",
			allEnemies, allAllies, playerShip, enemyStation, m_EnemyshotList, bad);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		pBoid->SetAttraction(CVector(enemyStation.GetX(), enemyStation.GetY() - 150));
		m_enemyboids.push_back(pBoid);
		allEnemies.push_back(pBoid);
	}
	if (spawnShip == 4)
	{
		CBoid* pBoid = new CBoid(playerShip.GetX() + 50, playerShip.GetY() + 50, GetTime(), "SimplePixelSpaceships_Ally.bmp",
			allAllies, allEnemies, playerShip, enemyStation, m_AllyshotList);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		m_allyboids1.push_back(pBoid);
		allAllies.push_back(pBoid);

	}
	if (spawnShip == 5)
	{
		CBoid* pBoid = new CBoid(playerShip.GetX() - 50, playerShip.GetY() - 50, GetTime(), "SimplePixelSpaceships_Red.bmp",
			allAllies, allEnemies, playerShip, enemyStation, m_AllyshotList);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		m_allyboids2.push_back(pBoid);
		allAllies.push_back(pBoid);
	}
	if (spawnShip == 6)
	{
		CBoid* pBoid = new CBoid(playerShip.GetX() - 50, playerShip.GetY(), GetTime(), "SimplePixelSpaceships_Purple.bmp",
			allAllies, allEnemies, playerShip, enemyStation, m_AllyshotList);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		m_allyboids3.push_back(pBoid);
		allAllies.push_back(pBoid);
	}

	//player movement
	if (IsKeyDown(SDLK_w))
	{
		playerShip.SetSpeed(30);

		if ((playerShip.GetX()-50 < 32 || playerShip.GetX()+50 > 988) || (playerShip.GetY()-50 < 32 || playerShip.GetY()+50 > 720))
		{
			playerShip.SetPosition(pos);
		}

	}
	else if (!IsKeyDown(SDLK_w))
	{
		playerShip.SetSpeed(0);
	}
	if (IsKeyDown(SDLK_a))
	{

		playerShip.Rotate(-0.5);
		playerShip.SetDirection(playerShip.GetRotation());

	}
	if (IsKeyDown(SDLK_d))
	{

		playerShip.Rotate(0.5);
		playerShip.SetDirection(playerShip.GetRotation());

	}

	

	//deletion code
	m_allyboids1.remove_if(deleted);
	m_allyboids2.remove_if(deleted);
	m_allyboids3.remove_if(deleted);
	m_AllyshotList.remove_if(deleted);
	m_EnemyshotList.remove_if(deleted);
	m_enemyboids.remove_if(deleted);
	allEnemies.remove_if(deleted);
	explosions.remove_if(deleted);
	
}

void CMyGame::OnDraw(CGraphics* g)
{

	g->Blit(CVectorI(0, 0), m_background);

	for each (CSprite * pSprite in m_AllyshotList)
		pSprite->Draw(g);

	for each (CSprite * pSprite in m_EnemyshotList)
		pSprite->Draw(g);


	playerShip.Draw(g);

	enemyStation.Draw(g);

	for each (CBoid * pBoid in m_allyboids1)
	{
		pBoid->Draw(g);
		DrawHealth(g, pBoid->GetPosition() + CVector(-10, 20), 10, 4, pBoid->GetHealth(), CColor::Blue());
	}

	for each (CBoid * pBoid in m_allyboids2)
	{
		pBoid->Draw(g);
		DrawHealth(g, pBoid->GetPosition() + CVector(-10, 20), 10, 4, pBoid->GetHealth(), CColor::Red());
	}
	for each (CBoid * pBoid in m_allyboids3)
	{
		pBoid->Draw(g);
		DrawHealth(g, pBoid->GetPosition() + CVector(-10, 20), 10, 4, pBoid->GetHealth(), CColor::Magenta());
	}

	for each (CBoid * pBoid in m_enemyboids)
	{
		pBoid->Draw(g);
		DrawHealth(g, pBoid->GetPosition() + CVector(-10, 20), 10, 4, pBoid->GetHealth(), CColor::DarkGray());
	}

	for (CSprite* pSprite : explosions) pSprite->Draw(g);

	DrawHealth(g, playerShip.GetPosition() + CVector(-50, 50), 1, 4, cooldown, CColor::LightRed());
	DrawHealth(g, playerShip.GetPosition() + CVector(-10, 30), 20, 4, playerShip.GetHealth(), CColor::LightGreen());

	DrawHealth(g, enemyStation.GetPosition() + CVector(-50, -30), 50, 10, enemyStation.GetHealth(), CColor::LightGray());


	if (activegroup == m_allyboids1)
	{
		*g << font(20) << color(CColor::LightBlue()) << xy(50, 725) << "Squad: 1";

		if (clicking)
		{
			g->DrawHLine(clickpos + CVector(-5, 0), clickpos.GetX() - 20, CColor::LightBlue());
			g->DrawHLine(clickpos + CVector(5, 0), clickpos.GetX() + 20, CColor::LightBlue());
			g->DrawVLine(clickpos + CVector(0, -5), clickpos.GetY() - 20, CColor::LightBlue());
			g->DrawVLine(clickpos + CVector(0, 5), clickpos.GetY() + 20, CColor::LightBlue());
		}

	}
	else if (activegroup == m_allyboids2)
	{
		*g << font(20) << color(CColor::Red()) << xy(50, 725) << "Squad: 2";
		if (clicking)
		{
			g->DrawHLine(clickpos + CVector(-5, 0), clickpos.GetX() - 20, CColor::Red());
			g->DrawHLine(clickpos + CVector(5, 0), clickpos.GetX() + 20, CColor::Red());
			g->DrawVLine(clickpos + CVector(0, -5), clickpos.GetY() - 20, CColor::Red());
			g->DrawVLine(clickpos + CVector(0, 5), clickpos.GetY() + 20, CColor::Red());
		}
	}
	else if (activegroup == m_allyboids3)
	{
		*g << font(20) << color(CColor::Magenta()) << xy(50, 725) << "Squad: 3";
		if (clicking)
		{
			g->DrawHLine(clickpos + CVector(-5, 0), clickpos.GetX() - 20, CColor::Magenta());
			g->DrawHLine(clickpos + CVector(5, 0), clickpos.GetX() + 20, CColor::Magenta());
			g->DrawVLine(clickpos + CVector(0, -5), clickpos.GetY() - 20, CColor::Magenta());
			g->DrawVLine(clickpos + CVector(0, 5), clickpos.GetY() + 20, CColor::Magenta());
		}
	}
	else if (activegroup == allAllies)
	{
		
		*g << font(20) << color(CColor::Cyan()) << xy(50, 725) << "Squad: All Squads";
		if (clicking)
		{
			g->DrawHLine(clickpos + CVector(-5, 0), clickpos.GetX() - 20, CColor::Cyan());
			g->DrawHLine(clickpos + CVector(5, 0), clickpos.GetX() + 20, CColor::Cyan());
			g->DrawVLine(clickpos + CVector(0, -5), clickpos.GetY() - 20, CColor::Cyan());
			g->DrawVLine(clickpos + CVector(0, 5), clickpos.GetY() + 20, CColor::Cyan());
		}
	}

	if (IsGameOver())
	{
		if (playerShip.GetHealth() <= 0)
			*g << font(48) << color(CColor::DarkRed()) << vcenter << center << "GAME OVER" << endl;
		else if (enemyStation.GetHealth() <= 0)
			*g << font(48) << color(CColor::DarkRed()) << vcenter << center << "YOU WIN" << endl;
	}
}

void CMyGame::DrawHealth(CGraphics* g, CVector pos, float w, float h, float health, CColor fillcolor)
{
	if (health <= 0) return;
	g->DrawRect(CRectangle(pos.m_x - 1, pos.m_y - 1, w + 1, h + 1), CColor::Black());
	g->FillRect(CRectangle(pos.m_x, pos.m_y - 1, ceil(w * health / 100.f), h), health > 20 ? fillcolor : CColor::DarkRed());
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	playerShip.SetHealth(100);
	enemyStation.SetHealth(200);
}

// called at the start of a new game - display menu here
void CMyGame::OnDisplayMenu()
{
	StartGame();	// this allows to start the game immediately
}

// called when Game Mode entered
void CMyGame::OnStartGame()
{
	//resets everything
	lastTime = 0;

	m_allyboids1.delete_all();
	m_allyboids2.delete_all();
	m_allyboids3.delete_all();
	m_enemyboids.delete_all();
	m_AllyshotList.delete_all();
	m_EnemyshotList.delete_all();

	for each (CBoid * pBoid in allAllies)
	{
		if (pBoid != &playerShip)
		{
			pBoid->Delete();
		}
	}
	for each (CBoid * pBoid in allEnemies)
	{
		if (pBoid != &enemyStation)
		{
			pBoid->Delete();
		}
	}
	allAllies.remove_if(deleted);
	allEnemies.remove_if(deleted);

	playerShip.SetPosition(200,300);
	playerShip.SetHealth(100);
	enemyStation.SetHealth(200);

	enemyStation.SetPosition(800,600);

	allAllies.push_back(&playerShip);
	allEnemies.push_back(&enemyStation);

	//spawns player and enemy ships
	for (int i = 0; i < BOIDNUM; i++)
	{
		//Spawns up to BOIDNUM ally ships in squad number 1
		CBoid* pBoid = new CBoid(playerShip.GetX() + 50, playerShip.GetY() + 50, GetTime(), "SimplePixelSpaceships_Ally.bmp",
			allAllies, allEnemies, playerShip, enemyStation, m_AllyshotList);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		m_allyboids1.push_back(pBoid);
		allAllies.push_back(pBoid);
	}

	for (int i = 0; i < BOIDNUM; i++)
	{
		//Spawns up to BOIDNUM ally ships in squad number 1
		CBoid* pBoid = new CBoid(playerShip.GetX() - 50, playerShip.GetY() - 50, GetTime(), "SimplePixelSpaceships_Red.bmp",
			allAllies, allEnemies, playerShip, enemyStation, m_AllyshotList);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		m_allyboids2.push_back(pBoid);
		allAllies.push_back(pBoid);
	}
	for (int i = 0; i < BOIDNUM; i++)
	{
		//Spawns up to BOIDNUM ally ships in squad number 1
		CBoid* pBoid = new CBoid(playerShip.GetX() - 50, playerShip.GetY(), GetTime(), "SimplePixelSpaceships_Purple.bmp",
			allAllies, allEnemies, playerShip, enemyStation, m_AllyshotList);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		m_allyboids3.push_back(pBoid);
		allAllies.push_back(pBoid);
	}

	for (int i = 0; i < ENEMYBOID; i++)
	{
		int spawnX;
		int spawnY;
		if (i >= 0 && i < 4) { spawnX = 750; spawnY = 368; }
		if (i >= 4 && i < 8) { spawnX = 100; spawnY = 680; }
		if (i >= 8 && i < 12) { spawnX = 400; spawnY = 68; }
		if (i >= 12 && i < 16) { spawnX = 550; spawnY = 568; }
		if (i >= 16 && i <= 20) { spawnX = 800; spawnY = 80; }
		bool bad = true;
		CBoid* pBoid = new CBoid(spawnX, spawnY, GetTime(), "SimplePixelSpaceships_Enemy.bmp",
			allEnemies, allAllies, playerShip, enemyStation, m_EnemyshotList, bad);
		pBoid->SetDirection(rand() % 360);
		pBoid->SetSpeed(100);
		m_enemyboids.push_back(pBoid);
		allEnemies.push_back(pBoid);
	}

	//sets active group to squad 1
	activegroup = m_allyboids1;
}

// called when Game is Over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	//depending on what key has been pressed last
	//the player will have access to a certain squad
	if (sym == SDLK_1)
	{
		activegroup = m_allyboids1;
	}
	if (sym == SDLK_2)
	{
		activegroup = m_allyboids2;
	}
	if (sym == SDLK_3)
	{
		activegroup = m_allyboids3;
	}
	if (sym == SDLK_4)
	{
		//Was going to implement a way for ALL boids to be selected
		//for some reason, when all allies are directed to go somewhere
		//the enemy station will move around randomely as well as disapear and reappear 
		//in random locations :-(

		//activegroup = allAllies;
	}

	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	//if I'm moving the mouse while clicking, make sure that the reticle updates
	//as well as attracts the selected group
	if (bLeft && clicking)
	{
		clickpos = CVector(x, y);
		for each (CBoid * pBoid in activegroup)
		{
			if (activegroup != allAllies)
			{
				pBoid->SetAttraction(clickpos);
			}
		}
	}
		
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	//sets clicking to true, so the reticle will spawn at player mouse position
	//as well as attracs the selected boids to the mouse position
	clicking = true;
	clickpos = CVector(x, y);
	
	for each (CBoid * pBoid in activegroup)
	{
		if (activegroup != allAllies)
		{
			pBoid->SetAttraction(clickpos); 
		}
	}
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
	clicking = false;
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{

	//if available, the player will shoot a large and long lazer
	//this does masssive damage to any enemy boids caught within it's radius
	clickpos = CVector(x, y);
	if (cooldown >= 10000) 
	{
		CSprite* explosion = new CSprite(playerShip.GetX(), playerShip.GetY(), 1, 1, GetTime());
		explosion->AddImage("Explosion2.bmp", "explode", 10, 1, 9, 0, 0, 0, CColor::Black());
		explosion->SetAnimation("explode", 50, 0, 50);
		explosion->SetBottom(playerShip.GetY());
		explosion->SetBottomLocal(-0.055);
		explosion->SetDirection((explosion->GetPosition() - clickpos) * -1);
		explosion->SetRotation(explosion->GetDirection());
		explosion->Die(300);
		explosions.push_back(explosion);

		lazer = true;
	}
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
	lazer = false;
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
