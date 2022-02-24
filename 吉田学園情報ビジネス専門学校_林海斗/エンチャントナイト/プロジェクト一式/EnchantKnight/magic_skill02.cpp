//===============================================-
//MP����X�L���iFlareStrike�j�̏���
//===============================================
#include "magic_ice.h"
#include "magic_skill02.h"
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
#include "magic_ice.h"
#include "sound.h"
#include "circlegauge.h"
#include "col_area.h"
#define MAGIC_SKILL02_MAZIC_MAX_TIME (20)
#define MAGIC_SKILL02_MAZIC_DELETE_TIME (150)
#define MAGIC_SKILL02_MAZIC_INTERVAL (30)
#define MAGIC_SKILL02_MAZIC_AREA_X (30.0f)//�����蔻��̃G���A
#define MAGIC_SKILL02_MAZIC_AREA_Z (800.0f)//�����蔻��̃G���A

#define MAGIC_SKILL02_POWER (1)//���@�̍U����
#define MAGIC_SKILL02_ATTACK_INTERVALTIME (80)//���@�̃N�[���^�C��

//========================================---
//�R���X�g���N�^
//========================================
C_Magic_Skill02::C_Magic_Skill02(OBJTYPE nPriority) : C_Magic(nPriority)
{
	//CManager::GetGame()->GetMPGauge()->SetGauge(MAGIC_SKILL02_MP);
	m_nTimer = 0;
	m_nMaxTime = MAGIC_SKILL02_MAZIC_MAX_TIME;
	CManager::GetGame()->GetMPGauge()->SetGauge(ICE_MP);
	m_nCnt = 0;
	m_fSpeed = 0.0f;
}

//==========================================
//�f�X�g���N�^
//==========================================
C_Magic_Skill02::~C_Magic_Skill02()
{

}

//==========================================
//������
//==========================================
HRESULT C_Magic_Skill02::Init()
{

	m_vtx[2].vtx.x = - MAGIC_SKILL02_MAZIC_AREA_X;
	m_vtx[2].vtx.z = 0.0f;
	m_vtx[3].vtx.x = MAGIC_SKILL02_MAZIC_AREA_X;
	m_vtx[3].vtx.z = 0.0f;

	return S_OK;
}
//==========================================
//�j��
//==========================================
void C_Magic_Skill02::Uninit()
{
	if (m_pArea != nullptr)
	{
		m_pArea->Uninit();
		m_pArea = nullptr;
	}
	Release();
}
//==========================================
//�X�V
//==========================================
void C_Magic_Skill02::Update()
{
	m_nTimer++;
	if (m_nTimer >= MAGIC_SKILL02_MAZIC_MAX_TIME+ MAGIC_SKILL02_MAZIC_DELETE_TIME)
	{
		m_bUninit = true;
	}
	else if (m_nTimer % 1 == 0&& m_nTimer <= MAGIC_SKILL02_MAZIC_MAX_TIME)
	{
		m_nCnt++;
		D3DXVECTOR3 posAdd;
		posAdd.x = (sinf(m_rot.y)*(MAGIC_SKILL02_MAZIC_INTERVAL * m_nCnt));
		posAdd.z = (cosf(m_rot.y)*(MAGIC_SKILL02_MAZIC_INTERVAL * m_nCnt));
		posAdd.y = 0.0f;
		m_vtx[0].vtx.x = -MAGIC_SKILL02_MAZIC_AREA_X;
		m_vtx[0].vtx.z = MAGIC_SKILL02_MAZIC_INTERVAL * m_nCnt;
		m_vtx[1].vtx.x = MAGIC_SKILL02_MAZIC_AREA_X;
		m_vtx[1].vtx.z = MAGIC_SKILL02_MAZIC_INTERVAL * m_nCnt;

		C_Magic_Ice::Create(m_pos - posAdd, m_rot, m_fSpeed);

	}

	//�����蔻��
	CScene *pScene_E = CScene::GetScene(OBJTYPE_ENEMY);
	CCollision *pCollision = new CCollision;

	//�G�Ƃ̂̓����蔻��
	if (m_bAttack == false)
	{
		while (pScene_E != nullptr)
		{
			if (pScene_E != nullptr)
			{
				CEnemy *pEnemy = (CEnemy*)pScene_E;
				if (pEnemy->bHitAttack() == false)
				{
					float fColisionArea = 0.0f;
					if (pEnemy->GetModel(0) != nullptr)
					{
						fColisionArea = pEnemy->GetModel(0)->GetMaxPos().x;
					}
					bool bHit = Collision(&pEnemy->GetEnemyPos(), fColisionArea);
					pEnemy->SetHit(bHit);
					if (bHit == true)
					{
						m_bAttack = true;
						pEnemy->SetbDamage(true);
						pEnemy->AddLife(-MAGIC_SKILL02_POWER, pEnemy->FIRE);
						CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);
						std::random_device random;	// �񌈒�I�ȗ���������
						std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
						std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
						//CP�𑝂₷
						CManager::GetGame()->GetCPGauge()->SetGauge(-1);

						float fAng = randAng(mt);
						//�U���p�̃��f���ƃv���C���[�̃x�N�g�������߂�
						D3DXVECTOR3 vec = m_pos - pEnemy->GetPos();
						float fAngle = atan2(vec.x, vec.y);//�G�̌���
														   //�q�b�g�G�t�F�N�g�̂��炵�ʒu�����߂�
						D3DXVECTOR3 Addmove = D3DXVECTOR3(
							sinf(fAngle)*10.0f,
							0.0f,
							cosf(fAngle)*10.0f);
						CEffect::Create
						({ pEnemy->GetPos().x, pEnemy->GetPos().y, pEnemy->GetPos().z  },
						{ 0.0f,0.0f,0.0f }, { 1.2f,1.0f,0.0f },
						{ 1.0f,0.5f,0.5f,1.0f }, false, 0.0f, 0.032f, true, CTexture::HitEffect, fAng, true);

					}
				}
			}
			pScene_E = pScene_E->GetSceneNext(pScene_E);
		}

	}

	//�U�����Ă����炢������U�����Ȃ�
	if (m_bAttack)
	{
		m_nAttackTimer++;
		if (m_nAttackTimer >= MAGIC_SKILL02_ATTACK_INTERVALTIME)
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
void C_Magic_Skill02::Draw()
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;//�v�Z�p�}�g���b�N�X

								//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	////�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y+(D3DX_PI), m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}

C_Magic_Skill02 * C_Magic_Skill02::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const float fSpeed)
{
	C_Magic_Skill02 *pIce = new C_Magic_Skill02(OBJTYPE_EFFECT);
	if (pIce != nullptr)
	{
		pIce->m_pos = pos;
		pIce->m_rot = rot;
		pIce->m_fSpeed = fSpeed;
		pIce->Init();

	}
	return pIce;
}
//==========================================
//�����蔻��
//==========================================

