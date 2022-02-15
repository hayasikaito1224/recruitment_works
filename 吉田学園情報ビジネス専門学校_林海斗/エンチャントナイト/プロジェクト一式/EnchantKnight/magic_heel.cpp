//===============================================-
//�X���@�̃N���X
//===============================================
#include "magic_heel.h"
#include "manager.h"
#include "game.h"
#include "gauge.h"
#include "Particle_HeelEffect.h"
#define HEEL_VOLUME (10)//MP�����

//========================================---
//�R���X�g���N�^
//========================================
C_Magic_Heel::C_Magic_Heel(OBJTYPE nPriority) : C_Magic(nPriority)
{
	CManager::GetGame()->GetMPGauge()->SetGauge(HEEL_MP);
	pEffect = nullptr;
}
//==========================================
//�f�X�g���N�^
//==========================================
C_Magic_Heel::~C_Magic_Heel()
{
}
//==========================================
//������
//==========================================
HRESULT C_Magic_Heel::Init()
{
	pEffect = new CParticle_HeelEffect;
	return S_OK;
}
//==========================================
//�j��
//==========================================
void C_Magic_Heel::Uninit()
{
	Release();
}
//==========================================
//�X�V
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
//�`��
//==========================================
void C_Magic_Heel::Draw()
{
}
//==========================================
//�C���X�^���X����
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
