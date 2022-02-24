//-----------------------------
//敵の処理
//-----------------------------
#include "Enemy_Purple.h"
#include "layer_structure.h"
#include "motion.h"
#include "collision.h"
#include "game.h"
#include "gauge.h"
#include "sound.h"
#include "effect.h"
#include "manager.h"
#include "player.h"
#include "enemy_magic_fire.h"
#include "MagicCircle.h"
#include "Particle.h"
#include "wall.h"
#include "map_polygon.h"
#include "enemy_magic_field_ice.h"
#include "col_area.h"
#include "col_area_bg.h"
#include "shadow.h"
//#define MAX_ENEMY_DEFENSE (2)//敵の防御力
#define PURPLE_POWER (10)//攻撃力
#define PURPLE_ATTACKSTART_RADIUS (1000.0f)//攻撃開始範囲
#define PURPLE_ATTACKEND_RADIUS (550.0f)//攻撃開始範囲
#define PURPLE_MOVERANGE (700.0f)//追跡する範囲
#define PURPLE_ATTACKTIME (30)//攻撃を出すまでの時間
#define PURPLE_ATTACKSTARTTIME (90)//攻撃開始までの時間
#define PURPLE_SPEED (2.0f)
#define PURPLE_ADDPOS (-80.0f)
#define PURPLE_BREAKTIME (120)//クールタイム
#define PURPLE_DEFENSE (2)//敵の防御力
#define PURPLE_FIRE_TIME (20)//攻撃開始までの時間
#define PURPLE_FIRE_ATTACK_STARTTIME (120)//攻撃開始までの時間
#define ENEMY_MAGIC_FIRE_MAZIC_SIRCLE_RADIUS (70.0)
#define ENEMY_MAGIC_ICE_MAZIC_SIRCLE_RADIUS (300.0)
#define PURPLE_ICE_ATTACK_STARTTIME (120)//攻撃開始までの時間
#define PURPLE_ICE_ATTACK_ENDTIME (180)//攻撃開始までの時間
#define PURPLE_ICE_ATTACK_ROT (45.0f)//発射する向き
#define PURPLE_ICE_COUNT (10)//発射する数
#define PURPLE_FIRE_COUNT (10)//発射する数

CEnemy_Purple::CEnemy_Purple(OBJTYPE nPriority) : CEnemy(nPriority)
{
	m_nPower = PURPLE_POWER;
	m_bBreakTime = false;
	m_pPartsModel.clear();
	m_pMotion = nullptr;
	m_bDraw = true;
	m_bAttackWait = false;
	m_nAttackType = FIRE;
	m_bAttackStart = false;
	m_FireState = {};
	m_col = { 1.0,1.0,1.0,1.0 };
	m_IceState = {};
	m_bDiscovery = false;
}

CEnemy_Purple::~CEnemy_Purple()
{

}

CEnemy_Purple *CEnemy_Purple::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	CEnemy_Purple *pPurple = NULL;
	pPurple = new CEnemy_Purple(OBJTYPE_ENEMY);
	pPurple->m_pos = pos;
	pPurple->m_rot = rot;
	pPurple->m_nLife = nLife;
	pPurple->Init();
	return pPurple;
}

HRESULT CEnemy_Purple::Init()
{
	m_nDefense = PURPLE_DEFENSE;
	m_fWalkSoundInterval = 0.0f;
	m_fSoundInterval = 0.0f;
	m_bDamage = false;

	if (m_pMapPolygon == nullptr)
	{
		m_pMapPolygon = CMap_Polygon::Create(m_pos, D3DXVECTOR3(7.0f, 0.0f, 7.0f), CTexture::Map_Enemy);
		m_pMapPolygon->SetCol({ 1.0f,0.0f,0.0f,1.0f });
		m_pMapPolygon->SetRot(m_rot.y);
	}

	m_rot.y = 0.0f;
	m_nType = NONE;
	//ファイル読み込み
	CLayer_Structure *pLayer = NULL;
	pLayer = new CLayer_Structure;
	pLayer->SetLayer_Structure("data/TEXT/ENEMYPURPLE/ModelParts.txt", m_pPartsModel,CModel::TYPE_PURPLEENEMY);

	if (m_pMotion == NULL)
	{
		m_pMotion = new CMotion;
		m_MotionType = 0;
		m_pMotion->MotionLoad("data/TEXT/ENEMYPURPLE/motion.txt");

	}
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create({ 0.0f,0.0f,0.0f }, 50.0f, CTexture::Effect);
	}

	return S_OK;
}

