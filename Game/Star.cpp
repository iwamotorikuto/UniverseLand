#include "stdafx.h"
#include "Star.h"
#include "Player.h"

Star::Star()
{
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/ClearStar/ClearStar.tkm");

	//プレイヤーのオブジェクトを探して持ってくる。
	player = FindGO<Player>("player");

	//大きさの変更
	m_modelRender.SetScale({ 10.0f,10.0f,10.0f });
}

Star::~Star()
{

}

void Star::Update()
{
	Move();

	//プレイヤーからStarに向かうベクトルを計算。
	Vector3 diff = player->m_position - m_position;

	//ベクトルの長さが100.0fより小さかったら。
	if (diff.Length() <= 100.0f)
	{
		//カウントを+1する
		player->m_starCount += 1;

		//自身を削除する
		DeleteGO(this);
	}

	//絵描きさんの更新処理
	m_modelRender.Update();
}

void Star::Move()
{
	m_modelRender.SetPosition(m_position);
}

void Star::Render(RenderContext& rc)
{
	//Itemを描画する。
	m_modelRender.Draw(rc);
}
