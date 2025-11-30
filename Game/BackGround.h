#pragma once

class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();

	//描画関数。
	void Render(RenderContext& rc);
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//モデルレンダー。
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
	PhysicsStaticObject m_physicsStaticObject;
};

