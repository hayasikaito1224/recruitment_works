//===============================================-
//�X���@�̃N���X
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

#define ENEMY_MAGIC_ICE_SIZE (5.0)//�΂̖��@�̑傫��
#define ENEMY_MAGIC_ICE_SPEED (7.0)//���@�̔��˃X�s�[�h
#define ENEMY_MAGIC_ICE_SPEEDLIMIT (10.0)//���@�̔��˃X�s�[�h
#define ENEMY_MAGIC_ICE_DELETE_TIME (200)//���@�̎������Ŏ���
#define ENEMY_MAGIC_ICE_POWER (4)//�U����
#define ENEMY_MAGIC_ICE_ROT_SPEED (0.2)

//========================================
//�R���X�g���N�^
//========================================
C_Enemy_Magic_Ice::C_Enemy_Magic_Ice(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_pIceModel = nullptr;
	m_bUninit = false;
}
//==========================================
//�f�X�g���N�^
//==========================================
C_Enemy_Magic_Ice::~C_Enemy_Magic_Ice()
{
}
//==========================================
//������
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
//�j��
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
//�X�V
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

	//�G�t�F�N�g
	CManager::GetGame()->GetParticle()->RandomCircleParticle(m_pos, { 0.0f, 0.3f, 1.0f, 1.0f}, false);

	//�ʒu���𔽉f

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
void C_Enemy_Magic_Ice::Draw()
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

	if (m_pIceModel != nullptr)
	{
		m_pIceModel->Draw();

	}

}
//==========================================
//�C���X�^���X��������
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