void CEnemy_Purple::Uninit()
{
	CEnemy::Uninit();
	// モデル破棄

	Release();
}

void CEnemy_Purple::Update()
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
		if (m_pShadow != nullptr)
		{
			m_pShadow->SetPos(0.0f, 0.0f, { m_pPartsModel[0]->GetMaxPos().x ,0.0,m_pPartsModel[0]->GetMaxPos().x });
			m_pShadow->SetPos(m_pos);
		}
		D3DXVECTOR3	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pos = CManager::GetGame()->GetPlayer()->GetPos();				//位置
		D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		PlayerVec = pos - m_pos;			//敵と弾のベクトル
		float fLengthX = 0.0f;
		m_fPlayerVecLength = sqrtf((PlayerVec.z*PlayerVec.z) + (PlayerVec.x*PlayerVec.x));
		m_fAng = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
		float fAngle = atan2(PlayerVec.x, PlayerVec.z);//敵の向き
													   //敵の出現
		//敵の攻撃処理
		AIAttack();
		//敵がプレイヤーを発見したときの処理
		if (m_bDiscovery)
		{
			//AIMove();
			//敵の追従
			Discovery();
		}
		if (m_bAIMoveStop == false && m_bDraw == true)
		{
			//敵が自動で動ける状態だったら
			if (m_bAIMove == true)
			{
				m_MotionType = MOVE;

				//どこまで動くかカウントする
				s_MovingCnt += PURPLE_SPEED;
				m_rot.y = s_MoveRandAng + (D3DX_PI);

				//動き出す方向を求める
				m_MoveSpeed.x = sinf(s_MoveRandAng)*PURPLE_SPEED;
				m_MoveSpeed.z = cosf(s_MoveRandAng)*PURPLE_SPEED;
				m_pos += m_MoveSpeed;

			}
		}

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
				if (m_bAttack == true && pPlayer->GetHit() == false && m_bHitCollision == true)
				{
					Colision();
				}
			}

			//ノックバック状態なら
			if (m_bKnockback == true)
			{
				//Knockback(pPlayer->GetPos());
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
			//クラス情報を破棄
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
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMY_DEATH, 0.5f);
			CManager::GetGame()->GetParticle()->PlayRandomCircle({ m_pos.x,m_pos.y + 30.0f,m_pos.z }, CParticle::HEELEFFECT, CTexture::GlitterEffect);
		}
	}
	if (m_pMapPolygon != nullptr)
	{
		m_pMapPolygon->SetPos(m_pos.x, m_pos.z);
		m_pMapPolygon->SetRot(m_rot.y);

	}


}

void CEnemy_Purple::Draw()
{
	CEnemy::Draw();
}

