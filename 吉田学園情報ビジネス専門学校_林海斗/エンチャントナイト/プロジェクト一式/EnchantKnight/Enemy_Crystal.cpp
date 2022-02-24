#include "Enemy_Crystal.h"
#include "layer_structure.h"
#include "motion.h"
#include "collision.h"
#include "game.h"
#include "gauge.h"
#include "sound.h"
#include "effect.h"
#include "manager.h"
#include "player.h"
#include "enemy_magic_ice.h"
#include "MagicCircle.h"
#define CRYSTAL_LIFE (15)//�G�̖�
//#define MAX_ENEMY_DEFENSE (2)//�G�̖h���
#define CRYSTAL_POWER (10)//�U����
#define CRYSTAL_ATTACKSTART_RADIUS (550.0f)//�U���J�n�͈�
#define CRYSTAL_ATTACKEND_RADIUS (550.0f)//�U���J�n�͈�
#define CRYSTAL_MOVERANGE (700.0f)//�ǐՂ���͈�
#define CRYSTAL_ATTACKTIME (30)//�U�����o���܂ł̎���

#define CRYSTAL_ATTACKSTARTTIME (90)//�U���J�n�܂ł̎���
#define CRYSTAL_SPEED (2.0f)
#define CRYSTAL_ADDPOS (-80.0f)

#define CRYSTAL_BREAKTIME (90)//�N�[���^�C��
#define ENEMY_MAGIC_FIRE_MAZIC_SIRCLE_RADIUS (70.0)


CEnemy_Crystal::CEnemy_Crystal(OBJTYPE nPriority) : CEnemy(nPriority)
{
	m_nPower = CRYSTAL_POWER;
	m_nLife = CRYSTAL_LIFE;
	m_bBreakTime = false;
	m_col = { 0.0f,0.3f,1.0f,1.0f };
}

CEnemy_Crystal::~CEnemy_Crystal()
{

}

CEnemy_Crystal *CEnemy_Crystal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy_Crystal *pCrystal = NULL;
	pCrystal = new CEnemy_Crystal(OBJTYPE_ENEMY);
	pCrystal->m_pos = pos;
	pCrystal->m_rot = rot;
	pCrystal->Init();
	return pCrystal;
}

HRESULT CEnemy_Crystal::Init()
{
	CEnemy::Init();
	m_nType = NONE;
	//�t�@�C���ǂݍ���
	CLayer_Structure *pLayer = NULL;
	pLayer = new CLayer_Structure;
	pLayer->SetLayer_Structure("data/TEXT/CRYSTALENEMY/ModelParts.txt", m_pPartsModel,CModel::TYPE_CRYSTALENEMY);

	if (m_pMotion == NULL)
	{
		m_pMotion = new CMotion;
		m_MotionType = 0;
		m_pMotion->MotionLoad("data/TEXT/CRYSTALENEMY/motion.txt");

	}

	return S_OK;
}

void CEnemy_Crystal::Uninit()
{
	CEnemy::Uninit();
	// ���f���j��

	Release();
}

void CEnemy_Crystal::Update()
{
	CEnemy::Update();


}

void CEnemy_Crystal::Draw()
{
	CEnemy::Draw();
}

void CEnemy_Crystal::Colision()
{
}

