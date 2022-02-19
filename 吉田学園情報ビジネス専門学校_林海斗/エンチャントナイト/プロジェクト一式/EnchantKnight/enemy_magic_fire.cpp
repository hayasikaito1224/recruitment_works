//===============================================-
//�����@�̃N���X
//===============================================
#include "enemy_magic_fire.h"
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
#define ENEMY_MAGIC_FIRE_SIZE (5.0)//�΂̖��@�̑傫��
#define ENEMY_MAGIC_FIRE_SPEED (7.0)//���@�̔��˃X�s�[�h
#define ENEMY_MAGIC_FIRE_SPEEDLIMIT (10.0)//���@�̔��˃X�s�[�h

#define ENEMY_MAGIC_FIRE_DELETE_TIME (200)//���@�̎������Ŏ���
#define ENEMY_MAGIC_FIRE_POWER (4)//�U����
#define ENEMY_MAGIC_FIRE_ROT_SPEED (0.2)

//========================================
//�R���X�g���N�^
//========================================
C_Enemy_Magic_Fire::C_Enemy_Magic_Fire(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_pFireModel = nullptr;
	m_bUninit = false;
	m_ControlPoint = { 100.0f,0.0f,200.0f };
	m_StartingPoint = { 0.0f,0.0f,0.0f };
	m_EndPoint = { 0.0f,0.0f,0.0f };
	m_nTime = 0.0f;
	m_nTimeAddSpeed = 0.04f;
	m_bBezier = false;
}
//==========================================
//�f�X�g���N�^
//==========================================
C_Enemy_Magic_Fire::~C_Enemy_Magic_Fire()
{
}
//==========================================
//������
//==========================================
HRESULT C_Enemy_Magic_Fire::Init()
{
	if (m_pFireModel == nullptr)
	{
		m_pFireModel = CModel::Create(m_pos, m_rot, 3, CModel::TYPE_OBJECT);
	}

	return S_OK;
}
//==========================================
//�j��
//==========================================
void C_Enemy_Magic_Fire::Uninit()
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
//�X�V
//==========================================
void C_Enemy_Magic_Fire::Update()
{
	m_nDeleteTimer++;
	if (m_nDeleteTimer >= ENEMY_MAGIC_FIRE_DELETE_TIME)
	{
		m_bUninit = true;
	}
	ModelRot.z += ENEMY_MAGIC_FIRE_ROT_SPEED;
	if (m_pFireModel != nullptr)
	{
		m_pFireModel->SetRot({ m_rot.x + ModelRot.x,0.0f,m_rot.z + ModelRot.z });
	}

	//���˂�����������߂�
	//�x�W�F�Ȑ����I���Ȃ�
	if (m_bBezier)
	{
		//�x�W�F�Ȑ��̏���
		BezierCurve();
	}
	else
	{
		m_pos.x -= sinf(m_rot.y)*ENEMY_MAGIC_FIRE_SPEED;
		m_pos.z -= cosf(m_rot.y)*ENEMY_MAGIC_FIRE_SPEED;
	}
	//�G�t�F�N�g
	CManager::GetGame()->GetParticle()->RandomCircleParticle(m_pos, { 1.0f, 0.3f, 0.3f, 1.0f}, false);

	//�ʒu���𔽉f

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	CCollision *pCollision = new CCollision;
	bool bHit = pCollision->CollisionAttack(m_pos,
		pPlayer->GetPos(), { pPlayer->GetParts(2)->GetMtxWorld()._41 ,pPlayer->GetParts(2)->GetMtxWorld()._42,pPlayer->GetParts(2)->GetMtxWorld()._43 },
		50.0f);
	if (bHit == true && pPlayer->GetDeth() == false)
	{
		m_bUninit = true;
		CManager::GetGame()->GetHPGauge()->SetGauge(ENEMY_MAGIC_FIRE_POWER);
		pPlayer->SetHit(true);
		std::random_device random;	// �񌈒�I�ȗ���������
		std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
		std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
		float fAng = randAng(mt);
		//�U���p�̃��f���ƃv���C���[�̃x�N�g�������߂�
		D3DXVECTOR3 vec = m_pos - pPlayer->GetPos();
		float fAngle = atan2(vec.x, vec.y);//�G�̌���
										   //�q�b�g�G�t�F�N�g�̂��炵�ʒu�����߂�
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

	//�������肪�I���ɂȂ��Ă��������
	if (m_bUninit == true)
	{
		Uninit();
	}
}
//==========================================
//�`��
//==========================================
void C_Enemy_Magic_Fire::Draw()
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

	if (m_pFireModel != nullptr)
	{
		m_pFireModel->Draw();

	}

}
//==========================================
//�C���X�^���X��������
//==========================================
C_Enemy_Magic_Fire * C_Enemy_Magic_Fire::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const bool bBezier,
	 const float Shotrot)
{
	C_Enemy_Magic_Fire *pMagic = new C_Enemy_Magic_Fire(CScene::OBJTYPE_MAGIC);
	if (pMagic != nullptr)
	{
		pMagic->Init();
		pMagic->m_pos = pos;
		pMagic->m_rot = rot;
		pMagic->m_rot.y += Shotrot;

		pMagic->m_bBezier = bBezier;
		//�x�W�F�Ȑ����L���Ȃ�
		if (bBezier)
		{
			pMagic->m_pPlayer = CManager::GetGame()->GetPlayer();
			pMagic->m_EndPoint = pMagic->m_pPlayer->GetPos();
			pMagic->m_StartingPoint = pos;
			pMagic->m_ControlPoint.y = pos.y;
			D3DXVECTOR3 vec = pMagic->m_EndPoint - pos;
			D3DXVECTOR3 Nomalaize;
			//�x�N�g����0.0~1.0�͈̔͂ɂ���
			D3DXVec3Normalize(&Nomalaize, &vec);

			//�x�N�g���̒��������߂�
			float fLength = sqrtf((vec.x * vec.x) + (vec.z * vec.z));

			//���K�������x�N�g���̒��������߂�
			float fLengthNomalaize = sqrtf((Nomalaize.x * Nomalaize.x) + (Nomalaize.z * Nomalaize.z));

			//�x�N�g���̒�������K�؂ȑ��������߂�
			float fSpeed = 1.0f / fLength;
			pMagic->m_nTimeAddSpeed = fSpeed*ENEMY_MAGIC_FIRE_SPEED*1.4f;
			//���x����
			if (pMagic->m_nTimeAddSpeed >= ENEMY_MAGIC_FIRE_SPEEDLIMIT)
			{
				pMagic->m_nTimeAddSpeed = ENEMY_MAGIC_FIRE_SPEEDLIMIT;
			}
			//�����ƂɃx�N�g�������߂�
			float fVecLengthX = sqrtf((vec.x * vec.x));
			float fVecLengthZ = sqrtf((vec.z * vec.z));

			//�ǂ̊p�x�ł������悤�Ȑ���_�ɂȂ�悤�ɂ���
			pMagic->m_ControlPoint.x = (pos.x + (sinf((rot.y - D3DXToRadian(Shotrot))*fLengthNomalaize))*(fLength / 4.5f));
			pMagic->m_ControlPoint.z = (pos.z + (cosf((rot.y - D3DXToRadian(Shotrot))*fLengthNomalaize))*(fLength / 4.5f));

		}


	}
	return pMagic;
}
//-------------------------------------
//�x�W�F�Ȑ�
//-------------------------------------
void C_Enemy_Magic_Fire::BezierCurve()
{
	m_nTime += m_nTimeAddSpeed;

	//else if (m_nTime >= 1.0f)
	//{
	//	m_bBezier = false;
	//}
	//�x�W�F�Ȑ��̌v�Z��
	m_pos.x = (1 - m_nTime)*(1 - m_nTime) * 
		m_StartingPoint.x + 2 * (1 - m_nTime) * m_nTime * 
		m_ControlPoint.x + m_nTime * m_nTime * m_EndPoint.x;
	m_pos.z = (1 - m_nTime)*(1 - m_nTime) *
		m_StartingPoint.z + 2 * (1 - m_nTime) * m_nTime *
		m_ControlPoint.z + m_nTime * m_nTime * m_EndPoint.z;
	if (m_nTime < 0.8f)
	{
		m_EndPoint = m_pPlayer->GetPos();
	}
}
