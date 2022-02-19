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
//�}�N����`
//------------------------------------
#define MAX_DELAY_TIME (30)
#define MAX_MAGIC_SHOT_TIME (2.0)
#define MAX_PLAYER_JUMP (1)//�ő�̃W�����v��
#define MAX_PLAYER_PARTS (16)
#define PLAYER_SIZE (20.0)
#define SWORDEFFECT_LONG (16)
#define PLAYER_RANGE (700.0)		//�v���C���[�ƓG�̋߂���}��͈�
#define PLAYER_ROCKON_SIZE (5.0)	//���b�N�I���摜�̃T�C�Y
#define PLAYER_POWER (10)			//�U����
#define PLAYER_HIT_MAXTIME (50)		//���G����̎���
#define PLAYER_DODGE_TIME (20.0f)		//����̎���
#define PLAYER_DODGE_SPEED (12.0f)		//����̑��x
#define PLAYER_ATTACK_SPEED (15.0f)		//�U���̈ړ����x

//--------------------------
//�R���X�g���N�g
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
//�f�X�g���N�g
//----------------------------
CPlayer::~CPlayer()
{

}
//----------------------------------------------
//�C���X�^���X����
//----------------------------------------------
CPlayer *CPlayer::Create()
{
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer;
	pPlayer->Init();
	return pPlayer;
}
//---------------------------------------------
//������
//---------------------------------------------
HRESULT CPlayer::Init()
{
	//�v���C���[�̃R���g���[���[�̌Ăяo��
	m_pController = new CPlayer_Controller;
	//�t�@�C���ǂݍ���
	//�K�w�\���̐ݒ�
	m_pLayer = new CLayer_Structure;
	m_pLayer->SetLayer_Structure("data/TEXT/PlayerParts000.txt", &m_pModel[0],CModel::TYPE_PLAYER);
	//����̓����蔻��p�̔��̓ǂݍ���
	FILE *pFile = fopen("data/TEXT/WeaponSet000.txt", "r");
	char string[6][255];
	D3DXVECTOR3 PartsPos, PartsRot;	//�e�p�[�c�̈ʒu�ƌ���
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
			//��P���ǂݍ���
			fscanf(pFile, "%s", &string[0]);

			while (strcmp(string[0], "COLLISIONSET") == 0)
			{
				fscanf(pFile, "%s", &string[1]);

				//�ʒu
				if (strcmp(string[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsPos.x, &PartsPos.y, &PartsPos.z);
				}
				//�ʒu
				if (strcmp(string[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsRot.x, &PartsRot.y, &PartsRot.z);
				}
				//�ʒu
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

				//�ʒu
				if (strcmp(string[1], "POS") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsPos.x, &PartsPos.y, &PartsPos.z);
				}
				//�ʒu
				if (strcmp(string[1], "ROT") == 0)
				{
					fscanf(pFile, "%s", &string[2]);
					fscanf(pFile, "%f %f %f", &PartsRot.x, &PartsRot.y, &PartsRot.z);
				}
				//�ʒu
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

	//���[�V�����f�[�^�̓ǂݍ���
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
	//m_fGravity = 0.0f;//�d�͓x
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
//�j��
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
	//���̋O�Ղ̍폜
	if (m_pSwordLocus)
	{
		m_pSwordLocus->Uninit();
		delete m_pSwordLocus;
		m_pSwordLocus = NULL;
	}
	Release();
}
//-------------------------------------------
//�X�V
//-------------------------------------------
void CPlayer::Update()
{
	CXInput *pGamePad = CManager::GetXInput();
	CInputKeyBoard *pKeyBoard = CManager::GetInputKeyboard();
	if (m_bDeth == false)
	{
		//�v���C���[�̈ړ�����
		if (!m_bAttack && !m_bDelay && !m_Magic.m_bMagic&&!m_bDodge)
		{
			m_pController->TestMove(m_pos, m_rot);
			m_pController->TestPadMove(m_pos, m_rot, m_bMove);

		}
		//����̏���
		if (!m_bAttack && !m_bSkill&&!m_Magic.m_bMagic&&m_State== STATE_NOWN)
		{
			Dodge();

		}
		//���b�N�I�����Ă��Ȃ�������G�Ƌ߂����𒲂ׂ�
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

		//���b�N�I���̏���
		//���b�N�I����������F��ς���
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


		//�}�b�v�\���p�̉摜���v���C���[�̉�]�ɍ��킹��
		m_pMapPolygon->SetRot(m_rot.y);

		bool bDraw = m_pController->bPorpose();


		//�ړI�n�p�̉摜���v���C���[�̈ʒu�ɍ��킹��
		m_pMapPolygon->SetPos(m_pos.x, m_pos.z);


		//�ړ�������ړ����[�V�����ɕς��� 
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

		//�f�B���C���I���ɂȂ�����
		if (m_bDelay)
		{
			m_nDelayTimer++;
			//�ŏI�R���{����Ȃ���Ύ��̃R���{���o����悤�ɂ���
			if (m_nAttackType[0] != CPlayer_Controller::COMBO_3)
			{
				m_bAttackNext = true;
			}
			else
			{
				m_bAttackNext = false;

			}
			//�w�肳�ꂽ�U���^�C�v�ɂ���ă��[�V������ς���

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
			//�R�}���h�Z���N�g
			if (m_State == STATE_NOWN)
			{
				//�U���Ɋւ��鏈��
				PlayerAttack();

			}
			//�K�E�Z
			//CP�����܂�����
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
		//�ݒu���Ƃ̓����蔻��
		CCollision *pCollision = new CCollision;
		float ModelPosY = 0.0f;
		float fShadowY = 0.0f;
		CScene *pScene_E = CScene::GetScene(OBJTYPE_ENEMY);

		//�G�Ƃ̂̓����蔻��
		while (pScene_E != NULL)
		{
			if (pScene_E != NULL)
			{
				CEnemy *pEnemy = (CEnemy*)pScene_E;
				pCollision->CollisionObjectEnemy((CEnemy*)pScene_E, &m_pos, &m_lastpos, 30.0f);
				//�U������
				bool bHitAttack = pEnemy->bHitAttack();
				if (bHitAttack == false && m_bAttack == true)
				{

					bool bhit = pCollision->CollisionWeapon((CEnemy*)pScene_E, m_pSwordLocus->GetS_Pos(), 100.0f);

					pEnemy->SetHit(bhit);
					if (bhit == true)
					{
						CManager::GetGame()->GetMPGauge()->SetGauge(-1);
						//CP�𑝂₷
						CManager::GetGame()->GetCPGauge()->SetGauge(-5);
						pEnemy->SetbDamage(true);
						pEnemy->AddLife(-PLAYER_POWER);
						CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);

						std::random_device random;	// �񌈒�I�ȗ���������
						std::mt19937_64 mt(random());            // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h
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
		//�ǂƂ̓����蔻��
		while (pScene_Wall != nullptr)
		{
			CWall *pWall = (CWall*)pScene_Wall;
			CScene *pNext_Wall = CScene::GetSceneNext(pScene_Wall);
			//�����蔻��
			bool bHit = pWall->TestCollision(&m_pos, &m_lastpos, 50.0f);
			pScene_Wall = pNext_Wall;

		}

		//�U�������猕�̋O�Ղ�\��
		m_pSwordLocus->SetDraw(m_bAttack);

		m_pCollision->Update();
		PlayerHit();
		//m_pCollision->SetPos(m_pos);
		CCamera *pCamera = CRenderer::GetCamera();
		pCamera->SetPlayerCamera(m_pos);
		CScene::SetPos(m_pos);
		//�v���C���[�ɃJ������Ǐ]������
		m_pController->CameraControl(m_pos);

		//���[�V�����̍Đ�
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
			//���@�֌W�̏���
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

	//���f������
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
	//�v���C���[�̈ړ�����
	//m_pController->Drawtext();
	//Drawtext();
	//�O��̈ʒu���̕ۑ�
	m_lastpos = m_pos;

}
void CPlayer::Drawtext()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[3000];
	int nNum = 0;

	nNum = sprintf(&str[0], "\n\n �Ǐ�� \n");

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
	// �e�L�X�g�`��
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}
//--------------------------------------
//����̏���
void CPlayer::Dodge()
{
	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//�Q�[���p�b�h�̃{�^�����̎擾
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//�J�����̎擾
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

	//����̔��肪�I����������
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
//���@�Ɋւ��鏈��
//----------------------------------------
void CPlayer::PlayerMagic()
{
	//�܂ق��R�}���h��I�����Ă�����
	if (!m_Magic.m_bMagic && m_State == STATE_MAGIC)
	{
		//�{�^���������Ė��@�����
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

	//���@�{�^�����������炿����ƌ�Ƀ|���S�����o��
	if (m_bMagicShot == true )
	{
		m_fMagicShotDelayCnt++;
		if (m_fMagicShotDelayCnt >= m_fMagicShotDelayMax)
		{
			m_fMagicShotDelayCnt = 0.0f;
			//���@�̃^�C�v���Ƃɏo�����@��ς���i�q�[���ȊO�j
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
//�U���Ɋւ��鏈��
//-------------------------------------------
void CPlayer::PlayerAttack()
{
	//�f�B���C�������ĂȂ��Ƃ��ɍU�����I����������P�R���{�ڂ��o��
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
	//�U�����肪�I���ɂȂ��Ă���A���̃��[�V�����ɂ���
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
	//�U�������Ă��Ȃ���Ԃ�������
	if (!m_bAttack)
	{
		//�U������
		//���̍U�����ł���悤�ɂȂ��Ă�����
		if (m_bAttackNext)
		{
			m_pController->Attack(m_pos, m_rot.y, m_nAttackType[0], m_bAttack, m_bAttackNext, m_bNearEnemy);

			//�U��������A���̒x�����O�ɂ���
			if (m_bAttack)
			{

				m_nDelayTimer = 0;
			}
		}
	}

}
//----------------------------------------
//�߂��G��T������
//----------------------------------------
void CPlayer::NearEnemySearch(D3DXVECTOR3 Enemy)
{
	D3DXVECTOR3 EnemyVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EnemyVec = Enemy - m_pos;			//�G�ƃv���C���[�̃x�N�g��
	float fLength = sqrtf((EnemyVec.z*EnemyVec.z) + (EnemyVec.x*EnemyVec.x));
	//�G������͈̔͂ɓ�������
	if (m_bSearchStop == false)
	{
		if (fLength <= PLAYER_RANGE && m_pController->GetNearEnemy() != nullptr)
		{
			//�G�Ƌ߂��ł�
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
			//�G�Ɖ����ł�
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
//�v���C���[���G�̍U���ɓ���������(m_bHit��true�ɂ��Ȃ��Ɠ����Ȃ�)
//-----------------------------------
void CPlayer::PlayerHit()
{
	//�U���������������̏������I���܂ōU�������G����ɂ���
	if (m_bHit==true && m_bHitStop == false)
	{
		//���ꂪtue�ɂȂ�����U�����󂯂Ȃ�
		m_bHitStop = true;
		m_bHit = false;
	}
	//���G���肾������
	else if (m_bHitStop == true)
	{
		//���G���Ԃ̃J�E���g
		m_nHitTime++;
		//���G���Ԃ����̒l�𒴂����疳�G���Ԃ�����
		if (m_nHitTime>= PLAYER_HIT_MAXTIME)
		{
			m_nHitTime = 0;
			//���G���ԉ���
			m_bHitStop = false;
		}
	}

}
