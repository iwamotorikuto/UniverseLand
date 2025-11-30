#include "stdafx.h"
#include "Player.h"

#include "Game.h"
#include "Enemy.h"

#include "collision/CollisionObject.h"

namespace {
	const Vector3 JUMPCOLSIZE = { 30.0f,30.0f,30.0f };

}


Player::Player()
{
	//アニメーションクリップをロードする。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	
	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisY);

	m_jumpcolPos = m_position;
	m_jumpcol = NewGO<CollisionObject>(0);
	m_jumpcol->SetPosition(m_jumpcolPos);
	m_jumpcol->SetIsEnableAutoDelete(false);
	m_jumpcol->CreateBox(m_jumpcolPos, Quaternion::Identity, JUMPCOLSIZE);
	m_jumpcol->SetName("jump");

	//キャラコンを初期化する。
	m_charaCon.Init(25.0f, 75.0f, m_position);
}

Player::~Player()
{
	DeleteGO(m_jumpcol);

}

void Player::Update()
{
	//移動処理。
	Move();

	//回転処理。
	Rotation();

	//ステート管理。
	ManageState();

	//アニメーションの再生。
	PlayAnimation();

	m_jumpcolPos = m_position;

	m_jumpcol->SetPosition(m_jumpcolPos);

	m_jumpcol->Update();

	Collision();

	m_bodyGhost.SetPosition(m_position);

	

	m_modelRender.Update();
}

void Player::Move()
{
	//xzの移動速度を0.0fにする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と200.0fを乗算。
	right *= stickL.x * 200.0f;
	forward *= stickL.y * 200.0f;

	//移動処理にスティックの入力量を加算する。
	m_moveSpeed += right + forward;

	bool isOnGround = m_charaCon.IsOnGround();


	// 1秒経ったらジャンプ回数リセット
	if (JumpTimer > 6.0f) {
		JumpCount = 0;
		JumpTimer = 0.0f;
	}

	if (isOnGround)
	{
		m_jumpcol->SetIsEnable(false);


		//Aボタンが押された瞬間
		if (isOnGround && g_pad[0]->IsTrigger(enButtonA))
		{
			// ジャンプ力を段階で変える
			if (JumpCount == 0)
				m_moveSpeed.y = 350.0f; // 1段目
			else if (JumpCount == 1)
				m_moveSpeed.y = 450.0f; // 2段目
			else
				m_moveSpeed.y = 600.0f; // 3段目以降

			JumpCount++;

			//3段目まで行ったらまた最初に戻す
			if (JumpCount > 2)
				JumpCount = 0;

			//ジャンプした瞬間にタイマーをリセット
			JumpTimer = 0.0f;
		}
		else
		{
			// 時間を加算していく
			JumpTimer += 1.0f / 60.0f;
			JumpCount = 0;

			m_jumpcol->SetIsEnable(true);
		}
	}
	else
	{
		//重力発生させる。
		m_moveSpeed.y -= 12.0f;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, 3.0f / 60.0f);

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//xかzの移動速度があったら（スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f);
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rotation);
	}
}

void Player::Collision()
{
	//被ダメージ、ダウン中、クリア時はダメージ判定をしない。
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_Down ||
		m_playerState == enPlayerState_Clear)
	{
		return;
	}

	{
		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
		//配列をfor文で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//HPを減らす。
				m_hp --;
				//HPが0になったら。
				if (m_hp == 0)
				{
					//ダウンさせる。
					m_playerState = enPlayerState_Down;
				}
				//HPが0ではなかったら。
				else {
					//被ダメージリアクションをさせる。
					m_playerState = enPlayerState_ReceiveDamage;
				}
				//効果音を再生する。

			}
		}
	}

}

void Player::Attack()
{
	Vector3 attackPos = m_position;

	// エネミー
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		if (enemy->m_position.x == attackPos.x && enemy->m_position.y == attackPos.y) {
			// 被弾
			enemy->Damage();
		}
		return true;
	});

	AttackFlag = true;
}

void Player::ManageState()
{
	//地面に付いていなかったら。

	if (m_charaCon.IsOnGround() == false)
	{
		//ステートを1(ジャンプ中)にする。
		m_playerState = 1;
		//ここでManageStateの処理を終わらせる。
		return;
	}

	//地面に付いていたら。
	//xかzの移動速度があったら（スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z))
	{
		//ステートを2(歩き)にする。
		m_playerState = 2;
	}

	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを0(待機)にする
		m_playerState = 0;
	}
}

void Player::SetPosition(const Vector3& pos)
{
	m_position = pos;
	m_modelRender.SetPosition(m_position);
}

Vector3 Player::GetPosition() const
{
	return m_position;
}


void Player::PlayAnimation()
{
	//switch文。
	switch (m_playerState) {
		//プレイヤーステートが0(待機)だったら。
	case 0:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//プレイヤーステートが1(ジャンプ中)だったら。
	case 1:
		//ジャンプアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
		//プレイヤーステートが2(歩き)だったら。
	case 2:
		//歩きアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Walk);
		break;
	}

}


void Player::Damage()
{
	//if (IsDamage == false)
	//{
	//	m_hp--;
	//	IsDamage = true;
	//	
	//}

	//if (IsDamage == false)
	//{
	//	IsDamage = true;
	//	m_invincibleTimer = 0.0f;  
	//}

	//Game* game = FindGO<Game>("game");
	//if (game != nullptr) {
	//	game->m_hp -= 1;
	//}

}

void Player::In()
{
	if (IsDamage == true)
	{
		m_invincibleTimer += g_gameTime->GetFrameDeltaTime();
		Modelblink();
		if (m_invincibleTimer >= 2.0f)
		{
			m_invincibleTimer = 0.0f;
			m_dispmodel = true;
			IsDamage = false;
		}
	}
}

void Player::Modelblink()
{
	switch (m_dispmodel)
	{
	case true:
		m_dispmodelTimer += 0.1f;
		if (m_dispmodelTimer >= 1.0f)
		{
			m_dispmodelTimer = 1.0f;
			m_dispmodel = false;
		}
		break;
		
	case false:
		m_dispmodelTimer -= 0.1f;
		if (m_dispmodelTimer <= 0.0f)
		{
			m_dispmodelTimer = 0.0f;
			m_dispmodel = true;
		}
		break;
	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	if (m_dispmodel == true)
	{
		m_modelRender.Draw(rc);
	}

	
}

