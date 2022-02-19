#include "main.h"
#include "manager.h"
#include "Renderer.h"
#include "player.h"
#include "model.h"
#include "motion.h"
#include "layer_structure.h"
#include "score.h"
#include "keyboard.h"
#include "game.h"
#include "collision.h"
#include "fade.h"
#include "camera.h"
#include "stage.h"
#include "field.h"
#include "model_spawner.h"
#include "Enemy.h"
#include "shadow.h"
#include "XInput.h"
#include "sound.h"
#include "keyboard.h"
#include "player_controller.h"
#include "map_polygon.h"
#include "goal.h"
#include "effect.h"
#include "swordeffect.h"
#include "commandUI.h"
#include "magic_fire.h"
#include "billboard.h"
#include "gauge.h"
#include "Polygon.h"
#include "magic_skill02.h"
#include "magic_heel.h"
#include "magic_skill01.h"
#include "Particle.h"
#include "wall.h"
#include "enemyspawner.h"
#include "circlegauge.h"
#include "directinput.h"
//------------------------------------
//マクロ定義
//------------------------------------
#define MAX_DELAY_TIME (30)
#define MAX_MAGIC_SHOT_TIME (2.0)
#define MAX_PLAYER_JUMP (1)//最大のジャンプ数
#define MAX_PLAYER_PARTS (16)
#define PLAYER_SIZE (20.0)
#define SWORDEFFECT_LONG (16)
#define PLAYER_RANGE (700.0)		//プレイヤーと敵の近さを図る範囲
#define PLAYER_ROCKON_SIZE (5.0)	//ロックオン画像のサイズ
#define PLAYER_POWER (10)			//攻撃力
#define PLAYER_HIT_MAXTIME (50)		//無敵判定の時間
#define PLAYER_DODGE_TIME (20.0f)		//回避の時間
#define PLAYER_DODGE_SPEED (12.0f)		//回避の速度
#define PLAYER_ATTACK_SPEED (15.0f)		//攻撃の移動速度

