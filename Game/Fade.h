#pragma once
//フェード。
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	//フェードイン。
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}

	//フェードアウト。
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	//フェード中。
	//フェード中ならtrue。
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}

	//α値を取得。
	//α値。
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}
private:
	enum EnState {
		enState_FadeIn,        //フェードイン中。
		enState_FadeOut,       //フェードアウト中。
		enState_Idle,          //アイドル中。
	};
	SpriteRender m_spriteRender;          //スプライトレンダー。
	EnState      m_state = enState_Idle;  //状態。
	float        m_currentAlpha = 1.0f;   //現在のα値。ここを変更すると画像が透明になる。
};