void CEnemy_Purple::AddLife(int nPower, int nType)
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
		CManager::GetGame()->GetBossHPGauge()->SetGauge(-fDamege);

	}

}
void CEnemy_Purple::AddLife(int nLife)
{
	if (m_bDamage == true && m_bHitCollision == true)
	{
		float fDamege = nLife + m_nDefense;
		m_nLife += fDamege;
		CManager::GetGame()->GetBossHPGauge()->SetGauge(-fDamege);

	}
}
void CEnemy_Purple::Colision()
{
}
//-----------------------------
//敵の攻撃処理
//-----------------------------
void CEnemy_Purple::AIAttack()
{
	//攻撃タイプによって色を変更
	switch (m_nAttackType)
	{
	case FIRE:
		m_col = { 1.0f,0.3f,0.3f,1.0f };
		break;
	case BULLIZAD:
		m_col = { 0.3f,0.3f,1.0f,1.0f };
		break;
	}
	//攻撃クールタイムがオンだったら
	if (m_bBreakTime)
	{
		m_nBreakTime++;
		if (m_nBreakTime >= PURPLE_BREAKTIME)
		{
			m_nBreakTime = 0;
			m_bBreakTime = false;
		}
	}
	else
	{
		//攻撃判定がオンの時
		if (m_bAttackStart)
		{
			m_bDiscovery = false;
			m_bAttackWait = false;
			switch (m_nAttackType)
			{
			case FIRE:
				FireAttack();
				break;
			case BULLIZAD:
				IceAttack();
				break;
			}

		}

		//プレイヤーが攻撃範囲に入ったら
		if (m_fPlayerVecLength < PURPLE_ATTACKSTART_RADIUS)
		{
			if (m_FireState.bAttackStart == false&& m_IceState.bAttackStart == false)
			{
				m_bAttackWait = true;

			}
		}
		//攻撃待機中なら
		if (m_bAttackWait)
		{
			AttackWait();
		}

	}
}
//-----------------------------
//火属性の攻撃
//-----------------------------
void CEnemy_Purple::FireAttack()
{
	m_bDiscovery = false;
	m_FireState.nCnt++;
	if (m_FireState.bEffectStop == false)
	{

		m_FireState.bEffectStop = true;
		//当たり判定ライン
		for (int nCnt = 0; nCnt < PURPLE_FIRE_COUNT; nCnt++)
		{
			float fAng = m_rot.y + ((D3DX_PI / PURPLE_FIRE_COUNT*2.0f)*nCnt);
			D3DXVECTOR3 posAdd;
			posAdd.x = m_pos.x + (sinf(fAng)*PURPLE_ADDPOS);
			posAdd.z = m_pos.z + (cosf(fAng)*PURPLE_ADDPOS);
			posAdd.y = m_pos.y + 40.0f;
			CCol_Area_Bg::Create(posAdd, fAng, 10.0f, (ENEMY_MAGIC_FIELD_ICE_SIZE_Z) * 20, 0.5f,
				PURPLE_FIRE_ATTACK_STARTTIME, CTexture::FADE);
			CCol_Area::Create(posAdd, fAng, 10.0f, 0.5f, (ENEMY_MAGIC_FIELD_ICE_SIZE_Z) * 20,
				PURPLE_FIRE_ATTACK_STARTTIME, CTexture::FADE);


		}
	}
	if (m_FireState.nCnt >= PURPLE_FIRE_ATTACK_STARTTIME)
	{
		if (m_FireState.bAttackEffectStop == false)
		{
			D3DXVECTOR3 posAdd;
			posAdd.x = m_pos.x + (sinf(m_rot.y)*PURPLE_ADDPOS);
			posAdd.z = m_pos.z + (cosf(m_rot.y)*PURPLE_ADDPOS);
			posAdd.y = m_pos.y + (m_pPartsModel[1]->GetMaxPos().y / 1.3f);
			//発動エフェクトを出す
			CEffect::Create(posAdd, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,0.0f },
			{ 1.0,1.0,1.0,1.0f }, false, 0.0f, 0.01f, true, CTexture::HitEffect, 0.5f, true);
			CEffect::Create(posAdd, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,0.0f },
			{ 1.0,1.0,1.0,1.0f }, false, 0.0f, 0.01f, true, CTexture::HitEffect, 0.5f, true);
			m_FireState.bAttackEffectStop = true;
		}
		m_FireState.bAttackStart = true;

		//火を出すためのカウントを進める
		m_FireState.nAttackCnt++;
		if (m_FireState.nAttackCnt >= PURPLE_FIRE_TIME &&
			m_FireState.nNumCnt <= PURPLE_FIRE_COUNT)
		{
			m_FireState.nAttackCnt = 0;
			//攻撃音
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_FIRE, 0.7f);
			for (int nCnt = 0; nCnt < PURPLE_FIRE_COUNT; nCnt++)
			{
				m_FireState.fMoveAng += 0.01f;
				float fAng = m_rot.y + ((D3DX_PI / PURPLE_FIRE_COUNT*2.0f)*nCnt) + m_FireState.fMoveAng;
				D3DXVECTOR3 posAdd;
				posAdd.x = m_pos.x + (sinf(fAng)*PURPLE_ADDPOS);
				posAdd.z = m_pos.z + (cosf(fAng)*PURPLE_ADDPOS);
				posAdd.y = m_pos.y + 40.0f;

				//炎の球を出す
				C_Enemy_Magic_Fire::Create(posAdd, {0.0f,fAng ,0.0f}, false);
				CMagicCircle::Create(posAdd, { 0.0f,fAng ,0.0f }, ENEMY_MAGIC_FIRE_MAZIC_SIRCLE_RADIUS, PURPLE_ATTACKTIME, true, true, m_col,
					CTexture::MagicCircle_TypeB);

			}
			m_FireState.nNumCnt++;
		}
		else if (m_FireState.nNumCnt >= PURPLE_FIRE_COUNT)
		{
			m_bBreakTime = true;
			m_bAttackStart = false;
			m_nTimer = 0;
			m_FireState.nNumCnt = 0;
			m_FireState.nCnt = 0;
			m_FireState.bAttackStart = false;
			//攻撃を氷属性のに変更
			m_nAttackType = BULLIZAD;
			m_bDiscovery = true;
			m_FireState.bEffectStop = false;
			m_FireState.bAttackEffectStop = false;


		}

	}


}
//-----------------------------
//氷属性の攻撃
//-----------------------------
void CEnemy_Purple::IceAttack()
{

	m_IceState.nAttackCnt++;
	//魔法陣エフェクトを一回だけ出す
	if (m_IceState.bEffectStop == false)
	{
		m_IceState.bEffectStop = true;
		CMagicCircle::Create(m_pos, { D3DXToRadian(90.0f),0.0f,0.0f }, ENEMY_MAGIC_ICE_MAZIC_SIRCLE_RADIUS, PURPLE_ICE_ATTACK_STARTTIME+ PURPLE_ICE_ATTACK_ENDTIME, true, true, m_col,
			CTexture::MagicCircle_TypeB,3.5f);
		for (int nCnt = 0; nCnt < PURPLE_ICE_COUNT; nCnt++)
		{
			float fAng = m_rot.y + ((D3DX_PI / PURPLE_ICE_COUNT*2.0f)*nCnt);
			D3DXVECTOR3 posAdd;
			posAdd.x = m_pos.x + (sinf(fAng)*PURPLE_ADDPOS);
			posAdd.z = m_pos.z + (cosf(fAng)*PURPLE_ADDPOS);
			posAdd.y = m_pos.y;
			CCol_Area_Bg::Create(posAdd, fAng, ENEMY_MAGIC_FIELD_ICE_SIZE_X, (ENEMY_MAGIC_FIELD_ICE_SIZE_Z) * 20, 0.5f,
				PURPLE_ICE_ATTACK_STARTTIME, CTexture::FADE);
			CCol_Area::Create(posAdd, fAng, ENEMY_MAGIC_FIELD_ICE_SIZE_X, 0.5f, (ENEMY_MAGIC_FIELD_ICE_SIZE_Z) * 20,
				PURPLE_ICE_ATTACK_STARTTIME, CTexture::FADE);

		}

	}
	//攻撃開始時間になったら
	if (m_IceState.nAttackCnt >= PURPLE_ICE_ATTACK_STARTTIME&&
		m_IceState.bAttackStart == false)
	{
		m_IceState.bAttackStart = true;
		m_IceState.bAttack = true;
	}
	//一回だけ氷の魔法を出す
	if (m_IceState.bAttack)
	{
		m_IceState.bAttack = false;
		//攻撃音
		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ICE);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ICE, 0.7f);
		//氷の塊を出す
		for (int nCnt = 0; nCnt < PURPLE_ICE_COUNT; nCnt++)
		{
			float fAng = m_rot.y + ((D3DX_PI / PURPLE_ICE_COUNT*2.0f)*nCnt);
			D3DXVECTOR3 posAdd;
			posAdd.x = m_pos.x + (sinf(fAng)*PURPLE_ADDPOS);
			posAdd.z = m_pos.z + (cosf(fAng)*PURPLE_ADDPOS);
			posAdd.y = m_pos.y;

			C_Enemy_Magic_Field_Ice::Create(posAdd, {0.0f,fAng ,0.0f}, 2.0f, 20);

		}
	}
	if (m_IceState.bAttackStart)
	{
		//攻撃時間終了までのカウント
		m_IceState.nEndCnt++;
	}
	//攻撃終了時間になったらインターバルを取る
	if (m_IceState.nEndCnt >= PURPLE_ICE_ATTACK_ENDTIME)
	{
		m_IceState.bEffectStop = false;

		m_IceState.nEndCnt = 0;
		m_IceState.nAttackCnt = 0;
		m_IceState.bAttackStart = false;
		m_bBreakTime = true;
		m_bAttackStart = false;
		m_bDiscovery = true;
		//攻撃を炎属性のに変更
		m_nAttackType = FIRE;
	}


}
//-----------------------------
//敵の攻撃待機の処理
//-----------------------------
void CEnemy_Purple::AttackWait()
{
	if (m_bAttack == false && m_bBreakTime == false)
	{
		m_nAttackStartCnt++;
		m_MotionType = ATTACKWAIT;

		//攻撃開始時間になったら
		if (m_nAttackStartCnt >= PURPLE_ATTACKSTARTTIME)
		{
			m_nAttackStartCnt = 0;
			//m_MotionType = ATTACK;
			m_bAttack = true;
			m_bAttackStart = true;
		}
		if (m_nAttackStartCnt % 4 == 0)
		{
			std::random_device random;	// 非決定的な乱数生成器
			std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
			std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);

			float fAng = (float)randAng(mt);
			float fAng2 = (float)randAng(mt);

			D3DXVECTOR3 posAdd = {
				m_pos.x + (sinf(fAng)*sinf(fAng2)*PURPLE_ADDPOS),
				m_pos.y + (cosf(fAng)*PURPLE_ADDPOS),
				m_pos.z + (sinf(fAng)*cosf(fAng2)*PURPLE_ADDPOS) };
			D3DXVECTOR3 vec = m_pos - posAdd;
			float frot = atan2f(vec.x, vec.z);
			float frotY = atan2f(vec.x, vec.y);

			D3DXVECTOR3 move = {
				sinf(frot)*PURPLE_SPEED,
				cosf(frot)*PURPLE_SPEED,
				cosf(frot)*PURPLE_SPEED
			};

			CEffect::Create(posAdd, move, { 2.0f,2.0f,0.0f }, m_col, false, 0.0f, 0.03f, true, CTexture::Effect,
				0.0f, false, false);
			CEffect::Create(posAdd, move, { 2.0f,2.0f,0.0f }, m_col, false, 0.0f, 0.03f, true, CTexture::Effect,
				0.0f, false, false);

		}
		else if (m_nAttackStartCnt % 30 == 0)
		{
			CEffect::Create({ m_pos.x,m_pos.y + 50.0f,m_pos.z }, { 0.0f,0.0f,0.0f }, { 23.0f,23.0f,0.0f }, m_col, false, 0.0f, 0.01f, true, CTexture::RingEffect,
				0.0f, false, false, true);

		}
	}

}
//-----------------------------
//敵の自動移動処理
//-----------------------------
void CEnemy_Purple::AIMove()
{
	//プレイヤーが一定範囲近づいたら
	if (m_fPlayerVecLength < PURPLE_MOVERANGE)
	{
		//発見した
		m_bDiscovery = true;
		//自動で動くできるやつをオフにする
		m_bAIMove = false;
		m_bAIMoveStop = true;

	}
	//範囲外の時
	else
	{
		//見失う
		m_bDiscovery = false;
		m_bAIMoveStop = false;
		s_bCntStop = true;
	}

	//敵がプレイヤーを発見したなら
	if (m_bDiscovery)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		m_bMove = true;
	}
	else
	{
		m_bMove = false;

	}
	//プレイヤーが一定範囲近づいたら
	if (m_fPlayerVecLength < PURPLE_ATTACKSTART_RADIUS)
	{
		m_bMove = false;
	}
	//自動移動ができるなら
	if (m_bMove)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		//発射する方向を求める
		m_MoveSpeed.x = sinf(m_fAng)*PURPLE_SPEED;
		m_MoveSpeed.z = cosf(m_fAng)*PURPLE_SPEED;
		m_MotionType = MOVE;

		m_fWalkSoundInterval -= 0.1f;

		if (m_fWalkSoundInterval <= 0.0f)
		{
			//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMYWALK);
			//CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMYWALK, 0.6f);
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
//-----------------------------
//敵がプレイヤーを発見したときの処理
//-----------------------------
void CEnemy_Purple::Discovery()
{
	//プレイヤーが一定範囲近づいたら
	if (m_fPlayerVecLength < PURPLE_ATTACKSTART_RADIUS)
	{
		//発見した
		m_bDiscovery = true;
	}
	//発見したら敵の向きをプレイヤーに追従
	if (m_bDiscovery)
	{
		m_rot.y = m_fAng + (D3DX_PI);

	}
}


