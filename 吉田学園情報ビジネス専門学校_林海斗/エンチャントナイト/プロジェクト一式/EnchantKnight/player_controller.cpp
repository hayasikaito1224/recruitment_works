//-----------------------------------------
//�v���C���[�̑���
//-----------------------------------------
#include "player_controller.h"
#include "camera.h"
#include "manager.h"
#include "keyboard.h"
#include "game.h"
#include "Renderer.h"
#include "XInput.h"
#include "directinput.h"
#include "player.h"
#include "commandUI.h"
#include "Enemy.h"
#include "scene.h"
#include "billboard.h"
#include "magicUI.h"
#include "gauge.h"
#include "magic_ice.h"
#include "magic_heel.h"
#include "magic_fire.h"
#include "sound.h"

static float s_fCntAngle = 0.0f;//�p�x�̃J�E���g
//--------------------------
//�R���X�g���N�g
//----------------------------
CPlayer_Controller::CPlayer_Controller()
{
	m_fGravitySpeed = 0.5f;
	m_nCommandType = CCommandUI::Co_Attack;
	m_fGravity = 0.0f;
	m_bPorpose = false;
	m_bCrossBoLock = false;
	m_bCrossDownLock = true;
	m_bCrossUpLock = true;
	m_bJumpLock = false;
	m_bRockOn = false;
	m_pNearEnemy = nullptr;
	m_bCommandMagic = false;
	m_nMagicCommandType = 0;
}
//--------------------------
//�f�X�g���N�g
//----------------------------
CPlayer_Controller::~CPlayer_Controller()
{
}
//============================================
//�e�L�X�g�`��
//============================================
void CPlayer_Controller::Drawtext(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[1200];
	int nNum = 0;
	CXInput *pXInput = CManager::GetXInput();
	CDirectInput *pGamePad = CManager::GetDirectInput();
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//�J�����̎擾
	CCamera *pCamera = CRenderer::GetCamera();
	//�R�}���h�̏��̎擾
	CCommandUI *pCommand[CCommandUI::Co_Max] = { 0 };


	nNum = sprintf(&str[0], "\n\n ���X�e�B�b�NX %d  \n", GamePad.lX);
	nNum += sprintf(&str[nNum], " ���X�e�B�b�NY %d  \n", GamePad.lY);
	nNum += sprintf(&str[nNum], " ���X�e�B�b�NZ %d  \n", GamePad.lZ);
	nNum += sprintf(&str[nNum], " lRx %d  \n", GamePad.lRx);
	nNum += sprintf(&str[nNum], " lRy %d  \n", GamePad.lRy);
	nNum += sprintf(&str[nNum], " lRz %d  \n", GamePad.lRz);
	nNum += sprintf(&str[nNum], " �J�����̌��� %.5f,%.5f,%.5f  \n", pCamera->GetRot().x, pCamera->GetRot().y, pCamera->GetRot().z);
	nNum += sprintf(&str[nNum], " ���_ %.5f,%.5f,%.5f  \n", pCamera->GetPosV().x, pCamera->GetPosV().y, pCamera->GetPosV().z);
	nNum += sprintf(&str[nNum], " �����_ %.5f,%.5f,%.5f  \n", pCamera->GetPosR().x, pCamera->GetPosR().y, pCamera->GetPosR().z);

	for (int nCnt = 0; nCnt <  CCommandUI::Co_Max; nCnt++)
	{
		nNum += sprintf(&str[nNum], " �R�}���h%d Pos X:%.2f  Y:%.2f  Z:%.2f  \n", nCnt, pCommand[nCnt]->GetPos().x, pCommand[nCnt]->GetPos().y, pCommand[nCnt]->GetPos().z);

	}
	nNum += sprintf(&str[nNum], " �R�}���h%d  \n", m_nCommandType);

	LPD3DXFONT pFont = CManager::GetRenderer()->GetFont();
	// �e�L�X�g�`��
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
//---------------------------------------
//�����ǂ��w�肵�Ă��̕����Ɍ�������֐�
//---------------------------------------
void CPlayer_Controller::Rotate(float& rot_y, float fAngle)
{
	m_rot_y = rot_y;
	m_fAngle = fAngle;

	//���݂̊p�x����ړI�̊p�x�ւ̍�
	//�J�����̎擾
	CCamera *pCamera = CRenderer::GetCamera();
	float fAngleSense = 0.0f;
	float fCurrentAngle = (D3DXToDegree(pCamera->GetRot().y) + D3DXToDegree(rot_y));
	fAngleSense = fAngle - fCurrentAngle;
	if (fAngleSense > 180.0f)
	{
		fAngleSense = fAngleSense - 360.0f;
	}
	m_fAngleScece = fAngleSense;

	//���l�̐��K���̕��@
	//fAngleSense = fabsf(fAngleSense);
	//�ړI�̊p�x�����̐���������J�E���g���v���X�ɂ���
	if (fAngleSense >= 0.0f)
	{
		s_fCntAngle += 1.0f;
		//�ړI�̊p�x�ɍs������~�߂�
		if (s_fCntAngle >= fAngleSense)
		{
			s_fCntAngle = 0;
		}
	}
	//�ړI�̊p�x�����̐���������J�E���g���}�C�i�X�ɂ���
	else if(fAngleSense <= 0.0f)
	{
		s_fCntAngle -= 1.0f;
		//�ړI�̊p�x�ɍs������~�߂�
		if (s_fCntAngle <= fAngleSense)
		{
			s_fCntAngle = 0;
		}
	}
	//�p�x���v���C���[�̊p�x�ɉ��Z
	rot_y += D3DXToRadian(s_fCntAngle);
	m_LastAngle = fAngle;

}
//===================================================
//�R�}���h�I������
//===================================================
int CPlayer_Controller::CommandSelect()
{
	////�R�}���h�̏��̎擾
	//CCommandUI *pCommand[CCommandUI::Co_Max] = { 0 };
	//for (int nCnt = 0; nCnt < CCommandUI::Co_Max; nCnt++)
	//{
	//	pCommand[nCnt] = CGame::GetCommand(nCnt);
	//}
	////DirectInput�̃Q�[���p�b�h�̎擾
	//CDirectInput *pGamePad = CManager::GetDirectInput();
	////�Q�[���p�b�h�̃{�^�����̎擾
	//DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	////�\���L�[������Ă�����
	//if (GamePad.rgdwPOV[0] == -1)
	//{
	//	m_bCrossDownLock = false;
	//	m_bCrossUpLock = false;
	//}
	////else
	////{
	////	m_bCrossBoLock = false;
	////}
	////�R�}���h�I��
	////��
	//if (GamePad.rgdwPOV[0] == CDirectInput::UP && m_bCrossUpLock == false)
	//{
	//	m_nCommandType--;
	//	if (m_nCommandType < 0)
	//	{
	//		m_nCommandType = (int)CCommandUI::Co_Enhance;
	//	}
	//	m_bCrossUpLock = true;
	//}
	////��
	//else if (GamePad.rgdwPOV[0] == CDirectInput::DOWN && m_bCrossDownLock == false)
	//{

	//	m_nCommandType++;
	//	if (m_nCommandType >= (int)CCommandUI::Co_Max)
	//	{
	//		m_nCommandType = (int)CCommandUI::Co_Attack;
	//	}
	//	m_bCrossDownLock = true;
	//}
	////�I�����Ă���R�}���h�ȊO�͌��̈ʒu�ɖ߂�
	//for (int nCnt = 0; nCnt < CCommandUI::Co_Max; nCnt++)
	//{
	//	if (nCnt != m_nCommandType)
	//	{
	//		pCommand[nCnt]->SetSelect(false);
	//		if (pGamePad->GetGamepadPress(CDirectInput::B) == false|| nCnt != m_nCommandType)
	//		{
	//			pCommand[nCnt]->SetPush(false);
	//		}
	//	}
	//	else
	//	{
	//		//�R�}���h��I��������Ԃɂ���
	//		pCommand[nCnt]->SetSelect(true);
	//		if (pGamePad->GetGamepadPress(CDirectInput::B) == true)
	//		{
	//			//�R�}���h����������Ԃɂ���
	//			pCommand[nCnt]->SetPush(true);
	//			//�܂ق��R�}���h��I��������
	//			if (m_nCommandType == (int)CCommandUI::Co_Magic)
	//			{
	//				//�܂ق��R�}���h���o�������锻��ɂ���
	//				m_bCommandMagic = true;
	//			}

	//		}
	//		else
	//		{
	//			//�R�}���h���������Ԃɂ���
	//			pCommand[nCnt]->SetPush(false);

	//		}
	//	}
	//}
	return m_nCommandType;
}
//===================================================
//���@�̃R�}���h�I������
//===================================================
int CPlayer_Controller::MagicCommandSelect()
{
	//�R�}���h�̏��̎擾
	CMagicUI *pCommand[CMagicUI::Magic_Max] = { 0 };
	for (int nCnt = 0; nCnt < CMagicUI::Magic_Max; nCnt++)
	{
		pCommand[nCnt] = CGame::GetMagicCommand(nCnt);
	}
	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//�\���L�[������Ă�����
	if (GamePad.rgdwPOV[0] == -1)
	{
		m_bCrossDownLock = false;
		m_bCrossUpLock = false;
	}

	//�R�}���h�I��
	//��
	if (GamePad.rgdwPOV[0] == CDirectInput::UP && m_bCrossUpLock == false)
	{
		m_nMagicCommandType--;
		if (m_nMagicCommandType < 0)
		{
			m_nMagicCommandType = (int)CMagicUI::Heal;
		}
		m_bCrossUpLock = true;
	}
	//��
	else if (GamePad.rgdwPOV[0] == CDirectInput::DOWN && m_bCrossDownLock == false)
	{

		m_nMagicCommandType++;
		if (m_nMagicCommandType >= (int)CMagicUI::Magic_Max)
		{
			m_nMagicCommandType = (int)CMagicUI::Fire;
		}
		m_bCrossDownLock = true;
	}
	//�I�����Ă���R�}���h�ȊO�͌��̈ʒu�ɖ߂�
	for (int nCnt = 0; nCnt < CMagicUI::Magic_Max; nCnt++)
	{
		if (nCnt != m_nMagicCommandType)
		{
			pCommand[nCnt]->SetSelect(false);
			if (pGamePad->GetGamepadPress(CDirectInput::B) == false || nCnt != m_nMagicCommandType)
			{
				pCommand[nCnt]->SetPush(false);
			}
		}
		else
		{
			//�R�}���h��I��������Ԃɂ���
			pCommand[nCnt]->SetSelect(true);
			if (pGamePad->GetGamepadPress(CDirectInput::B) == true)
			{
				//�R�}���h����������Ԃɂ���
				pCommand[nCnt]->SetPush(true);

			}
			else
			{
				//�R�}���h���������Ԃɂ���
				pCommand[nCnt]->SetPush(false);

			}
		}
	}
	//�L�����Z���{�^������������܂ق��R�}���h��������悤�ɂ���
	if (pGamePad->GetButtonTrigger(CDirectInput::A) == true)
	{
		m_bCommandMagic = false;
	}
	return m_nMagicCommandType;

}
//===================================================
//�}�W�b�N�U������
//===================================================
int CPlayer_Controller::MagicAttack(D3DXVECTOR3 & pos, float& frotY, int& nMotionType, bool& bMagic, const bool& bNearEnemy, const int& nMagicType)
{
	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//�U������ɕς���
	if (pGamePad->GetGamepadPress(CDirectInput::L) == true)
	{

		//�R�}���h�̏��̎擾
		CMagicUI *pCommand[CMagicUI::Magic_Max] = { 0 };
		for (int nCnt = 0; nCnt < CMagicUI::Magic_Max; nCnt++)
		{
			pCommand[nCnt] = CGame::GetMagicCommand(nCnt);
		}


		//�΂̖��@
		if (pGamePad->GetButtonTrigger(CDirectInput::B) == true && bMagic == false)
		{
			m_nMagicCommandType = (int)CMagicUI::Fire;
			//�v���C���[��G�̕����Ɍ�������
			PlayerFace(pos, frotY, bNearEnemy);
			nMotionType = CPlayer::N_MAGIC_ATTACK;
			bMagic = true;

			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

		}
		//�X�̖��@
		if (pGamePad->GetButtonTrigger(CDirectInput::X) == true && bMagic == false)
		{
			m_nMagicCommandType = (int)CMagicUI::Ice;

			//�v���C���[��G�̕����Ɍ�������
			PlayerFace(pos, frotY, bNearEnemy);
			nMotionType = CPlayer::N_MAGIC_ATTACK;
			bMagic = true;
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

		}
		//�񕜖��@
		if (pGamePad->GetButtonTrigger(CDirectInput::A) == true && bMagic == false)
		{
			m_nMagicCommandType = (int)CMagicUI::Heal;
			//�v���C���[��G�̕����Ɍ�������
			PlayerFace(pos, frotY, bNearEnemy);
			nMotionType = CPlayer::N_MAGIC_ATTACK;
			bMagic = true;
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

		}
		//�I�������R�}���h����������Ԃɂ���
		if (pGamePad->GetGamepadPress(CDirectInput::B) ||
			pGamePad->GetGamepadPress(CDirectInput::X) ||
			pGamePad->GetGamepadPress(CDirectInput::A))
		{
			m_bPush = true;

		}
		else
		{
			m_bPush = false;
		}
		if (m_bPush)
		{
			pCommand[m_nMagicCommandType]->SetPush(true);
		}


	}
	return m_nMagicCommandType;

}
//----------------------------
//���b�N�I���̏���
//----------------------------
bool CPlayer_Controller::RockOn(D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//���b�N�I�������ς���
	if (pGamePad->GetButtonTrigger(CDirectInput::R) == true)
	{
		CScene *pScene_E = CScene::GetScene(CScene::OBJTYPE_ENEMY);//��ԍŏ��̓G
		if (pScene_E != nullptr)
		{
			m_bRockOn = m_bRockOn ? false : true;

		}
		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

	}

	//���b�N�I�����Ă��Ȃ��������ԋ߂��G�����߂�
	if (m_bRockOn==false)
	{
		CScene *pScene_E = CScene::GetScene(CScene::OBJTYPE_ENEMY);//��ԍŏ��̓G
		float fLength = 0.0f;//�v���C���[�ƓG�̊Ԃ̋���
		float fLength2 = 0.0f;//�v���C���[�ƓG�̊Ԃ̋���

		//��ԋ߂��G�̏��̕ۑ��p
		m_pNearEnemy = (CEnemy*)pScene_E; 
		//��ԋ߂��G�����߂�
		while (pScene_E != NULL)
		{
			//���̓G�̋����̒��������߂�

			if (m_pNearEnemy != NULL)
			{
				D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
				D3DXVECTOR3 EnemyVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				EnemyVec = EnemyPos - pos;			//�G�ƃv���C���[�̃x�N�g��
				fLength = sqrtf((EnemyVec.z*EnemyVec.z) + (EnemyVec.x*EnemyVec.x));

			}
			if (pScene_E != NULL)
			{
				//���̓G�̋����̒��������߂�
				CScene *pSceneNext = pScene_E->GetSceneNext(pScene_E);
				if (pSceneNext != NULL)
				{
					CEnemy *pEnemyNext = (CEnemy*)pSceneNext;
					D3DXVECTOR3 EnemyPosNext = pEnemyNext->GetEnemyPos();
					D3DXVECTOR3 EnemyVecNext = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					EnemyVecNext = EnemyPosNext - pos;			//�G�ƃv���C���[�̃x�N�g��
					fLength2 = sqrtf((EnemyVecNext.z*EnemyVecNext.z) + (EnemyVecNext.x*EnemyVecNext.x));

					//���̓G�����̓G��艓��������
					if (fLength > fLength2)
					{
						m_pNearEnemy = pEnemyNext;
					}
				}
			}
			pScene_E = pScene_E->GetSceneNext(pScene_E);
		}
		if (m_pNearEnemy == nullptr)
		{
			return false;
		}

		if (m_pNearEnemy->GetModel(0) == nullptr)
		{
			m_pNearEnemy = nullptr;
		}
	}
	else
	{
		if (m_pNearEnemy == nullptr)
		{
			return false;
		}
		else
		{
			D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
			m_NearEnemyVec = EnemyPos - pos;			//�G�ƃv���C���[�̃x�N�g��

														//�v���C���[�ƓG�̌��������߂�
			float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//�G�̌���

																   //�J�����̎擾
			CCamera *pCamera = CRenderer::GetCamera();
			D3DXVECTOR3 PosV = pCamera->GetPosV();
			D3DXVECTOR3 PosR = pCamera->GetPosR();
			D3DXVECTOR3 Rot = pCamera->GetRot();
			if (Rot.y > -0.1f)
			{
				Rot.y = -0.1f;
			}
			else if (Rot.y < -2.0f)
			{
				Rot.y = -2.0f;
			}
			//�J�����̎擾
			CCamera *pMapCamera = CRenderer::GetMapCamera();

			pMapCamera->SetRot({ fAng,Rot.y,0.0f });
			pMapCamera->SetMapCamera(pos);

			PosV.x = PosR.x + sinf(Rot.y)* sinf(fAng) * pCamera->GetLong();
			PosV.y = PosR.y + cosf(Rot.y)			   * pCamera->GetLong();
			PosV.z = PosR.z + sinf(Rot.y)* cosf(fAng) * pCamera->GetLong();
			pCamera->SetPosV(PosV);
			pCamera->SetRot({ fAng,Rot.y,0.0f });
			//���b�N�I�����ɓG�����񂾂Ƃ�
			/*if(m_pNearEnemy!=)*/
			if (m_pNearEnemy->GetModel(0) == nullptr)
			{
				m_pNearEnemy = nullptr;
				m_bRockOn = false;
				return false;

			}
		}


	}
	return m_bRockOn;
}
//----------------------------
//���b�N�I���p�̉摜�̕\������
//----------------------------
void CPlayer_Controller::DrawRockOn(D3DXVECTOR3 & pos)
{
}
//-----------------------------------------------------------------------------------------------
//�W�����v�̏���(�v���C���[�ɍ��W�����v�ł��邩��bool(�����l��false)���������āA�������ɓ����)
//-----------------------------------------------------------------------------------------------
void CPlayer_Controller::Jump(float& fGravity, bool& bJump)
{
	CXInput *pXInput = CManager::GetXInput();
	//�W�����v�̈ړ��ʂɉ��Z
	if (pXInput->GetButtonTrigger(XINPUT_GAMEPAD_A) == true && bJump == false)	//���؁[��
	{
		//�d�͂���ɂ�����

		fGravity = 13.0f;
		bJump = true;
	}

}
//-----------------------------------------------------------------------------------------------
//�d�͂̏���(�v���C���[�ɏd�͂����炷�l(�����l�͌��炵�������̐�)���������āA��O�����ɓ����)
//-----------------------------------------------------------------------------------------------
void CPlayer_Controller::Gravity(D3DXVECTOR3 & pos, float& fGravity, const float& fGravitySpeed, bool & bJump)
{
	////�d�͉��Z
	if (m_fGravitySpeed > 0.0f)
	{

		fGravity -= m_fGravitySpeed;
		pos.y += m_fGravity;
	}
	//���ɂ������ǂ���
	if (pos.y <= 0.0f)
	{
		//�Q�[�W�𑝂₷
		pos.y = 0.0f;
		fGravity = 0.0f;
		bJump = false;
		//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WATER_FALL);
	}
}
//----------------------------
//��ԋ߂��G�����߂�
//----------------------------
void CPlayer_Controller::LengthNearEnemy(D3DXVECTOR3 & pos)
{
}
//----------------------------
//��ԃv���C���[�������Ă�������ɋ߂��G�����߂�
//----------------------------
void CPlayer_Controller::RotNearEnemy()
{
}
//----------------------------
//�ړ�
//----------------------------
void CPlayer_Controller::TestPadMove(D3DXVECTOR3& pos, D3DXVECTOR3& rot,bool& bMove)
{
	//X�̈ړ���
	float fvec_x = 0.0f;
	//Z�̈ړ���
	float fvec_z = 0.0f;
	float fLength = 0.0f;
	float fRot_Yx = 0.0f;
	float fRot_Yz = 0.0f;

	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//�J�����̎擾
	CCamera *pCamera = CRenderer::GetCamera();
	CXInput *pXInput = CManager::GetXInput();

	//�O�ɐi��

	if ((float)GamePad.lX >= MAX_DEAD_ZOON || (float)GamePad.lY >= MAX_DEAD_ZOON||
		(float)GamePad.lX <= -MAX_DEAD_ZOON || (float)GamePad.lY <= -MAX_DEAD_ZOON)
	{
		//�ړ����[�V�����ɂ���
		bMove = true;
		//�X�e�B�b�N�̌X���̒��������߂�
		fLength = (float)sqrt(GamePad.lX * GamePad.lX + GamePad.lY * GamePad.lY);
		fLength = fLength / 1000.f;
		float fRot = atan2f((float)GamePad.lX, (float)GamePad.lY);
		//float fRot = atan2f(pXInput->GetGamePad()->m_state.Gamepad.sThumbLX, pXInput->GetGamePad()->m_state.Gamepad.sThumbLY);
		rot.y = -(fRot-pCamera->GetRot().x);
		pos.x -= (sinf(rot.y)*PLAYER_MOVE_SPEED)*fLength;
		pos.z -= (cosf(rot.y)*PLAYER_MOVE_SPEED)*fLength;
		m_fSoundInterval += 0.1f;
		if (m_fSoundInterval >= 1.3f)
		{
			m_fSoundInterval = 0.0f;
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);

		}

	}
	else
	{
		//�ҋ@���[�V�����ɖ߂�
		bMove = false;
		m_fSoundInterval = 1.3f;

		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

	}
	//�W�����v

}
void CPlayer_Controller::SkillPadMove(D3DXVECTOR3 & pos, D3DXVECTOR3 & rot, bool & bMove, const bool& bNearEnemy)
{
	//X�̈ړ���
	float fvec_x = 0.0f;
	//Z�̈ړ���
	float fvec_z = 0.0f;
	float fLength = 0.0f;
	float fRot_Yx = 0.0f;
	float fRot_Yz = 0.0f;

	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//�J�����̎擾
	CCamera *pCamera = CRenderer::GetCamera();
	CXInput *pXInput = CManager::GetXInput();

	//�O�ɐi��

	if ((float)GamePad.lX >= MAX_DEAD_ZOON || (float)GamePad.lY >= MAX_DEAD_ZOON ||
		(float)GamePad.lX <= -MAX_DEAD_ZOON || (float)GamePad.lY <= -MAX_DEAD_ZOON)
	{
		//�ړ����[�V�����ɂ���
		bMove = true;
		//�X�e�B�b�N�̌X���̒��������߂�
		fLength = (float)sqrt(GamePad.lX * GamePad.lX + GamePad.lY * GamePad.lY);
		fLength = fLength / 1000.f;
		float fRot = atan2f((float)GamePad.lX, (float)GamePad.lY);

		//�v���C���[��G�̕����Ɍ�������
		if (m_pNearEnemy != nullptr&&bNearEnemy == true)
		{
			D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
			m_NearEnemyVec = EnemyPos - pos;			//�G�ƃv���C���[�̃x�N�g��

														//�v���C���[�ƓG�̌��������߂�
			float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//�G�̌���
			rot.y = fAng + D3DX_PI;
		}
		else
		{
			rot.y = -(fRot - pCamera->GetRot().x);

		}
		//float fRot = atan2f(pXInput->GetGamePad()->m_state.Gamepad.sThumbLX, pXInput->GetGamePad()->m_state.Gamepad.sThumbLY);
		pos.x -= (sinf(-(fRot - pCamera->GetRot().x))*PLAYER_MOVE_SPEED)*fLength;
		pos.z -= (cosf(-(fRot - pCamera->GetRot().x))*PLAYER_MOVE_SPEED)*fLength;
	}
	else
	{
		//�ҋ@���[�V�����ɖ߂�
		bMove = false;
	}

}
//--------------------------------
//�v���C���[��G�̕����Ɍ�������
//--------------------------------
void CPlayer_Controller::PlayerFace(D3DXVECTOR3 & pos, float & frotY, const bool & bNearEnemy)
{
	if (m_pNearEnemy != nullptr&&bNearEnemy == true)
	{
		D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
		m_NearEnemyVec = EnemyPos - pos;			//�G�ƃv���C���[�̃x�N�g��

													//�v���C���[�ƓG�̌��������߂�
		float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//�G�̌���
		frotY = fAng + D3DX_PI;
	}

}
//----------------------------
//�ړ�
//----------------------------
void CPlayer_Controller::TestMove(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	//X�̈ړ���
	float fvec_x = 0.0f;
	//Z�̈ړ���
	float fvec_z = 0.0f;
	float fLength = 0.0f;
	float fRot_Yx = 0.0f;
	float fRot_Yz = 0.0f;

	//�L�[�{�[�h�̎擾
	CInputKeyBoard *Key = CManager::GetInputKeyboard();
	//�J�����̎擾
	CCamera *pCamera = CRenderer::GetCamera();
	//�ړ��{�^������������ړ����[�V������������
	if (Key->GetPress(DIK_W) == true ||
		Key->GetPress(DIK_A) == true ||
		Key->GetPress(DIK_S) == true ||
		Key->GetPress(DIK_D) == true)
	{
		//�ړ����[�V�����ɂ���

	}
	else
	{
		//�ҋ@���[�V�����ɖ߂�

	}
	//�O�ɐi��
	if (Key->GetPress(DIK_W) == true)
	{
		rot.y = D3DXToRadian(180.0f);
		fvec_x += sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z += cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		//�E�ɐi��
		if (Key->GetPress(DIK_D) == true)
		{
			rot.y = D3DXToRadian(225.0f);
		}
		//���ɐi��
		else if (Key->GetPress(DIK_A) == true)
		{
			rot.y = D3DXToRadian(135.0f);
		}
	}
	//���ɐi��
	else if (Key->GetPress(DIK_S) == true)
	{
		rot.y = D3DXToRadian(0.0f);
		fvec_x += -sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z += -cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		if (Key->GetPress(DIK_D) == true)
		{
			rot.y = D3DXToRadian(315.0f);
		}
		//���ɐi��
		else if (Key->GetPress(DIK_A) == true)
		{
			rot.y = D3DXToRadian(45.0f);
		}

	}
	//�E�ɐi��
	if (Key->GetPress(DIK_D) == true)
	{
		rot.y = D3DXToRadian(270.0f);
		fvec_x +=  cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z += -sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		//�O�ɐi��
		if (Key->GetPress(DIK_W) == true)
		{
			rot.y = D3DXToRadian(225.0f);
		}
		//���ɐi��
		else if (Key->GetPress(DIK_S) == true)
		{
			rot.y = D3DXToRadian(315.0f);
		}

	}
	//���ɐi��
	else if (Key->GetPress(DIK_A) == true)
	{
		rot.y = D3DXToRadian(90.0f);
		fvec_x += -cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z +=  sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		//�O�ɐi��
		if (Key->GetPress(DIK_W) == true)
		{
			rot.y = D3DXToRadian(135.0f);

		}
		//���ɐi��
		else if (Key->GetPress(DIK_S) == true)
		{
			rot.y = D3DXToRadian(45.0f);
		}
	}

	//�ړ��ʂ�����Έʒu�Ɉړ��ʂ����Z����
	if (fvec_x != 0.0f || fvec_z != 0.0f)
	{
		//�ړ��̒��������߂�
		fLength = sqrt(fvec_x * fvec_x + fvec_z * fvec_z);

		float fnormal_x = fvec_x / fLength;
		float fnormal_z = fvec_z / fLength;

		fnormal_x *= PLAYER_MOVE_SPEED;
		fnormal_z *= PLAYER_MOVE_SPEED;

		pos.x += fnormal_x;
		pos.z += fnormal_z;
	}


}
//----------------------------
//�U��
//----------------------------
void CPlayer_Controller::Attack(const D3DXVECTOR3& pos, float& frotY, int& nComboType, bool& bAttack, bool& bAttackNext,const bool& bNearEnemy)
{
	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//�U������ɕς���
	if (pGamePad->GetButtonTrigger(CDirectInput::B) == true && bAttack ==false)
	{
		//�v���C���[��G�̕����Ɍ�������
		if (m_pNearEnemy != nullptr&&bNearEnemy == true)
		{
			D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
			m_NearEnemyVec = EnemyPos - pos;			//�G�ƃv���C���[�̃x�N�g��

														//�v���C���[�ƓG�̌��������߂�
			float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//�G�̌���
			frotY = fAng + D3DX_PI;
		}

		nComboType++;
		if (nComboType >= COMBO_MAX)
		{
			nComboType = COMBO_1;
		}
		bAttack = true;
		bAttackNext = false;
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SWORD_ATTACK);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_SWORD_ATTACK,0.5f);

	}
}
//----------------------------
//�d��
//----------------------------
void CPlayer_Controller::Gravity(D3DXVECTOR3& pos)
{
	////�d�͉��Z
	if (m_fGravitySpeed > 0.0f)
	{

		m_fGravity -= m_fGravitySpeed;
		pos.y += m_fGravity;
	}
	//���ɂ������ǂ���
	if (pos.y <= 0.0f)
	{
		//�Q�[�W�𑝂₷
		pos.y =0.0f;
		m_fGravity = 0.0f;
		m_bJumpLock = false;
		//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WATER_FALL);
	}
}
//-----------------------------------
//�}�b�v�̖ړI�n���������鏈��
//-----------------------------------
bool CPlayer_Controller::bPorpose(void)
{
	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	if (pGamePad->GetButtonTrigger(CDirectInput::R3) == true)
	{
		m_bPorpose = m_bPorpose ? false : true;
	}
	return m_bPorpose;
}
//-------------------------------
//�v���C���[�̃J�����̃R���g���[��
//--------------------------------
void CPlayer_Controller::CameraControl(const D3DXVECTOR3 pos)
{
	if (m_bRockOn == false)
	{
		//DirectInput�̃Q�[���p�b�h�̎擾
		CDirectInput *pGamePad = CManager::GetDirectInput();
		//�Q�[���p�b�h�̃{�^�����̎擾
		DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
		//�J�����̎擾
		CCamera *pCamera = CRenderer::GetCamera();
		D3DXVECTOR3 PosV = pCamera->GetPosV();
		D3DXVECTOR3 PosR = pCamera->GetPosR();
		D3DXVECTOR3 Rot = pCamera->GetRot();
		float fLong = 0.0f;

		if ((float)GamePad.lRy >= MAX_DEAD_ZOON || (float)GamePad.lRx >= MAX_DEAD_ZOON ||
			(float)GamePad.lRy <= -MAX_DEAD_ZOON || (float)GamePad.lRx <= -MAX_DEAD_ZOON)
		{
			Rot.y += (float)GamePad.lRy / 50000.0f;
			Rot.x += (float)GamePad.lRx / 20000.0f;
		}

		if (Rot.y > -0.1f)
		{
			Rot.y = -0.1f;
		}
		else if (Rot.y < -1.5f)
		{
			Rot.y = -1.5f;
		}
		if (Rot.x > D3DX_PI)
		{
			Rot.x = -D3DX_PI;
		}
		else if (Rot.x < -D3DX_PI)
		{
			Rot.x = D3DX_PI;
		}
		//�J�����̃Y�[��
		//if (pGamePad->GetGamepadPress(CDirectInput::L2) == true)
		//{
		//	fLong += 2.0f;
		//}
		//else if (pGamePad->GetGamepadPress(CDirectInput::R2) == true)
		//{
		//	fLong -= 2.0f;
		//}
		pCamera->SetLong(fLong);

		PosV.x = PosR.x + sinf(Rot.y)* sinf(Rot.x) * pCamera->GetLong();
		PosV.y = PosR.y + cosf(Rot.y)			   * pCamera->GetLong();
		PosV.z = PosR.z + sinf(Rot.y)* cosf(Rot.x) * pCamera->GetLong();
		//�J�����̎擾
		CCamera *pMapCamera = CRenderer::GetMapCamera();
		pMapCamera->SetMapCamera(pos);
		pMapCamera->SetRot(Rot);

		pCamera->SetPosR(PosR);
		pCamera->SetPosV(PosV);
		pCamera->SetRot(Rot);

	}
}
