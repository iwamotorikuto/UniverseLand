#pragma once

class Player;



	/// <summary>
	/// ボス。
	/// </summary>
	class Boss : public IGameObject
	{
	public:
		//エネミーステート。
		enum EnBossState {
			enBossState_Idle,					//待機。
			enBossState_Chase,					//追跡。
			enBossState_Attack,				//攻撃。
			enBossState_MagicAttack,			//魔法攻撃。
			enBossState_ReceiveDamage,			//被ダメージ。
			enBossState_Down,					//ダウン。
		};
	public:
		Boss();
		~Boss();
		bool Start();
		void Update();
		void Render(RenderContext& rc);
		/// <summary>
		/// 座標を設定する。
		/// </summary>
		/// <param name="position">座標。</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// 座標を取得する。
		/// </summary>
		/// <returns>座標。</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// 回転を設定する。
		/// </summary>
		/// <param name="rotation">回転。</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// 大きさを設定する。
		/// </summary>
		/// <param name="scale">大きさ。</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
			m_modelRender.SetScale(m_scale);
		}



		Player* m_player;							//プレイヤー。
	 private:
		/// <summary>
		/// 追跡処理。
		/// </summary>
		void Chase();
		/// <summary>
		/// 回転処理。
		/// </summary>
		void Rotation();
		/// <summary>
		/// 攻撃処理。
		/// </summary>
		void Attack();
		/// <summary>
		/// プレイヤーの攻撃との当たり判定処理。
		/// </summary>
		void Collision();


		void BossDamage();
		/// <summary>
		/// プレイヤーを探索する。
		/// </summary>
		/// <returns>プレイヤーが見つかったらtrue。</returns>
		const bool SearchPlayer() const;
		void ManageState();
		/// 共通のステート遷移処理。
		void ProcessCommonStateTransition();
		/// 待機ステートの遷移処理。
		void ProcessIdleStateTransition();
		/// 追跡ステートの背遷移処理。
		void ProcessChaseStateTransition();

		void OnPlayerStop(Player* m_player);

		const bool IsCanAttack() const;

		//追跡をするかしないか
		bool isChase = false;

		float Deletetimer = 0.0f;

		bool bossdie = false;

		void Bossdie();




		enum EnAnimationClip {						//アニメーション。
			enAnimationClip_Idle,					//待機アニメーション。
			enAnimationClip_Walk,					//歩きアニメーション。
			enAnimationClip_Run,					//走りアニメーション。
			enAnimationClip_Attack,					//攻撃アニメーション。
			enAnimationClip_Damage,					//被ダメージアニメーション。
			enAnimationClip_Down,					//ダウンアニメーション。
			enAnimationClip_Num,					//アニメーションの数。
		};
		AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
		ModelRender					m_modelRender;								//モデルレンダー。
		Vector3						m_position;									//座標。
		Vector3						m_moveSpeed;								//移動速度。
		Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
		Quaternion					m_rotation;									//回転。
		Vector3						m_scale = Vector3::One;						//大きさ。
		CharacterController			m_charaCon;									//キャラコン。
		EnBossState				m_bossState = enBossState_Idle;			//ボスステート。
		bool						m_isUnderAttack = false;					//攻撃中か？
		int							m_hp = 0;									//HP。

		float						m_chaseTimer = 0.0f;						//追跡タイマー。
		float						m_idleTimer = 0.0f;							//待機タイマー。
	};