bool C_Magic_Skill02::Collision(D3DXVECTOR3 *pPos,float fRadius)
{
	bool bHit = false;//�����������ǂ�����Ԃ�
					  //���݂̃f�[�^
	D3DXVECTOR3 NorAsk;//�@��
	D3DXVECTOR3 NorNormalization;//�@���̐��K��
	D3DXVECTOR3 VecNor;//�@���x�N�g��
	D3DXVECTOR3 VecVtx[SKILL02_COLLISIONVTX];//���_�ƒ��_�̊Ԃ̃x�N�g��
	D3DXVECTOR3 VecPair;//����̈ʒu���玩���̈ʒu�܂ł̃x�N�g��
	D3DXVECTOR3 PairPos;//����̈ʒu
	D3DXVECTOR3 VecVtxWhile[SKILL02_COLLISIONVTX];////���_�ƒ��_�̊Ԃ̃x�N�g��
	D3DXVECTOR3 VecVtxPair[SKILL02_COLLISIONVTX];//����̈ʒu���玩���̒��_�ʒu�܂ł̃x�N�g��

	NorNormalization = { 0.0f,0.0f,0.0f };
	NorAsk = { 0.0f,0.0f,0.0f };
	VecNor = { 0.0f,0.0f,0.0f };
	VecPair = { 0.0f,0.0f,0.0f };
	PairPos = *pPos;

	//���[�J���ϐ��̏�����
	for (int nCnt = 0; nCnt < SKILL02_COLLISIONVTX; nCnt++)
	{
		VecVtx[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxPair[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxWhile[nCnt] = { 0.0f,0.0f,0.0f };

	}
	//�q��pos�𑊎�̕������₷
	//��
	m_vtx[0].vtx.x -= fRadius;
	m_vtx[0].vtx.z -= fRadius;

	m_vtx[1].vtx.x += fRadius;
	m_vtx[1].vtx.z -= fRadius;

	m_vtx[2].vtx.x -= fRadius;
	m_vtx[2].vtx.z -= fRadius;

	m_vtx[3].vtx.x += fRadius;
	m_vtx[3].vtx.z -= fRadius;

	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���̂���}�g���b�N�X�ɔ��f������
	for (int nNumVtx = 0; nNumVtx < SKILL02_COLLISIONVTX; nNumVtx++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X

							 //�e���_�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_vtx[nNumVtx].mtxWorld);
		////�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_vtx[nNumVtx].rot.y, m_vtx[nNumVtx].rot.x, m_vtx[nNumVtx].rot.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxRotModel);
		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel, m_vtx[nNumVtx].vtx.x, m_vtx[nNumVtx].vtx.y, m_vtx[nNumVtx].vtx.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxTransModel);
		mtxParent = m_mtxWorld;
		//�Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld,
			&m_vtx[nNumVtx].mtxWorld,
			&m_mtxWorld);

		//�e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_vtx[nNumVtx].mtxWorld);
	}

	//���_�ƒ��_�̊Ԃ̃x�N�g�������߂�
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

	//����̈ʒu���玩���̒��_�ʒu�܂ł̃x�N�g��
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

	float fVecClossY[SKILL02_COLLISIONVTX];//�O��

	//�U�������蔻��̊O�ς����߂�
	for (int nCnt = 0; nCnt < SKILL02_COLLISIONVTX; nCnt++)
	{
		fVecClossY[nCnt] = (VecVtxWhile[nCnt].x*VecVtxPair[nCnt].z) -
			(VecVtxPair[nCnt].x * VecVtxWhile[nCnt].z);

	}

	//�@���x�N�g��������̔��a�ȉ��ɂȂ��Ă�&&�O�̖@���x�N�g��������̔��a�ȏゾ������
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
