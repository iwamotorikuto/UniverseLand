#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"

GameOver::GameOver()
{
	////画像処理
	m_spriteRender.Init("Assets/GameOver/GameOver.DDS", 1920.0f, 1080.0f);
	//m_spriteRender.SetPosition({ 0.0f, 0.0f, 0.0f });
}

GameOver::~GameOver()
{
	
}

void GameOver::Update()
{
	////Bボタンの処理
	//if (g_pad[0]->IsTrigger(enButtonB)) {
	//	//タイトル
	//		//効果音の再生
	//	SoundSource* se = NewGO<SoundSource>(0);
	//	se->Init(6);
	//	se->Play(false);
	//	NewGO<Title>(0);
	//	DeleteGO(this);
	//}

	//Aボタンの処理
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//リトライ
		//効果音の再生
		//SoundSource* se = NewGO<SoundSource>(0);
		//se->Init(6);
		//se->Play(false);
		NewGO<Game>(0);
		DeleteGO(this);
	}
}

void GameOver::Render(RenderContext& rc)
{
	//描画処理
	m_spriteRender.Draw(rc);
}
