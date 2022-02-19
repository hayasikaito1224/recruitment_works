//===============================================-
//CP����X�L���i�\�j�b�N�X�g���C�N�j�̏���
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
#include "sound.h"
#include "circlegauge.h"

#define MAGIC_SKILL01_MAZIC_MAX_TIME (88)
#define MAGIC_SKILL01_MAZIC_INTERVAL (20)
#define MAGIC_SKILL01_POWER (6)
#define MAGIC_SKILL01_SIZE (200)

//========================================---
//�R���X�g���N�^
//========================================
C_Magic_Skill01::C_Magic_Skill01(OBJTYPE nPriority) : C_Magic(nPriority)
{
	//CManager::GetGame()->GetMPGauge()->SetGauge(MAGIC_SKILL01_MP);
	m_nTimer = 0;
	m_nMaxTime = MAGIC_SKILL01_MAZIC_MAX_TIME;
	m_pMagicCircle = nullptr;
}
//==========================================
//�f�X�g���N�^
//==========================================
C_Magic_Skill01::~C_Magic_Skill01()
{

}
//==========================================
//������
//==========================================
HRESULT C_Magic_Skill01::Init()
{
	return S_OK;
}
//==========================================
//�j��
//==========================================
void C_Magic_Skill01::Uninit()
{
	Release();
}
//==========================================
//�X�V
//==========================================
void C_Magic_Skill01::Update()
{
	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	//�v���C���[���U����Ԃɂ�������
	pPlayer->SetbAttack(true);
	pPlayer->SetbSkill(true);

	m_nTimer++;
	//CP��0�ɂ���
	CManager::GetGame()->GetCPGauge()->ResetGauge();
	CScene *pScene_E = CScene::GetScene(OBJTYPE_ENEMY);
	CCollision *pCollision = new CCollision;


	if (m_nTimer >= m_nMaxTime)
	{
		m_nTimer = 0;
		//�X�L���̓������߂�
		m_bUninit = true;
		pPlayer->SetbAttack(false);
		pPlayer->SetbSkill(false);


	}
	else if(m_nTimer % 11 == 0)
	{
		//�G�Ƃ̂̓����蔻��
		while (pScene_E != NULL)
		{
			if (pScene_E != NULL)
			{
				CEnemy *pEnemy = (CEnemy*)pScene_E;
				if (pEnemy->bHitAttack() == false && m_bAttack == false)
				{
					bool bHit = pCollision->CollisionWeapon((CEnemy*)pScene_E, m_pos, MAGIC_SKILL01_SIZE);
					pEnemy->SetHit(bHit);
					if (bHit == true)
					{
						//m_bAttack = true;
						pEnemy->SetbDamage(true);
						pEnemy->AddLife(-MAGIC_SKILL01_POWER, pEnemy->FIRE);
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
						({ pEnemy->GetEnemyPos().x + Addmove.x,pEnemy->GetEnemyPos().y + 20.0f,pEnemy->GetEnemyPos().z + Addmove.z },
						{ 0.0f,0.0f,0.0f }, { 1.2f,1.0f,0.0f },
						{ 1.0f,0.3f,0.3f,1.0f }, false, 0.0f, 0.032f, true, CTexture::HitEffect, fAng, true);

					}
				}
			}
			pScene_E = pScene_E->GetSceneNext(pScene_E);
		}

	}
	else
	{
		pPlayer->SetMotionType(pPlayer->N_SKILL002);
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
void C_Magic_Skill01::Draw()
{
}
//==========================================
//�C���X�^���X����
//==========================================

C_Magic_Skill01 * C_Magic_Skill01::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot)
{
	C_Magic_Skill01 *pSkill = new C_Magic_Skill01(OBJTYPE_EFFECT);
	if (pSkill != nullptr)
	{
		pSkill->Init();
		pSkill->m_pos = pos;
		pSkill->m_rot = rot;
		if (pSkill->m_pMagicCircle == nullptr)
		{
			pSkill->m_pMagicCircle = CMagicCircle::Create(pos, { D3DXToRadian(90.0f),0.0f,0.0f }, 200.0f, MAGIC_SKILL01_MAZIC_MAX_TIME, false, true, { 0.5f,0.5f,0.5f,1.0f }, CTexture::MagicCircle_TypeB);
			pSkill->m_pMagicCircle->SetbEraseStart(true,5.5f);
		}

	}
	return pSkill;
}
