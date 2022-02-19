//===============================================-
//炎魔法のクラス
//===============================================
#include "magic_fire.h"
#include "model.h"
#include "Enemy.h"
#include "collision.h"
#include "manager.h"
#include "Particle.h"
#include "game.h"
#include "manager.h"
#include "gauge.h"
#include "Renderer.h"
#include "MagicCircle.h"
#include "sound.h"
#include "circlegauge.h"
#define FIRE_SIZE (5.0)//火の魔法の大きさ
#define FIRE_SPEED (4.0)//魔法の発射スピード
#define FIRE_SPEEDLIMIT (10.0)//魔法の発射スピード

#define FIRE_DELETE_TIME (90)//魔法の自動消滅時間
#define FIRE_POWER (4)//攻撃力
#define FIRE_ROT_SPEED (0.2)
#define FIRE_MAZIC_SIRCLE_RADIUS (20.0)

//========================================
//コンストラクタ
//========================================
C_Magic_Fire::C_Magic_Fire(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_pFireModel = nullptr;
	m_bUninit = false;
	CManager::GetGame()->GetMPGauge()->SetGauge(FIRE_MP);
	m_ControlPoint = { 100.0f,0.0f,200.0f };
	m_StartingPoint = { 0.0f,0.0f,0.0f };
	m_EndPoint = { 0.0f,0.0f,0.0f };
	m_nTime = 0.0f;
	m_nTimeAddSpeed = 0.04f;
	m_bBezier = false;
}
//==========================================
//デストラクタ
//==========================================
C_Magic_Fire::~C_Magic_Fire()
{
}
//==========================================
//初期化
//==========================================
HRESULT C_Magic_Fire::Init()
{
	if (m_pFireModel == nullptr)
	{
		m_pFireModel = CModel::Create(m_pos, m_rot, 3, CModel::TYPE_OBJECT);
	}

	return S_OK;
}
//==========================================
//破棄
//==========================================
void C_Magic_Fire::Uninit()
{
	if (m_pFireModel != nullptr)
	{
		m_pFireModel->Uninit();
		delete m_pFireModel;
		m_pFireModel = nullptr;
	}
	Release();
}
//==========================================
//更新
//==========================================
void C_Magic_Fire::Update()
{
	m_nDeleteTimer++;
	if (m_nDeleteTimer >= FIRE_DELETE_TIME)
	{
		m_bUninit = true;
	}
	ModelRot.z += FIRE_ROT_SPEED;
	if (m_pFireModel != nullptr)
	{
		m_pFireModel->SetRot({ m_rot.x + ModelRot.x,0.0f,m_rot.z + ModelRot.z });
	}

	//発射する方向を求める
	//ベジェ曲線がオンなら
	if (m_bBezier)
	{
		//ベジェ曲線の処理
		BezierCurve();
	}
	else
	{
		m_pos.x -= sinf(m_rot.y)*FIRE_SPEED;
		m_pos.z -= cosf(m_rot.y)*FIRE_SPEED;
	}
	//エフェクト
	CManager::GetGame()->GetParticle()->RandomCircleParticle(m_pos, { 1.0f, 0.3f, 0.3f, 1.0f}, false);

	//位置情報を反映

	CScene *pScene_E = CScene::GetScene(OBJTYPE_ENEMY);
	CCollision *pCollision = new CCollision;

	//敵とのの当たり判定
	while (pScene_E != NULL)
	{
		if (pScene_E != NULL)
		{
			CEnemy *pEnemy = (CEnemy*)pScene_E;
			//if (pEnemy->bHitAttack() == false)
			{
				bool bHit = pCollision->CollisionWeapon((CEnemy*)pScene_E, m_pos, m_pFireModel->GetMaxPos().x);
				pEnemy->SetHit(bHit);
				if (bHit == true)
				{
					m_bAttack = true;
					m_bUninit = true;
					pEnemy->SetbDamage(true);
					pEnemy->AddLife(-FIRE_POWER, pEnemy->FIRE);
					CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);
					std::random_device random;	// 非決定的な乱数生成器
					std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
					std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
					//CPを増やす
					CManager::GetGame()->GetCPGauge()->SetGauge(-5);

					float fAng = randAng(mt);
					//攻撃用のモデルとプレイヤーのベクトルを求める
					D3DXVECTOR3 vec = m_pos - pEnemy->GetPos();
					float fAngle = atan2(vec.x, vec.y);//敵の向き
													   //ヒットエフェクトのずらし位置を求める
					D3DXVECTOR3 Addmove = D3DXVECTOR3(
						sinf(fAngle)*10.0f,
						0.0f,
						cosf(fAngle)*10.0f);
					CEffect::Create
					({ m_pos.x + Addmove.x, m_pos.y + Addmove.y, m_pos.z + Addmove.z },
					{ 0.0f,0.0f,0.0f }, { 1.2f,1.0f,0.0f },
					{ 1.0f,0.3f,0.3f,1.0f }, false, 0.0f, 0.032f, true, CTexture::HitEffect, fAng, true);

				}
			}
		}
		pScene_E = pScene_E->GetSceneNext(pScene_E);
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
void C_Magic_Fire::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
										  //各パーツのワールドマトリックスの初期化gtryg

	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	//向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pFireModel != nullptr)
	{
		m_pFireModel->Draw();

	}

}
//==========================================
//インスタンス生成処理
//==========================================
C_Magic_Fire * C_Magic_Fire::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const bool bBezier,
	CEnemy *pEnemy, const float Shotrot)
{
	C_Magic_Fire *pMagic = new C_Magic_Fire(CScene::OBJTYPE_MAGIC);
	if (pMagic != nullptr)
	{
		pMagic->Init();
		pMagic->m_pos = pos;
		pMagic->m_rot = rot;
		pMagic->m_rot.y += Shotrot;

		pMagic->m_bBezier = bBezier;
		//ベジェ曲線が有効なら
		if (bBezier)
		{
			pMagic->m_pEnemy = pEnemy;
			pMagic->m_EndPoint = pEnemy->GetEnemyPos();
			pMagic->m_StartingPoint = pos;
			pMagic->m_ControlPoint.y = pos.y;
			D3DXVECTOR3 vec = pMagic->m_EndPoint - pos;
			D3DXVECTOR3 Nomalaize;
			//ベクトルを0.0~1.0の範囲にする
			D3DXVec3Normalize(&Nomalaize, &vec);

			//ベクトルの長さを求める
			float fLength = sqrtf((vec.x * vec.x) + (vec.z * vec.z));

			//正規化したベクトルの長さを求める
			float fLengthNomalaize = sqrtf((Nomalaize.x * Nomalaize.x) + (Nomalaize.z * Nomalaize.z));

			//ベクトルの長さから適切な速さを求める
			float fSpeed = 1.0f / fLength;
			pMagic->m_nTimeAddSpeed = fSpeed*FIRE_SPEED*1.4f;
			//速度制限
			if (pMagic->m_nTimeAddSpeed >= FIRE_SPEEDLIMIT)
			{
				pMagic->m_nTimeAddSpeed = FIRE_SPEEDLIMIT;
			}
			//軸ごとにベクトルを求める
			float fVecLengthX = sqrtf((vec.x * vec.x));
			float fVecLengthZ = sqrtf((vec.z * vec.z));

			//どの角度でも同じような制御点になるようにする
			pMagic->m_ControlPoint.x = (pos.x + (sinf((rot.y - D3DXToRadian(Shotrot))*fLengthNomalaize))*(fLength / 4.5f));
			pMagic->m_ControlPoint.z = (pos.z + (cosf((rot.y - D3DXToRadian(Shotrot))*fLengthNomalaize))*(fLength / 4.5f));

		}

		CMagicCircle::Create(pos, { 0.0f,rot.y,0.0f }, FIRE_MAZIC_SIRCLE_RADIUS, 10, true, true, { 1.0f, 0.3f, 0.3f, 1.0f },
			CTexture::MagicCircle_TypeB);

	}
	return pMagic;
}
//-------------------------------------
//ベジェ曲線
//-------------------------------------
void C_Magic_Fire::BezierCurve()
{
	m_nTime += m_nTimeAddSpeed;
	if (m_pEnemy->GetUninit() ==false)
	{
		m_EndPoint = m_pEnemy->GetEnemyPos();
	}
	//ベジェ曲線の計算式
	m_pos.x = (1 - m_nTime)*(1 - m_nTime) * 
		m_StartingPoint.x + 2 * (1 - m_nTime) * m_nTime * 
		m_ControlPoint.x + m_nTime * m_nTime * m_EndPoint.x;
	m_pos.z = (1 - m_nTime)*(1 - m_nTime) *
		m_StartingPoint.z + 2 * (1 - m_nTime) * m_nTime *
		m_ControlPoint.z + m_nTime * m_nTime * m_EndPoint.z;

}
