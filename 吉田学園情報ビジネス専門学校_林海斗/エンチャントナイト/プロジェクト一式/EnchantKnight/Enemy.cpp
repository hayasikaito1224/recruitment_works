//-----------------------------
//敵の処理
//-----------------------------
#include "Enemy.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "layer_structure.h"
#include "game.h"
#include "player.h"
#include "collision.h"
#include "stage.h"
#include "sound.h"
#include "effect.h"
#include "gauge.h"
#include "Particle.h"
#include "wall.h"
#include "map_polygon.h"
#define MIN_MOVE (80.0)		//敵が動く最小の範囲
#define MAX_MOVE (100.0)	//敵が動く最大の範囲
#define MIN_TIME (180.0)		//敵が動くまでの時間の最小の範囲
#define MAX_TIME (240.0)	//敵が動くまでの時間の最大の範囲
#define MAX_SPEED (1.8)
#define KNOCK_BACK_SPEED (3.0)
#define MAX_HIT_TIME (30)//敵の無敵時間
#define MAX_ENEMY_LIFE (10)//敵の命
#define MAX_ENEMY_DEFENSE (2)//敵の防御力
#define MAX_ATTACKSTARTTIME (20)//攻撃開始までの時間
#define ADDCP (10)		//無敵判定の時間

CEnemy::CEnemy(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bAIMove = false;
	m_bAIMoveStop = false;
	s_bCntStop = true;
	m_bHit = false;
	m_bInvincible = false;
	m_pPartsModel.clear();
	m_pMotion = nullptr;
	m_nAttackStartCnt = MAX_ATTACKSTARTTIME;
	m_bDeth = false;
	m_bMotionStop = false;
	m_pAttackModel.clear();
	m_pMapPolygon = nullptr;
	m_bMove = false;
}

CEnemy::~CEnemy()
{

}
//----------------------------------
//インスタンス生成
//----------------------------------
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pPlayer = NULL;
	pPlayer = new CEnemy(OBJTYPE_ENEMY);
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->Init();
	return pPlayer;
}
//----------------------------------
//初期化
//----------------------------------
HRESULT CEnemy::Init()
{
	m_nDefense = MAX_ENEMY_DEFENSE;
	m_fWalkSoundInterval = 0.0f;
	m_fSoundInterval = 0.0f;
	m_nLife = MAX_ENEMY_LIFE;
	m_bDamage = false;

	std::mt19937_64 s_mt(s_random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
	std::uniform_real_distribution<> s_randAng(-3.14, 3.14);
	std::uniform_real_distribution<> s_randMove(MIN_MOVE, MAX_MOVE);
	std::uniform_real_distribution<> s_randTime(MIN_TIME, MAX_TIME);
	if (m_pMapPolygon == nullptr)
	{
		m_pMapPolygon = CMap_Polygon::Create(m_pos, D3DXVECTOR3(5.0f, 0.0f, 5.0f), CTexture::Map_Enemy);
		m_pMapPolygon->SetCol({ 1.0f,0.0f,0.0f,1.0f });
		m_pMapPolygon->SetRot(m_rot.y);

	}

	s_MaxTimer = (float)s_randTime(s_mt);
	m_fMovingRange = (float)s_randMove(s_mt);
	m_rot.y = (float)s_randAng(s_mt)+(D3DX_PI);
	s_MoveRandAng = (float)s_randAng(s_mt);
	return S_OK;
}

void CEnemy::Uninit()
{
	// モデル破棄
	for (int nCnt = 0; nCnt < m_pPartsModel.size(); nCnt++)
	{
		if (m_pPartsModel[nCnt] != nullptr)
		{
			m_pPartsModel[nCnt]->Uninit();
			m_pPartsModel[nCnt] = nullptr;
		}
	}
	for (int nCnt = 0; nCnt < m_pAttackModel.size(); nCnt++)
	{
		if (m_pAttackModel[nCnt] != nullptr)
		{
			m_pAttackModel[nCnt]->Uninit();
			m_pAttackModel[nCnt] = nullptr;
		}
	}
	if (m_pMapPolygon != nullptr)
	{
		m_pMapPolygon->Uninit();
		m_pMapPolygon = nullptr;
	}
	Release();
}
//-----------------------------------------
//更新
//-----------------------------------------
void CEnemy::Update()
{
	//前回の位置情報の保存
	m_lastPos = m_pos;

	//敵のAI移動
	//死んでいなかったら行動する
	if (m_nLife <= 0)
	{
		m_bDeth = true;
		m_MotionType = DETH;
	}
	if (m_bDeth == false)
	{
		AIBehavior();

		if (m_bDraw == true)
		{
			CCollision *pCollision = new CCollision;
			//ダメージを受けたら無敵状態にする
			if (m_bDamage == true)
			{
				if (m_bEffect == false)
				{
					m_bEffect = true;
				}
				//カウンターを増やす
				m_nTimer++;
			}
			//カウンターが特定の時間に行ったら
			if (m_nTimer >= INVINCIBLE_TIME)
			{
				//通常状態に戻す
				m_bDamage = false;
				m_bEffect = false;
			}
			CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

			//プレイヤーに対しての当たり判定
			if (pPlayer != NULL)
			{
				pCollision->CollisionObjectPlayer(pPlayer, &m_pos, &m_lastPos, m_pPartsModel[0]->GetMaxPos().x);
				//攻撃判定が当たったかどうか調べる
				if (m_bAttack == true && pPlayer->GetHit() == false)
				{
					Colision();
				}
			}

			//ノックバック状態なら
			if (m_bKnockback == true)
			{
				Knockback(pPlayer->GetPos());
			}

			CScene *pEnemy = CScene::GetScene(OBJTYPE_ENEMY);

			CScene *pScene_Wall = CScene::GetScene(OBJTYPE_WALL);
			//壁との当たり判定
			while (pScene_Wall != nullptr)
			{
				CWall *pWall = (CWall*)pScene_Wall;
				CScene *pNext_Wall = CScene::GetSceneNext(pScene_Wall);
				//当たり判定
				bool bHit = pWall->TestCollision(&m_pos, &m_lastPos, m_pPartsModel[0]->GetMaxPos().x);
				pScene_Wall = pNext_Wall;

			}

			//敵同士の当たり判定
			while (pEnemy != NULL)
			{
				if (pEnemy != NULL /*&& pEnemy == this*/)
				{
					pCollision->CollisionObjectEnemy((CEnemy*)pEnemy, &m_pos, &m_lastPos, m_pPartsModel[0]->GetMaxPos().x);
				}
				pEnemy = pEnemy->GetSceneNext(pEnemy);
			}



			//モーション再生
			if (m_pMotion != NULL)
			{
				m_pMotion->PlayMotion(m_pPartsModel.size(), &m_pPartsModel[0], m_MotionType, m_MotionLastType, m_bMotionStop, m_bAttack, m_bNeutral, false);
			}

			delete pCollision;
		}

	}
	else
	{
		//モーション再生
		if (m_pMotion != NULL)
		{
			m_pMotion->NoLoopPlayMotion(m_pPartsModel.size(), &m_pPartsModel[0], m_MotionType, m_MotionLastType, m_bMotionStop);
		}
		if (m_bMotionStop == true)
		{
			m_bUninit = true;
			//消える音
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMY_DEATH);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMY_DEATH,0.5f);
			CManager::GetGame()->GetParticle()->PlayRandomCircle({ m_pos.x,m_pos.y + 30.0f,m_pos.z }, CParticle::HEELEFFECT, CTexture::GlitterEffect);
		}
		//敵の行動パターン実装予定
		//if (m_bUninit == true)
		//{
		//	Uninit();
		//}
	}
	if (m_pMapPolygon != nullptr)
	{
		m_pMapPolygon->SetPos(m_pos.x, m_pos.z);
		m_pMapPolygon->SetRot(m_rot.y);

	}
}