//--------------------------
//コンストラクト
//--------------------------
CPlayer::CPlayer(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(m_pMotion, NULL, sizeof(m_pMotion));
	memset(m_pModel, NULL, sizeof(m_pModel));
	memset(m_pSwordEffect, NULL, sizeof(m_pSwordEffect));
	m_fMagicShotDelayMax = MAX_MAGIC_SHOT_TIME;
	m_bAttack = false;
	m_bAttackNext = true;
	m_nMaxDelatTime = MAX_DELAY_TIME;
	m_bDelay = false;
	m_Magic.m_bRetryMagic = false;
	m_pRockOnPolygon = nullptr;
	m_nCommandType = 0;
	m_nMagicCommandType = 0;
	m_bDraw = true;
	m_bHit = false;
	m_bHitStop = false;
	m_State = STATE_NOWN;
	m_bDeth = false;
	m_bLockOn = false;
	m_bDodge = false;
	m_bBeginDodge = false;
	m_bEndDodge = false;
	m_fDodgeAddSpeed = 1.0f;
	m_fDodgeTimer = 0.0f;
	m_fDodgerot = 0.0f;
}
//--------------------------
//デストラクト
//----------------------------
CPlayer::~CPlayer()
{

}
//----------------------------------------------
//インスタンス生成
//----------------------------------------------
CPlayer *CPlayer::Create()
{
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;
	pPlayer->Init();
	return pPlayer;
}
//---------------------------------------------
//初期化
//---------------------------------------------
HRESULT CPlayer::Init()
{
	//プレイヤーのコントローラーの呼び出し
	m_pController = new CPlayer_Controller;
	//ファイル読み込み
	//階層構造の設定
	m_pLayer = new CLayer_Structure;
	m_pLayer->SetLayer_Structure("data/TEXT/PlayerParts000.txt", &m_pModel[0],CModel::TYPE_PLAYER);
	//武器の当たり判定用の箱の読み込み
	FILE *pFile = fopen("data/TEXT/WeaponSet000.txt", "r");
	char string[6][255];
	D3DXVECTOR3 PartsPos, PartsRot;	//各パーツの位置と向き
	int nCntModel = 0;
	int nNumModel = 0;
	int nCntWeapon = 0;
	int nCntCollision = 0;
	int nParent = 0;
	m_bNearEnemy = false;
	if (pFile != NULL)
	{
		while (true)
		{
			//一単語を読み込む
			fscanf(pFile, "%s", &string[0]);

			while (strcmp(string[0], "COLLISIONSET") == 0)
			{
				fscanf(pFile, "%s", &string[1]);

				//位置
				if (strcmp(string[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsPos.x, &PartsPos.y, &PartsPos.z);
				}
				//位置
				if (strcmp(string[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsRot.x, &PartsRot.y, &PartsRot.z);
				}
				//位置
				if (strcmp(string[1], "END_COLLISIONSET") == 0)
				{
					m_pCollision = CModel::Create(PartsPos, PartsRot, 0, CModel::TYPE_OBJECT);
					m_pCollision->SetParent(m_pModel[15]);
					m_pCollision->SetDraw(false);
					break;
				}
			}
			while (strcmp(string[0], "EFFECTSET") == 0)
			{
				fscanf(pFile, "%s", &string[1]);

				//位置
				if (strcmp(string[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsPos.x, &PartsPos.y, &PartsPos.z);
				}
				//位置
				if (strcmp(string[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsRot.x, &PartsRot.y, &PartsRot.z);
				}
				//位置
				if (strcmp(string[1], "END_EFFECTSET") == 0)
				{
					m_pSwordEffect[nCntModel] = CModel::Create(PartsPos, PartsRot, 0, CModel::TYPE_OBJECT);
					m_pSwordEffect[nCntModel]->SetParent(m_pModel[15]);
					m_pSwordEffect[nCntModel]->SetDraw(false);
					nCntModel++;
					break;
				}
			}

			if (strcmp(string[0], "END_SCRIPTS") == 0)
			{
				break;
			}

		}
	}

	//モーションデータの読み込み
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pMotion[nCnt] == NULL)
		{
			m_pMotion[nCnt] = new CMotion;
			m_nMotionType[nCnt] = 0;

		}
	}
	m_pMotion[TYPE_SWORD]->MotionLoad("data/TEXT/PlayerMotion.txt");
	m_pos = D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z);
	m_pMapPolygon = CMap_Polygon::Create(m_pos, D3DXVECTOR3(6.0f, 0.0f, 12.0f), CTexture::Map_Player);
	m_pMapPolygon->SetRot(m_rot.y);

	m_move = -2.0f;
	m_fSoundInterval = 1.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_fGravity = 0.0f;//重力度
	m_bland = true;
	m_nWeaponType = 0;
	m_bMotionStop = false;
	m_bAttack = false;
	m_bMove = false;
	pShadow = CShadow::Create(m_pos, PLAYER_SIZE, CTexture::SHADOW);
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_LastSwordpos[nCnt] = D3DXVECTOR3(m_pSwordEffect[nCnt]->GetMtxWorld().m[3][0], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][1], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][2]);
	}
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_Swordpos[nCnt] = D3DXVECTOR3(m_pSwordEffect[nCnt]->GetMtxWorld().m[3][0], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][1], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][2]);
	}
	m_pSwordLocus =CSwordEffect::Create(m_Swordpos[0], m_Swordpos[1], m_pCollision->GetPos(), SWORDEFFECT_LONG,
		CTexture::SwordEffect);
	return S_OK;
}
///-------------------------------------------
//破棄
//------------------------------------------
void CPlayer::Uninit()
{
	CManager::GetSound()->StopSound(CManager::GetSound()->SOUND_LABEL_SE_WALK);

	for (int nCntParts = 0; nCntParts < m_pLayer->GetMaxParts(); nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			m_pModel[nCntParts]->Uninit();
			m_pModel[nCntParts] = NULL;
		}
	}
	if (pShadow)
	{
		pShadow->Uninit();
		delete pShadow;
		pShadow = NULL;
	}
	//剣の軌跡の削除
	if (m_pSwordLocus)
	{
		m_pSwordLocus->Uninit();
		delete m_pSwordLocus;
		m_pSwordLocus = NULL;
	}
	Release();
}
//-------------------------------------------
//更新
//-------------------------------------------
void CPlayer::Update()
{
	CXInput *pGamePad = CManager::GetXInput();
	CInputKeyBoard *pKeyBoard = CManager::GetInputKeyboard();
	if (m_bDeth == false)
	{
		//プレイヤーの移動処理
		if (!m_bAttack && !m_bDelay && !m_Magic.m_bMagic&&!m_bDodge)
		{
			m_pController->TestMove(m_pos, m_rot);
			m_pController->TestPadMove(m_pos, m_rot, m_bMove);

		}
		//回避の処理
		if (!m_bAttack && !m_bSkill&&!m_Magic.m_bMagic&&m_State== STATE_NOWN)
		{
			Dodge();

		}
		//ロックオンしていなかったら敵と近いかを調べる
		if (m_pController->GetNearEnemy() == nullptr)
		{
			m_bSearchStop = true;
			if (m_bLockOn == false)
			{
				if (m_pRockOnPolygon != nullptr)
				{
					m_pRockOnPolygon->Uninit();
					delete m_pRockOnPolygon;
					m_pRockOnPolygon = nullptr;
				}

			}

		}
		else
		{
			m_bSearchStop = false;

			NearEnemySearch(m_pController->GetNearEnemy()->GetEnemyPos());
		}

		m_bLockOn = m_pController->RockOn(m_pos, m_rot);

		//ロックオンの処理
		//ロックオンだったら色を変える
		if (m_bLockOn == true)
		{
			if (m_pRockOnPolygon != nullptr)
			{
				m_pRockOnPolygon->SetColor({ 1.0f,0.3f,0.3f,1.0f});
			}
		}
		else
		{
			if (m_pRockOnPolygon != nullptr)
			{
				m_pRockOnPolygon->SetColor({ 1.0,1.0,1.0,1.0 });
			}

		}


		//マップ表示用の画像をプレイヤーの回転に合わせる
		m_pMapPolygon->SetRot(m_rot.y);

		bool bDraw = m_pController->bPorpose();


		//目的地用の画像をプレイヤーの位置に合わせる
		m_pMapPolygon->SetPos(m_pos.x, m_pos.z);


		//移動したら移動モーションに変える 
		if (m_bSkill == false&&m_bDodge==false)
		{
			if (m_bMove && !m_bDelay)
			{
				m_nMotionType[m_nWeaponType] = N_MOVE;
				m_bAttackNext = true;

			}
			else if (!m_bMove && !m_bDelay && !m_Magic.m_bMagic&&!m_bDodge)
			{
				m_nMotionType[m_nWeaponType] = N_NEUTRAL;
			}

		}

		//ディレイがオンになったら
		if (m_bDelay)
		{
			m_nDelayTimer++;
			//最終コンボじゃなければ次のコンボを出せるようにする
			if (m_nAttackType[0] != CPlayer_Controller::COMBO_3)
			{
				m_bAttackNext = true;
			}
			else
			{
				m_bAttackNext = false;

			}
			//指定された攻撃タイプによってモーションを変える

			if (m_nDelayTimer >= m_nMaxDelatTime)
			{
				m_bDelay = false;
				m_nDelayTimer = 0;
				if (m_nAttackType[0] == CPlayer_Controller::COMBO_3)
				{
					m_bAttackNext = true;
				}
				m_nAttackType[0] = 0;
			}
		}
		m_pController->Gravity(m_pos);
		pShadow->SetPos(m_pos.x, m_pos.z, D3DXVECTOR3(PLAYER_SIZE, 0.0f, PLAYER_SIZE));
		if (m_bDodge == false)
		{
			//コマンドセレクト
			if (m_State == STATE_NOWN)
			{
				//攻撃に関する処理
				PlayerAttack();

			}
			//必殺技
			//CPがたまったら
			int nCP = CManager::GetGame()->GetCPGauge()->GetGaugeValue();
			if (nCP >= PLAYER_CP&&m_bDelay==false)
			{
				bool bSkill = m_pController->SpecalSkill();
				if (bSkill == true && m_bSkill == false)
				{

					C_Magic_Skill01::Create(m_pos, m_rot);
				}
			}

		}
		//設置物との当たり判定
		CCollision *pCollision = new CCollision;
		float ModelPosY = 0.0f;
		float fShadowY = 0.0f;
		CScene *pScene_E = CScene::GetScene(OBJTYPE_ENEMY);

		//敵とのの当たり判定
		while (pScene_E != NULL)
		{
			if (pScene_E != NULL)
			{
				CEnemy *pEnemy = (CEnemy*)pScene_E;
				pCollision->CollisionObjectEnemy((CEnemy*)pScene_E, &m_pos, &m_lastpos, 30.0f);
				//攻撃判定
				bool bHitAttack = pEnemy->bHitAttack();
				if (bHitAttack == false && m_bAttack == true)
				{

					bool bhit = pCollision->CollisionWeapon((CEnemy*)pScene_E, m_pSwordLocus->GetS_Pos(), 100.0f);

					pEnemy->SetHit(bhit);
					if (bhit == true)
					{
						CManager::GetGame()->GetMPGauge()->SetGauge(-1);
						//CPを増やす
						CManager::GetGame()->GetCPGauge()->SetGauge(-5);
						pEnemy->SetbDamage(true);
						pEnemy->AddLife(-PLAYER_POWER);
						CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);

						std::random_device random;	// 非決定的な乱数生成器
						std::mt19937_64 mt(random());            // メルセンヌ・ツイスタの64ビット版、引数は初期シード
						std::uniform_real_distribution<> randAng(-D3DX_PI, D3DX_PI);
						float fAng = randAng(mt);
						CEffect::Create({ pEnemy->GetEnemyPos().x,pEnemy->GetEnemyPos().y + 40.0f,pEnemy->GetEnemyPos().z }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,0.0f },
						{ 1.0,1.0,1.0,0.5f }, false, 0.0f, 0.01f, true, CTexture::HitEffect, fAng, true);
						CEffect::Create({ pEnemy->GetEnemyPos().x,pEnemy->GetEnemyPos().y + 40.0f,pEnemy->GetEnemyPos().z }, { 0.0f,0.0f,0.0f }, { 0.4f,0.2f,0.0f },
						{ 1.0,1.0,1.0,0.7f }, false, 0.0f, 0.015f, true, CTexture::HitEffect, fAng, true);

						CEffect::Create({ pEnemy->GetEnemyPos().x,pEnemy->GetEnemyPos().y + 40.0f,pEnemy->GetEnemyPos().z }, { 0.0f,0.0f,0.0f }, { 1.5f,1.0f,0.0f },
						{ 1.0f,0.5f,0.0f,1.0f }, false, 0.0f, 0.03f, true, CTexture::Effect, fAng, false, true);
						CEffect::Create({ pEnemy->GetEnemyPos().x,pEnemy->GetEnemyPos().y + 40.0f,pEnemy->GetEnemyPos().z }, { 0.0f,0.0f,0.0f }, { 1.5f,1.0f,0.0f },
						{ 1.0f,0.5f,0.0f,1.0f }, false, 0.0f, 0.03f, true, CTexture::Effect, fAng, false, true);

					}
				}
			}
			pScene_E = pScene_E->GetSceneNext(pScene_E);
		}
		CScene *pScene_Wall = CScene::GetScene(OBJTYPE_WALL);
		//壁との当たり判定
		while (pScene_Wall != nullptr)
		{
			CWall *pWall = (CWall*)pScene_Wall;
			CScene *pNext_Wall = CScene::GetSceneNext(pScene_Wall);
			//当たり判定
			bool bHit = pWall->TestCollision(&m_pos, &m_lastpos, 50.0f);
			pScene_Wall = pNext_Wall;

		}

		//攻撃したら剣の軌跡を表示
		m_pSwordLocus->SetDraw(m_bAttack);

		m_pCollision->Update();
		PlayerHit();
		//m_pCollision->SetPos(m_pos);
		CCamera *pCamera = CRenderer::GetCamera();
		pCamera->SetPlayerCamera(m_pos);
		CScene::SetPos(m_pos);
		//プレイヤーにカメラを追従させる
		m_pController->CameraControl(m_pos);

		//モーションの再生
		switch (m_State)
		{
		case STATE_NOWN:
			//if (m_bDelay == false)
			//{
			//	m_Magic.m_bMagic = false;

			//}
			if (m_Magic.m_bMagic)
			{
				m_pMotion[m_nWeaponType]->PlayMotion(MAX_PLAYER_PARTS, &m_pModel[0], m_nMotionType[m_nWeaponType],
					m_nMotionLastType[m_nWeaponType], m_bMotionStop, m_Magic.m_bMagic, m_bDelay, m_Magic.m_bRetryMagic);

			}
			else
			{
				m_pMotion[m_nWeaponType]->PlayMotion(MAX_PLAYER_PARTS, &m_pModel[0], m_nMotionType[m_nWeaponType],
					m_nMotionLastType[m_nWeaponType], m_bMotionStop, m_bAttack, m_bDelay, false);

			}
			break;

		case STATE_MAGIC:
			//if (m_bDelay == false)
			//{
			//	m_bAttack = false;

			//}
			//魔法関係の処理
			if (m_bSkill == false&& m_bDodge==false)
			{
				PlayerMagic();

			}
			if (m_bAttack)
			{
				m_pMotion[m_nWeaponType]->PlayMotion(MAX_PLAYER_PARTS, &m_pModel[0], m_nMotionType[m_nWeaponType],
					m_nMotionLastType[m_nWeaponType], m_bMotionStop, m_bAttack, m_bDelay, false);
			}
			else
			{
				m_pMotion[m_nWeaponType]->PlayMotion(MAX_PLAYER_PARTS, &m_pModel[0], m_nMotionType[m_nWeaponType],
					m_nMotionLastType[m_nWeaponType], m_bMotionStop, m_Magic.m_bMagic, m_bDelay, m_Magic.m_bRetryMagic);

			}
			break;
		}
		
	}
	else
	{
		m_nMotionType[m_nWeaponType] = N_DETH;
		m_pMotion[m_nWeaponType]->NoLoopPlayMotion(MAX_PLAYER_PARTS, &m_pModel[0], m_nMotionType[m_nWeaponType],
			m_nMotionLastType[m_nWeaponType], m_bGameStop);
	}


}

