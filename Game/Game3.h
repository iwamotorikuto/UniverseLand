#pragma once

class Player;
class GameCamera;
class Stage3;
class Item;
class Fade;

class Game3 : public IGameObject
{
public:
	Game3();
	~Game3();
	bool Start();
	void Update();
	void NotifyGameClear();
	void NotifyGameOver();
	void Render(RenderContext& rc);

	Player* m_player;
	GameCamera* m_gameCamera;
	Stage3* m_stage3;
	Item* m_item;
	SkyCube* m_skyCube = nullptr;
	Vector3         Position;
	Fade* m_fade = nullptr;
	FontRender      m_fontRender;
	int             m_skyCubeType = enSkyCubeType_Night;
	bool            m_isWaitFadeout = false;
};

