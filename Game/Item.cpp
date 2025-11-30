#include "stdafx.h"
#include "Item.h"
#include "Player.h"

Item::Item()
{
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/Item/Item.tkm");

	//プレイヤーのオブジェクトを探して持ってくる。
	player = FindGO<Player>("player");

	//大きさの変更
	m_modelRender.SetScale({ 10.0f,10.0f,10.0f });
}

Item::~Item()
{

}

void Item::Update()
{
	Move();

	//プレイヤーからItemに向かうベクトルを計算。
	Vector3 diff = player->m_position - m_position;
	//ベクトルの長さが100.0fより小さかったら。
	if (diff.Length() <= 40.0f)
	{
		player->m_itemCount += 1;

		Vector3 warpPos = { -2250.0f, 4000.0f, 2800.0f };

		player->m_position = warpPos;

		player->m_charaCon.SetPosition(warpPos);

		//自身を削除する
		DeleteGO(this);
	}

	//絵描きさんの更新処理
	m_modelRender.Update();
}

void Item::Move()
{
	m_modelRender.SetPosition(m_position);
}

void Item::Render(RenderContext& rc)
{
	//Itemを描画する。
	m_modelRender.Draw(rc);
}
