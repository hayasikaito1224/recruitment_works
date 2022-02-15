//===============================================-
//�����@�̃N���X
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
#define FIRE_SIZE (5.0)//�΂̖��@�̑傫��
#define FIRE_SPEED (4.0)//���@�̔��˃X�s�[�h
#define FIRE_DELETE_TIME (90)//���@�̎������Ŏ���
#define FIRE_POWER (4)//�U����
#define FIRE_ROT_SPEED (0.2)
#define FIRE_MAZIC_SIRCLE_RADIUS (20.0)

//========================================
//�R���X�g���N�^
//========================================
C_Magic_Fire::C_Magic_Fire(OBJTYPE nPriority) : C_Magic(nPriority)
{
	m_pFireModel = nullptr;
	m_bUninit = false;
	CManager::GetGame()->GetMPGauge()->SetGauge(FIRE_MP);

}
//==========================================
//�f�X�g���N�^
//==========================================
C_Magic_Fire::~C_Magic_Fire()
{
}
//==========================================
//������
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
//�j��
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
//�X�V
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

	//���˂�����������߂�
	m_pos.x -= sinf(m_rot.y)*FIRE_SPEED;
	m_pos.z -= cosf(m_rot.y)*FIRE_SPEED;
	//�G�t�F�N�g
	CManager::GetGame()->GetParticle()->RandomCircleParticle(m_pos, { 1.0f, 0.3f, 0.3f, 1.0f}, false);

	//�ʒu���𔽉f

	CScene *pScene_E = CScene::GetScene(OBJTYPE_ENEMY);
	CCollision *pCollision = new CCollision;

	//�G�Ƃ̂̓����蔻��
	while (pScene_E != NULL)
	{
		if (pScene_E != NULL)
		{
			CEnemy *pEnemy = (CEnemy*)pScene_E;
			if (pEnemy->bHitAttack() == false && m_bAttack == false)
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
					std::random_device random;	// �񌈒�I�ȗ���������
					std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
					std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);

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
					({ m_pos.x + Addmove.x, m_pos.y + Addmove.y, m_pos.z + Addmove.z },
					{ 0.0f,0.0f,0.0f }, { 1.2f,1.0f,0.0f },
					{ 1.0f,0.3f,0.3f,1.0f }, false, 0.0f, 0.032f, true, CTexture::HitEffect, fAng, true);

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
void C_Magic_Fire::Draw()
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
C_Magic_Fire * C_Magic_Fire::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	C_Magic_Fire *pMagic = new C_Magic_Fire(CScene::OBJTYPE_MAGIC);
	if (pMagic != nullptr)
	{
		pMagic->Init();
		pMagic->m_pos = pos;
		pMagic->m_rot = rot;
		CMagicCircle::Create(pos, { 0.0f,rot.y,0.0f }, FIRE_MAZIC_SIRCLE_RADIUS, 10, true, true, { 1.0f, 0.3f, 0.3f, 1.0f },
			CTexture::MagicCircle_TypeB);

	}
	return pMagic;
}
