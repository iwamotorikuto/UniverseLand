#pragma once

class Collision;
class Game;
class Enemy;
class Player;

class Player :public IGameObject
{
public:
	//プレイヤーのステート。
	enum EnPlayerState {
		enPlayerState_Idle,					//待機。
		enPlayerState_Walk,					//歩き。
		enPlayerState_Run,					//走る。
		enPlayerState_Attack,				//攻撃。
		enPlayerState_MagicAttack,			//魔法攻撃。
		enPlayerState_PushLever,			//レバーを押す。
		enPlayerState_ReceiveDamage,		//ダメ―ジ受けた。
		enPlayerState_Down,					//HPが0。
		enPlayerState_Clear					//クリアー。
	};
	Player();
	~Player();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& renderCotext);
	//移動処理。
	void Move();
	//回転処理。
	void Rotation();

	void Attack();
	//ステート管理。
	void ManageState();
	//アニメーション再生。
	void PlayAnimation();

	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Attack &&
			m_playerState != enPlayerState_MagicAttack &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_PushLever &&
			m_playerState != enPlayerState_Down &&
			m_playerState != enPlayerState_Clear;
	}

	void GameOver();

	void Damage();

	//Playerの生存確認
	bool isDed = false;

	void Collision();

	/// 攻撃の際の当たり判定用のコリジョンを作成する。
	void MakeAttackCollision();

	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

	Vector3 GetPosition() const;   
	void SetPosition(const Vector3& pos);

	bool IsDamage = false;

	void In();

	void Modelblink();

	bool m_dispmodel = true;

	//メンバ変数。
	ModelRender m_modelRender;   //モデルレンダー。
	Player* player;
	Vector3     m_position; 
//座標。
	enum EnAnimationClip {       //アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Jump,
		enAnimationClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip m_animationClips[enAnimationClip_Num];//アニメーションクリップ。
	PhysicsGhostObject m_bodyGhost;
	CharacterController m_charaCon;  
	CollisionObject* m_jumpcol;
	Vector3 m_jumpcolPos;
	Vector3 m_moveSpeed; //移動速度。
	Quaternion m_rotation;  //クォータニオン.
	Enemy* m_targetEnemy = nullptr;
	bool AttackFlag = false;			// 攻撃したかどうか
	bool  m_isUnderAttack = false;
	float JumpTimer = 0;
	float m_invincibleTimer = 0.0f;
	float m_dispmodelTimer = 0.0f;
	int m_playerState = 0;
	int JumpCount = 0;
	int MaxJumpCount = 3;
	int m_starCount = 0;
	int m_itemCount = 0;
	int m_hp = 3;
	int m_maxHp = 3;
};

