//--------------------------------------
//�Q�[����ʂ̏���
//--------------------------------------
#include "game.h"
#include "Renderer.h"//��ŏ���
#include "bg.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "player.h"
#include "fade.h"
#include "Scene3D.h"
#include "time.h"
#include "model_spawner.h"
#include "collision.h"
#include "score.h"
#include "field.h"
#include "meshsphere.h"
#include "effect_sphere.h"
#include "stage.h"
#include "Enemy.h"
#include "Polygon.h"
#include "pause.h"
#include "XInput.h"
#include "camera.h"
#include "HierarchyData.h"
#include "goal.h"
#include "commandUI.h"
#include "magic.h"
#include "bumper.h"
#include "Enemy_poyo.h"
#include "Particle.h"
#include "gauge.h"
#include "directinput.h"
#include "magicUI.h"
#include "magic_ice.h"
#include "magic_heel.h"
#include "magic_fire.h"
#include "enemyspawner.h"
#include "miss.h"
#define MAGICCOMMAND_SIZE_X (200)
#define MAGICCOMMAND_SIZE_Y (30)
#define MAGICCOMMAND_INTERVAL (200)
#define PLAYER_LIFE (100)		//������
#define PLAYER_MP (100)		//�}�i�̑���
#define PLAYER_CP (100)		//�}�i�̑���
#define MAX_DELAY (30)//�f�B���C�̍ő�
//�ÓI�����o�ϐ��錾
CBg		*CGame::m_pBg = nullptr;
CPlayer	*CGame::m_Player = nullptr;
CModel_Spawner *CGame::m_pModel = nullptr;
CScore	*CGame::m_pScore = nullptr;
CField	*CGame::m_pField = nullptr;
CField	*CGame::m_pSea = nullptr;
CPolygon *CGame::m_Polygon = nullptr;
CPolygon *CGame::m_Cursol = nullptr;
CMeshSphere *CGame::m_pMeshSphere = nullptr;
CEffectSphere *CGame::m_pEffectSphere = nullptr;
CStage  *CGame::m_pStage = nullptr;
CGoal   *CGame::m_Goal = nullptr;
CCommandUI   *CGame::m_pCommand = nullptr;
CMagicUI   *CGame::m_pMagicCommand[MAX_MAGICCOMMAND] = {};
CParticle   *CGame::m_Particle = nullptr;
CGauge   *CGame::m_pHPGauge = nullptr;
CGauge   *CGame::m_pMPGauge = nullptr;
CGauge   *CGame::m_pCPGauge = nullptr;
CEnemy_Spawner   *CGame::m_pEnemySpawner[MAX_SPAWNER] = {};

std::vector<CPolygon*>   CGame::m_pCStock = {};

int   CGame::m_nChancePoint = 0;
int   CGame::m_nChanceStock = 0;

static float s_texrotx = 0.0f;
static float s_texseax = 0.0f;
static int s_nTime = 0;
static bool s_bTime = false;

