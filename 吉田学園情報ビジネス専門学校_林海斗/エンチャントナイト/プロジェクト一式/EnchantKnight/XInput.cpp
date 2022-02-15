//-----------------------------------------------------------------------------
//�Q�[���p�b�h����̏���
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include<Windows.h>
#include "XInput.h"
#include "Player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CXInput::CXInput()
{
	ZeroMemory(&m_GamePad.m_state, sizeof(XINPUT_STATE));
	ZeroMemory(&m_GamePad.m_stateLast, sizeof(XINPUT_STATE));

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CXInput::~CXInput()
{

}
//-----------------------------------------------------------------------------
//�X�V����
//-----------------------------------------------------------------------------
void CXInput::UpdateGamepad(void)
{
	XINPUT_STATE state;

	DWORD dwResult = XInputGetState(0, &state);

	m_GamePad.m_stateLast = m_GamePad.m_state;
	m_GamePad.m_state = state;

	//���̓f�o�C�X����f�[�^���擾

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