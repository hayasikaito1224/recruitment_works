//--------------------------------------
//マネージャーの処理
//--------------------------------------
#include "manager.h"
#include "Renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "keyboard.h"
#include "texture.h"
#include "Scene3D.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "model_spawner.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "XInput.h"
#include "mouse.h"
#include "sound.h"
#include "pause.h"
#include "xload.h"
#include "directinput.h"
#include "HierarchyData.h"
#include "MyAllocateHierarchy.h"
#include <random>
//静的メンバ変数宣言
CInputKeyBoard	*CManager::m_pInputKeyboard = NULL;
CRenderer		*CManager::m_pRenderer = NULL;
CTexture		*CManager::m_pTexture = NULL;
CLight			*CManager::m_pLight[3] = {};
CPlayer			*CManager::m_pPlayer = NULL;
CModel_Spawner	*CManager::m_pModel = NULL;
CGame			*CManager::m_pGame = NULL;
CTitle			*CManager::m_pTitle = NULL;
CResult			*CManager::m_pResult = NULL;
CFade			*CManager::m_Fade = NULL;
CXInput			*CManager::m_XInput = NULL;
CManager::MODE	 CManager::m_Mode = MODE_TITLE;		// 初期モード
CMouse			*CManager::m_Mouse = NULL;
CSound			*CManager::m_pSound = NULL;
CPause			*CManager::m_pPause = NULL;
CXload			*CManager::m_pXload = NULL;
CDirectInput	*CManager::m_pDirectInput = NULL;
bool			CManager::m_bPause = false;
bool			CManager::m_bStop = false;
bool			CManager::m_bEnd = false;
bool			CManager::m_bClear = true;
int g_nTimer = 0;

//--------------------------------------------
//コンストラクタ
//--------------------------------------------
CManager::CManager()
{
}
//--------------------------------------------
//デストラクタ
//--------------------------------------------
CManager::~CManager()
{
}
//--------------------------------------------
// 初期化
//--------------------------------------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダラーの生成
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
	}

	// テクスチャの生成
	if (m_pTexture == NULL)
	{
		//インスタンス生成
		m_pTexture = new CTexture;
	}

	// レンダラーの初期化
	if (m_pRenderer != NULL)
	{
		if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))
		{
			return-1;
		}

		// テクスチャの読み込み
		m_pTexture->Init();
	}
	//全体のモデルデータの読み込み
	if (m_pXload == NULL)
	{
		m_pXload = new CXload;
		m_pXload->Init();
	}
	// サウンドの生成
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		m_pSound->InitSound();
	}

	//フェードの生成
	if (m_Fade == NULL)
	{
		m_Fade = CFade::Create(CTexture::Text, m_Mode);

		SetMode(m_Mode);

		//モードの設定
	}

	// キーボードの生成
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyBoard;
		if (m_pInputKeyboard != NULL)
		{
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	// マウスの生成
	if (m_Mouse == NULL)
	{
		m_Mouse = new CMouse;
		m_Mouse->Init(hInstance, hWnd);
	}

	//Xinput生成
	if (m_XInput == NULL)
	{
		m_XInput = new CXInput;
	}
	//directinputの生成
	if (m_pDirectInput == NULL)
	{
		m_pDirectInput = new CDirectInput;
		m_pDirectInput->Init(hInstance, hWnd);
	}
	// ライトの生成
	for (int nLight = 0; nLight < 3; nLight++)
	{
		m_pLight[nLight] = new CLight;
	}
	m_pLight[0]->Init(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.22f, -0.87f, 0.44f));
	m_pLight[1]->Init(D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f), D3DXVECTOR3(-0.18f, 0.88f, -0.44f));
	m_pLight[2]->Init(D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f), D3DXVECTOR3(0.89f, -0.11f, 0.44f));

	HierarchyData AH;
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
	return S_OK;
}
//--------------------------------------------
// 終了
//--------------------------------------------
void CManager::Uninit(void)
{
	//キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// ライトの破棄
	for (int nLight = 0; nLight < 3; nLight++)
	{
		if (m_pLight[nLight] != NULL)
		{
			delete m_pLight[nLight];
			m_pLight[nLight] = NULL;
		}
	}

	// マウスの破棄
	if (m_Mouse != NULL)
	{
		m_Mouse->Uninit();
		delete m_Mouse;
		m_Mouse = NULL;
	}
	// ゲームパッドの破棄
	if (m_pDirectInput != NULL)
	{
		m_pDirectInput->Uninit();
		delete m_pDirectInput;
		m_pDirectInput = NULL;
	}
	// シーンの破棄
	CScene::Delete();

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Uniinit();
		delete m_pTexture;
		m_pTexture = NULL;
	}
	if (m_pXload != NULL)
	{
		m_pXload->Uninit();
		delete m_pXload;
		m_pXload = NULL;
	}
	/*if (m_pSecCamera != NULL)
	{
		m_pSecCamera->Uninit();
		delete m_pSecCamera;
		m_pSecCamera = NULL;
	}*/
	// フェードの破棄
	if (m_Fade != NULL)
	{
		m_Fade->Uninit();
		m_Fade = NULL;
	}

	// レンダラーの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = NULL;
	}
}
//--------------------------------------------
// 更新
//--------------------------------------------
void CManager::Update(void)
{
	// レンダラーの更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//キーボードの更新
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// ゲームパッドの更新
	if (m_XInput != NULL)
	{
		m_XInput->UpdateGamepad();
	}
	if (m_pDirectInput != NULL)
	{
		m_pDirectInput->Update();

	}
	// マウスの更新
	if (m_Mouse != NULL)
	{
		m_Mouse->Update();
	}



	// モード毎の更新
	switch (m_Mode)
	{
	case MODE_TITLE:		//タイトル画面
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;

	case MODE_GAME:			//ゲーム画面
		if (m_pGame != NULL)
		{
			m_pGame->Update();
			//ポーズ
			if (m_XInput->GetButtonTrigger(XINPUT_GAMEPAD_START) == true&&m_bEnd==false)
			{
				m_bStop = m_bStop ? false : true;

				m_bPause = m_bPause ? false : true;
				m_pSound->PlaySound(m_pSound->SOUND_LABEL_SE_ENTER);	// タイトルサウンド
				m_pSound->ControllVoice(m_pSound->SOUND_LABEL_SE_ENTER,0.6f);	// タイトルサウンド

			}
			if (m_bStop == true)
			{
				if (m_pPause == NULL)
				{
					m_pPause = CPause::Create(D3DXVECTOR3(640.0f, 400.0f, 0.0f));
				}
			}
			else
			{
				if (m_pPause != NULL)
				{
					m_pPause = NULL;
				}

			}

		}
		break;

	case MODE_RESULT:		//リザルト画面
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;
	}
}
//--------------------------------------------
// 描画
//--------------------------------------------
void CManager::Draw(void)
{

	// レンダラーの描画
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
	switch (m_Mode)
	{
	case MODE_TITLE:		//タイトル画面
		if (m_pTitle != NULL)
		{
			//m_pTitle->Update();
		}
		break;

	case MODE_GAME:			//ゲーム画面

		break;

	case MODE_RESULT:		//リザルト画面
		if (m_pResult != NULL)
		{
			//m_pResult->Update();
		}
		break;
	}

}

