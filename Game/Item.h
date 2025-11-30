#pragma once

class Player;
class Item;
extern Item* item;
class Item :public IGameObject
{
public:
	Item();
	~Item();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& renderContext);
	//移動処理。
	void Move();

	//ここからメンバ変数
	ModelRender m_modelRender;
	Vector3 m_position;
	Player* player;
};

