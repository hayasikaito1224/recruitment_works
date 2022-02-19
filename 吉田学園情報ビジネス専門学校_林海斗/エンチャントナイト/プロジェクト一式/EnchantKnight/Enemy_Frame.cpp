#include "Enemy_Frame.h"
#include "layer_structure.h"
#include "motion.h"
#include "collision.h"
#include "game.h"
#include "gauge.h"
#include "sound.h"
#include "effect.h"
#include "manager.h"
#include "player.h"
#include "enemy_magic_fire.h"
#include "MagicCircle.h"
#define FRAME_LIFE (15)//�G�̖�
//#define MAX_ENEMY_DEFENSE (2)//�G�̖h���
#define FRAME_POWER (10)//�U����
#define FRAME_ATTACKSTART_RADIUS (550.0f)//�U���J�n�͈�
#define FRAME_ATTACKEND_RADIUS (550.0f)//�U���J�n�͈�
#define FRAME_MOVERANGE (700.0f)//�ǐՂ���͈�
#define FRAME_ATTACKTIME (30)//�U�����o���܂ł̎���

#define FRAME_ATTACKSTARTTIME (90)//�U���J�n�܂ł̎���
#define FRAME_SPEED (2.0f)
#define FRAME_ADDPOS (-80.0f)

#define FRAME_BREAKTIME (90)//�N�[���^�C��
#define ENEMY_MAGIC_FIRE_MAZIC_SIRCLE_RADIUS (70.0)


CEnemy_Frame::CEnemy_Frame(OBJTYPE nPriority) : CEnemy(nPriority)
{
	m_nPower = FRAME_POWER;
	m_nLife = FRAME_LIFE;
	m_bBreakTime = false;
}

CEnemy_Frame::~CEnemy_Frame()
{

}

CEnemy_Frame *CEnemy_Frame::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy_Frame *pFrame = NULL;
	pFrame = new CEnemy_Frame(OBJTYPE_ENEMY);
	pFrame->m_pos = pos;
	pFrame->m_rot = rot;
	pFrame->Init();
	return pFrame;
}

HRESULT CEnemy_Frame::Init()
{
	CEnemy::Init();
	m_nType = NONE;
	//�t�@�C���ǂݍ���
	CLayer_Structure *pLayer = NULL;
	pLayer = new CLayer_Structure;
	pLayer->SetLayer_Structure("data/TEXT/ENEMYFRAME/ModelParts.txt", m_pPartsModel,CModel::TYPE_FRAMEENEMY);

	if (m_pMotion == NULL)
	{
		m_pMotion = new CMotion;
		m_MotionType = 0;
		m_pMotion->MotionLoad("data/TEXT/ENEMYFRAME/motion.txt");

	}

	return S_OK;
}

void CEnemy_Frame::Uninit()
{
	CEnemy::Uninit();
	// ���f���j��

	Release();
}

void CEnemy_Frame::Update()
{
	CEnemy::Update();


}

void CEnemy_Frame::Draw()
{
	CEnemy::Draw();
}

void CEnemy_Frame::Colision()
{
}

void CEnemy_Frame::AIAttack()
{
	//�v���C���[���U���͈͂ɓ�������
	if (m_fPlayerVecLength < FRAME_ATTACKSTART_RADIUS)
	{

		if (m_bAttack == false&& m_bBreakTime==false)
		{
			m_nAttackStartCnt++;
			m_MotionType = ATTACKWAIT;

			//�U���J�n���ԂɂȂ�����
			if (m_nAttackStartCnt >= FRAME_ATTACKSTARTTIME)
			{
				m_nAttackStartCnt = 0;
				m_MotionType = ATTACK;
				m_bAttack = true;
				D3DXVECTOR3 posAdd;
				posAdd.x = m_pos.x + (sinf(m_rot.y)*FRAME_ADDPOS);
				posAdd.z = m_pos.z + (cosf(m_rot.y)*FRAME_ADDPOS);
				posAdd.y = m_pos.y + 40.0f;

				CMagicCircle::Create(posAdd, m_rot, ENEMY_MAGIC_FIRE_MAZIC_SIRCLE_RADIUS, FRAME_ATTACKTIME, true, true, { 1.0f, 0.3f, 0.3f, 1.0f },
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
					m_pos.x+(sinf(fAng)*sinf(fAng2)*FRAME_ADDPOS),
					m_pos.y+(cosf(fAng)*FRAME_ADDPOS),
					m_pos.z+(sinf(fAng)*cosf(fAng2)*FRAME_ADDPOS )};
				D3DXVECTOR3 vec = m_pos - posAdd;
				float frot = atan2f(vec.x, vec.z);
				float frotY = atan2f(vec.x, vec.y);

				D3DXVECTOR3 move ={
					sinf(frot)*FRAME_SPEED,
					cosf(frot)*FRAME_SPEED,
					cosf(frot)*FRAME_SPEED
									};

				CEffect::Create(posAdd, move, { 2.0f,2.0f,0.0f }, { 1.0,0.3f,0.0,1.0 }, false, 0.0f, 0.03f, true, CTexture::Effect,
					0.0f, false, false);
				CEffect::Create(posAdd, move, { 2.0f,2.0f,0.0f }, { 1.0,0.3f,0.0,1.0 }, false, 0.0f, 0.03f, true, CTexture::Effect,
					0.0f, false, false);

			}
			else if (m_nAttackStartCnt % 30 == 0)
			{
				CEffect::Create({ m_pos.x,m_pos.y + 50.0f,m_pos.z }, { 0.0f,0.0f,0.0f }, { 23.0f,23.0f,0.0f }, { 1.0,0.3f,0.0,1.0 }, false, 0.0f, 0.01f, true, CTexture::RingEffect,
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
		if (m_nTimer >= FRAME_ATTACKTIME)
		{
			m_bAttack = false;
			m_nTimer = 0;
			D3DXVECTOR3 posAdd;
			posAdd.x = m_pos.x + (sinf(m_rot.y)*FRAME_ADDPOS);
			posAdd.z = m_pos.z + (cosf(m_rot.y)*FRAME_ADDPOS);
			posAdd.y = m_pos.y + 40.0f;
			//�U����
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_FIRE, 0.7f);


			//���̋����o��
			C_Enemy_Magic_Fire::Create(posAdd, m_rot, false);
			m_bBreakTime = true;
		}
	}
	//�U���N�[���^�C�����I����������
	if (m_bBreakTime)
	{
		m_nBreakTime++;
		if (m_nBreakTime >= FRAME_BREAKTIME)
		{
			m_nBreakTime = 0;
			m_bBreakTime = false;
		}
	}
}

void CEnemy_Frame::AIMove()
{
	//�v���C���[�����͈͋߂Â�����
	if (m_fPlayerVecLength < FRAME_MOVERANGE)
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
	if (m_fPlayerVecLength < FRAME_ATTACKSTART_RADIUS)
	{
		m_bMove = false;
	}
	//�����ړ����ł���Ȃ�
	if (m_bMove)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		//���˂�����������߂�
		m_MoveSpeed.x = sinf(m_fAng)*FRAME_SPEED;
		m_MoveSpeed.z = cosf(m_fAng)*FRAME_SPEED;
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
