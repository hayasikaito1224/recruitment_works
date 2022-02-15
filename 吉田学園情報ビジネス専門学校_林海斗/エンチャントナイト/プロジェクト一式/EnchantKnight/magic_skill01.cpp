//===============================================-
//CP消費スキル（フリージングショット）の処理
//===============================================
#include "magic_ice.h"
#include "magic_skill01.h"
#include "model.h"
#include "manager.h"
#include "Renderer.h"
#include "collision.h"
#include "Enemy.h"
#include "Particle.h"
#include "game.h"
#include "gauge.h"
#include "MagicCircle.h"
#include "player.h"
#include "billboard.h"
#include "magic_big_Ice.h"
#define MAGIC_SKILL01_MAZIC_MAX_TIME (200)
#define MAGIC_SKILL01_MAZIC_INTERVAL (20)

//========================================---
//コンストラクタ
//========================================
C_Magic_Skill01::C_Magic_Skill01(OBJTYPE nPriority) : C_Magic(nPriority)
{
	//CManager::GetGame()->GetMPGauge()->SetGauge(MAGIC_SKILL01_MP);
	m_nTimer = 0;
	m_nMaxTime = MAGIC_SKILL01_MAZIC_MAX_TIME;
}
//==========================================
//デストラクタ
//==========================================
C_Magic_Skill01::~C_Magic_Skill01()
{
	CManager::GetGame()->GetPlayer()->SetbAttack(false);
	CManager::GetGame()->GetPlayer()->SetbSkill(false);

}
//==========================================
//初期化
//==========================================
HRESULT C_Magic_Skill01::Init()
{
	return S_OK;
}
//==========================================
//破棄
//==========================================
void C_Magic_Skill01::Uninit()
{
	Release();
}
//==========================================
//更新
//==========================================
void C_Magic_Skill01::Update()
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	//プレイヤーを攻撃状態にし続ける
	pPlayer->SetbAttack(true);
	pPlayer->SetbSkill(true);

	m_nTimer++;
	if (m_nTimer >= m_nMaxTime)
	{
		m_nTimer = 0;
		//スキルの動作をやめる
		m_bUninit = true;
		pPlayer->SetbAttack(false);
		pPlayer->SetbSkill(false);
		CBillboard *pBill = pPlayer->GetNearEnemyPos();
		D3DXVECTOR3 Enemypos = {0.0f,0.0f,0.0f};
		if (pBill != nullptr)
		{
			Enemypos = pBill->GetPos();
			//氷の球を出す
			C_Magic_Big_Ice::Create({ Enemypos.x,
				Enemypos.y + 300.0f,
				Enemypos.z }, { D3DXToRadian(90.0f),0.0f,0.0f},false, 2.0f);

		}
		else
		{
			//氷の球を出す
			C_Magic_Big_Ice::Create({ pPlayer->GetPos().x - sinf(pPlayer->GetRot().y)*200.0f
				,pPlayer->GetPos().y + 300.0f,pPlayer->GetPos().z - cosf(pPlayer->GetRot().y) * 200.0f }, 
				{ D3DXToRadian(90.0f),0.0f,0.0f },false, 2.0f);

		}


	}
	else if (m_nTimer % MAGIC_SKILL01_MAZIC_INTERVAL == 0)
	{
		//氷の球を出す
		C_Magic_Ice::Create({ pPlayer->GetPos() .x - sinf(pPlayer->GetRot().y)*80.0f
			,pPlayer->GetPos() .y + 50.0f,pPlayer->GetPos() .z - cosf(pPlayer->GetRot().y) * 80.0f }, pPlayer->GetRot(), 9.0f);
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
void C_Magic_Skill01::Draw()
{
}

C_Magic_Skill01 * C_Magic_Skill01::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot)
{
	C_Magic_Skill01 *pIce = new C_Magic_Skill01(OBJTYPE_EFFECT);
	if (pIce != nullptr)
	{
		pIce->Init();
		pIce->m_pos = pos;
		pIce->m_rot = rot;

	}
	return pIce;
}
