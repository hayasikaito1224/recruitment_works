//===============================================-
//�X���@�̃N���X
//===============================================
#include "magic_big_Ice.h"
#include "model.h"
#include "manager.h"
#include "Renderer.h"
#include "collision.h"
#include "Enemy.h"
#include "Particle.h"
#include "game.h"
#include "gauge.h"
#include "MagicCircle.h"
#define MAGIC_BIG_ICE_ROT_SPEED (0.2)
#define MAGIC_BIG_ICE_POWER (7)//���@�̍U����
#define MAGIC_BIG_ICE_MAZIC_SIRCLE_RADIUS (60.0)

//========================================---
//�R���X�g���N�^
//========================================
C_Magic_Big_Ice::C_Magic_Big_Ice(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_pModel = nullptr;
}
//==========================================
//�f�X�g���N�^
//==========================================
C_Magic_Big_Ice::~C_Magic_Big_Ice()
{
}
//==========================================
//������
//==========================================
HRESULT C_Magic_Big_Ice::Init()
{
	if (m_pModel == nullptr)
	{
		m_pModel = CModel::Create(m_pos, m_rot, 2, CModel::TYPE_OBJECT);
	}

	return S_OK;
}
//==========================================
//�j��
//==========================================
void C_Magic_Big_Ice::Uninit()
{
	if (m_pModel != nullptr)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;

	}
	Release();

}
//==========================================
//�X�V
//==========================================
void C_Magic_Big_Ice::Update()
{

	//ModelRot.z += MAGIC_BIG_ICE_ROT_SPEED;

	//if (m_pModel != nullptr)
	//{
	//	m_pModel->SetRot({ m_rot.x + ModelRot.x,0.0f,m_rot.z + ModelRot.z });
	//}

	m_pos.y -= m_fSpeed;
	if (m_pos.y <= 0.0f)
	{
		m_bUninit = true;

	}
	//�G�t�F�N�g
	CManager::GetGame()->GetParticle()->RandomCircleParticle(m_pos, { 0.2f, 0.3f, 1.0, 1.0 }, false);

	//�����蔻��
	CScene *pScene_E = CScene::GetScene(OBJTYPE_ENEMY);
	CCollision *pCollision = new CCollision;

	//�G�Ƃ̂̓����蔻��
	while (pScene_E != nullptr)
	{
		if (pScene_E != nullptr&&m_pModel != nullptr)
		{
			CEnemy *pEnemy = (CEnemy*)pScene_E;
			if (pEnemy->bHitAttack() == false && m_bAttack == false)
			{
				bool bHit = pCollision->CollisionWeapon((CEnemy*)pScene_E, m_pos, m_pModel->GetMaxPos().x);
				pEnemy->SetHit(bHit);
				if (bHit == true)
				{
					pEnemy->SetbDamage(true);
					pEnemy->AddLife(-MAGIC_BIG_ICE_POWER, pEnemy->FIRE);

				}
			}
		}
		pScene_E = pScene_E->GetSceneNext(pScene_E);
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
void C_Magic_Big_Ice::Draw()
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

C_Magic_Big_Ice * C_Magic_Big_Ice::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, bool bRotX, float m_fSpeed)
{
	C_Magic_Big_Ice *pIce = new C_Magic_Big_Ice(OBJTYPE_EFFECT);
	if (pIce != nullptr)
	{
		pIce->Init();
		pIce->m_pos = pos;
		pIce->m_rot = rot;
		pIce->m_fSpeed = m_fSpeed;
		CMagicCircle::Create(pos, rot, MAGIC_BIG_ICE_MAZIC_SIRCLE_RADIUS, 10, bRotX, true, { 0.2f, 0.3f, 1.0, 1.0 },
			CTexture::MagicCircle_TypeB);

	}
	return pIce;
}
