//--------------------------------------
//マネージャーの処理
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
//コンストラクタ
//--------------------------------------------
CTitle::CTitle()
{
	m_Polygon = nullptr;
	m_bOpra = false;
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CTitle::~CTitle()
{

}

//--------------------------------------------
//初期化
//--------------------------------------------
HRESULT CTitle::Init(void)
{
	m_bNextMode = false;
	CBg::Create(CTexture::Title, CScene::OBJTYPE_BG, false);	//背景
	m_Polygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT / 2 + 280.0f, 0.0f), D3DXVECTOR3(430.0f, 80.0f, 0.0f), CTexture::GameStart);	//タイトルロゴ
	return S_OK;
}
//--------------------------------------------
//終了
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
//更新
//--------------------------------------------
void CTitle::Update(void)
{
	CXInput *pGamePad = CManager::GetXInput();
	CDirectInput *pDGamePad = CManager::GetDirectInput();

	//Aボタンを押すと
	if (pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_A) == true ||
		pGamePad->GetButtonTrigger(XINPUT_GAMEPAD_START) == true||
		pDGamePad->GetButtonTrigger(CDirectInput::START)==true)
	{
		if (m_bNextMode == false)
		{
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_ENTER, 0.6f);

			//ゲームモードへ行く
			CFade::SetFade(CManager::MODE_GAME);
			////二回以上通らないようにする
			m_bNextMode = true;

		}
	}
}
//--------------------------------------------
//描画
//--------------------------------------------
void CTitle::Draw(void)
{

}