void CEnemy_Crystal::AIAttack()
{
	//�v���C���[���U���͈͂ɓ�������
	if (m_fPlayerVecLength < CRYSTAL_ATTACKSTART_RADIUS)
	{

		if (m_bAttack == false&& m_bBreakTime==false)
		{
			m_nAttackStartCnt++;
			m_MotionType = ATTACKWAIT;

			//�U���J�n���ԂɂȂ�����
			if (m_nAttackStartCnt >= CRYSTAL_ATTACKSTARTTIME)
			{
				m_nAttackStartCnt = 0;
				m_MotionType = ATTACK;
				m_bAttack = true;
				D3DXVECTOR3 posAdd;
				posAdd.x = m_pos.x + (sinf(m_rot.y)*CRYSTAL_ADDPOS);
				posAdd.z = m_pos.z + (cosf(m_rot.y)*CRYSTAL_ADDPOS);
				posAdd.y = m_pos.y + 40.0f;

				CMagicCircle::Create(posAdd, m_rot, ENEMY_MAGIC_FIRE_MAZIC_SIRCLE_RADIUS, CRYSTAL_ATTACKTIME, true, true, m_col,
					CTexture::MagicCircle_TypeB);
			}
			if(m_nAttackStartCnt % 4 == 0)
			{
				std::random_device random;	// �񌈒�I�ȗ���������
				std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
				std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);

				float fAng = (float)randAng(mt);
				float fAng2 = (float)randAng(mt);

				D3DXVECTOR3 posAdd = {
					m_pos.x+(sinf(fAng)*sinf(fAng2)*CRYSTAL_ADDPOS),
					m_pos.y+(cosf(fAng)*CRYSTAL_ADDPOS),
					m_pos.z+(sinf(fAng)*cosf(fAng2)*CRYSTAL_ADDPOS )};
				D3DXVECTOR3 vec = m_pos - posAdd;
				float frot = atan2f(vec.x, vec.z);
				float frotY = atan2f(vec.x, vec.y);

				D3DXVECTOR3 move ={
					sinf(frot)*CRYSTAL_SPEED,
					cosf(frot)*CRYSTAL_SPEED,
					cosf(frot)*CRYSTAL_SPEED
									};

				CEffect::Create(posAdd, move, { 2.0f,2.0f,0.0f }, m_col, false, 0.0f, 0.03f, true, CTexture::Effect,
					0.0f, false, false);

			}
			else if (m_nAttackStartCnt % 30 == 0)
			{
				CEffect::Create({ m_pos.x,m_pos.y + 50.0f,m_pos.z }, { 0.0f,0.0f,0.0f }, { 23.0f,23.0f,0.0f }, m_col, false, 0.0f, 0.01f, true, CTexture::RingEffect,
					0.0f, false, false, true);

			}
		}
	}
	else
	{
		m_nAttackStartCnt = 0;

	}
	//�U�����肪�I���̎�
	if (m_bAttack)
	{
		m_nTimer++;
		if (m_nTimer >= CRYSTAL_ATTACKTIME)
		{
			m_bAttack = false;
			m_nTimer = 0;
			D3DXVECTOR3 posAdd;
			posAdd.x = m_pos.x + (sinf(m_rot.y)*CRYSTAL_ADDPOS);
			posAdd.z = m_pos.z + (cosf(m_rot.y)*CRYSTAL_ADDPOS);
			posAdd.y = m_pos.y + 40.0f;
			//�U����
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ICE);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ICE, 0.7f);


			//�X�̋����o��
			C_Enemy_Magic_Ice::Create(posAdd, m_rot);
			m_bBreakTime = true;
		}
	}
	//�U���N�[���^�C�����I����������
	if (m_bBreakTime)
	{
		m_nBreakTime++;
		if (m_nBreakTime >= CRYSTAL_BREAKTIME)
		{
			m_nBreakTime = 0;
			m_bBreakTime = false;
		}
	}
}

void CEnemy_Crystal::AIMove()
{
	//�v���C���[�����͈͋߂Â�����
	if (m_fPlayerVecLength < CRYSTAL_MOVERANGE)
	{
		//��������
		m_bDiscovery = true;
		//�����œ����ł������I�t�ɂ���
		m_bAIMove = false;
		m_bAIMoveStop = true;

	}
	//�͈͊O�̎�
	else
	{
		//������
		m_bDiscovery = false;
		m_bAIMoveStop = false;
		s_bCntStop = true;
	}

	//�G���v���C���[�𔭌������Ȃ�
	if (m_bDiscovery)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		m_bMove = true;
	}
	else
	{
		m_bMove = false;

	}
	//�v���C���[�����͈͋߂Â�����
	if (m_fPlayerVecLength < CRYSTAL_ATTACKSTART_RADIUS)
	{
		m_bMove = false;
	}
	//�����ړ����ł���Ȃ�
	if (m_bMove)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		//���˂�����������߂�
		m_MoveSpeed.x = sinf(m_fAng)*CRYSTAL_SPEED;
		m_MoveSpeed.z = cosf(m_fAng)*CRYSTAL_SPEED;
		m_MotionType = MOVE;

		m_fWalkSoundInterval -= 0.1f;

		if (m_fWalkSoundInterval <= 0.0f)
		{
			//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMYWALK);
			//CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMYWALK, 0.6f);
			m_fWalkSoundInterval = 2.3f;
		}
		m_pos += m_MoveSpeed;

	}
	else
	{
		m_MotionType = NEUTRAL;
		m_MoveSpeed.x = 0.0f;
		m_MoveSpeed.y = 0.0f;
		m_MoveSpeed.z = 0.0f;
	}

}

void CEnemy_Crystal::AddLife(int nPower, int nType)
{
	if (m_bDamage == true)
	{
		float fDamege = 0;
		switch (nType)
		{
		case FIRE:
			fDamege = (nPower + m_nDefense)*1.5f;

			break;
		case BULLIZAD:
			fDamege = nPower + m_nDefense;

			break;
		}
		m_nLife += fDamege;

	}

}
//----------------------------------------------------------
//�̗͂̑���
//----------------------------------------------------------
void CEnemy_Crystal::AddLife(int nLife)
{
	if (m_bDamage == true && m_bHitCollision == true)
	{
		float fDamege = nLife + m_nDefense;
		m_nLife += fDamege;

	}
}