void CEnemy::Draw()
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
	//各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	//向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	//モデル描画
	for (int nCnt = 0; nCnt < m_pPartsModel.size(); nCnt++)
	{
		if (m_pPartsModel[nCnt] != nullptr)
		{
			m_pPartsModel[nCnt]->SetDraw(m_bDraw);
			m_pPartsModel[nCnt]->Draw();
		}
	}
	for (int nCnt = 0; nCnt < m_pAttackModel.size(); nCnt++)
	{
		if (m_pAttackModel[nCnt] != nullptr)
		{
			m_pAttackModel[nCnt]->SetDraw(false);
			m_pAttackModel[nCnt]->Draw();
		}
	}

}
void CEnemy::Colision()
{
}
void CEnemy::AIAttack()
{
}
void CEnemy::AIMove()
{
	if (m_fPlayerVecLength < MAX_COLRADIUS)
	{
		m_bMove = true;
		//自動で動くできるやつをオフにする
		m_bAIMove = false;
		m_bAIMoveStop = true;

	}
	//範囲外の時
	else
	{
		m_bMove = false;
		m_bAIMoveStop = false;

		s_bCntStop = true;
	}
	//自動移動ができるなら
	if (m_bMove)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		//発射する方向を求める
		m_MoveSpeed.x = sinf(m_fAng)*MAX_SPEED;
		m_MoveSpeed.z = cosf(m_fAng)*MAX_SPEED;
		m_MotionType = MOVE;

		m_fWalkSoundInterval -= 0.1f;

		if (m_fWalkSoundInterval <= 0.0f)
		{
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMYWALK);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMYWALK, 0.6f);
			m_fWalkSoundInterval = 2.3f;
		}
		m_pos += m_MoveSpeed;

	}
	else
	{
		m_MotionType = NEUTRAL;
		m_MoveSpeed.x = 0.0f;
		m_MoveSpeed.y = 0.0f;
		m_MoveSpeed.z = 0.0f;
	}

}
//----------------------------------------------------------
//体力の増減
//----------------------------------------------------------
void CEnemy::AddLife(int nLife)
{
	if (m_bDamage == true)
	{
		float fDamege = nLife+ m_nDefense;
		m_nLife += fDamege;

	}
}
void CEnemy::AddLife(int nPower, int nType)
{
	if (m_bDamage == true)
	{
		float fDamege = 0;
		switch (nType)
		{
		case FIRE:
			fDamege = (nPower + m_nDefense)*1.5f;

			break;
		case BULLIZAD:
			fDamege = nPower + m_nDefense;

			break;
		}
		m_nLife += fDamege;

	}

}

