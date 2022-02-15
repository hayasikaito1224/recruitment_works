//---------------------
//入力処理
//---------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"


class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUTDEVICE8 m_lpDIDevice;//ジョイスティックで使うやつ
	static LPDIRECTINPUT8 m_pInput;//DirectInputオブジェクトのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;//入力デバイスへのポインタ
};


//プロトタイプ宣言
#endif _INPUT_H_
