#include "Enemy_poyo.h"
#include "layer_structure.h"
#include "motion.h"
#include "model.h"
#include "manager.h"
#include "player.h"
#include "collision.h"
#include "game.h"
#include "gauge.h"
#include "sound.h"
#include "effect.h"
#define POYO_LIFE (10)//敵の命
//#define MAX_ENEMY_DEFENSE (2)//敵の防御力
#define POYO_POWER (10)//攻撃力
#define POYO_ATTACKSTART_RADIUS (150.0f)//攻撃開始範囲
#define POYO_ATTACKSTARTTIME (20)//攻撃開始までの時間
#define POYO_SPEED (1.8)

CEnemy_Poyo::CEnemy_Poyo(OBJTYPE nPriority) : CEnemy(nPriority)
{
	m_nPower = POYO_POWER;
	m_nLife = POYO_LIFE;
}

CEnemy_Poyo::~CEnemy_Poyo()
{

}

CEnemy_Poyo *CEnemy_Poyo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy_Poyo *pPoyo = NULL;
	pPoyo = new CEnemy_Poyo(OBJTYPE_ENEMY);
	pPoyo->m_pos = pos;
	pPoyo->m_rot = rot;
	pPoyo->Init();
	return pPoyo;
}

HRESULT CEnemy_Poyo::Init()
{
	CEnemy::Init();
	m_nType = NONE;
	//ファイル読み込み
	CLayer_Structure *pLayer = NULL;
	pLayer = new CLayer_Structure;
	pLayer->SetLayer_Structure("data/TEXT/EnemyParts000.txt", m_pPartsModel,CModel::TYPE_ENEMY);
	CModel *pmodel = new CModel;
	//攻撃判定用の空のオブジェクト生成
	m_pAttackModel.push_back(pmodel);
	m_pAttackModel[0] = CModel::Create({ 0.0f,0.0f,55.0f }, { 0.0f,0.0f,0.0f }, 0, CModel::TYPE_OBJECT);
	//しっぽに親子関係をつける
	m_pAttackModel[0]->SetParent(m_pPartsModel[3]);
	m_pAttackModel[0]->SetNumParent(3);

	if (m_pMotion == NULL)
	{
		m_pMotion = new CMotion;
		m_MotionType = 0;
		m_pMotion->MotionLoad("data/TEXT/poyo_motion.txt");

	}

	return S_OK;
}

void CEnemy_Poyo::Uninit()
{
	CEnemy::Uninit();
	// モデル破棄

	Release();
}

void CEnemy_Poyo::Update()
{
	CEnemy::Update();


}

void CEnemy_Poyo::Draw()
{
	CEnemy::Draw();
}
//-----------------------------------------
//当たり判定
//-----------------------------------------

void CEnemy_Poyo::Colision()
{
	CCollision *pCollision = new CCollision;
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

	bool bHit = pCollision->CollisionAttack({ m_pAttackModel[0]->GetMtxWorld()._41 , m_pAttackModel[0]->GetMtxWorld()._42, m_pAttackModel[0]->GetMtxWorld()._43 },
		pPlayer->GetPos(), { pPlayer->GetParts(2)->GetMtxWorld()._41 ,pPlayer->GetParts(2)->GetMtxWorld()._42,pPlayer->GetParts(2)->GetMtxWorld()._43 },
		80.0f);
	if (bHit == true && pPlayer->GetDeth() == false && pPlayer->GetHit() == false)
	{
		CManager::GetGame()->GetHPGauge()->SetGauge(m_nPower);
		pPlayer->SetHit(true);
		std::random_device random;	// 非決定的な乱数生成器
		std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
		std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
		D3DXVECTOR3 AttackModelPos = { m_pAttackModel[0]->GetMtxWorld()._41 , m_pAttackModel[0]->GetMtxWorld()._42, m_pAttackModel[0]->GetMtxWorld()._43 };
		float fAng = randAng(mt);
		//攻撃用のモデルとプレイヤーのベクトルを求める
		D3DXVECTOR3 vec = AttackModelPos - pPlayer->GetPos();
		float fAngle = atan2(vec.x, vec.y);//敵の向き
										   //ヒットエフェクトのずらし位置を求める
		D3DXVECTOR3 Addmove = D3DXVECTOR3(
			sinf(fAngle)*20.0f,
			0.0f,
			cosf(fAngle)*20.0f);
		D3DXVECTOR3 PlayerPos = { pPlayer->GetPos().x,pPlayer->GetPos().y + 60.0f,pPlayer->GetPos().z };
		CEffect::Create
		(PlayerPos + Addmove,
		{ 0.0f,0.0f,0.0f }, { 1.2f,1.0f,0.0f },
		{ 1.0,0.5,0.5,1.0f }, false, 0.0f, 0.03f, true, CTexture::HitEffect, fAng, true);
		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);

	}

}
//-----------------------------------------
//自動で攻撃する処理
//-----------------------------------------

void CEnemy_Poyo::AIAttack()
{
	//プレイヤーが攻撃範囲に入ったら
	if (m_fPlayerVecLength < POYO_ATTACKSTART_RADIUS)
	{
		if (m_bAttack == false)
		{
			m_nAttackStartCnt++;
			//攻撃開始時間になったら
			if (m_nAttackStartCnt >= POYO_ATTACKSTARTTIME)
			{
				m_nAttackStartCnt = 0;
				m_MotionType = ATTACK;
				m_bAttack = true;
				//攻撃音
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SWORD_ATTACK);
				CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_SWORD_ATTACK, 0.7f);

			}

		}
	}
	else
	{
		m_nAttackStartCnt = 0;

	}
}

void CEnemy_Poyo::AIMove()
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
		m_MoveSpeed.x = sinf(m_fAng)*POYO_SPEED;
		m_MoveSpeed.z = cosf(m_fAng)*POYO_SPEED;
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

void CEnemy_Poyo::AddLife(int nPower, int nType)
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

void CEnemy_Poyo::AddLife(int nLife)
{
	if (m_bDamage == true && m_bHitCollision == true)
	{
		float fDamege = nLife + m_nDefense;
		m_nLife += fDamege;

	}
}
