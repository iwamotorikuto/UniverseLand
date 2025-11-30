#pragma once
class Stage3 : public IGameObject
{
	public:
		Stage3();
		~Stage3();

		//描画関数
		void Render(RenderContext & rc);

		//モデルレンダー。
		ModelRender m_modelRender;

		PhysicsStaticObject m_physicsStaticObject;
};

