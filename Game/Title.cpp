#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"


Title::Title()
{

}

Title::~Title()
{

}

bool Title::Start()
{
	//画像を読み込む。
	m_spriteRender.Init("Assets/Title/Title.DDS", 1920.0f, 1080.0f);

	m_fade = FindGO<Fade>("fade");
	if (m_fade == nullptr)
	{
		m_fade = NewGO<Fade>(0, "fade");
	}
	m_fade->StartFadeIn();

	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0);
		DeleteGO(this);
	}

	//α値を変更させる。
	if (m_isWaitFadeout)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	}


	//画像の更新。
	m_spriteRender.Update();
}

void Title::Render(RenderContext& rc)
{
	//画像を描画する。
	m_spriteRender.Draw(rc);
}
