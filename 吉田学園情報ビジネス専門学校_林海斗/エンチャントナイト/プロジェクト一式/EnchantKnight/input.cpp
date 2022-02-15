//---------------------
//入力処理
//---------------------
#include "input.h"
//マクロ定義
//グローバル変数
LPDIRECTINPUT8 CInput::m_pInput = NULL;//DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 CInput::m_lpDIDevice = NULL;//DirectInputオブジェクトのポインタ

//----------------------------------------
//コンストラクタ
//----------------------------------------
CInput::CInput()
{
	m_pDevice = NULL;
}
//----------------------------------------
//デストラクタ
//----------------------------------------

CInput::~CInput()
{
}

//----------------------------------------
//初期化処理
//----------------------------------------
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	return S_OK;

}
//----------------------------------------
//終了処理
//----------------------------------------
void CInput::Uninit(void)
{
	//入力デバイスの開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
	//デバイスの開放
	if (m_lpDIDevice != NULL)
	{
		m_lpDIDevice->Unacquire();
		m_lpDIDevice->Release();
		m_lpDIDevice = NULL;
	}

}

