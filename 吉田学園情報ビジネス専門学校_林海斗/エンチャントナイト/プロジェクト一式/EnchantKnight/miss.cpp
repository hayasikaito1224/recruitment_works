//---------------------------------------------------
//�A�C�e���̏���
//---------------------------------------------------
#include "miss.h"
#include "manager.h"
#include "Renderer.h"
#include "scene2D.h"
#include "polygon.h"
#include "texture.h"
#include "sound.h"
#include "keyboard.h"
#include "fade.h"
#include "XInput.h"
#define MAX_MOVE (240.0f)
#define MAX_MISS_TIME_LIMIT_1 (0)

//--------------------------
//�R���X�g���N�g
//----------------------------
CMiss::CMiss(OBJTYPE nPriority) :CScene(nPriority)
{
	memset(m_pMiss, 0, sizeof(m_pMiss));
	m_pBg = NULL;
}

//--------------------------
//�f�X�g���N�g
//----------------------------
CMiss::~CMiss()
{

}

//--------------------------
//������
//----------------------------
HRESULT CMiss::Init(void)
{
	m_nType = 0;
	CManager::SetPause(true, false);

	CManager::GetSound()->StopSound();
	//�T�E���h��炷
	//CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_SE_MISS);
	//CManager::GetSound()->ControllVoice(CManager::GetSound()->SOUND_LABEL_SE_MISS, 2.0f);

	m_pBg = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT /2.0f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, 0.0f), CTexture::FADE);

	m_pMiss[0] = CPolygon::Create(m_pos, D3DXVECTOR3(300.0f, 90.0f, 0.0f), CTexture::Gameover000);
	//Retry
	m_pMiss[1] = CPolygon::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 300.0f, 0.0f), D3DXVECTOR3(230.0f, 50.0f, 0.0f), CTexture::Retry);
	//�^�C�g��
	m_pMiss[2] = CPolygon::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 450.0f, 0.0f), D3DXVECTOR3(230.0f, 50.0f, 0.0f), CTexture::TitleBack001);
	//�J�[�\��
	m_pMiss[3] = CPolygon::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 300.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), CTexture::Cursol);
	m_pBg->SetCol(D3DXCOLOR(1.0, 1.0, 1.0, 0.5));
	for (int nCnt = 0; nCnt < MAX_MISS_POLYGON; nCnt++)
	{
		m_pMiss[nCnt]->SetCol(D3DXCOLOR(1.0, 1.0, 1.0, 0.0));
	}
	m_bSelectOn = false;
	m_fMove = 1.0f;//�ړ���
	m_fAlpha = 0.0f;
	m_fAlpha2 = 0.0f;
	m_nTimer = 0;
	m_bNextMode = false;
	return S_OK;
}

//--------------------------
//�I��
//----------------------------
void CMiss::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_MISS_POLYGON; nCnt++)
	{
		if (m_pMiss[nCnt] != NULL)
		{
			m_pMiss[nCnt]->Uninit();
			delete m_pMiss[nCnt];
		}
	}
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		delete m_pBg;
	}

	CManager::SetPause(false,false);

	Release();
}

//--------------------------
//�X�V
//--------------------------
void CMiss::Update(void)
{
	//�Q�[���p�b�h�̎擾
	CXInput *pXInput = CManager::GetXInput();
	D3DXVECTOR3 Retrypos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Titlepos = { 0.0f,0.0f,0.0f };

	if (m_pMiss[0] != NULL)
	{

		m_pos.y += m_fMove;
		m_fAlpha += 0.01f;
		if (m_fAlpha >= 1.0f)
		{
			m_fAlpha = 1.0f;
			m_nTimer++;
		}
		if (m_nTimer >= MAX_MISS_TIME_LIMIT_1)
		{
			m_fAlpha2 += 0.05f;

			if (m_fAlpha2 >= 1.0f)
			{
				m_fAlpha2 = 1.0f;
				m_bSelectOn = true;

			}

		}
		if (m_pos.y >= MAX_MOVE)
		{
			m_fMove = 0.0f;
			m_pos.y = MAX_MOVE;
		}
		//�|���S���̈ʒu���f
		m_pMiss[0]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f));
		m_pMiss[0]->SetCol(D3DXCOLOR(1.0, 1.0, 1.0, m_fAlpha));
		m_pMiss[1]->SetCol(D3DXCOLOR(1.0, 1.0, 1.0, m_fAlpha2));
		m_pMiss[2]->SetCol(D3DXCOLOR(1.0, 1.0, 1.0, m_fAlpha2));
		m_pMiss[3]->SetCol(D3DXCOLOR(1.0, 1.0, 1.0, m_fAlpha2));

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true ||
			pXInput->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
		{
			CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_SE_SELECT);
			//CManager::GetSound()->ControllVoice(CManager::GetSound()->SOUND_LABEL_SE_CURSOL, 2.0f);

			m_nType--;
			if (m_nType < 0)
			{
				m_nType = MAX_MISS_TYPE - 1;
			}
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true ||
			pXInput->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{
			CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_SE_SELECT);
			//CManager::GetSound()->ControllVoice(CManager::GetSound()->SOUND_LABEL_SE_CURSOL, 2.0f);

			m_nType++;
			if (m_nType >= MAX_MISS_TYPE)
			{
				m_nType = 0;
			}
		}

		switch (m_nType)
		{
		case 0:
			Retrypos = m_pMiss[1]->GetPos();
			
			m_pMiss[3]->SetPos(D3DXVECTOR3(Retrypos.x - 250.0f, Retrypos.y, 0.0f));
			break;

		case 1:
			Titlepos = m_pMiss[2]->GetPos();

			m_pMiss[3]->SetPos(D3DXVECTOR3(Titlepos.x - 250.0f, Titlepos.y, 0.0f));
			break;

		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			pXInput->GetButtonTrigger(XINPUT_GAMEPAD_A) == true&& m_bNextMode==false)
		{
			//�I���ł����ԂɂȂ�����
			if (m_bSelectOn == true)
			{
			//	CManager::GetSound()->PlaySound(CManager::GetSound()->SOUND_LABEL_SE_PUSH);
			//	CManager::GetSound()->ControllVoice(CManager::GetSound()->SOUND_LABEL_SE_PUSH, 2.0f);
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);

				CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENTER, 0.6f);

				switch (m_nType)
				{
				case 0://���g���C
					   //���g���C
					m_bUninit = true;
					CFade::SetFade(CManager::MODE_GAME);
					break;
				case 1://�o�����
					   //�o����ʂɍs��
					m_bUninit = true;
					CFade::SetFade(CManager::MODE_TITLE);
					break;
				}
				m_bNextMode = true;
			}
		}

	}

}

//--------------------------
//�`��
//----------------------------
void CMiss::Draw(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}

	for (int nCnt = 0; nCnt < MAX_MISS_POLYGON; nCnt++)
	{
		if (m_pMiss[nCnt] != NULL)
		{
			m_pMiss[nCnt]->Draw();
		}
	}
}
//---------------------------------------------------------------
//�C���X�^���X��������
//---------------------------------------------------------------
CMiss *CMiss::Create(D3DXVECTOR3 pos)
{
	//�C���X�^���X����
	CMiss *pMiss = new CMiss(OBJTYPE_UI);
	if (pMiss != NULL)
	{
		pMiss->m_pos = pos;
		pMiss->Init();
	}

	return pMiss;
}
