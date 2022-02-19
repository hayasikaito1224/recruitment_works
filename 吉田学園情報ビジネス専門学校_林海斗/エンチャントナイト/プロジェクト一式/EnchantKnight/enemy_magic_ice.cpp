//===============================================-
//氷魔法のクラス
//===============================================
#include "enemy_magic_ice.h"
#include "model.h"
#include "player.h"
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

#define ENEMY_MAGIC_ICE_SIZE (5.0)//火の魔法の大きさ
#define ENEMY_MAGIC_ICE_SPEED (7.0)//魔法の発射スピード
#define ENEMY_MAGIC_ICE_SPEEDLIMIT (10.0)//魔法の発射スピード
#define ENEMY_MAGIC_ICE_DELETE_TIME (200)//魔法の自動消滅時間
#define ENEMY_MAGIC_ICE_POWER (4)//攻撃力
#define ENEMY_MAGIC_ICE_ROT_SPEED (0.2)

//========================================
//コンストラクタ
//========================================
C_Enemy_Magic_Ice::C_Enemy_Magic_Ice(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_pIceModel = nullptr;
	m_bUninit = false;
}
//==========================================
//デストラクタ
//==========================================
C_Enemy_Magic_Ice::~C_Enemy_Magic_Ice()
{
}
//==========================================
//初期化
//==========================================
HRESULT C_Enemy_Magic_Ice::Init()
{
	if (m_pIceModel == nullptr)
	{
		m_pIceModel = CModel::Create(m_pos, m_rot, 2, CModel::TYPE_OBJECT);
	}
	return S_OK;
}
//==========================================
//破棄
//==========================================
void C_Enemy_Magic_Ice::Uninit()
{
	if (m_pIceModel != nullptr)
	{
		m_pIceModel->Uninit();
		delete m_pIceModel;
		m_pIceModel = nullptr;
	}
	Release();
}
//==========================================
//更新
//==========================================
void C_Enemy_Magic_Ice::Update()
{
	m_nDeleteTimer++;

	if (m_nDeleteTimer >= ENEMY_MAGIC_ICE_DELETE_TIME)
	{
		m_bUninit = true;
	}

	ModelRot.z += ENEMY_MAGIC_ICE_ROT_SPEED;

	if (m_pIceModel != nullptr)
	{
		m_pIceModel->SetRot({ m_rot.x + ModelRot.x,0.0f,m_rot.z + ModelRot.z });
	}

	m_pos.x -= sinf(m_rot.y)*ENEMY_MAGIC_ICE_SPEED;
	m_pos.z -= cosf(m_rot.y)*ENEMY_MAGIC_ICE_SPEED;

	//エフェクト
	CManager::GetGame()->GetParticle()->RandomCircleParticle(m_pos, { 0.0f, 0.3f, 1.0f, 1.0f}, false);

	//位置情報を反映

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	CCollision *pCollision = new CCollision;
	bool bHit = pCollision->CollisionAttack(m_pos,
		pPlayer->GetPos(), { pPlayer->GetParts(2)->GetMtxWorld()._41 ,pPlayer->GetParts(2)->GetMtxWorld()._42,pPlayer->GetParts(2)->GetMtxWorld()._43 },
		50.0f);
	if (bHit == true && pPlayer->GetDeth() == false)
	{
		m_bUninit = true;
		CManager::GetGame()->GetHPGauge()->SetGauge(ENEMY_MAGIC_ICE_POWER);
		pPlayer->SetHit(true);
		std::random_device random;	// 非決定的な乱数生成器
		std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
		std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
		float fAng = randAng(mt);
		//攻撃用のモデルとプレイヤーのベクトルを求める
		D3DXVECTOR3 vec = m_pos - pPlayer->GetPos();
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

	//消去判定がオンになっていたら消す
	if (m_bUninit == true)
	{
		Uninit();
	}
}
//==========================================
//描画
//==========================================
void C_Enemy_Magic_Ice::Draw()
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

	if (m_pIceModel != nullptr)
	{
		m_pIceModel->Draw();

	}

}
//==========================================
//インスタンス生成処理
//==========================================
C_Enemy_Magic_Ice * C_Enemy_Magic_Ice::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	C_Enemy_Magic_Ice *pMagic = new C_Enemy_Magic_Ice(CScene::OBJTYPE_MAGIC);
	if (pMagic != nullptr)
	{
		pMagic->Init();
		pMagic->m_pos = pos;
		pMagic->m_rot = rot;
	}
	return pMagic;
}
