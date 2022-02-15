//---------------------
//キーボードの入力処理
//---------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "main.h"
#include "input.h"
#define NUM_KEY_MAX (256)//キーの最大数

class CInputKeyBoard : public CInput
{
public:
	CInputKeyBoard();
	~CInputKeyBoard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int Key);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];//キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];

};

#endif _KEYBOARD_H_