//-----------------------------------------------------------------------------
// Renderer取得処理
//-----------------------------------------------------------------------------
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//-----------------------------------------------------------------------------
// キーボードのインスタンス取得処理
//-----------------------------------------------------------------------------
CInputKeyBoard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//-----------------------------------------------------------------------------
// テクスチャのインスタンス取得処理
//-----------------------------------------------------------------------------
CTexture *CManager::GetTexture(void)
{
	return m_pTexture;
}

//-----------------------------------------------------------------------------
// モードの処理
//-----------------------------------------------------------------------------
CManager::MODE CManager::GetMode(void)
{
	return m_Mode;
}

//-----------------------------------------------------------------------------
// モードの処理
//-----------------------------------------------------------------------------
void CManager::SetMode(MODE mode)
{
	m_bEnd = false;
	m_bStop = false;
	m_bPause = false;
	switch (m_Mode)
	{
	case MODE_TITLE:		//タイトル画面
		if (m_pTitle != NULL)
		{
			m_pSound->StopSound(m_pSound->SOUND_LABEL_BGM_TITLE);
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_GAME:			//ゲーム画面
		if (m_pGame != NULL)
		{
			m_pSound->StopSound(m_pSound->SOUND_LABEL_BGM_GAME);
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:		//リザルト画面
		if (m_pResult != NULL)
		{
			m_pSound->StopSound(m_pSound->SOUND_LABEL_SE_WIN);
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	}

	//現在選択しているモードの削除+そのモードで生成したオブジェクトの削除
	CScene::Delete();

	//次に移行するモードの生成
	switch (mode)
	{
	case MODE_TITLE:		//タイトル画面
		if (m_pTitle == NULL)
		{
			m_bClear = false;
			m_pTitle = new CTitle;
			m_pTitle->Init();
			m_pSound->PlaySound(m_pSound->SOUND_LABEL_BGM_TITLE);	// タイトルサウンド
			m_pSound->ControllVoice(m_pSound->SOUND_LABEL_BGM_TITLE,0.7f);	// タイトルサウンド

		}
		break;
	case MODE_GAME:			//ゲーム画面
		if (m_pGame == NULL)
		{
			m_pGame = new CGame;
			m_pGame->Init();
			m_pSound->PlaySound(m_pSound->SOUND_LABEL_BGM_GAME);	// タイトルサウンド
			m_pSound->ControllVoice(m_pSound->SOUND_LABEL_BGM_GAME, 0.5f);

		}
		break;
	case MODE_RESULT:		//リザルト画面
		if (m_pResult == NULL)
		{
			m_pResult = new CResult;
			m_pResult->Init();
			m_pSound->PlaySound(m_pSound->SOUND_LABEL_SE_WIN);	// リザルトサウンド
			m_pSound->ControllVoice(m_pSound->SOUND_LABEL_SE_WIN,0.5f);	// タイトルサウンド

		}
		break;
	}
	//次のモードを現在のモードに上書き
	m_Mode = mode;
}