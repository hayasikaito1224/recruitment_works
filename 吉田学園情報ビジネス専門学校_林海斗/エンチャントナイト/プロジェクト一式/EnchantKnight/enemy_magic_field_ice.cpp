//===============================================-
//MP消費スキル（FlareStrike）の処理
//===============================================
#include "magic_ice.h"
#include "enemy_magic_field_ice.h"
#include "model.h"
#include "manager.h"
#include "Renderer.h"
#include "collision.h"
#include "player.h"
#include "Particle.h"
#include "game.h"
#include "gauge.h"
#include "MagicCircle.h"
#include "player.h"
#include "billboard.h"
#include "magic_ice.h"
#include "sound.h"
#include "circlegauge.h"
#include "col_area.h"
#define ENEMY_MAGIC_FIELD_ICE_MAX_TIME (20)
#define ENEMY_MAGIC_FIELD_ICE_DELETE_TIME (150)
#define ENEMY_MAGIC_FIELD_ICE_INTERVAL (30)
#define ENEMY_MAGIC_FIELD_ICE_AREA_X (30.0f)//当たり判定のエリア
#define ENEMY_MAGIC_FIELD_ICE_AREA_Z (800.0f)//当たり判定のエリア
#define ENEMY_MAGIC_FIELD_ICE_POWER (5)//魔法の攻撃力
#define ENEMY_MAGIC_FIELD_ICE_ATTACK_INTERVALTIME (80)//魔法のクールタイム

//========================================---
//コンストラクタ
//========================================
C_Enemy_Magic_Field_Ice::C_Enemy_Magic_Field_Ice(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_nTimer = 0;
	m_nMaxTime = ENEMY_MAGIC_FIELD_ICE_MAX_TIME;
	m_nCnt = 0;
	m_fSpeed = 0.0f;
	m_nMaxCnt = 0;
}
//==========================================
//デストラクタ
//==========================================
C_Enemy_Magic_Field_Ice::~C_Enemy_Magic_Field_Ice()
{

}
//==========================================
//初期化
//==========================================
HRESULT C_Enemy_Magic_Field_Ice::Init()
{

	m_vtx[2].vtx.x = - ENEMY_MAGIC_FIELD_ICE_AREA_X;
	m_vtx[2].vtx.z = 0.0f;
	m_vtx[3].vtx.x = ENEMY_MAGIC_FIELD_ICE_AREA_X;
	m_vtx[3].vtx.z = 0.0f;

	return S_OK;
}
//==========================================
//破棄
//==========================================
void C_Enemy_Magic_Field_Ice::Uninit()
{
	if (m_pArea != nullptr)
	{
		m_pArea->Uninit();
		m_pArea = nullptr;
	}
	Release();
}
//==========================================
//更新
//==========================================
void C_Enemy_Magic_Field_Ice::Update()
{
	m_nTimer++;
	if (m_nTimer >= m_nMaxCnt + ENEMY_MAGIC_FIELD_ICE_DELETE_TIME)
	{
		m_bUninit = true;
	}
	else if (m_nTimer <= m_nMaxCnt)
	{
		m_nCnt++;
		D3DXVECTOR3 posAdd;
		posAdd.x = (sinf(m_rot.y)*(ENEMY_MAGIC_FIELD_ICE_INTERVAL * m_nCnt));
		posAdd.z = (cosf(m_rot.y)*(ENEMY_MAGIC_FIELD_ICE_INTERVAL * m_nCnt));
		posAdd.y = 0.0f;
		m_vtx[0].vtx.x = -ENEMY_MAGIC_FIELD_ICE_AREA_X;
		m_vtx[0].vtx.z = ENEMY_MAGIC_FIELD_ICE_INTERVAL * m_nCnt;
		m_vtx[1].vtx.x = ENEMY_MAGIC_FIELD_ICE_AREA_X;
		m_vtx[1].vtx.z = ENEMY_MAGIC_FIELD_ICE_INTERVAL * m_nCnt;

		C_Magic_Ice::Create(m_pos - posAdd, m_rot, m_fSpeed);
	}

	//当たり判定
	CScene *pScene_P = CScene::GetScene(OBJTYPE_PLAYER);
	CCollision *pCollision = new CCollision;

	//敵とのの当たり判定
	if (m_bAttack == false)
	{
		if (pScene_P != nullptr)
		{
			CPlayer *pPlayer = (CPlayer*)pScene_P;
			float fColisionArea = 0.0f;
			if (pPlayer->GetParts(0) != nullptr)
			{
				fColisionArea = pPlayer->GetParts(0)->GetMaxPos().x;
			}
			bool bHit = Collision(&pPlayer->GetPos(), fColisionArea);
			pPlayer->SetHit(bHit);
			if (bHit == true && pPlayer->GetDeth() == false && pPlayer->GetHit() == false)
			{
				m_bAttack = true;
				pPlayer->SetHit(true);
				CManager::GetGame()->GetHPGauge()->SetGauge(ENEMY_MAGIC_FIELD_ICE_POWER);
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);
				std::random_device random;	// 非決定的な乱数生成器
				std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
				std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
				//CPを増やす
				CManager::GetGame()->GetCPGauge()->SetGauge(-1);

				float fAng = randAng(mt);
				//攻撃用のモデルとプレイヤーのベクトルを求める
				D3DXVECTOR3 vec = m_pos - pPlayer->GetPos();
				float fAngle = atan2(vec.x, vec.y);//敵の向き
												   //ヒットエフェクトのずらし位置を求める
				D3DXVECTOR3 Addmove = D3DXVECTOR3(
					sinf(fAngle)*10.0f,
					0.0f,
					cosf(fAngle)*10.0f);
				CEffect::Create
				({ pPlayer->GetPos().x, pPlayer->GetPos().y, pPlayer->GetPos().z },
				{ 0.0f,0.0f,0.0f }, { 1.2f,1.0f,0.0f },
				{ 1.0f,0.5f,0.5f,1.0f }, false, 0.0f, 0.032f, true, CTexture::HitEffect, fAng, true);

			}

		}
	}

	//攻撃していたらいったん攻撃しない
	if (m_bAttack)
	{
		m_nAttackTimer++;
		if (m_nAttackTimer >= ENEMY_MAGIC_FIELD_ICE_ATTACK_INTERVALTIME)
		{
			m_nAttackTimer = 0;
			m_bAttack = false;
		}
	}
	//消去判定がオンになっていたら消す
	if (m_bUninit == true)
	{
		Uninit();
	}
}
//==========================================
//描画
//==========================================
void C_Enemy_Magic_Field_Ice::Draw()
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;//計算用マトリックス

								//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	////向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y+(D3DX_PI), m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}
