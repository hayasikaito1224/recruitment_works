//-------------------------------
// モーションの実装
// Author:村元翼
//-------------------------------
#include "motion.h"
#include "model.h"
#include "manager.h"
#include "Renderer.h"
//------------------------------------
// コンストラクタ
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
// デストラクタ
//------------------------------------
CMotion::~CMotion()
{

}

//------------------------------------
// モーション初期化
//------------------------------------
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//------------------------------------
// モーションの再生
//------------------------------------
void CMotion::PlayMotion(int nNumParts, CModel **apModel, int& motionType, int& motionTypeLast)
{
	//-------------------------------------------------------------------------
	// 位置と回転の計算に使用するローカル変数
	//-------------------------------------------------------------------------
	float fposDiffX[MAX_PARTS];		// 位置情報の差分X
	float fposDiffY[MAX_PARTS];		// 位置情報の差分Y
	float fposDiffZ[MAX_PARTS];		// 位置情報の差分Z
	float frotDiffX[MAX_PARTS];		// 回転情報の差分X
	float frotDiffY[MAX_PARTS];		// 回転情報の差分Y
	float frotDiffZ[MAX_PARTS];		// 回転情報の差分Z

	float fposAskX[MAX_PARTS];		// 求める値の位置X
	float fposAskY[MAX_PARTS];		// 求める値の位置Y
	float fposAskZ[MAX_PARTS];		// 求める値の位置Z
	float frotAskX[MAX_PARTS];		// 求める値の回転X
	float frotAskY[MAX_PARTS];		// 求める値の回転Y
	float frotAskZ[MAX_PARTS];		// 求める値の回転Z

	//-------------------------------------------------------------------------
	// 前回のモーションと現在のモーションの比較
	//-------------------------------------------------------------------------

	// モーションが変わった時
	if (motionTypeLast != motionType)
	{
		m_NumKey = 0;
	}

	// 現在のモーションタイプを保存
	motionTypeLast = motionType;

	// 現在のキーが最大のキーより小さかったら(例:最大キーが4なら"%d < 4 -1")
	if (m_NumKey < (m_aMotionInfo[motionType].nMaxKey - 1))
	{
		while (1)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 次の番号のキーと現在のキーの差分を求める処理
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// 現在のキーと求めた差分を足して位置と回転の値を求める処理
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// 求めた値を代入
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

			// モーションカウンターが最大フレーム数を超えたら
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

	// モーションループの処理(キー番号が最大になったら通る処理)
	if (m_NumKey == m_aMotionInfo[motionType].nMaxKey - 1)
	{
		// ループ状態だったら
		if (m_aMotionInfo[motionType].bLoop == true)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 0番目のキーと現在のキーの差分を求める処理
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// 現在のキーと求めた差分を足して位置と回転の値を求める処理
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// 求めた値を代入
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

			// モーションカウンターが最大フレーム数を超えたら
			if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
			{
				m_motionCounter = 0;
				m_NumKey = 0;
			}


		}

		// ループしていなかったら
		else
		{

			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 次のキーと現在のキーの差分を求める処理
				fposDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// 現在のキーと求めた差分を足して位置と回転の値を求める処理
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				// 求めた値を代入
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

			// フレーム数を加算する
			m_motionCounter++;

			// モーションカウンターが最大フレーム数を超えたら
			if (m_motionCounter > m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame)
			{
				m_motionCounter = 0;
				motionType = MOTIONTYPE_NEUTRAL;
			}

		}
	}


	

}
//------------------------------------
// モーションの再生
//------------------------------------
void CMotion::PlayMotion(const int nNumParts, CModel **apModel, int& motionType, int& motionTypeLast, bool& bStop,
	bool& bAttack, bool& bDelay, bool bMotionReset)
{
	//-------------------------------------------------------------------------
	// 位置と回転の計算に使用するローカル変数
	//-------------------------------------------------------------------------
	float fposDiffX[MAX_PARTS];		// 位置情報の差分X
	float fposDiffY[MAX_PARTS];		// 位置情報の差分Y
	float fposDiffZ[MAX_PARTS];		// 位置情報の差分Z
	float frotDiffX[MAX_PARTS];		// 回転情報の差分X
	float frotDiffY[MAX_PARTS];		// 回転情報の差分Y
	float frotDiffZ[MAX_PARTS];		// 回転情報の差分Z
	float fposAskX[MAX_PARTS];		// 求める値の位置X
	float fposAskY[MAX_PARTS];		// 求める値の位置Y
	float fposAskZ[MAX_PARTS];		// 求める値の位置Z
	float frotAskX[MAX_PARTS];		// 求める値の回転X
	float frotAskY[MAX_PARTS];		// 求める値の回転Y
	float frotAskZ[MAX_PARTS];		// 求める値の回転Z

	//-------------------------------------------------------------------------
	// 前回のモーションと現在のモーションの比較
	//-------------------------------------------------------------------------

	// モーションが変わった時
	if (motionTypeLast != motionType|| bMotionReset == true)
	{
		m_NumKey = 0;
		m_Timer = 0;
		m_motionCounter = 0;
		bMotionReset = false;
	}

	// 現在のモーションタイプを保存
	motionTypeLast = motionType;

	// 現在のキーが最大のキーより小さかったら(例:最大キーが4なら"%d < 4 -1")
	if (m_NumKey < (m_aMotionInfo[motionType].nMaxKey - 1))
	{
		while (1)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 次の番号のキーと現在のキーの差分を求める処理
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// 現在のキーと求めた差分を足して位置と回転の値を求める処理
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// 求めた値を代入
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

			// モーションカウンターが最大フレーム数を超えたら
			if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
			{
				m_motionCounter = 0;
				m_NumKey++;
				//攻撃している状態でキーが最大値に行ったら
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

	// モーションループの処理(キー番号が最大になったら通る処理)
	if (m_NumKey == m_aMotionInfo[motionType].nMaxKey - 1)
	{
		// ループ状態だったら
		if (m_aMotionInfo[motionType].bLoop == true)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 0番目のキーと現在のキーの差分を求める処理
				fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// 現在のキーと求めた差分を足して位置と回転の値を求める処理
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

				// 求めた値を代入
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

			// モーションカウンターが最大フレーム数を超えたら
			if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
			{
				m_motionCounter = 0;
				m_NumKey = 0;
			}


		}

		// ループしていなかったら
		else
		{

			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 次のキーと現在のキーの差分を求める処理
				fposDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// 現在のキーと求めた差分を足して位置と回転の値を求める処理
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				// 求めた値を代入
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
			// フレーム数を加算する
			m_motionCounter++;
			// モーションカウンターが最大フレーム数を超えたら
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
	// 位置と回転の計算に使用するローカル変数
	//-------------------------------------------------------------------------
	float fposDiffX[MAX_PARTS];		// 位置情報の差分X
	float fposDiffY[MAX_PARTS];		// 位置情報の差分Y
	float fposDiffZ[MAX_PARTS];		// 位置情報の差分Z
	float frotDiffX[MAX_PARTS];		// 回転情報の差分X
	float frotDiffY[MAX_PARTS];		// 回転情報の差分Y
	float frotDiffZ[MAX_PARTS];		// 回転情報の差分Z
	float fposAskX[MAX_PARTS];		// 求める値の位置X
	float fposAskY[MAX_PARTS];		// 求める値の位置Y
	float fposAskZ[MAX_PARTS];		// 求める値の位置Z
	float frotAskX[MAX_PARTS];		// 求める値の回転X
	float frotAskY[MAX_PARTS];		// 求める値の回転Y
	float frotAskZ[MAX_PARTS];		// 求める値の回転Z

									//-------------------------------------------------------------------------
									// 前回のモーションと現在のモーションの比較
									//-------------------------------------------------------------------------

	// モーションが変わった時
	if (motionTypeLast != motionType)
	{
		m_NumKey = 0;
		m_Timer = 0;
		m_motionCounter = 0;
	}

	// 現在のモーションタイプを保存
	motionTypeLast = motionType;
	//モーションが止まってなかったら
	if (bStop == false)
	{
		// 現在のキーが最大のキーより小さかったら(例:最大キーが4なら"%d < 4 -1")
		if (m_NumKey < (m_aMotionInfo[motionType].nMaxKey - 1))
		{
			while (1)
			{
				for (int nCnt = 0; nCnt < nNumParts; nCnt++)
				{
					// 次の番号のキーと現在のキーの差分を求める処理
					fposDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
					fposDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
					fposDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

					frotDiffX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
					frotDiffY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
					frotDiffZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey + 1].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

					// 現在のキーと求めた差分を足して位置と回転の値を求める処理
					fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

					frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);
					frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame);

					// 求めた値を代入
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

				// モーションカウンターが最大フレーム数を超えたら
				if (m_motionCounter > m_aMotionInfo[motionType].aKeyInfo[m_NumKey].nFrame)
				{
					m_motionCounter = 0;
					m_NumKey++;
					//攻撃している状態でキーが最大値に行ったら
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

		// モーションループの処理(キー番号が最大になったら通る処理)
		if (m_NumKey == m_aMotionInfo[motionType].nMaxKey - 1)
		{
			for (int nCnt = 0; nCnt < nNumParts; nCnt++)
			{
				// 次のキーと現在のキーの差分を求める処理
				fposDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX;
				fposDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY;
				fposDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fPosZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ;

				frotDiffX[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotX - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX;
				frotDiffY[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotY - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY;
				frotDiffZ[nCnt] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[nCnt].fRotZ - m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ;

				// 現在のキーと求めた差分を足して位置と回転の値を求める処理
				fposAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosX + fposDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosY + fposDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				fposAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fPosZ + fposDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				frotAskX[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotX + frotDiffX[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskY[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotY + frotDiffY[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);
				frotAskZ[nCnt] = m_aMotionInfo[motionType].aKeyInfo[m_NumKey].aKey[nCnt].fRotZ + frotDiffZ[nCnt] * (m_motionCounter / m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame);

				// 求めた値を代入
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
			// フレーム数を加算する
			m_motionCounter++;
			bStop = true;

			// モーションカウンターが最大フレーム数を超えたら
			if (m_motionCounter >= m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame)
			{
				//モーションを止める
				m_motionCounter = 0;
				motionType = 0;
			}

		}

	}
}

//------------------------------------
// モーションテキストの読み込み
//------------------------------------
void CMotion::MotionLoad(const char *sMotionFileName)
{
	int motionType;	// モーションタイプ
	int nNumParts = 0;		// パーツ数を入れる変数
	int nCntParts = 0;		// パーツのカウンター
	int nCntKey = 0;		// キーのカウンター
	
	char sString[6][255];	// 読み込み用の変数

	// ファイル読み込み
	FILE *pFile = fopen(sMotionFileName, "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// END_SCRIPTが呼ばれるまでループする
		while (1)
		{
			// １単語を読み込む
			fscanf(pFile, "%s", &sString[0]);

			// パーツ数の読み込み
			if (strcmp(sString[0], "NUM_PARTS") == 0)
			{

				fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
				fscanf(pFile, "%d", &nNumParts);	// パーツ数を読み込む

			}

			// タイプの読み込み
			if (strcmp(sString[0], "TYPE") == 0)
			{
				fscanf(pFile, "%s", &sString[1]);	// イコールを噛ませる
				fscanf(pFile, "%d", &motionType);	// タイプを読み込む
				MOTION_INFO Securement = {};
				m_aMotionInfo.push_back(Securement);

			}

			// モーションセット
			while (strcmp(sString[0], "MOTIONSET") == 0)
			{
				// １単語を読み込む
				fscanf(pFile, "%s", &sString[1]);

				// ループ判定の読み込み
				if (strcmp(sString[1], "LOOP") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);						// イコールを噛ませる
					fscanf(pFile, "%d", &m_aMotionInfo[motionType].bLoop);	// ループするモーションかを読み込む
				}

				// 最大キー数の読み込み
				if (strcmp(sString[1], "NUM_KEY") == 0)
				{
					fscanf(pFile, "%s", &sString[2]);							// イコールを噛ませる
					fscanf(pFile, "%d", &m_aMotionInfo[motionType].nMaxKey);	// キーの数を読み込む
					for (int nCnt = 0; nCnt < m_aMotionInfo[motionType].nMaxKey; nCnt++)
					{
						//キー情報の構造体の動的確保
						KEY_INFO Securement = {};
						m_aMotionInfo[motionType].aKeyInfo.push_back(Securement);
						for (int nCntPartsKey = 0; nCntPartsKey < nNumParts; nCntPartsKey++)
						{
							//キーをパーツ数分動的確保する
							KEY Securement = {};
							m_aMotionInfo[motionType].aKeyInfo[nCnt].aKey.push_back(Securement);

						}

					}

				}

				// キーセット
				while (strcmp(sString[1], "KEYSET") == 0)
				{
					// １単語を読み込む
					fscanf(pFile, "%s", &sString[2]);

					// フレームの読み込み
					if (strcmp(sString[2], "FRAME") == 0)
					{
						fscanf(pFile, "%s", &sString[3]);											// イコールを噛ませる
						fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].nFrame);	// キーの数を読み込む
					}

					// 各パーツのキーセット
					while (strcmp(sString[2], "KEY") == 0)
					{
						// １単語を読み込む
						fscanf(pFile, "%s", &sString[3]);

						// 位置の読み込み
						if (strcmp(sString[3], "POS") == 0)
						{

							fscanf(pFile, "%s", &sString[4]);															// イコールを噛ませる
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fPosX,
								&m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fPosY, 
								&m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);
						}

						// 回転の読み込み
						if (strcmp(sString[3], "ROT") == 0)
						{
							fscanf(pFile, "%s", &sString[4]);															// イコールを噛ませる
							fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fRotX);	// 回転Xを読み込む
							fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fRotY);	// 回転Yを読み込む
							fscanf(pFile, "%f", &m_aMotionInfo[motionType].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);	// 回転Zを読み込む
						}

						// 
						if (strcmp(sString[3], "END_KEY") == 0)
						{
							nCntParts++;
							// パーツ数の最大以上になったら
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
						// パーツ数の最大以上になったら
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
		// ファイルを閉じる
		fclose(pFile);

	}

	else
	{
		// エラー出力
		perror("ファイルが開けませんでした");
	}

}

void CMotion::Drawtext()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[1200];
	int nNum = 0;

	nNum = sprintf(&str[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n モーション情報 \n");
	nNum += sprintf(&str[nNum], " [NumKey] %d \n", m_NumKey);
	nNum += sprintf(&str[nNum], " [m_motionCounter] %.2f \n", m_motionCounter);


	//nNum += sprintf(&str[nNum], " 左スティックY %d  \n", GamePad.lY);

	LPD3DXFONT pFont = CManager::GetRenderer()->GetFont();
	// テキスト描画
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}