//--------------------------------------------
//�R���X�g���N�^
//--------------------------------------------
CGame::CGame()
{
	m_pHPGauge = nullptr;
	m_pMPGauge = nullptr;
	m_pCPGauge = nullptr;
	m_pCStock.clear();
	m_Player = nullptr;
	m_pFlore = nullptr;
	m_pBg = nullptr;
	m_pModel = nullptr;
	m_pScore = nullptr;
	m_pField = nullptr;
	m_pMeshSphere = nullptr;
	m_pEffectSphere = nullptr;
	m_pSea = nullptr;
	m_Goal = nullptr;
	m_Particle = nullptr;
	m_pCommand = nullptr;
	m_pStage = nullptr;
	memset(&m_pEnemySpawner, NULL, sizeof(m_pEnemySpawner));
	m_nCntDelay = 0;
	memset(&m_pMagicCommand, NULL, sizeof(m_pMagicCommand));
	m_bPush = false;
	m_bEnd = false;
}
//--------------------------------------------
//�f�X�g���N�^
//--------------------------------------------
CGame::~CGame()
{
}
//--------------------------------------------
//������
//--------------------------------------------
HRESULT CGame::Init(void)
{

	//�R�}���h�̐���
	if (!m_pCommand)
	{
		m_pCommand = CCommandUI::Create(D3DXVECTOR3(80.0f, 745.0f, 0.0f), D3DXVECTOR3(80.0f, 30.0f, 0.0f), CTexture::L_BUTTON);
	}
	//�G�̃X�|�i�[�̐���
	if (m_pEnemySpawner[0] == nullptr)
	{
		m_pEnemySpawner[0] = CEnemy_Spawner::Create({ 977.71f, 0.00f, 1985.35f }, 500.0f, 400.0f, 10);
	}
	if (m_pEnemySpawner[1] == nullptr)
	{
		m_pEnemySpawner[1] = CEnemy_Spawner::Create({ 60.21f, 0.00f, 3572.87f }, 500.0f, 400.0f, 15);
	}
	if (m_pEnemySpawner[2] == nullptr)
	{
		m_pEnemySpawner[2] = CEnemy_Spawner::Create({ 1135.01f, 0.00f, 6678.77f }, 600.0f, 600.0f, 20);
	}

	//�܂ق��R�}���h�̐���
	for (int nCnt = 0; nCnt < CCommandUI::Magic_Max; nCnt++)
	{
		if (m_pMagicCommand[nCnt] == nullptr)
		{
			CTexture::Type TexType = CTexture::None;
			//���@�^�C�v���ƂɃe�N�X�`����ς���
			switch (nCnt)
			{
			case CMagicUI::Fire:
				TexType = CTexture::BLAZE;
				break;
			case CMagicUI::Ice:
				TexType = CTexture::ICECLE;
				break;
			case CMagicUI::Heal:
				TexType = CTexture::HEEL;
				break;
			}
			//�R�}���h�p�̃|���S���̐���
			m_pMagicCommand[nCnt] = CMagicUI::Create({
				MAGICCOMMAND_INTERVAL,
				800.0f + (MAGICCOMMAND_SIZE_Y *2.1f*nCnt),
				0.0f },
				{ MAGICCOMMAND_SIZE_X ,MAGICCOMMAND_SIZE_Y,0.0f }, TexType);
		}
	}

	//�v���C���[�̐���
	if (!m_Player)
	{
		m_Player = CPlayer::Create();
	}
	if (m_Particle == nullptr)
	{
		m_Particle = new CParticle;
	}
	//HP�o�[�̐���
	if (m_pHPGauge == nullptr)
	{
		m_pHPGauge = CGauge::Create({ 1900.0f,930.0f,0.0f }, { 600.0f,20.0f,0.0f }, { 0.5,1.0,0.5,1.0 }, 600.0f, PLAYER_LIFE, CGauge::L_ADD);
	}
	//MP�o�[�̐���
	if (m_pMPGauge == nullptr)
	{
		m_pMPGauge = CGauge::Create({ 1900.0f,970.0f,0.0f }, { 600.0f,15.0f,0.0f }, { 0.5,0.5,1.0,1.0 }, 600.0f, PLAYER_MP, CGauge::L_ADD);
	}
	//CP�o�[�̐���
	if (m_pCPGauge == nullptr)
	{
		m_pCPGauge = CGauge::Create({ 10.0f,600.0f,0.0f }, { 0.0f,20.0f,0.0f }, { 1.0,1.0,0.0,1.0 }, 300.0f, PLAYER_CP, CGauge::R_ADD);
		m_pCPGauge->SetGaugeValue(0);
	}

	//CBumper::Create({ 100.0f,0.0f,50.0f }, { 0.0f,0.0f,0.0f });
	//if (m_pField == NULL)
	//{
	//	m_pField = CField::Create(D3DXVECTOR3(0.0f, -200.0f, -0.0f), D3DXVECTOR3(500.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, -0.0f)
	//		, 50, 50, CTexture::SEA000);
	//	m_pField->SetCol(0.8);
	//}
	//if (m_pSea == NULL)
	//{
	//	m_pSea = CField::Create(D3DXVECTOR3(0.0f, -200.0f, -0.0f), D3DXVECTOR3(600.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, -0.0f)
	//		, 50, 50, CTexture::SEA001);
	//	m_pSea->SetCol(0.4);
	//}

	//��̐���
	if (m_pMeshSphere == NULL)
	{
		m_pMeshSphere = CMeshSphere::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			8, 4, 40000.0f, 1.0f, CTexture::FADE);
	}
	//�X�e�[�W�̐���
	if (m_pStage == NULL)
	{
		m_pStage = new CStage;
		m_pStage->Load("data/TEXT/StageData000.txt");
	}

	//�ړI�\��
	CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 40.0f, 0.0f),
		D3DXVECTOR3(430.0f, 40.0f, 0.0f), CTexture::TargetText);
	//������@
	CPolygon::Create(D3DXVECTOR3(200.0f, SCREEN_HEIGHT/2.0f, 0.0f),
		D3DXVECTOR3(200.0f, 100.0f, 0.0f), CTexture::Operation);

	m_fAlpha = 1.0f;
	m_bNextMode = false;
	m_nTimer = 0;
	return S_OK;
}
//--------------------------------------------
//�I��
//--------------------------------------------
void CGame::Uninit(void)
{

	if (m_Player != nullptr)
	{
		m_Player->Uninit();
		m_Player = nullptr;
	}
	if (m_pModel != nullptr)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pScore != nullptr)
	{
		m_pScore->Uninit();
		delete m_pScore;
		m_pScore = nullptr;
	}
	if (m_pMeshSphere != nullptr)
	{
		m_pMeshSphere->Uninit();
		delete m_pMeshSphere;
		m_pMeshSphere = nullptr;
	}
	if (m_Cursol != nullptr)
	{
		m_Cursol->Uninit();
		m_Cursol = nullptr;
	}
	if (m_pMPGauge != nullptr)
	{
		m_pMPGauge->Uninit();
		m_pMPGauge = nullptr;
	}
	if (m_pHPGauge != nullptr)
	{
		m_pHPGauge->Uninit();
		m_pHPGauge = nullptr;
	}
	//if (m_pCPGauge != nullptr)
	//{
	//	m_pCPGauge->Uninit();
	//	m_pCPGauge = nullptr;
	//}
	if (!m_pCommand)
	{
		m_pCommand->Uninit();
		m_pCommand = nullptr;
	}
	for (int nCnt = 0; nCnt < MAX_SPAWNER; nCnt++)
	{
		if (m_pEnemySpawner[nCnt] != nullptr)
		{
			m_pEnemySpawner[nCnt]->Uninit();
			m_pEnemySpawner[nCnt] = nullptr;
		}
	}
	for (int nCnt = 0; nCnt < MAX_MAGICCOMMAND; nCnt++)
	{
		if (m_pMagicCommand[nCnt] != nullptr)
		{
			m_pMagicCommand[nCnt]->Uninit();
			delete m_pMagicCommand[nCnt];
			m_pMagicCommand[nCnt] = nullptr;
		}
	}
}
//--------------------------------------------
//�X�V
//--------------------------------------------
void CGame::Update(void)
{
	//DirectInput�̃Q�[���p�b�h�̎擾
	CDirectInput *pGamePad = CManager::GetDirectInput();

	//�Q�[���������Ă�����
	if (m_bEnd == false)
	{
		//�S�Ă̓G��|�����烊�U���g�֍s��
		if (m_pEnemySpawner[0]->IsGimmickLock() == false &&
			m_pEnemySpawner[1]->IsGimmickLock() == false &&
			m_pEnemySpawner[2]->IsGimmickLock() == false)
		{
			CFade::SetFade(CManager::MODE_RESULT);
			m_bEnd = true;
		}
		//L�{�^���𒷉������Ă�����
		if (m_Player != nullptr)
		{
			//�̗͂�0�ɂȂ�����
			if (m_pHPGauge->GetGaugeValue() <= 0)
			{
				m_Player->SetDeth(true);
			}
			if (pGamePad->GetGamepadPress(CDirectInput::L) == true)
			{

				//���@��łĂ��Ԃɂ���
				m_Player->SetState(m_Player->STATE_MAGIC);
				if (m_pCommand != nullptr)
				{
					m_pCommand->SetPush(true);
				}
				for (int nCnt = 0; nCnt < MAX_MAGICCOMMAND; nCnt++)
				{
					//�܂ق��R�}���h�̕\��
					if (m_pMagicCommand[nCnt] != nullptr)
					{
						m_pMagicCommand[nCnt]->SetAlphaFade(false);
					}
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

				int nMP = CGame::GetMPGauge()->GetGaugeValue();

				if (nMP < FIRE_MP)
				{
					//�R�}���h����������Ԃɂ���
					m_pMagicCommand[CCommandUI::Fire]->SetPush(true);
				}
				else if (m_bPush == false && nMP > FIRE_MP)
				{
					//�R�}���h����������Ԃɂ���
					m_pMagicCommand[CCommandUI::Fire]->SetPush(false);
				}
				if (nMP < ICE_MP)
				{
					//�R�}���h����������Ԃɂ���
					m_pMagicCommand[CCommandUI::Ice]->SetPush(true);
				}
				else if (m_bPush == false && nMP > ICE_MP)
				{
					//�R�}���h����������Ԃɂ���
					m_pMagicCommand[CCommandUI::Ice]->SetPush(false);
				}
				if (nMP< HEEL_MP)
				{
					//�R�}���h����������Ԃɂ���
					m_pMagicCommand[CCommandUI::Heal]->SetPush(true);
				}
				else if (m_bPush == false && nMP > HEEL_MP)
				{
					//�R�}���h����������Ԃɂ���
					m_pMagicCommand[CCommandUI::Heal]->SetPush(false);
				}

			}
			else
			{
				//�ʏ��Ԃɂ���
				m_Player->SetState(m_Player->STATE_NOWN);

				//�܂ق��R�}���h�̑I��������������
				if (m_pCommand != nullptr)
				{
					m_pCommand->SetPush(false);
				}
				for (int nCnt = 0; nCnt < MAX_MAGICCOMMAND; nCnt++)
				{
					//�܂ق��R�}���h�̍폜
					if (m_pMagicCommand[nCnt] != nullptr)
					{
						m_pMagicCommand[nCnt]->SetAlphaFade(true);
					}
				}
			}
			//���񂾃��[�V�������~�܂�����f�B���C
			if (m_Player->GetGameStop() == true && m_bEnd == false)
			{

				m_nCntDelay++;
				if (m_nCntDelay >= MAX_DELAY)
				{
					//���s���
					CMiss::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 200.0f, 0.0f));
					m_bEnd = true;

				}
			}
		}
		if (m_pMeshSphere != nullptr)
		{
			m_pMeshSphere->SetMesh(D3DXVECTOR3(0.0f, -1000.0f, 0.0f),
				D3DXCOLOR(1.0, 1.0, 1.0, 1.0),
				40000.0f,
				s_texrotx);
		}
		if (m_pMPGauge != nullptr)
		{
			m_pMPGauge->SetGauge(-0.06);
		}

	}
	//�`�����X�Q�[�W���P�O�O�炲�ƂɃQ�[�W���Z�b�g
	//if (m_pCPGauge->GetGaugeValue() >= PLAYER_CP)
	//{
	//	CPolygon *pPoly = {};
	//	m_pCStock.push_back(pPoly);
	//	m_pCStock[m_nChanceStock] = CPolygon::Create({ 20.0f + (20.0f*m_nChanceStock),560.0f,0.0f }, { 10.0f,20.0f,0.0f }, CTexture::Text);
	//	m_nChanceStock++;
	//	m_pCPGauge->ResetGauge();
	//}


}

//--------------------------------------------
//�`��
//--------------------------------------------
void CGame::Draw(LPD3DXMATRIX mtrix)
{
}

