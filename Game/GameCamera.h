#pragma once

class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	Player* m_player;
	Vector3 m_toCameraPos;
	Quaternion m_rotation;
};

