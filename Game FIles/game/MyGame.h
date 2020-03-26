#pragma once

#define BOIDNUM 4
#define ENEMYBOID 20

class CMyGame : public CGame
{

	CSprite playerShip;
	CSprite enemyStation;

	CSpriteList m_allyboids1;
	CSpriteList m_allyboids2;
	CSpriteList m_allyboids3;

	CSpriteList activegroup;

	CSpriteList allAllies;
	CSpriteList allEnemies;

	CSpriteList explosions;

	CSpriteList m_enemyboids;
	CSpriteList m_obstacles;

	CSpriteList m_AllyshotList;
	CSpriteList m_EnemyshotList;

	CGraphics m_background;

	CVector clickpos;
	bool clicking;

	bool lazer;
	float lastTime;
	float cooldown;


public:
	CMyGame(void);
	~CMyGame(void);

	void DrawHealth(CGraphics* g, CVector pos, float w, float h, float health, CColor fillcolor);

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
