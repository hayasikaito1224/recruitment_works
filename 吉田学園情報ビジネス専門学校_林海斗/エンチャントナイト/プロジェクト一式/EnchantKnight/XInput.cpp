//-----------------------------------------------------------------------------
//ゲームパッド操作の処理
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include<Windows.h>
#include "XInput.h"
#include "Player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CXInput::CXInput()
{
	ZeroMemory(&m_GamePad.m_state, sizeof(XINPUT_STATE));
	ZeroMemory(&m_GamePad.m_stateLast, sizeof(XINPUT_STATE));

}

//=============================================================================
// デストラクタ
//=============================================================================
CXInput::~CXInput()
{

}
//-----------------------------------------------------------------------------
//更新処理
//-----------------------------------------------------------------------------
void CXInput::UpdateGamepad(void)
{
	XINPUT_STATE state;

	DWORD dwResult = XInputGetState(0, &state);

	m_GamePad.m_stateLast = m_GamePad.m_state;
	m_GamePad.m_state = state;

	//入力デバイスからデータを取得

}
bool CXInput::GetButtonTrigger(int nButton)
{
	return (m_GamePad.m_stateLast.Gamepad.wButtons & nButton)==false&& (m_GamePad.m_state.Gamepad.wButtons & nButton) ? true : false;
}
bool CXInput::GetGamepadPress(int nButton)
{
	return (m_GamePad.m_state.Gamepad.wButtons & nButton);

}

CXInput::GAMEPAD *CXInput::GetGamePad(void)
{
	return &m_GamePad;
}