//-----------------------------------------
//無敵時間の処理
//-----------------------------------------

bool CEnemy::bHitAttack()
{
	//ヒットされたら無敵時間用の判定をオンにする
	if (m_bHit == true)
	{
		m_bInvincible = true;
		m_bHit = false;
		//ノックバック状態にする
		m_bKnockback = true;
	}
	//ヒットしていたら攻撃あたらないようにする
	if (m_bInvincible == true)
	{
		m_MotionType = DETH;
		m_bAttack = false;
		m_nCntHitInterval++;
		if (m_nCntHitInterval >= MAX_HIT_TIME)
		{
			m_nCntHitInterval = 0;

			m_bInvincible = false;
			m_bKnockback = false;


		}
	}
	return m_bInvincible;
}
//-----------------------------------------
//ノックバックの処理
//-----------------------------------------
void CEnemy::Knockback(D3DXVECTOR3& Playerpos)
{
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = Playerpos - m_pos;			//敵と弾のベクトル
	m_fAng = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
	//動き出す方向を求める
	float fSpeed_x = sinf(m_fAng)*KNOCK_BACK_SPEED;
	float fSpeed_z = cosf(m_fAng)*KNOCK_BACK_SPEED;
	//プレイヤーを動かす
	m_pos.x -= fSpeed_x;
	m_pos.z -= fSpeed_z;

}


bool  CEnemy::bColision()
{
	return 0;
}
//-----------------------------------------
//AIの処理
//-----------------------------------------

void CEnemy::AIBehavior()
{
	D3DXVECTOR3	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos = CManager::GetGame()->GetPlayer()->GetPos();				//位置
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = pos - m_pos;			//敵と弾のベクトル
	float fLengthX = 0.0f;
	m_fPlayerVecLength = sqrtf((PlayerVec.z*PlayerVec.z) + (PlayerVec.x*PlayerVec.x));
	m_fAng = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
	float fAngle = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
	//敵の出現
	if (m_fPlayerVecLength < MAX_DRAWRADIUS)
	{
		m_bDraw = true;
	}
	else
	{
		m_bDraw = false;
	}

	//敵の追従
	if (m_bAttack == false)
	{
		AIMove();
	}
	//プレイヤーが攻撃範囲に入ったら
	AIAttack();

	if (m_bAIMoveStop == false && m_bDraw == true)
	{
		//敵が自動で動ける状態だったら
		if (m_bAIMove == true)
		{
			m_MotionType = MOVE;

			//どこまで動くかカウントする
			s_MovingCnt += MAX_SPEED;
			m_rot.y = s_MoveRandAng + (D3DX_PI);

			//動き出す方向を求める
			m_MoveSpeed.x = sinf(s_MoveRandAng)*MAX_SPEED;
			m_MoveSpeed.z = cosf(s_MoveRandAng)*MAX_SPEED;
			m_pos += m_MoveSpeed;

		}
		else
		{
			s_CntTimer += 1.0f;
			//カウンターが指定の範囲までカウントされたら敵が動く
			if (s_CntTimer >= s_MaxTimer&&s_bCntStop == true)
			{
				s_CntTimer = 0.0f;
				//敵が自動で動くための判定をオンにする
				m_bAIMove = true;

				static std::random_device s_random;	// 非決定的な乱数生成器
				std::mt19937_64 s_mt(s_random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
				std::uniform_real_distribution<> s_randMove(MIN_MOVE, MAX_MOVE);

				m_fMovingRange = (float)s_randMove(s_mt);
				//向きを設定
				m_rot.y = s_MoveRandAng + (D3DX_PI);
				s_bCntStop = false;
			}

			s_MovingCnt = 0.0f;
			m_MoveSpeed.x = 0.0f;
			m_MoveSpeed.z = 0.0f;

		}
		//移動する指定の範囲に来たらr
		if (s_MovingCnt > m_fMovingRange)
		{
			static std::random_device s_random;	// 非決定的な乱数生成器

			std::mt19937_64 s_mt(s_random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
			std::uniform_real_distribution<> s_randAng(-3.14, 3.14);
			std::uniform_real_distribution<> s_randTime(MIN_TIME, MAX_TIME);

			s_MoveRandAng = (float)s_randAng(s_mt);
			s_MaxTimer = (float)s_randTime(s_mt);
			m_fMovingRange = 0.0f;
			s_MovingCnt = 0.0f;
			//カウントを０にしてまた数え始める
			s_bCntStop = true;
			s_CntTimer = 0.0f;
			m_MoveSpeed.x = 0.0f;
			m_MoveSpeed.z = 0.0f;
			m_bAIMove = false;
		}

	}
	//敵が勝手に動く処理

}
