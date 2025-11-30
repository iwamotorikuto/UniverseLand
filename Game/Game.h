#pragma once

class Player;
class GameCamera;
class BackGround;
class Fade;
class Star;
class Enemy;
class Item;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void NotifyGameClear();
	void NotifyGameOver();
	void DamagePlayer(int damage);
	void Render(RenderContext& rc);
	LevelRender	m_levelRender;
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	BackGround* m_backGround = nullptr;
	ModelRender m_modelRender;
	GameCamera* m_gameCamera = nullptr;
	Vector3* m_position;
	Vector3* m_moveSpeed;
	SkyCube* m_skyCube = nullptr;
	Fade* m_fade = nullptr;
	Star* m_star;
	SoundSource* m_gameBGM;
	FontRender m_fontRender;
	FontRender m_hpText;
	bool                  m_isWaitFadeout = false;
	int                   m_skyCubeType = enSkyCubeType_SpaceToon_2;
	int                   m_hp = 3;
	int                   m_maxhp = 3;
};
