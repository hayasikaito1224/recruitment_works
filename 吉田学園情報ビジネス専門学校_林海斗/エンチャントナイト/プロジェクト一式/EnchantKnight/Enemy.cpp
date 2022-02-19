//-----------------------------
//�G�̏���
//-----------------------------
#include "Enemy.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "layer_structure.h"
#include "game.h"
#include "player.h"
#include "collision.h"
#include "stage.h"
#include "sound.h"
#include "effect.h"
#include "gauge.h"
#include "Particle.h"
#include "wall.h"
#include "map_polygon.h"
#define MIN_MOVE (80.0)		//�G�������ŏ��͈̔�
#define MAX_MOVE (100.0)	//�G�������ő�͈̔�
#define MIN_TIME (180.0)		//�G�������܂ł̎��Ԃ̍ŏ��͈̔�
#define MAX_TIME (240.0)	//�G�������܂ł̎��Ԃ̍ő�͈̔�
#define MAX_SPEED (1.8)
#define KNOCK_BACK_SPEED (3.0)
#define MAX_HIT_TIME (30)//�G�̖��G����
#define MAX_ENEMY_LIFE (10)//�G�̖�
#define MAX_ENEMY_DEFENSE (2)//�G�̖h���
#define MAX_ATTACKSTARTTIME (20)//�U���J�n�܂ł̎���
#define ADDCP (10)		//���G����̎���

CEnemy::CEnemy(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bAIMove = false;
	m_bAIMoveStop = false;
	s_bCntStop = true;
	m_bHit = false;
	m_bInvincible = false;
	m_pPartsModel.clear();
	m_pMotion = nullptr;
	m_nAttackStartCnt = MAX_ATTACKSTARTTIME;
	m_bDeth = false;
	m_bMotionStop = false;
	m_pAttackModel.clear();
	m_pMapPolygon = nullptr;
	m_bMove = false;
}

