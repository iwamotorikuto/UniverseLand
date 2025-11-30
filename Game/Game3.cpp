#include "stdafx.h"
#include "Game3.h"
#include "Player.h"
#include "GameCamera.h"
#include "Item.h"
#include "Title.h"
#include "Stage3.h"
#include "Fade.h"
#include "GameClear.h"
#include "nature/SkyCube.h"
#include "Star.h"
#include "Enemy.h"
#include "GameOver.h"
#include "Boss.h"



Game3::Game3()
{
	//プレイヤーのオブジェクトを作る。
	m_player = NewGO<Player>(0, "player");

	//ゲームカメラのオブジェクトを作る。
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//背景のオブジェクトを作る。
	m_stage3 = NewGO<Stage3>(0);

}

Game3::~Game3()
{
	//プレイヤーを削除する。
	DeleteGO(m_player);
	//ゲームカメラを削除する。
	DeleteGO(m_gameCamera);
	//ゲーム中のBGMを削除する。
	//背景を削除する。
	DeleteGO(m_stage3);

	DeleteGO(m_skyCube);
}

bool Game3::Start()
{
	//現在の空を破棄
	DeleteGO(m_skyCube);
	//空もオブジェクトを作成する。
	m_skyCube = NewGO<SkyCube>(0, "skyCube");

	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	//環境の計算のためのILBテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 1.0f);
	//環境日光の影響が分かりやすいように、ディレクションライトはオフに。
	g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);

	m_fade = FindGO<Fade>("fade");
	if (m_fade == nullptr) {
		m_fade = NewGO<Fade>(0, "fade");
	}
	m_fade->StartFadeIn();
	return true;
}

void Game3::NotifyGameClear()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
}

void Game3::NotifyGameOver()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
}

void Game3::Update()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<GameClear>(0, "gameclear");
			DeleteGO(this);
		}
	}
}

void Game3::Render(RenderContext& rc)
{
	
}