//==========================================
//インスタンス生成
//==========================================
C_Enemy_Magic_Field_Ice * C_Enemy_Magic_Field_Ice::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const float fSpeed,
	const int nMaxCnt)
{
	C_Enemy_Magic_Field_Ice *pIce = new C_Enemy_Magic_Field_Ice(OBJTYPE_EFFECT);
	if (pIce != nullptr)
	{
		pIce->m_pos = pos;
		pIce->m_rot = rot;
		pIce->m_fSpeed = fSpeed;
		pIce->m_nMaxCnt = nMaxCnt;
		pIce->Init();

	}
	return pIce;
}
//==========================================
//当たり判定
//==========================================
bool C_Enemy_Magic_Field_Ice::Collision(D3DXVECTOR3 *pPos,float fRadius)
{
	bool bHit = false;//当たったかどうかを返す
					  //現在のデータ
	D3DXVECTOR3 NorAsk;//法線
	D3DXVECTOR3 NorNormalization;//法線の正規化
	D3DXVECTOR3 VecNor;//法線ベクトル
	D3DXVECTOR3 VecVtx[SKILL02_COLLISIONVTX];//頂点と頂点の間のベクトル
	D3DXVECTOR3 VecPair;//相手の位置から自分の位置までのベクトル
	D3DXVECTOR3 PairPos;//相手の位置
	D3DXVECTOR3 VecVtxWhile[SKILL02_COLLISIONVTX];////頂点と頂点の間のベクトル
	D3DXVECTOR3 VecVtxPair[SKILL02_COLLISIONVTX];//相手の位置から自分の頂点位置までのベクトル

	NorNormalization = { 0.0f,0.0f,0.0f };
	NorAsk = { 0.0f,0.0f,0.0f };
	VecNor = { 0.0f,0.0f,0.0f };
	VecPair = { 0.0f,0.0f,0.0f };
	PairPos = *pPos;

	//ローカル変数の初期化
	for (int nCnt = 0; nCnt < SKILL02_COLLISIONVTX; nCnt++)
	{
		VecVtx[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxPair[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxWhile[nCnt] = { 0.0f,0.0f,0.0f };

	}
	//子のposを相手の幅分増やす
	//上
	m_vtx[0].vtx.x -= fRadius;
	m_vtx[0].vtx.z -= fRadius;

	m_vtx[1].vtx.x += fRadius;
	m_vtx[1].vtx.z -= fRadius;

	m_vtx[2].vtx.x -= fRadius;
	m_vtx[2].vtx.z -= fRadius;

	m_vtx[3].vtx.x += fRadius;
	m_vtx[3].vtx.z -= fRadius;

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//↑のやつをマトリックスに反映させる
	for (int nNumVtx = 0; nNumVtx < SKILL02_COLLISIONVTX; nNumVtx++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
		D3DXMATRIX mtxParent;//親のマトリックス

							 //各頂点のワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_vtx[nNumVtx].mtxWorld);
		////向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_vtx[nNumVtx].rot.y, m_vtx[nNumVtx].rot.x, m_vtx[nNumVtx].rot.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxRotModel);
		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel, m_vtx[nNumVtx].vtx.x, m_vtx[nNumVtx].vtx.y, m_vtx[nNumVtx].vtx.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxTransModel);
		mtxParent = m_mtxWorld;
		//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld,
			&m_vtx[nNumVtx].mtxWorld,
			&m_mtxWorld);

		//各パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_vtx[nNumVtx].mtxWorld);
	}

	//頂点と頂点の間のベクトルを求める
	VecVtxWhile[0] = { m_vtx[1].mtxWorld._41 - m_vtx[0].mtxWorld._41,
		m_vtx[1].mtxWorld._42 - m_vtx[0].mtxWorld._42,
		m_vtx[1].mtxWorld._43 - m_vtx[0].mtxWorld._43 };

	VecVtxWhile[1] = { m_vtx[3].mtxWorld._41 - m_vtx[1].mtxWorld._41,
		m_vtx[3].mtxWorld._42 - m_vtx[1].mtxWorld._42,
		m_vtx[3].mtxWorld._43 - m_vtx[1].mtxWorld._43 };

	VecVtxWhile[2] = { m_vtx[2].mtxWorld._41 - m_vtx[3].mtxWorld._41,
		m_vtx[2].mtxWorld._42 - m_vtx[3].mtxWorld._42,
		m_vtx[2].mtxWorld._43 - m_vtx[3].mtxWorld._43 };

	VecVtxWhile[3] = { m_vtx[0].mtxWorld._41 - m_vtx[2].mtxWorld._41,
		m_vtx[0].mtxWorld._42 - m_vtx[2].mtxWorld._42,
		m_vtx[0].mtxWorld._43 - m_vtx[2].mtxWorld._43 };

	//相手の位置から自分の頂点位置までのベクトル
	VecVtxPair[0] = { PairPos.x - m_vtx[0].mtxWorld._41 ,
		PairPos.y - m_vtx[0].mtxWorld._42 ,
		PairPos.z - m_vtx[0].mtxWorld._43 };

	VecVtxPair[1] = { PairPos.x - m_vtx[1].mtxWorld._41 ,
		PairPos.y - m_vtx[1].mtxWorld._42 ,
		PairPos.z - m_vtx[1].mtxWorld._43 };

	VecVtxPair[2] = { PairPos.x - m_vtx[3].mtxWorld._41 ,
		PairPos.y - m_vtx[3].mtxWorld._42 ,
		PairPos.z - m_vtx[3].mtxWorld._43 };

	VecVtxPair[3] = { PairPos.x - m_vtx[2].mtxWorld._41 ,
		PairPos.y - m_vtx[2].mtxWorld._42 ,
		PairPos.z - m_vtx[2].mtxWorld._43 };

	float fVecClossY[SKILL02_COLLISIONVTX];//外積

	//攻撃当たり判定の外積を求める
	for (int nCnt = 0; nCnt < SKILL02_COLLISIONVTX; nCnt++)
	{
		fVecClossY[nCnt] = (VecVtxWhile[nCnt].x*VecVtxPair[nCnt].z) -
			(VecVtxPair[nCnt].x * VecVtxWhile[nCnt].z);

	}

	//法線ベクトルが相手の半径以下になってる&&前の法線ベクトルが相手の半径以上だったら
	if (fVecClossY[0] < 0.00f && fVecClossY[1] < 0.00f && fVecClossY[2] < 0.00f && fVecClossY[3] < 0.00f)
	{
		bHit = true;
	}
	m_vtx[0].vtx.x -= -fRadius;
	m_vtx[0].vtx.z -= -fRadius;
	m_vtx[1].vtx.x += -fRadius;
	m_vtx[1].vtx.z -= -fRadius;
	m_vtx[2].vtx.x -= -fRadius;
	m_vtx[2].vtx.z -= -fRadius;
	m_vtx[3].vtx.x += -fRadius;
	m_vtx[3].vtx.z -= -fRadius;

	return bHit;

}
