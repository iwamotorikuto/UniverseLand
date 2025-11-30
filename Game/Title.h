#pragma once

//クラス宣言。
class Fade;

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	//更新処理
	void Update();
	//描画関数
	void Render(RenderContext& rc);

private:
	SpriteRender   m_spriteRender;           //タイトルの画像。
	Fade* m_fade = nullptr;         //フェード。
	SoundSource* m_TitleBGM;
	float          m_alpha = 0.0f;           //pressbuttonのα値。
	bool           m_isWaitFadeout = false;
};