CEnemy::~CEnemy()
{

}
//----------------------------------
//�C���X�^���X����
//----------------------------------
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pPlayer = NULL;
	pPlayer = new CEnemy(OBJTYPE_ENEMY);
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->Init();
	return pPlayer;
}
//----------------------------------
//������
//----------------------------------
HRESULT CEnemy::Init()
{
	m_nDefense = MAX_ENEMY_DEFENSE;
	m_fWalkSoundInterval = 0.0f;
	m_fSoundInterval = 0.0f;
	m_nLife = MAX_ENEMY_LIFE;
	m_bDamage = false;

	std::mt19937_64 s_mt(s_random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
	std::uniform_real_distribution<> s_randAng(-3.14, 3.14);
	std::uniform_real_distribution<> s_randMove(MIN_MOVE, MAX_MOVE);
	std::uniform_real_distribution<> s_randTime(MIN_TIME, MAX_TIME);
	if (m_pMapPolygon == nullptr)
	{
		m_pMapPolygon = CMap_Polygon::Create(m_pos, D3DXVECTOR3(5.0f, 0.0f, 5.0f), CTexture::Map_Enemy);
		m_pMapPolygon->SetCol({ 1.0f,0.0f,0.0f,1.0f });
		m_pMapPolygon->SetRot(m_rot.y);

	}

	s_MaxTimer = (float)s_randTime(s_mt);
	m_fMovingRange = (float)s_randMove(s_mt);
	m_rot.y = (float)s_randAng(s_mt)+(D3DX_PI);
	s_MoveRandAng = (float)s_randAng(s_mt);
	return S_OK;
}

void CEnemy::Uninit()
{
	// ���f���j��
	for (int nCnt = 0; nCnt < m_pPartsModel.size(); nCnt++)
	{
		if (m_pPartsModel[nCnt] != nullptr)
		{
			m_pPartsModel[nCnt]->Uninit();
			m_pPartsModel[nCnt] = nullptr;
		}
	}
	for (int nCnt = 0; nCnt < m_pAttackModel.size(); nCnt++)
	{
		if (m_pAttackModel[nCnt] != nullptr)
		{
			m_pAttackModel[nCnt]->Uninit();
			m_pAttackModel[nCnt] = nullptr;
		}
	}
	if (m_pMapPolygon != nullptr)
	{
		m_pMapPolygon->Uninit();
		m_pMapPolygon = nullptr;
	}
	Release();
}
//-----------------------------------------
//�X�V
//-----------------------------------------
void CEnemy::Update()
{
	//�O��̈ʒu���̕ۑ�
	m_lastPos = m_pos;

	//�G��AI�ړ�
	//����ł��Ȃ�������s������
	if (m_nLife <= 0)
	{
		m_bDeth = true;
		m_MotionType = DETH;
	}
	if (m_bDeth == false)
	{
		AIBehavior();

		if (m_bDraw == true)
		{
			CCollision *pCollision = new CCollision;
			//�_���[�W���󂯂��疳�G��Ԃɂ���
			if (m_bDamage == true)
			{
				if (m_bEffect == false)
				{
					m_bEffect = true;
				}
				//�J�E���^�[�𑝂₷
				m_nTimer++;
			}
			//�J�E���^�[������̎��Ԃɍs������
			if (m_nTimer >= INVINCIBLE_TIME)
			{
				//�ʏ��Ԃɖ߂�
				m_bDamage = false;
				m_bEffect = false;
			}
			CPlayer *pPlayer = CManager::GetGame()->GetPlayer();

			//�v���C���[�ɑ΂��Ă̓����蔻��
			if (pPlayer != NULL)
			{
				pCollision->CollisionObjectPlayer(pPlayer, &m_pos, &m_lastPos, m_pPartsModel[0]->GetMaxPos().x);
				//�U�����肪�����������ǂ������ׂ�
				if (m_bAttack == true && pPlayer->GetHit() == false)
				{
					Colision();
				}
			}

			//�m�b�N�o�b�N��ԂȂ�
			if (m_bKnockback == true)
			{
				Knockback(pPlayer->GetPos());
			}

			CScene *pEnemy = CScene::GetScene(OBJTYPE_ENEMY);

			CScene *pScene_Wall = CScene::GetScene(OBJTYPE_WALL);
			//�ǂƂ̓����蔻��
			while (pScene_Wall != nullptr)
			{
				CWall *pWall = (CWall*)pScene_Wall;
				CScene *pNext_Wall = CScene::GetSceneNext(pScene_Wall);
				//�����蔻��
				bool bHit = pWall->TestCollision(&m_pos, &m_lastPos, m_pPartsModel[0]->GetMaxPos().x);
				pScene_Wall = pNext_Wall;

			}

			//�G���m�̓����蔻��
			while (pEnemy != NULL)
			{
				if (pEnemy != NULL /*&& pEnemy == this*/)
				{
					pCollision->CollisionObjectEnemy((CEnemy*)pEnemy, &m_pos, &m_lastPos, m_pPartsModel[0]->GetMaxPos().x);
				}
				pEnemy = pEnemy->GetSceneNext(pEnemy);
			}



			//���[�V�����Đ�
			if (m_pMotion != NULL)
			{
				m_pMotion->PlayMotion(m_pPartsModel.size(), &m_pPartsModel[0], m_MotionType, m_MotionLastType, m_bMotionStop, m_bAttack, m_bNeutral, false);
			}

			delete pCollision;
		}

	}
	else
	{
		//���[�V�����Đ�
		if (m_pMotion != NULL)
		{
			m_pMotion->NoLoopPlayMotion(m_pPartsModel.size(), &m_pPartsModel[0], m_MotionType, m_MotionLastType, m_bMotionStop);
		}
		if (m_bMotionStop == true)
		{
			m_bUninit = true;
			//�����鉹
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMY_DEATH);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMY_DEATH,0.5f);
			CManager::GetGame()->GetParticle()->PlayRandomCircle({ m_pos.x,m_pos.y + 30.0f,m_pos.z }, CParticle::HEELEFFECT, CTexture::GlitterEffect);
		}
		//�G�̍s���p�^�[�������\��
		//if (m_bUninit == true)
		//{
		//	Uninit();
		//}
	}
	if (m_pMapPolygon != nullptr)
	{
		m_pMapPolygon->SetPos(m_pos.x, m_pos.z);
		m_pMapPolygon->SetRot(m_rot.y);

	}
}

