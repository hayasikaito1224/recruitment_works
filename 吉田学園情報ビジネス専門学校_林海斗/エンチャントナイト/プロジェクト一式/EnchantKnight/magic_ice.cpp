//===============================================-
//�X���@�̃N���X
//===============================================
#include "magic_ice.h"
#include "model.h"
#include "manager.h"
#include "Renderer.h"
#include "collision.h"
#include "Enemy.h"
#include "Particle.h"
#include "game.h"
#include "gauge.h"
#include "MagicCircle.h"
#include "sound.h"
#include "circlegauge.h"
#include "shadow.h"

#define ICE_MOVE (6)
#define ICE_ROT_SPEED (0.2)
#define ICE_POWER (1)//���@�̍U����
#define ICE_DELETE_TIME (150)//���@�̎������Ŏ���
#define ICE_MAZIC_SIRCLE_RADIUS (20.0)
#define ICE_UPWARD_TIME (20)//���@�̏㏸����
#define ICE_ATTACK_INTERVALTIME (80)//���@�̃N�[���^�C��

//========================================---
//�R���X�g���N�^
//========================================
C_Magic_Ice::C_Magic_Ice(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_pModel = nullptr;

}
//==========================================
//�f�X�g���N�^
//==========================================
C_Magic_Ice::~C_Magic_Ice()
{
}
//==========================================
//������
//==========================================
HRESULT C_Magic_Ice::Init()
{
	if (m_pModel == nullptr)
	{
		std::random_device random;	// �񌈒�I�ȗ���������
		std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
		std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);


		m_pModel = CModel::Create(m_pos, {m_rot.x,(float)randAng(mt),m_rot.z}, 4, CModel::TYPE_OBJECT);
		m_pModel->SetPos({ m_pos.x,m_pos.y - m_pModel->GetMaxPos().y,m_pos.z });
	}
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create({ 0.0f,0.0f,0.0f }, 50.0f, CTexture::Effect);
	}

	return S_OK;
}
//==========================================
//�j��
//==========================================
void C_Magic_Ice::Uninit()
{
	if (m_pModel != nullptr)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;

	}
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	Release();

}
//==========================================
//�X�V
//==========================================
void C_Magic_Ice::Update()
{
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPos(0.0f, 0.0f, { m_pModel->GetMaxPos().x ,0.0,m_pModel->GetMaxPos().z });
		m_pShadow->SetPos({m_pos.x,0.1f ,m_pos.z});
	}

	m_nDeleteTimer++;
	if (m_nDeleteTimer >= ICE_DELETE_TIME)
	{
		m_bUninit = true;
	}

	//���@�̏㏸���Ԃ��J�E���g
	m_nTimer++;
	if (m_nTimer <= ICE_UPWARD_TIME)
	{
		m_pos.y += m_fSpeed;
	}
	////�G�t�F�N�g
	//CManager::GetGame()->GetParticle()->RandomCircleParticle(m_pos, { 0.2f, 0.3f, 1.0, 1.0 }, false);

	//�U�����Ă����炢������U�����Ȃ�
	if (m_bAttack)
	{
		m_nAttackTimer++;
		if (m_nAttackTimer >= ICE_ATTACK_INTERVALTIME)
		{
			m_nAttackTimer = 0;
			m_bAttack = false;
		}
	}
	//�������肪�I���ɂȂ��Ă��������
	if (m_bUninit == true)
	{
		Uninit();
	}
}
//==========================================
//�`��
//==========================================
void C_Magic_Ice::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRotModel, mtxTransModel;//�v�Z�p�}�g���b�N�X
										  //�e�p�[�c�̃��[���h�}�g���b�N�X�̏�����gtryg

	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	//�����𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//�e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pModel != nullptr)
	{
		m_pModel->Draw();

	}
}

C_Magic_Ice * C_Magic_Ice::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, float m_fSpeed)
{
	C_Magic_Ice *pIce = new C_Magic_Ice(OBJTYPE_EFFECT);
	if (pIce != nullptr)
	{
		pIce->Init();
		pIce->m_pos = pos;
		pIce->m_rot = rot;
		pIce->m_fSpeed = m_fSpeed;
		CMagicCircle::Create(pos, {D3DXToRadian(90.0f),0.0f,0.0f }, ICE_MAZIC_SIRCLE_RADIUS, 10, true, true, { 0.2f, 0.3f, 1.0, 1.0 },
			CTexture::MagicCircle_TypeB);

	}
	return pIce;
}
