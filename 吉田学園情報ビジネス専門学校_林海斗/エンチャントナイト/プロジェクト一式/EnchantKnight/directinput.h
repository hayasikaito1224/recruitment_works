//入力処理
//---------------------
#ifndef _DIRECTINPUT_H_
#define _DIRECTINPUT_H_
#include "main.h"
#include "input.h"
//-----------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------
#define MAX_CROSS_BOTTON (4)
#define MAX_DEAD_ZOON (300.0)

//-----------------------------------------------------------------------------
//クラス定義
//-----------------------------------------------------------------------------
class CDirectInput : public CInput
{
public:
	typedef enum
	{
		A = 0,
		B,
		X,
		Y,
		L,
		R,
		L2,
		R2,
		L3,
		R3,
		START,
		SELECT,
		BUTTOM,
		HOME,
		D_BUTTON_MAX,
	} PADBUTTON;
	typedef enum
	{
		UP = 0,
		DOWN =18000,
		RIGHT =9000,
		LIGHT = 27000
	}CROSS_BUTTON;
	CDirectInput();
	~CDirectInput();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetGamepadPress(int nButton);
	bool GetButtonTrigger(int nButton);
	bool GetButtonRelease(int nButton);
	bool GetClossButtonPress(int nButton);
	bool GetClossButtonTrigger(int nButton);
	bool GetClossButtonRelease(int nButton);

	DIJOYSTATE2 GetJoyState(void) {return m_aButtonState; }
	static BOOL CALLBACK EnumJoysticksCallback(const LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);
private:
	static LPDIDEVICEOBJECTINSTANCE m_Instance;
	LPDIRECTINPUT8       m_lpDI;
	DIDEVCAPS            m_diDevCaps;
	DIJOYSTATE2 m_aButtonState;//キーボードの入力情報（プレス情報）
	DIJOYSTATE2 m_aButtonStateTrigger;
	DIJOYSTATE2 m_aButtonStateRelease;

};
#endif _DIRECTINPUT_H_
