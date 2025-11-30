#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "Player.h"

#include "collision/CollisionObject.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/Enemy/Enemy1.tkm");

	//座標を設定する。
	m_position = { 0.0f,0.0f,700.0f };
	m_modelRender.SetPosition(m_position);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//大きさを設定する。
	m_modelRender.SetScale(1.0f,1.0f,1.0f);

	//キャラクターコントロールを初期化。
	m_charaCon.Init(
		20.0f,
		100.0f,
		m_position
	);

	//m_attack = NewGO<CollisionObject>(0);
	//m_attack->CreateBox(m_position, Quaternion::Identity, { 50.0f,50.0f,50.0f });
	//m_attack->SetName("enemy_attack");


	return true;
}

void Enemy::Update()
{
	//追跡処理
	Chase();

	//回転処理
	Rotation();

	//当たり判定。
	Collision();

	//アニメーションの再生。
   //PlayAnimation();

	//ステートの遷移処理。
	ManageState();

	const auto& jumpCol = g_collisionObjectManager->FindCollisionObjects("jump");
	for (auto jumpattack : jumpCol) {
		if (jumpattack->IsHit(m_charaCon)) {
			//死亡したときの処理を出す
			enemydie = true;
		}
	}

	m_modelRender.Update();

	Enemydie();

	// モデルに位置反映
	m_modelRender.SetPosition(m_position);
}


void Enemy::Enemydie()
{
	if (enemydie == true)
	{
		m_charaCon.RemoveRigidBoby();
		m_moveSpeed = { 0.0f,0.0f,0.0f };
		m_modelRender.SetScale(m_scale.x, 0.1f, m_scale.z);
		Deletetimer += g_gameTime->GetFrameDeltaTime();

		if (Deletetimer >= 1.0f)
		{
			DeleteGO(this);
		}
	}
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}


void Enemy::Chase()
{
	if (enemydie == false)
	{
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.y = 0.0f;
		m_moveSpeed.z = 0.0f;

		//プレイヤーが見つからなければ、これ以降の処理を切り上げる。
		if (m_player == nullptr)
		{
			return;
		}

		//プレイヤーの死亡確認
		//死んでいたらfalseにする
		if (m_player->isDed)
		{
			isChase = false;
			return;
		}

		//プレイヤーの現在位置を取得する
		Vector3 Player_Position = m_player->GetPosition();

		//敵の位置からプレイヤーの位置を求める
		Vector3 diff = Player_Position - m_position;

		//Y軸方向の差を無視する
		diff.y = 0.0f;

		//距離を計算する
		float Distance = diff.Length();

		//追跡を開始する距離
		const float StateChase = 300.0f;

		if (isChase == false && Distance < StateChase)
		{
			isChase = true;
		}

		//追跡処理
		if (isChase == true)
		{
			//方向ベクトルを正規化して、長さを求める
			//向きだけを取り出すことで、距離に関係なく一定速度で移動できる
			diff.Normalize();

			//追いかける速さ
			float Speed = 50.0f;

			m_moveSpeed.x = diff.x * Speed;

			m_moveSpeed.z = diff.z * Speed;

			//キャラコンを使って移動
			m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

			m_modelRender.SetPosition(m_position);
		}

	}
}

void Enemy::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	{
		//プレイヤーの攻撃用のコリジョンを取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
		//コリジョンの配列をfor文で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//HPを1減らす。
				m_hp -= 1;
				//HPが0になったら。
				if (m_hp == 0)
				{
					//ダウンステートに遷移する。
					m_enemyState = enEnemyState_Down;
				}
				else {
					//被ダメージステートに遷移する。
					m_enemyState = enEnemyState_ReceiveDamage;
				}
				return;
			}
		}
	}

}

//void Enemy::Attack()
//{
//	//攻撃ステートでないなら処理をしない。
//	if (m_enemyState != enEnemyState_Attack)
//	{
//		return;
//	}
//
//	//攻撃中であれば。
//	if (m_isUnderAttack == true)
//	{
//		//攻撃用のコリジョンを作成する。
//		MakeAttackCollision();
//	}
//}

void Enemy::Damage()
{
	// ここに敵がダメージを受けたときの処理を書く
	m_hp--;

	if (m_hp <= 0) {
		// 死亡処理
		DeleteGO(this);
	}
}

const bool Enemy::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	float dist = diff.Length();
	return dist < 50.0f;
}

void Enemy::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;
		//攻撃できる距離なら。
		if (IsCanAttack() == true)
		{
			//乱数によって、攻撃するか待機させるかを決定する。	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//攻撃ステートに遷移する。
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//待機ステートに遷移する。
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_enemyState = enEnemyState_Idle;
		return;
    }
}

void Enemy::ProcessChaseStateTransition()
{
	//攻撃できる距離なら。
	if (IsCanAttack() == true)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.8f)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}

}


void Enemy::ManageState()
{

}

const bool Enemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//攻撃できる！
		return true;
	}
	//攻撃できない。
	return false;
}

void Enemy::Render(RenderContext& rc) 
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
}
