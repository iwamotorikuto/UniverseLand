#pragma once

class Player;
class Star;
extern Star* star;
class Star : public IGameObject
{
public:
	Star();
	~Star();
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& renderContext);
	//移動処理
	void Move();

	//ここからメンバ変数
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 firstPosition;       //最初の座標。
	Player* player;              //プレイヤー。
};