void CEnemy::Draw()
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRotModel, mtxTransModel;//�v�Z�p�}�g���b�N�X
	//�e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	//�����𔽉f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//�e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	//���f���`��
	for (int nCnt = 0; nCnt < m_pPartsModel.size(); nCnt++)
	{
		if (m_pPartsModel[nCnt] != nullptr)
		{
			m_pPartsModel[nCnt]->SetDraw(m_bDraw);
			m_pPartsModel[nCnt]->Draw();
		}
	}
	for (int nCnt = 0; nCnt < m_pAttackModel.size(); nCnt++)
	{
		if (m_pAttackModel[nCnt] != nullptr)
		{
			m_pAttackModel[nCnt]->SetDraw(false);
			m_pAttackModel[nCnt]->Draw();
		}
	}

}
void CEnemy::Colision()
{
}
void CEnemy::AIAttack()
{
}
void CEnemy::AIMove()
{
	if (m_fPlayerVecLength < MAX_COLRADIUS)
	{
		m_bMove = true;
		//�����œ����ł������I�t�ɂ���
		m_bAIMove = false;
		m_bAIMoveStop = true;

	}
	//�͈͊O�̎�
	else
	{
		m_bMove = false;
		m_bAIMoveStop = false;

		s_bCntStop = true;
	}
	//�����ړ����ł���Ȃ�
	if (m_bMove)
	{
		m_rot.y = m_fAng + (D3DX_PI);
		//���˂�����������߂�
		m_MoveSpeed.x = sinf(m_fAng)*MAX_SPEED;
		m_MoveSpeed.z = cosf(m_fAng)*MAX_SPEED;
		m_MotionType = MOVE;

		m_fWalkSoundInterval -= 0.1f;

		if (m_fWalkSoundInterval <= 0.0f)
		{
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMYWALK);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENEMYWALK, 0.6f);
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
//----------------------------------------------------------
//�̗͂̑���
//----------------------------------------------------------
void CEnemy::AddLife(int nLife)
{
	if (m_bDamage == true)
	{
		float fDamege = nLife+ m_nDefense;
		m_nLife += fDamege;

	}
}
void CEnemy::AddLife(int nPower, int nType)
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

//-----------------------------------------
//���G���Ԃ̏���
//-----------------------------------------

bool CEnemy::bHitAttack()
{
	//�q�b�g���ꂽ�疳�G���ԗp�̔�����I���ɂ���
	if (m_bHit == true)
	{
		m_bInvincible = true;
		m_bHit = false;
		//�m�b�N�o�b�N��Ԃɂ���
		m_bKnockback = true;
	}
	//�q�b�g���Ă�����U��������Ȃ��悤�ɂ���
	if (m_bInvincible == true)
	{
		m_MotionType = DETH;
		m_bAttack = false;
		m_nCntHitInterval++;
		if (m_nCntHitInterval >= MAX_HIT_TIME)
		{
			m_nCntHitInterval = 0;

			m_bInvincible = false;
			m_bKnockback = false;


		}
	}
	return m_bInvincible;
}
//-----------------------------------------
//�m�b�N�o�b�N�̏���
//-----------------------------------------
void CEnemy::Knockback(D3DXVECTOR3& Playerpos)
{
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = Playerpos - m_pos;			//�G�ƒe�̃x�N�g��
	m_fAng = atan2(PlayerVec.x, PlayerVec.z);//�G�̌���
	//�����o�����������߂�
	float fSpeed_x = sinf(m_fAng)*KNOCK_BACK_SPEED;
	float fSpeed_z = cosf(m_fAng)*KNOCK_BACK_SPEED;
	//�v���C���[�𓮂���
	m_pos.x -= fSpeed_x;
	m_pos.z -= fSpeed_z;

}


bool  CEnemy::bColision()
{
	return 0;
}
//-----------------------------------------
//AI�̏���
//-----------------------------------------

void CEnemy::AIBehavior()
{
	D3DXVECTOR3	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos = CManager::GetGame()->GetPlayer()->GetPos();				//�ʒu
	D3DXVECTOR3 PlayerVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerVec = pos - m_pos;			//�G�ƒe�̃x�N�g��
	float fLengthX = 0.0f;
	m_fPlayerVecLength = sqrtf((PlayerVec.z*PlayerVec.z) + (PlayerVec.x*PlayerVec.x));
	m_fAng = atan2(PlayerVec.x, PlayerVec.z);//�G�̌���
	float fAngle = atan2(PlayerVec.x, PlayerVec.z);//�G�̌���
	//�G�̏o��
	if (m_fPlayerVecLength < MAX_DRAWRADIUS)
	{
		m_bDraw = true;
	}
	else
	{
		m_bDraw = false;
	}

	//�G�̒Ǐ]
	if (m_bAttack == false)
	{
		AIMove();
	}
	//�v���C���[���U���͈͂ɓ�������
	AIAttack();

	if (m_bAIMoveStop == false && m_bDraw == true)
	{
		//�G�������œ������Ԃ�������
		if (m_bAIMove == true)
		{
			m_MotionType = MOVE;

			//�ǂ��܂œ������J�E���g����
			s_MovingCnt += MAX_SPEED;
			m_rot.y = s_MoveRandAng + (D3DX_PI);

			//�����o�����������߂�
			m_MoveSpeed.x = sinf(s_MoveRandAng)*MAX_SPEED;
			m_MoveSpeed.z = cosf(s_MoveRandAng)*MAX_SPEED;
			m_pos += m_MoveSpeed;

		}
		else
		{
			s_CntTimer += 1.0f;
			//�J�E���^�[���w��͈̔͂܂ŃJ�E���g���ꂽ��G������
			if (s_CntTimer >= s_MaxTimer&&s_bCntStop == true)
			{
				s_CntTimer = 0.0f;
				//�G�������œ������߂̔�����I���ɂ���
				m_bAIMove = true;

				static std::random_device s_random;	// �񌈒�I�ȗ���������
				std::mt19937_64 s_mt(s_random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
				std::uniform_real_distribution<> s_randMove(MIN_MOVE, MAX_MOVE);

				m_fMovingRange = (float)s_randMove(s_mt);
				//������ݒ�
				m_rot.y = s_MoveRandAng + (D3DX_PI);
				s_bCntStop = false;
			}

			s_MovingCnt = 0.0f;
			m_MoveSpeed.x = 0.0f;
			m_MoveSpeed.z = 0.0f;

		}
		//�ړ�����w��͈̔͂ɗ�����r
		if (s_MovingCnt > m_fMovingRange)
		{
			static std::random_device s_random;	// �񌈒�I�ȗ���������

			std::mt19937_64 s_mt(s_random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
			std::uniform_real_distribution<> s_randAng(-3.14, 3.14);
			std::uniform_real_distribution<> s_randTime(MIN_TIME, MAX_TIME);

			s_MoveRandAng = (float)s_randAng(s_mt);
			s_MaxTimer = (float)s_randTime(s_mt);
			m_fMovingRange = 0.0f;
			s_MovingCnt = 0.0f;
			//�J�E���g���O�ɂ��Ă܂������n�߂�
			s_bCntStop = true;
			s_CntTimer = 0.0f;
			m_MoveSpeed.x = 0.0f;
			m_MoveSpeed.z = 0.0f;
			m_bAIMove = false;
		}

	}
	//�G������ɓ�������

}
