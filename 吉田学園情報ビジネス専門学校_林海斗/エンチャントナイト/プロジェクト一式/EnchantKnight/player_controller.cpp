//-----------------------------------------
//プレイヤーの操作
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

static float s_fCntAngle = 0.0f;//角度のカウント
//--------------------------
//コンストラクト
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
//デストラクト
//----------------------------
CPlayer_Controller::~CPlayer_Controller()
{
}
//============================================
//テキスト描画
//============================================
void CPlayer_Controller::Drawtext(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[1200];
	int nNum = 0;
	CXInput *pXInput = CManager::GetXInput();
	CDirectInput *pGamePad = CManager::GetDirectInput();
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	//コマンドの情報の取得
	CCommandUI *pCommand[CCommandUI::Co_Max] = { 0 };


	nNum = sprintf(&str[0], "\n\n 左スティックX %d  \n", GamePad.lX);
	nNum += sprintf(&str[nNum], " 左スティックY %d  \n", GamePad.lY);
	nNum += sprintf(&str[nNum], " 左スティックZ %d  \n", GamePad.lZ);
	nNum += sprintf(&str[nNum], " lRx %d  \n", GamePad.lRx);
	nNum += sprintf(&str[nNum], " lRy %d  \n", GamePad.lRy);
	nNum += sprintf(&str[nNum], " lRz %d  \n", GamePad.lRz);
	nNum += sprintf(&str[nNum], " カメラの向き %.5f,%.5f,%.5f  \n", pCamera->GetRot().x, pCamera->GetRot().y, pCamera->GetRot().z);
	nNum += sprintf(&str[nNum], " 視点 %.5f,%.5f,%.5f  \n", pCamera->GetPosV().x, pCamera->GetPosV().y, pCamera->GetPosV().z);
	nNum += sprintf(&str[nNum], " 注視点 %.5f,%.5f,%.5f  \n", pCamera->GetPosR().x, pCamera->GetPosR().y, pCamera->GetPosR().z);

	for (int nCnt = 0; nCnt <  CCommandUI::Co_Max; nCnt++)
	{
		nNum += sprintf(&str[nNum], " コマンド%d Pos X:%.2f  Y:%.2f  Z:%.2f  \n", nCnt, pCommand[nCnt]->GetPos().x, pCommand[nCnt]->GetPos().y, pCommand[nCnt]->GetPos().z);

	}
	nNum += sprintf(&str[nNum], " コマンド%d  \n", m_nCommandType);

	LPD3DXFONT pFont = CManager::GetRenderer()->GetFont();
	// テキスト描画
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
//---------------------------------------
//かくどを指定してその方向に向かせる関数
//---------------------------------------
void CPlayer_Controller::Rotate(float& rot_y, float fAngle)
{
	m_rot_y = rot_y;
	m_fAngle = fAngle;

	//現在の角度から目的の角度への差
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	float fAngleSense = 0.0f;
	float fCurrentAngle = (D3DXToDegree(pCamera->GetRot().y) + D3DXToDegree(rot_y));
	fAngleSense = fAngle - fCurrentAngle;
	if (fAngleSense > 180.0f)
	{
		fAngleSense = fAngleSense - 360.0f;
	}
	m_fAngleScece = fAngleSense;

	//数値の正規化の方法
	//fAngleSense = fabsf(fAngleSense);
	//目的の角度が正の数だったらカウントをプラスにする
	if (fAngleSense >= 0.0f)
	{
		s_fCntAngle += 1.0f;
		//目的の角度に行ったら止める
		if (s_fCntAngle >= fAngleSense)
		{
			s_fCntAngle = 0;
		}
	}
	//目的の角度が負の数だったらカウントをマイナスにする
	else if(fAngleSense <= 0.0f)
	{
		s_fCntAngle -= 1.0f;
		//目的の角度に行ったら止める
		if (s_fCntAngle <= fAngleSense)
		{
			s_fCntAngle = 0;
		}
	}
	//角度をプレイヤーの角度に加算
	rot_y += D3DXToRadian(s_fCntAngle);
	m_LastAngle = fAngle;

}
//===================================================
//コマンド選択処理
//===================================================
int CPlayer_Controller::CommandSelect()
{
	////コマンドの情報の取得
	//CCommandUI *pCommand[CCommandUI::Co_Max] = { 0 };
	//for (int nCnt = 0; nCnt < CCommandUI::Co_Max; nCnt++)
	//{
	//	pCommand[nCnt] = CGame::GetCommand(nCnt);
	//}
	////DirectInputのゲームパッドの取得
	//CDirectInput *pGamePad = CManager::GetDirectInput();
	////ゲームパッドのボタン情報の取得
	//DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	////十字キーを放していたら
	//if (GamePad.rgdwPOV[0] == -1)
	//{
	//	m_bCrossDownLock = false;
	//	m_bCrossUpLock = false;
	//}
	////else
	////{
	////	m_bCrossBoLock = false;
	////}
	////コマンド選択
	////上
	//if (GamePad.rgdwPOV[0] == CDirectInput::UP && m_bCrossUpLock == false)
	//{
	//	m_nCommandType--;
	//	if (m_nCommandType < 0)
	//	{
	//		m_nCommandType = (int)CCommandUI::Co_Enhance;
	//	}
	//	m_bCrossUpLock = true;
	//}
	////下
	//else if (GamePad.rgdwPOV[0] == CDirectInput::DOWN && m_bCrossDownLock == false)
	//{

	//	m_nCommandType++;
	//	if (m_nCommandType >= (int)CCommandUI::Co_Max)
	//	{
	//		m_nCommandType = (int)CCommandUI::Co_Attack;
	//	}
	//	m_bCrossDownLock = true;
	//}
	////選択しているコマンド以外は元の位置に戻す
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
	//		//コマンドを選択した状態にする
	//		pCommand[nCnt]->SetSelect(true);
	//		if (pGamePad->GetGamepadPress(CDirectInput::B) == true)
	//		{
	//			//コマンドを押した状態にする
	//			pCommand[nCnt]->SetPush(true);
	//			//まほうコマンドを選択したら
	//			if (m_nCommandType == (int)CCommandUI::Co_Magic)
	//			{
	//				//まほうコマンドを出現させる判定にする
	//				m_bCommandMagic = true;
	//			}

	//		}
	//		else
	//		{
	//			//コマンドを放した状態にする
	//			pCommand[nCnt]->SetPush(false);

	//		}
	//	}
	//}
	return m_nCommandType;
}
//===================================================
//魔法のコマンド選択処理
//===================================================
int CPlayer_Controller::MagicCommandSelect()
{
	//コマンドの情報の取得
	CMagicUI *pCommand[CMagicUI::Magic_Max] = { 0 };
	for (int nCnt = 0; nCnt < CMagicUI::Magic_Max; nCnt++)
	{
		pCommand[nCnt] = CGame::GetMagicCommand(nCnt);
	}
	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//十字キーを放していたら
	if (GamePad.rgdwPOV[0] == -1)
	{
		m_bCrossDownLock = false;
		m_bCrossUpLock = false;
	}

	//コマンド選択
	//上
	if (GamePad.rgdwPOV[0] == CDirectInput::UP && m_bCrossUpLock == false)
	{
		m_nMagicCommandType--;
		if (m_nMagicCommandType < 0)
		{
			m_nMagicCommandType = (int)CMagicUI::Heal;
		}
		m_bCrossUpLock = true;
	}
	//下
	else if (GamePad.rgdwPOV[0] == CDirectInput::DOWN && m_bCrossDownLock == false)
	{

		m_nMagicCommandType++;
		if (m_nMagicCommandType >= (int)CMagicUI::Magic_Max)
		{
			m_nMagicCommandType = (int)CMagicUI::Fire;
		}
		m_bCrossDownLock = true;
	}
	//選択しているコマンド以外は元の位置に戻す
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
			//コマンドを選択した状態にする
			pCommand[nCnt]->SetSelect(true);
			if (pGamePad->GetGamepadPress(CDirectInput::B) == true)
			{
				//コマンドを押した状態にする
				pCommand[nCnt]->SetPush(true);

			}
			else
			{
				//コマンドを放した状態にする
				pCommand[nCnt]->SetPush(false);

			}
		}
	}
	//キャンセルボタンを押したらまほうコマンドを消せるようにする
	if (pGamePad->GetButtonTrigger(CDirectInput::A) == true)
	{
		m_bCommandMagic = false;
	}
	return m_nMagicCommandType;

}
//===================================================
//マジック攻撃処理
//===================================================
int CPlayer_Controller::MagicAttack(D3DXVECTOR3 & pos, float& frotY, int& nMotionType, bool& bMagic, const bool& bNearEnemy, const int& nMagicType)
{
	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//攻撃判定に変える
	if (pGamePad->GetGamepadPress(CDirectInput::L) == true)
	{

		//コマンドの情報の取得
		CMagicUI *pCommand[CMagicUI::Magic_Max] = { 0 };
		for (int nCnt = 0; nCnt < CMagicUI::Magic_Max; nCnt++)
		{
			pCommand[nCnt] = CGame::GetMagicCommand(nCnt);
		}


		//火の魔法
		if (pGamePad->GetButtonTrigger(CDirectInput::B) == true && bMagic == false)
		{
			m_nMagicCommandType = (int)CMagicUI::Fire;
			//プレイヤーを敵の方向に向かせる
			PlayerFace(pos, frotY, bNearEnemy);
			nMotionType = CPlayer::N_MAGIC_ATTACK;
			bMagic = true;

			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

		}
		//氷の魔法
		if (pGamePad->GetButtonTrigger(CDirectInput::X) == true && bMagic == false)
		{
			m_nMagicCommandType = (int)CMagicUI::Ice;

			//プレイヤーを敵の方向に向かせる
			PlayerFace(pos, frotY, bNearEnemy);
			nMotionType = CPlayer::N_MAGIC_ATTACK;
			bMagic = true;
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

		}
		//回復魔法
		if (pGamePad->GetButtonTrigger(CDirectInput::A) == true && bMagic == false)
		{
			m_nMagicCommandType = (int)CMagicUI::Heal;
			//プレイヤーを敵の方向に向かせる
			PlayerFace(pos, frotY, bNearEnemy);
			nMotionType = CPlayer::N_MAGIC_ATTACK;
			bMagic = true;
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

		}
		//選択したコマンドを押した状態にする
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
//ロックオンの処理
//----------------------------
bool CPlayer_Controller::RockOn(D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//ロックオン判定を変える
	if (pGamePad->GetButtonTrigger(CDirectInput::R) == true)
	{
		CScene *pScene_E = CScene::GetScene(CScene::OBJTYPE_ENEMY);//一番最初の敵
		if (pScene_E != nullptr)
		{
			m_bRockOn = m_bRockOn ? false : true;

		}
		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SELECT);

	}

	//ロックオンしていなかったら一番近い敵を求める
	if (m_bRockOn==false)
	{
		CScene *pScene_E = CScene::GetScene(CScene::OBJTYPE_ENEMY);//一番最初の敵
		float fLength = 0.0f;//プレイヤーと敵の間の距離
		float fLength2 = 0.0f;//プレイヤーと敵の間の距離

		//一番近い敵の情報の保存用
		m_pNearEnemy = (CEnemy*)pScene_E; 
		//一番近い敵を求める
		while (pScene_E != NULL)
		{
			//今の敵の距離の長さを求める

			if (m_pNearEnemy != NULL)
			{
				D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
				D3DXVECTOR3 EnemyVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				EnemyVec = EnemyPos - pos;			//敵とプレイヤーのベクトル
				fLength = sqrtf((EnemyVec.z*EnemyVec.z) + (EnemyVec.x*EnemyVec.x));

			}
			if (pScene_E != NULL)
			{
				//次の敵の距離の長さを求める
				CScene *pSceneNext = pScene_E->GetSceneNext(pScene_E);
				if (pSceneNext != NULL)
				{
					CEnemy *pEnemyNext = (CEnemy*)pSceneNext;
					D3DXVECTOR3 EnemyPosNext = pEnemyNext->GetEnemyPos();
					D3DXVECTOR3 EnemyVecNext = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					EnemyVecNext = EnemyPosNext - pos;			//敵とプレイヤーのベクトル
					fLength2 = sqrtf((EnemyVecNext.z*EnemyVecNext.z) + (EnemyVecNext.x*EnemyVecNext.x));

					//今の敵が次の敵より遠かったら
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
			m_NearEnemyVec = EnemyPos - pos;			//敵とプレイヤーのベクトル

														//プレイヤーと敵の向きを求める
			float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//敵の向き

																   //カメラの取得
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
			//カメラの取得
			CCamera *pMapCamera = CRenderer::GetMapCamera();

			pMapCamera->SetRot({ fAng,Rot.y,0.0f });
			pMapCamera->SetMapCamera(pos);

			PosV.x = PosR.x + sinf(Rot.y)* sinf(fAng) * pCamera->GetLong();
			PosV.y = PosR.y + cosf(Rot.y)			   * pCamera->GetLong();
			PosV.z = PosR.z + sinf(Rot.y)* cosf(fAng) * pCamera->GetLong();
			pCamera->SetPosV(PosV);
			pCamera->SetRot({ fAng,Rot.y,0.0f });
			//ロックオン中に敵が死んだとき
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
//ロックオン用の画像の表示処理
//----------------------------
void CPlayer_Controller::DrawRockOn(D3DXVECTOR3 & pos)
{
}
//-----------------------------------------------------------------------------------------------
//ジャンプの処理(プレイヤーに今ジャンプできるかのbool(初期値はfalse)を持たして、第二引数に入れて)
//-----------------------------------------------------------------------------------------------
void CPlayer_Controller::Jump(float& fGravity, bool& bJump)
{
	CXInput *pXInput = CManager::GetXInput();
	//ジャンプの移動量に加算
	if (pXInput->GetButtonTrigger(XINPUT_GAMEPAD_A) == true && bJump == false)	//すぺーす
	{
		//重力を上にかける

		fGravity = 13.0f;
		bJump = true;
	}

}
//-----------------------------------------------------------------------------------------------
//重力の処理(プレイヤーに重力を減らす値(初期値は減らしたい正の数)を持たして、第三引数に入れて)
//-----------------------------------------------------------------------------------------------
void CPlayer_Controller::Gravity(D3DXVECTOR3 & pos, float& fGravity, const float& fGravitySpeed, bool & bJump)
{
	////重力加算
	if (m_fGravitySpeed > 0.0f)
	{

		fGravity -= m_fGravitySpeed;
		pos.y += m_fGravity;
	}
	//床についたかどうか
	if (pos.y <= 0.0f)
	{
		//ゲージを増やす
		pos.y = 0.0f;
		fGravity = 0.0f;
		bJump = false;
		//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WATER_FALL);
	}
}
//----------------------------
//一番近い敵を求める
//----------------------------
void CPlayer_Controller::LengthNearEnemy(D3DXVECTOR3 & pos)
{
}
//----------------------------
//一番プレイヤーが向いている方向に近い敵を求める
//----------------------------
void CPlayer_Controller::RotNearEnemy()
{
}
//----------------------------
//移動
//----------------------------
void CPlayer_Controller::TestPadMove(D3DXVECTOR3& pos, D3DXVECTOR3& rot,bool& bMove)
{
	//Xの移動量
	float fvec_x = 0.0f;
	//Zの移動量
	float fvec_z = 0.0f;
	float fLength = 0.0f;
	float fRot_Yx = 0.0f;
	float fRot_Yz = 0.0f;

	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	CXInput *pXInput = CManager::GetXInput();

	//前に進む

	if ((float)GamePad.lX >= MAX_DEAD_ZOON || (float)GamePad.lY >= MAX_DEAD_ZOON||
		(float)GamePad.lX <= -MAX_DEAD_ZOON || (float)GamePad.lY <= -MAX_DEAD_ZOON)
	{
		//移動モーションにする
		bMove = true;
		//スティックの傾きの長さを求める
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
		//待機モーションに戻る
		bMove = false;
		m_fSoundInterval = 1.3f;

		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

	}
	//ジャンプ

}
void CPlayer_Controller::SkillPadMove(D3DXVECTOR3 & pos, D3DXVECTOR3 & rot, bool & bMove, const bool& bNearEnemy)
{
	//Xの移動量
	float fvec_x = 0.0f;
	//Zの移動量
	float fvec_z = 0.0f;
	float fLength = 0.0f;
	float fRot_Yx = 0.0f;
	float fRot_Yz = 0.0f;

	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	CXInput *pXInput = CManager::GetXInput();

	//前に進む

	if ((float)GamePad.lX >= MAX_DEAD_ZOON || (float)GamePad.lY >= MAX_DEAD_ZOON ||
		(float)GamePad.lX <= -MAX_DEAD_ZOON || (float)GamePad.lY <= -MAX_DEAD_ZOON)
	{
		//移動モーションにする
		bMove = true;
		//スティックの傾きの長さを求める
		fLength = (float)sqrt(GamePad.lX * GamePad.lX + GamePad.lY * GamePad.lY);
		fLength = fLength / 1000.f;
		float fRot = atan2f((float)GamePad.lX, (float)GamePad.lY);

		//プレイヤーを敵の方向に向かせる
		if (m_pNearEnemy != nullptr&&bNearEnemy == true)
		{
			D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
			m_NearEnemyVec = EnemyPos - pos;			//敵とプレイヤーのベクトル

														//プレイヤーと敵の向きを求める
			float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//敵の向き
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
		//待機モーションに戻る
		bMove = false;
	}

}
//--------------------------------
//プレイヤーを敵の方向に向かせる
//--------------------------------
void CPlayer_Controller::PlayerFace(D3DXVECTOR3 & pos, float & frotY, const bool & bNearEnemy)
{
	if (m_pNearEnemy != nullptr&&bNearEnemy == true)
	{
		D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
		m_NearEnemyVec = EnemyPos - pos;			//敵とプレイヤーのベクトル

													//プレイヤーと敵の向きを求める
		float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//敵の向き
		frotY = fAng + D3DX_PI;
	}

}
//----------------------------
//移動
//----------------------------
void CPlayer_Controller::TestMove(D3DXVECTOR3& pos, D3DXVECTOR3& rot)
{
	//Xの移動量
	float fvec_x = 0.0f;
	//Zの移動量
	float fvec_z = 0.0f;
	float fLength = 0.0f;
	float fRot_Yx = 0.0f;
	float fRot_Yz = 0.0f;

	//キーボードの取得
	CInputKeyBoard *Key = CManager::GetInputKeyboard();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	//移動ボタンを押したら移動モーションをさせる
	if (Key->GetPress(DIK_W) == true ||
		Key->GetPress(DIK_A) == true ||
		Key->GetPress(DIK_S) == true ||
		Key->GetPress(DIK_D) == true)
	{
		//移動モーションにする

	}
	else
	{
		//待機モーションに戻る

	}
	//前に進む
	if (Key->GetPress(DIK_W) == true)
	{
		rot.y = D3DXToRadian(180.0f);
		fvec_x += sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z += cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		//右に進む
		if (Key->GetPress(DIK_D) == true)
		{
			rot.y = D3DXToRadian(225.0f);
		}
		//左に進む
		else if (Key->GetPress(DIK_A) == true)
		{
			rot.y = D3DXToRadian(135.0f);
		}
	}
	//後ろに進む
	else if (Key->GetPress(DIK_S) == true)
	{
		rot.y = D3DXToRadian(0.0f);
		fvec_x += -sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z += -cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		if (Key->GetPress(DIK_D) == true)
		{
			rot.y = D3DXToRadian(315.0f);
		}
		//左に進む
		else if (Key->GetPress(DIK_A) == true)
		{
			rot.y = D3DXToRadian(45.0f);
		}

	}
	//右に進む
	if (Key->GetPress(DIK_D) == true)
	{
		rot.y = D3DXToRadian(270.0f);
		fvec_x +=  cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z += -sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		//前に進む
		if (Key->GetPress(DIK_W) == true)
		{
			rot.y = D3DXToRadian(225.0f);
		}
		//後ろに進む
		else if (Key->GetPress(DIK_S) == true)
		{
			rot.y = D3DXToRadian(315.0f);
		}

	}
	//左に進む
	else if (Key->GetPress(DIK_A) == true)
	{
		rot.y = D3DXToRadian(90.0f);
		fvec_x += -cosf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		fvec_z +=  sinf(pCamera->GetRot().y)*PLAYER_MOVE_SPEED;
		//前に進む
		if (Key->GetPress(DIK_W) == true)
		{
			rot.y = D3DXToRadian(135.0f);

		}
		//後ろに進む
		else if (Key->GetPress(DIK_S) == true)
		{
			rot.y = D3DXToRadian(45.0f);
		}
	}

	//移動量があれば位置に移動量を加算する
	if (fvec_x != 0.0f || fvec_z != 0.0f)
	{
		//移動の長さを求める
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
//攻撃
//----------------------------
void CPlayer_Controller::Attack(const D3DXVECTOR3& pos, float& frotY, int& nComboType, bool& bAttack, bool& bAttackNext,const bool& bNearEnemy)
{
	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//攻撃判定に変える
	if (pGamePad->GetButtonTrigger(CDirectInput::B) == true && bAttack ==false)
	{
		//プレイヤーを敵の方向に向かせる
		if (m_pNearEnemy != nullptr&&bNearEnemy == true)
		{
			D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetEnemyPos();
			m_NearEnemyVec = EnemyPos - pos;			//敵とプレイヤーのベクトル

														//プレイヤーと敵の向きを求める
			float fAng = atan2(m_NearEnemyVec.x, m_NearEnemyVec.z);//敵の向き
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
//重力
//----------------------------
void CPlayer_Controller::Gravity(D3DXVECTOR3& pos)
{
	////重力加算
	if (m_fGravitySpeed > 0.0f)
	{

		m_fGravity -= m_fGravitySpeed;
		pos.y += m_fGravity;
	}
	//床についたかどうか
	if (pos.y <= 0.0f)
	{
		//ゲージを増やす
		pos.y =0.0f;
		m_fGravity = 0.0f;
		m_bJumpLock = false;
		//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WATER_FALL);
	}
}
//-----------------------------------
//マップの目的地を可視化する処理
//-----------------------------------
bool CPlayer_Controller::bPorpose(void)
{
	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	if (pGamePad->GetButtonTrigger(CDirectInput::R3) == true)
	{
		m_bPorpose = m_bPorpose ? false : true;
	}
	return m_bPorpose;
}
//-------------------------------
//プレイヤーのカメラのコントロール
//--------------------------------
void CPlayer_Controller::CameraControl(const D3DXVECTOR3 pos)
{
	if (m_bRockOn == false)
	{
		//DirectInputのゲームパッドの取得
		CDirectInput *pGamePad = CManager::GetDirectInput();
		//ゲームパッドのボタン情報の取得
		DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
		//カメラの取得
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
		//カメラのズーム
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
		//カメラの取得
		CCamera *pMapCamera = CRenderer::GetMapCamera();
		pMapCamera->SetMapCamera(pos);
		pMapCamera->SetRot(Rot);

		pCamera->SetPosR(PosR);
		pCamera->SetPosV(PosV);
		pCamera->SetRot(Rot);

	}
}
