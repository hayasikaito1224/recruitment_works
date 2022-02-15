//-------------------------------
// ���[�V�����̎���
// Author:������
//-------------------------------
#include "motion.h"
#include "model.h"
#include "manager.h"
#include "Renderer.h"
//------------------------------------
// �R���X�g���N�^
//------------------------------------
CMotion::CMotion()
{
	m_NumKey = 0;
	m_aMotionInfo.clear();
	m_MotionType = MOTIONTYPE_NEUTRAL;
	m_MotionTypeLast = MOTIONTYPE_NEUTRAL;
	m_NumKey = 0;
	m_motionCounter = 0.0f;
	m_Timer = 0;
}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
CMotion::~CMotion()
{

}

//------------------------------------
// ���[�V����������
//------------------------------------
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//------------------------------------
// ���[�V�����̍Đ�
//------------------------------------
void CMotion::PlayMotion(int nNumParts, CModel **apModel, int& motionType, int& motionTypeLast)
{
	//-------------------------------------------------------------------------
	// �ʒu�Ɖ�]�̌v�Z�Ɏg�p���郍�[�J���ϐ�
	//-------------------------------------------------------------------------
	float fposDiffX[MAX_PARTS];		// �ʒu���̍���X
	float fposDiffY[MAX_PARTS];		// �ʒu���̍���Y
	float fposDiffZ[MAX_PARTS];		// �ʒu���̍���Z
	float frotDiffX[MAX_PARTS];		// ��]���̍���X
	float frotDiffY[MAX_PARTS];		// ��]���̍���Y
	float frotDiffZ[MAX_PARTS];		// ��]���̍���Z

	float fposAskX[MAX_PARTS];		// ���߂�l�̈ʒuX
	float fposAskY[MAX_PARTS];		// ���߂�l�̈ʒuY
	float fposAskZ[MAX_PARTS];		// ���߂�l�̈ʒuZ
	float frotAskX[MAX_PARTS];		// ���߂�l�̉�]X
	float frotAskY[MAX_PARTS];		// ���߂�l�̉�]Y
	float frotAskZ[MAX_PARTS];		// ���߂�l�̉�]Z

	//-------------------------------------------------------------------------
	// �O��̃��[�V�����ƌ��݂̃��[�V�����̔�r
	//-------------------------------------------------------------------------

	// ���[�V�������ς������
	if (motionTypeLast != motionType)
	{
		m_NumKey = 0;
	}

	// ���݂̃��[�V�����^�C�v��ۑ�
	motionTypeLast = motionType;

	// ���݂̃L�[���ő�̃L�[��菬����������(��:�ő�L�[��4�Ȃ�"%d < 4 -1")
	if (m_NumKey < (m_aMotionInfo[motionType].nMaxKey - 1))
	{
		while (1)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// ���̔ԍ��̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// ���߂��l����
				apModel[nCnt]->SetPos(D3DXVECTOR3
				(
					fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
					fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
					fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
				);

				apModel[nCnt]->SetRot(D3DXVECTOR3
				(
					frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
					frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
					frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
				);

			}

			m_motionCounter++;

			// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
			if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
			{
				m_motionCounter = 0;
				m_NumKey++;
				if (m_NumKey >= m_aMotionInfo[motionType].nMaxKey - 1)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		
	}

	// ���[�V�������[�v�̏���(�L�[�ԍ����ő�ɂȂ�����ʂ鏈��)
	if (m_NumKey == m_aMotionInfo[motionType].nMaxKey - 1)
	{
		// ���[�v��Ԃ�������
		if (m_aMotionInfo[motionType].bLoop == true)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 0�Ԗڂ̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// ���߂��l����
				apModel[nCnt]->SetPos(D3DXVECTOR3
				(
					fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
					fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
					fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
				);

				apModel[nCnt]->SetRot(D3DXVECTOR3
				(
					frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
					frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
					frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
				);

			}

			m_motionCounter++;

			// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
			if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
			{
				m_motionCounter = 0;
				m_NumKey = 0;
			}


		}

		// ���[�v���Ă��Ȃ�������
		else
		{

			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// ���̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
				fposDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				// ���߂��l����
				apModel[nCnt]->SetPos(D3DXVECTOR3
				(
					fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
					fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
					fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
				);

				apModel[nCnt]->SetRot(D3DXVECTOR3
				(
					frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
					frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
					frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
				);

			}

			// �t���[���������Z����
			m_motionCounter++;

			// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
			if (m_motionCounter > m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame)
			{
				m_motionCounter = 0;
				motionType = MOTIONTYPE_NEUTRAL;
			}

		}
	}


	

}
//------------------------------------
// ���[�V�����̍Đ�
//------------------------------------
void CMotion::PlayMotion(const int nNumParts, CModel **apModel, int& motionType, int& motionTypeLast, bool& bStop,
	bool& bAttack, bool& bDelay, bool bMotionReset)
{
	//-------------------------------------------------------------------------
	// �ʒu�Ɖ�]�̌v�Z�Ɏg�p���郍�[�J���ϐ�
	//-------------------------------------------------------------------------
	float fposDiffX[MAX_PARTS];		// �ʒu���̍���X
	float fposDiffY[MAX_PARTS];		// �ʒu���̍���Y
	float fposDiffZ[MAX_PARTS];		// �ʒu���̍���Z
	float frotDiffX[MAX_PARTS];		// ��]���̍���X
	float frotDiffY[MAX_PARTS];		// ��]���̍���Y
	float frotDiffZ[MAX_PARTS];		// ��]���̍���Z
	float fposAskX[MAX_PARTS];		// ���߂�l�̈ʒuX
	float fposAskY[MAX_PARTS];		// ���߂�l�̈ʒuY
	float fposAskZ[MAX_PARTS];		// ���߂�l�̈ʒuZ
	float frotAskX[MAX_PARTS];		// ���߂�l�̉�]X
	float frotAskY[MAX_PARTS];		// ���߂�l�̉�]Y
	float frotAskZ[MAX_PARTS];		// ���߂�l�̉�]Z

	//-------------------------------------------------------------------------
	// �O��̃��[�V�����ƌ��݂̃��[�V�����̔�r
	//-------------------------------------------------------------------------

	// ���[�V�������ς������
	if (motionTypeLast != motionType|| bMotionReset == true)
	{
		m_NumKey = 0;
		m_Timer = 0;
		m_motionCounter = 0;
		bMotionReset = false;
	}

	// ���݂̃��[�V�����^�C�v��ۑ�
	motionTypeLast = motionType;

	// ���݂̃L�[���ő�̃L�[��菬����������(��:�ő�L�[��4�Ȃ�"%d < 4 -1")
	if (m_NumKey < (m_aMotionInfo[motionType].nMaxKey - 1))
	{
		while (1)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// ���̔ԍ��̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// ���߂��l����
				apModel[nCnt]->SetPos(D3DXVECTOR3
				(
					fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
					fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
					fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
				);

				apModel[nCnt]->SetRot(D3DXVECTOR3
				(
					frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
					frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
					frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
				);

			}

			m_motionCounter++;
			if (bAttack)
			{
				bDelay = false;
			}

			// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
			if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
			{
				m_motionCounter = 0;
				m_NumKey++;
				//�U�����Ă����ԂŃL�[���ő�l�ɍs������
				if (m_NumKey >= m_aMotionInfo[motionType].nMaxKey - 1)
				{
					if (bAttack == true)
					{
						bDelay = true;
					}
					bAttack = false;
					break;
				}
			}
			else
			{
				break;
			}
		}

	}

	// ���[�V�������[�v�̏���(�L�[�ԍ����ő�ɂȂ�����ʂ鏈��)
	if (m_NumKey == m_aMotionInfo[motionType].nMaxKey - 1)
	{
		// ���[�v��Ԃ�������
		if (m_aMotionInfo[motionType].bLoop == true)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 0�Ԗڂ̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// ���߂��l����
				apModel[nCnt]->SetPos(D3DXVECTOR3
				(
					fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
					fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
					fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
				);

				apModel[nCnt]->SetRot(D3DXVECTOR3
				(
					frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
					frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
					frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
				);

			}

			m_motionCounter++;

			// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
			if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
			{
				m_motionCounter = 0;
				m_NumKey = 0;
			}


		}

		// ���[�v���Ă��Ȃ�������
		else
		{

			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// ���̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
				fposDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				// ���߂��l����
				apModel[nCnt]->SetPos(D3DXVECTOR3
				(
					fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
					fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
					fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
				);

				apModel[nCnt]->SetRot(D3DXVECTOR3
				(
					frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
					frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
					frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
				);

			}
			// �t���[���������Z����
			m_motionCounter++;
			// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
			if (m_motionCounter >= m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame)
			{
				bStop = true;
				m_motionCounter = 0;
				motionType = 0;
				bAttack = false;

			}
		}
	}
}

void CMotion::NoLoopPlayMotion(const int nNumParts, CModel ** apModel, int & motionType,
	int & motionTypeLast, bool & bStop)
{
	//-------------------------------------------------------------------------
	// �ʒu�Ɖ�]�̌v�Z�Ɏg�p���郍�[�J���ϐ�
	//-------------------------------------------------------------------------
	float fposDiffX[MAX_PARTS];		// �ʒu���̍���X
	float fposDiffY[MAX_PARTS];		// �ʒu���̍���Y
	float fposDiffZ[MAX_PARTS];		// �ʒu���̍���Z
	float frotDiffX[MAX_PARTS];		// ��]���̍���X
	float frotDiffY[MAX_PARTS];		// ��]���̍���Y
	float frotDiffZ[MAX_PARTS];		// ��]���̍���Z
	float fposAskX[MAX_PARTS];		// ���߂�l�̈ʒuX
	float fposAskY[MAX_PARTS];		// ���߂�l�̈ʒuY
	float fposAskZ[MAX_PARTS];		// ���߂�l�̈ʒuZ
	float frotAskX[MAX_PARTS];		// ���߂�l�̉�]X
	float frotAskY[MAX_PARTS];		// ���߂�l�̉�]Y
	float frotAskZ[MAX_PARTS];		// ���߂�l�̉�]Z

									//-------------------------------------------------------------------------
									// �O��̃��[�V�����ƌ��݂̃��[�V�����̔�r
									//-------------------------------------------------------------------------

	// ���[�V�������ς������
	if (motionTypeLast != motionType)
	{
		m_NumKey = 0;
		m_Timer = 0;
		m_motionCounter = 0;
	}

	// ���݂̃��[�V�����^�C�v��ۑ�
	motionTypeLast = motionType;
	//���[�V�������~�܂��ĂȂ�������
	if (bStop == false)
	{
		// ���݂̃L�[���ő�̃L�[��菬����������(��:�ő�L�[��4�Ȃ�"%d < 4 -1")
		if (m_NumKey < (m_aMotionInfo[motionType].nMaxKey - 1))
		{
			while (1)
			{
				for (int nCnt = 0; nCnt < nNumParts; nCnt++)
				{
					// ���̔ԍ��̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
					fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
					fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
					fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

					frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
					frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
					frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

					// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
					fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

					frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

					// ���߂��l����
					apModel[nCnt]->SetPos(D3DXVECTOR3
					(
						fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
						fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
						fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
					);

					apModel[nCnt]->SetRot(D3DXVECTOR3
					(
						frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
						frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
						frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
					);

				}

				m_motionCounter++;

				// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
				if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
				{
					m_motionCounter = 0;
					m_NumKey++;
					//�U�����Ă����ԂŃL�[���ő�l�ɍs������
					if (m_NumKey >= m_aMotionInfo[motionType].nMaxKey - 1)
					{

						break;
					}
				}
				else
				{
					break;
				}
			}

		}

		// ���[�V�������[�v�̏���(�L�[�ԍ����ő�ɂȂ�����ʂ鏈��)
		if (m_NumKey == m_aMotionInfo[motionType].nMaxKey - 1)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// ���̃L�[�ƌ��݂̃L�[�̍��������߂鏈��
				fposDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// ���݂̃L�[�Ƌ��߂������𑫂��Ĉʒu�Ɖ�]�̒l�����߂鏈��
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				// ���߂��l����
				apModel[nCnt]->SetPos(D3DXVECTOR3
				(
					fposAskX[nCnt] + apModel[nCnt]->GetLayerPos().x,
					fposAskY[nCnt] + apModel[nCnt]->GetLayerPos().y,
					fposAskZ[nCnt] + apModel[nCnt]->GetLayerPos().z)
				);

				apModel[nCnt]->SetRot(D3DXVECTOR3
				(
					frotAskX[nCnt] + apModel[nCnt]->GetLayerRot().x,
					frotAskY[nCnt] + apModel[nCnt]->GetLayerRot().y,
					frotAskZ[nCnt] + apModel[nCnt]->GetLayerRot().z)
				);

			}
			// �t���[���������Z����
			m_motionCounter++;
			bStop = true;

			// ���[�V�����J�E���^�[���ő�t���[�����𒴂�����
			if (m_motionCounter >= m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame)
			{
				//���[�V�������~�߂�
				m_motionCounter = 0;
				motionType = 0;
			}

		}

	}
}

//------------------------------------
// ���[�V�����e�L�X�g�̓ǂݍ���
//------------------------------------
void CMotion::MotionLoad(const char *sMotionFileName)
{
	int motionType;	// ���[�V�����^�C�v
	int nNumParts = 0;		// �p�[�c��������ϐ�
	int nCntParts = 0;		// �p�[�c�̃J�E���^�[
	int nCntKey = 0;		// �L�[�̃J�E���^�[
	
	char sString[6][255];	// �ǂݍ��ݗp�̕ϐ�

	// �t�@�C���ǂݍ���
	FILE *pFile = fopen(sMotionFileName, "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// END_SCRIPT���Ă΂��܂Ń��[�v����
		while (1)
		{
			// �P�P���ǂݍ���
			fscanf(pFile, "%s", &sString[0]);

			// �p�[�c���̓ǂݍ���
			if (strcmp(sString[0], "NUM_PARTS") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
				fscanf(pFile, "%d", &nNumParts);	// �p�[�c����ǂݍ���

			}

			// �^�C�v�̓ǂݍ���
			if (strcmp(sString[0], "TYPE") == 0)
			{
				fscanf(pFile, "%s", &sString[1]);	// �C�R�[�������܂���
				fscanf(pFile, "%d", &motionType);	// �^�C�v��ǂݍ���
				MOTION_INFO Securement = {};
				m_aMotionInfo.push_back(Securement);

			}

			// ���[�V�����Z�b�g
			while (strcmp(sString[0], "MOTIONSET") == 0)
			{
				// �P�P���ǂݍ���
				fscanf(pFile, "%s", &sString[1]);

				// ���[�v����̓ǂݍ���
				if (strcmp(sString[1], "LOOP") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);						// �C�R�[�������܂���
					fscanf(pFile, "%d", &m_aMotionInfo[motionType].bLoop);	// ���[�v���郂�[�V��������ǂݍ���
				}

				// �ő�L�[���̓ǂݍ���
				if (strcmp(sString[1], "NUM_KEY") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);							// �C�R�[�������܂���
					fscanf(pFile, "%d", &m_aMotionInfo[motionType].nMaxKey);	// �L�[�̐���ǂݍ���
					for (int nCnt = 0; nCnt < m_aMotionInfo[motionType].nMaxKey; nCnt++)
					{
						//�L�[���̍\���̂̓��I�m��
						KEY_INFO Securement = {};
						m_aMotionInfo[motionType].aKeyInfo.push_back(Securement);
						for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
						{
							//�L�[���p�[�c�������I�m�ۂ���
							KEY Securement = {};
							m_aMotionInfo[motionType].aKeyInfo[nCnt].aKey.push_back(Securement);

						}

					}

				}

				// �L�[�Z�b�g
				while (strcmp(sString[1], "KEYSET") == 0)
				{
					// �P�P���ǂݍ���
					fscanf(pFile, "%s", &sString[2]);

					// �t���[���̓ǂݍ���
					if (strcmp(sString[2], "FRAME") == 0)
					{
						fscanf(pFile, "%s", &sString[3]);											// �C�R�[�������܂���
						fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].nFrame);	// �L�[�̐���ǂݍ���
					}

					// �e�p�[�c�̃L�[�Z�b�g
					while (strcmp(sString[2], "KEY") == 0)
					{
						// �P�P���ǂݍ���
						fscanf(pFile, "%s", &sString[3]);

						// �ʒu�̓ǂݍ���
						if (strcmp(sString[3], "POS") == 0)
						{

							fscanf(pFile, "%s", &sString[4]);															// �C�R�[�������܂���
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fPosX,
								&m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fPosY, 
								&m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);
						}

						// ��]�̓ǂݍ���
						if (strcmp(sString[3], "ROT") == 0)
						{
							fscanf(pFile, "%s", &sString[4]);															// �C�R�[�������܂���
							fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fRotX);	// ��]X��ǂݍ���
							fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fRotY);	// ��]Y��ǂݍ���
							fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);	// ��]Z��ǂݍ���
						}

						// 
						if (strcmp(sString[3], "END_KEY") == 0)
						{
							nCntParts++;
							// �p�[�c���̍ő�ȏ�ɂȂ�����
							if (nCntParts >= nNumParts)
							{
								nCntParts = 0;
							}
							break;
						}
					}

					if (strcmp(sString[2], "END_KEYSET") == 0)
					{
						nCntKey++;
						// �p�[�c���̍ő�ȏ�ɂȂ�����
						if (nCntKey >= m_aMotionInfo[motionType].nMaxKey)
						{
							nCntKey = 0;
						}
						break;
					}
				}

				if (strcmp(sString[1], "END_MOTIONSET") == 0)
				{
					break;
				}
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}

		}
		// �t�@�C�������
		fclose(pFile);

	}

	else
	{
		// �G���[�o��
		perror("�t�@�C�����J���܂���ł���");
	}

}

void CMotion::Drawtext()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[1200];
	int nNum = 0;

	nNum = sprintf(&str[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n ���[�V������� \n");
	nNum += sprintf(&str[nNum], " [NumKey] %d \n", m_NumKey);
	nNum += sprintf(&str[nNum], " [m_motionCounter] %.2f \n", m_motionCounter);


	//nNum += sprintf(&str[nNum], " ���X�e�B�b�NY %d  \n", GamePad.lY);

	LPD3DXFONT pFont = CManager::GetRenderer()->GetFont();
	// �e�L�X�g�`��
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}

