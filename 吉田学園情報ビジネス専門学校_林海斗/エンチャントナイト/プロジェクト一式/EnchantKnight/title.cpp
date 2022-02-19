//--------------------------------------
//�}�l�[�W���[�̏���
//--------------------------------------
#include "title.h"
#include "bg.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "Polygon.h"
#include "XInput.h"
#include "sound.h"
#include "directinput.h"
//--------------------------------------------
//�R���X�g���N�^
//--------------------------------------------
CTitle::CTitle()
{
	m_Polygon = nullptr;
	m_bOpra = false;
}
//--------------------------------------------
//�f�X�g���N�^
//--------------------------------------------
CTitle::~CTitle()
{

}

//--------------------------------------------
//������
//--------------------------------------------
HRESULT CTitle::Init(void)
{
	m_bNextMode = false;
	CBg::Create(CTexture::Title, CScene::OBJTYPE_BG, false);	//�w�i
	m_Polygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f), D3DXVECTOR3(430.0f, 80.0f, 0.0f), CTexture::GameStart);	//�^�C�g�����S
	return S_OK;
}
//--------------------------------------------
//�I��
//--------------------------------------------
void CTitle::Uninit(void)
{
	if (m_Polygon != NULL)
	{
		m_Polygon->Uninit();
		m_Polygon= NULL;
	}

	CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_TITLE);
}
//--------------------------------------------
//�X�V
//--------------------------------------------
void CTitle::Update(void)
{
	CXInput *pGamePad = CManager::GetXInput();
	CDirectInput *pDGamePad = CManager::GetDirectInput();

	//A�{�^����������
	if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A) == true ||
		pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START) == true||
		pDGamePad->GetButtonTrigger(CDirectInput::START)==true)
	{
		if (m_bNextMode == false)
		{
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENTER, 0.6f);

			//�Q�[�����[�h�֍s��
			CFade::SetFade(CManager::MODE_GAME);
			////���ȏ�ʂ�Ȃ��悤�ɂ���
			m_bNextMode = true;

		}
	}
}
//--------------------------------------------
//�`��
//--------------------------------------------
void CTitle::Draw(void)
{

}