void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
	//各パーツのワールドマトリックスの初期化gtryg
	D3DXMatrixIdentity(&m_mtxWorld);      
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	//向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデル生成
	if (m_bDraw == true)
	{
		for (int nCntParts = 0; nCntParts < m_pLayer->GetMaxParts(); nCntParts++)
		{
			m_pModel[nCntParts]->Draw();
		}
	}

	m_pCollision->Draw();
	m_pSwordEffect[0]->Draw();
	m_pSwordEffect[1]->Draw();


	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_Swordpos[nCnt] = D3DXVECTOR3(m_pSwordEffect[nCnt]->GetMtxWorld().m[3][0], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][1], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][2]);
	}
	m_pSwordLocus->SetSwordPos(m_Swordpos[0], m_Swordpos[1]);
	m_pSwordLocus->SetPos(D3DXVECTOR3(m_pModel[15]->GetMtxWorld().m[3][0], m_pModel[15]->GetMtxWorld().m[3][1], m_pModel[15]->GetMtxWorld().m[3][2]));
	m_pSwordLocus->SetLastSwordPos(m_LastSwordpos[0], m_LastSwordpos[1]);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_LastSwordpos[nCnt] = D3DXVECTOR3(m_pSwordEffect[nCnt]->GetMtxWorld().m[3][0], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][1], m_pSwordEffect[nCnt]->GetMtxWorld().m[3][2]);
	}

	//m_pMotion[m_nWeaponType]->Drawtext();
	//プレイヤーの移動処理
	//m_pController->Drawtext();
	//Drawtext();
	//前回の位置情報の保存
	m_lastpos = m_pos;

}
void CPlayer::Drawtext()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[3000];
	int nNum = 0;

	nNum = sprintf(&str[0], "\n\n 壁情報 \n");

		nNum += sprintf(&str[nNum], " [modelPos] X%.2f,Y%.2f,Z%.2f \n", m_pos.x, m_pos.y, m_pos.z);
		nNum += sprintf(&str[nNum], " [modelPos] X%.2f,Y%.2f,Z%.2f \n", m_lastpos.x, m_lastpos.y, m_lastpos.z);

	//CScene *pScene_Wall = CScene::GetScene(OBJTYPE_WALL);
	//for (int nCnt = 0; nCnt < MAX_PLAYER_PARTS; nCnt++)
	//{
	//	nNum += sprintf(&str[nNum], " [modelPos] X%.2f,Y%.2f,Z%.2f \n", m_pModel[nCnt]->GetPos().x, m_pModel[nCnt]->GetPos().y, m_pModel[nCnt]->GetPos().z);
	//	nNum += sprintf(&str[nNum], " [modelPos] X%.2f,Y%.2f,Z%.2f \n", m_pModel[nCnt]->GetLayerPos().x, m_pModel[nCnt]->GetLayerPos().y, m_pModel[nCnt]->GetLayerPos().z);

	//}

	//nNum += sprintf(&str[nNum], " [nMotionType] %d \n", m_nMotionType[m_nWeaponType]);
	//nNum += sprintf(&str[nNum], " [bAttack] %d \n", m_bAttack);
	//nNum += sprintf(&str[nNum], " [bAttackNext] %d \n", m_bAttackNext);
	//nNum += sprintf(&str[nNum], " [nAttackType] %d \n", m_nAttackType[0]);
	//nNum += sprintf(&str[nNum], " [bMotionStop] %d \n", m_bMotionStop);
	//nNum += sprintf(&str[nNum], " [bDelay] %d \n", m_bDelay);
	//nNum += sprintf(&str[nNum], " [nDelayTimer] %d \n", m_nDelayTimer);
	//nNum += sprintf(&str[nNum], " [ColliPos] X%.2f,Y%.2f,Z%.2f \n", m_pCollision->GetPos().x, m_pCollision->GetPos().y, m_pCollision->GetPos().z);
	//nNum += sprintf(&str[nNum], " [m_Magic.m_bMagic] %d \n", m_Magic.m_bMagic);
	//for (int nCnt = 0; nCnt < 2; nCnt++)
	//{
	//	nNum += sprintf(&str[nNum], " [SwordEffectStartPos] X%.2f,Y%.2f,Z%.2f \n", m_pSwordLocus->GetStartPos().x, m_pSwordLocus->GetStartPos().y, m_pSwordLocus->GetStartPos().z);
	//	nNum += sprintf(&str[nNum], " [SwordEffectEndPos] X%.2f,Y%.2f,Z%.2f \n", m_pSwordLocus->GetEndPos().x, m_pSwordLocus->GetEndPos().y, m_pSwordLocus->GetEndPos().z);
	//}



	LPD3DXFONT pFont = CManager::GetRenderer()->GetFont();
	// テキスト描画
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
//--------------------------------------
//回避の処理
void CPlayer::Dodge()
{
	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	if (m_bDodge == false&& m_Magic.m_bMagic==false&&m_bAttack==false&& m_bDelay==false)
	{
		if ((float)GamePad.lX >= MAX_DEAD_ZOON || (float)GamePad.lY >= MAX_DEAD_ZOON ||
			(float)GamePad.lX <= -MAX_DEAD_ZOON || (float)GamePad.lY <= -MAX_DEAD_ZOON)
		{
			float fRot = atan2f((float)GamePad.lX, (float)GamePad.lY);

			m_fDodgerot = -(fRot - pCamera->GetRot().x);
			m_rot.y = m_fDodgerot;
		}
		else
		{
			m_fDodgerot = m_rot.y;
		}

	}
	if (pGamePad->GetButtonTrigger(CDirectInput::X)&& m_bDodge==false && m_bDelay == false)
	{
		m_bDodge = true;
	}

	//回避の判定がオンだったら
	if (m_bDodge)
	{
		m_nMotionType[m_nWeaponType] = N_DODGE;

		m_fDodgeTimer++;

		if (m_fDodgeTimer >= PLAYER_DODGE_TIME)
		{
			m_fDodgeTimer = 0.0f;
			m_bDodge = false;
		}
		else
		{
			m_pos.x -= sinf(m_fDodgerot)*PLAYER_DODGE_SPEED;
			m_pos.z -= cosf(m_fDodgerot)*PLAYER_DODGE_SPEED;

		}

	}
}
bool  CPlayer::bColision()
{
	return 0;
}
//----------------------------------------
//魔法に関する処理
//----------------------------------------
void CPlayer::PlayerMagic()
{
	//まほうコマンドを選択していたら
	if (!m_Magic.m_bMagic && m_State == STATE_MAGIC)
	{
		//ボタンを押して魔法を放つ
		bool bStop = false;
		CGauge *pGauge = CManager::GetGame()->GetMPGauge();

		switch (m_nMagicCommandType)
		{
		case CCommandUI::Fire:
			if (pGauge->GetGaugeValue() < FIRE_MP)
			{
				bStop = true;
			}

			break;
		case CCommandUI::Ice:
			if (pGauge->GetGaugeValue() < MAGIC_SKILL02_MP)
			{
				bStop = true;
			}
			break;
		case CCommandUI::Heal:
			if (pGauge->GetGaugeValue() < HEEL_MP)
			{
				bStop = true;
			}
			break;
		}
		if (bStop == false)
		{
			m_nMagicCommandType = m_pController->MagicAttack(m_pos, m_rot.y, m_nMotionType[m_nWeaponType], m_Magic.m_bMagic, m_bNearEnemy, m_nMagicCommandType);

		}

		if (m_Magic.m_bMagic&&m_bDelay == false)
		{
			m_bMagicShot = true;

			m_bMove = false;
		}
	}

	//魔法ボタンを押したらちょっと後にポリゴンを出す
	if (m_bMagicShot == true )
	{
		m_fMagicShotDelayCnt++;
		if (m_fMagicShotDelayCnt >= m_fMagicShotDelayMax)
		{
			m_fMagicShotDelayCnt = 0.0f;
			//魔法のタイプごとに出す魔法を変える（ヒール以外）
			switch (m_nMagicCommandType)
			{
			case CCommandUI::Fire:
				if (m_pController->GetNearEnemy()!=nullptr&&m_bNearEnemy)
				{
					C_Magic_Fire::Create(m_Swordpos[0], m_rot,true, m_pController->GetNearEnemy(),80.0f);
					C_Magic_Fire::Create(m_Swordpos[0], m_rot, true, m_pController->GetNearEnemy(),-80.0f);

				}
				else
				{
					C_Magic_Fire::Create(m_Swordpos[0], m_rot,false,nullptr,50.0f);
					C_Magic_Fire::Create(m_Swordpos[0], m_rot, false, nullptr, -50.0f);

				}
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_FIRE);

				break;
			case CCommandUI::Ice:
				C_Magic_Skill02::Create(m_pos, m_rot, 2.0f);
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ICE);

				break;
			case CCommandUI::Heal:
				C_Magic_Heel::Create(m_pos);
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_HEEL);

				break;
			}
			m_bMagicShot = false;
		}
	}




}
//-------------------------------------------
//攻撃に関する処理
//-------------------------------------------
void CPlayer::PlayerAttack()
{
	//ディレイが入ってないときに攻撃がオンだったら１コンボ目を出す
	if (!m_bDelay&&m_bAttack)
	{
		switch (m_nAttackType[0])
		{
		case CPlayer_Controller::COMBO_1:
			m_nMotionType[m_nWeaponType] = N_ATTACK_1;
			m_pos.x -= sinf(m_rot.y)*PLAYER_ATTACK_SPEED*0.5f;
			m_pos.z -= cosf(m_rot.y)*PLAYER_ATTACK_SPEED*0.5f;

			break;
		}
	}
	//攻撃判定がオンになってたら連撃のモーションにする
	if (m_bAttack)
	{
		switch (m_nAttackType[0])
		{
		case CPlayer_Controller::COMBO_2:
			m_nMotionType[m_nWeaponType] = N_ATTACK_2;
			m_pos.x -= sinf(m_rot.y)*PLAYER_ATTACK_SPEED*0.1f;
			m_pos.z -= cosf(m_rot.y)*PLAYER_ATTACK_SPEED*0.1f;

			break;
		case CPlayer_Controller::COMBO_3:
			m_nMotionType[m_nWeaponType] = N_ATTACK_3;
			m_pos.x -= sinf(m_rot.y)*PLAYER_ATTACK_SPEED*0.1f;
			m_pos.z -= cosf(m_rot.y)*PLAYER_ATTACK_SPEED*0.1f;

			break;
		}
	}
	//攻撃をしていない状態だったら
	if (!m_bAttack)
	{
		//攻撃処理
		//次の攻撃ができるようになっていたら
		if (m_bAttackNext)
		{
			m_pController->Attack(m_pos, m_rot.y, m_nAttackType[0], m_bAttack, m_bAttackNext, m_bNearEnemy);

			//攻撃したら連撃の遅延を０にする
			if (m_bAttack)
			{

				m_nDelayTimer = 0;
			}
		}
	}

}
//----------------------------------------
//近い敵を探す処理
//----------------------------------------
void CPlayer::NearEnemySearch(D3DXVECTOR3 Enemy)
{
	D3DXVECTOR3 EnemyVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EnemyVec = Enemy - m_pos;			//敵とプレイヤーのベクトル
	float fLength = sqrtf((EnemyVec.z*EnemyVec.z) + (EnemyVec.x*EnemyVec.x));
	//敵が特定の範囲に入ったら
	if (m_bSearchStop == false)
	{
		if (fLength <= PLAYER_RANGE && m_pController->GetNearEnemy() != nullptr)
		{
			//敵と近いです
			m_bNearEnemy = true;
			if (m_pRockOnPolygon == nullptr)
			{
				m_pRockOnPolygon = nullptr;
				D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
				if (m_pController->GetNearEnemy()->GetModel(0) != nullptr)
				{
					pos = { m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._41 ,
						m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._42 ,
						m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._43 };
				}

				m_pRockOnPolygon = CBillboard::Create(pos, { PLAYER_ROCKON_SIZE ,PLAYER_ROCKON_SIZE ,0.0f },
					CTexture::RockOn);
			}
			else
			{
				if (m_pController->GetNearEnemy() != nullptr)
				{
					if (m_pController->GetNearEnemy()->GetModel(0) != nullptr)
					{
						m_pRockOnPolygon->Setpos({ m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._41,
							m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._42,
							m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._43 }, { PLAYER_ROCKON_SIZE ,PLAYER_ROCKON_SIZE ,0.0f });
					}
				}
			}

		}
		else
		{
			//敵と遠いです
			m_bNearEnemy = false;
			if (m_bLockOn == false)
			{
				if (m_pRockOnPolygon != nullptr)
				{
					m_pRockOnPolygon->Uninit();
					delete m_pRockOnPolygon;
					m_pRockOnPolygon = nullptr;
				}

			}
			else
			{
				if (m_pController->GetNearEnemy() != nullptr&&m_pRockOnPolygon!=nullptr)
				{
					if (m_pController->GetNearEnemy()->GetModel(0) != nullptr)
					{
						m_pRockOnPolygon->Setpos({ m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._41,
							m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._42,
							m_pController->GetNearEnemy()->GetModel(0)->GetMtxWorld()._43 }, { PLAYER_ROCKON_SIZE ,PLAYER_ROCKON_SIZE ,0.0f });
					}
				}

			}
		}

	}
}
//-----------------------------------
//プレイヤーが敵の攻撃に当たった時(m_bHitをtrueにしないと動かない)
//-----------------------------------
void CPlayer::PlayerHit()
{
	//攻撃が当たった時の処理が終わるまで攻撃が無敵判定にする
	if (m_bHit==true && m_bHitStop == false)
	{
		//これがtueになったら攻撃を受けない
		m_bHitStop = true;
		m_bHit = false;
	}
	//無敵判定だったら
	else if (m_bHitStop == true)
	{
		//無敵時間のカウント
		m_nHitTime++;
		//無敵時間が一定の値を超えたら無敵時間を解除
		if (m_nHitTime>= PLAYER_HIT_MAXTIME)
		{
			m_nHitTime = 0;
			//無敵時間解除
			m_bHitStop = false;
		}
	}

}
