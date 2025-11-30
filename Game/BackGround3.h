#pragma once
class BackGround3 : public IGameObject
{
public:
	BackGround3();
	~BackGround3();

	//描画関数。
	void Render(RenderContext& rc);

	//モデルレンダー。
	ModelRender m_modelRender;

	PhysicsStaticObject m_physicsStaticObject;
};

