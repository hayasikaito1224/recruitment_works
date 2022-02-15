//===============================================-
//氷魔法のクラス
//===============================================
#include "magic_heel.h"
#include "manager.h"
#include "game.h"
#include "gauge.h"
#include "Particle_HeelEffect.h"
#define HEEL_VOLUME (10)//MP消費量

//========================================---
//コンストラクタ
//========================================
C_Magic_Heel::C_Magic_Heel(OBJTYPE nPriority) : C_Magic(nPriority)
{
	CManager::GetGame()->GetMPGauge()->SetGauge(HEEL_MP);
	pEffect = nullptr;
}
//==========================================
//デストラクタ
//==========================================
C_Magic_Heel::~C_Magic_Heel()
{
}
//==========================================
//初期化
//==========================================
HRESULT C_Magic_Heel::Init()
{
	pEffect = new CParticle_HeelEffect;
	return S_OK;
}
//==========================================
//破棄
//==========================================
void C_Magic_Heel::Uninit()
{
	Release();
}
//==========================================
//更新
//==========================================
void C_Magic_Heel::Update()
{
	if (pEffect != nullptr)
	{
		pEffect->PlayHeelEffect(m_pos);
		if (pEffect->GetbHeel() == true)
		{
			CManager::GetGame()->GetHPGauge()->SetGauge(-HEEL_VOLUME);
			Uninit();
		}
	}


}
//==========================================
//描画
//==========================================
void C_Magic_Heel::Draw()
{
}
//==========================================
//インスタンス生成
//==========================================

C_Magic_Heel * C_Magic_Heel::Create(D3DXVECTOR3 pos)
{
	C_Magic_Heel *pMagic = new C_Magic_Heel(CScene::OBJTYPE_MAGIC);
	if (pMagic != nullptr)
	{
		pMagic->Init();
		pMagic->m_pos = pos;

	}
	return pMagic;

}
