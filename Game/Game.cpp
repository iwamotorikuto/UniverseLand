#include "stdafx.h"
#include "Game.h"
#include "nature/SkyCube.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "GameOver.h"
#include "Fade.h"
#include "GameClear.h"
#include "Title.h"
#include "Star.h"
#include "Item.h"

Game::Game()
{
	

}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	DeleteGO(m_backGround);
	DeleteGO(m_skyCube);
	const auto& enemys = FindGOs<Enemy>("enemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}
}

bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");

	m_backGround = NewGO<BackGround>(0, "backGround");

	m_enemy = NewGO<Enemy>(0, "enemy");
	m_enemy->m_player = m_player;

	Star* star1 = NewGO<Star>(0, "star");
	star1->m_position = { -2700.0f,6500.0f, -5300.0f };
	star1->firstPosition = star1->m_position;

	Item* item1 = NewGO<Item>(0, "item");
	item1->m_position = { -2250.0f,1700.0f, 2800.0f };

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//ポイントライト
	/*auto* pointLight = g_sceneLight->NewPointLight();
	pointLight->SetPosition(Vector3(-2700.0f, 6500.0f, -5300.0f));
	pointLight->SetColor(0.2f, 2.9f, 30.0f);
	pointLight->SetRange(10000.0f);
	pointLight->SetAffectPowParam(5.0f);
	pointLight->Update();*/


	g_camera3D->SetPosition({ 0.0f,20.0f,-600.0f });
	//g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	g_camera3D->SetFar(40000.0f);
    
	//カメラのオブジェクトを作成する。
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//空もオブジェクトを作成する。
	m_skyCube = NewGO<SkyCube>(0, "skyCube");

	m_skyCube->SetLuminance(1.0f);
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	//環境の計算のためのILBテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 1.0f);

	g_renderingEngine->SetCascadeNearAreaRates(0.01f, 0.1f, 0.5f);
	//環境日光の影響が分かりやすいように、ディレクションライトはオフに。
	//g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);


	m_fade = FindGO<Fade>("fade");
	if (m_fade == nullptr) {
		m_fade = NewGO<Fade>(0, "fade");
	}
	m_fade->StartFadeIn();

	g_renderingEngine->DisableTonemap();

	m_hpText.SetPosition({ 670.0f, 480.0f, 0.0f });  // 左上
	m_hpText.SetScale(1.0f);

	// 初期テキスト
	m_hpText.SetText(L"HP: 3");

	return true;
}

void Game::NotifyGameClear()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
}

void Game::NotifyGameOver()
{
	m_isWaitFadeout = true;
	m_fade->StartFadeOut();
}

void Game::Update()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<Title>(0, "Title");
			DeleteGO(this);

		}
	}

	if (m_player->m_position.y <= -1500.0f)
	{
		/*m_player->isDed = true;*/
		NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}

	if (m_player->m_hp <= 0)
	{
		NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}

	wchar_t buffer[64];
	swprintf(buffer, 64, L"HP: %d", m_player->m_hp);
	m_hpText.SetText(buffer);

	if (m_hp <= 0)
	{
		NewGO<GameOver>(0, "gameOver");
		DeleteGO(this);
	}

	//wchar_t buffer[64];
	swprintf(buffer, 64, L"HP: %d", m_hp);
	m_hpText.SetText(buffer);
}

void Game::DamagePlayer(int damage)
{
	m_player->m_hp -= damage;

	if (m_hp <= 0) {
		NewGO<GameOver>(0, "gameover");
		DeleteGO(this);
	}
}

void Game::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);

	//ゲームクリア
	if (m_player->m_starCount >= 1)
	{
		NewGO<GameClear>(0, "gameclear");
		DeleteGO(this);
	}

	m_hpText.Draw(rc);
}
