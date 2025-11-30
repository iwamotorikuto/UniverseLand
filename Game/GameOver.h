#pragma once

class GameOver : public IGameObject

{
public:
	GameOver();
	~GameOver();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender m_spriteRender;
};

