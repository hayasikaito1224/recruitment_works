//---------------------
//�L�[�{�[�h�̓��͏���
//---------------------
#include "directinput.h"

LPDIDEVICEOBJECTINSTANCE CDirectInput::m_Instance;

//----------------------------------------
//�R���X�g���N�^
//----------------------------------------

CDirectInput::CDirectInput()
{

}
//----------------------------------------
//�f�X�g���N�^
//----------------------------------------

CDirectInput::~CDirectInput()
{

}

BOOL CALLBACK CDirectInput::EnumJoysticksCallback(const LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	if (FAILED(m_pInput->CreateDevice(lpddi->guidInstance, &m_lpDIDevice, NULL)))
	{
		return DIENUM_STOP;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_lpDIDevice->SetDataFormat(&c_dfDIJoystick2)))
	{
		m_lpDIDevice->Release();
		return DIENUM_STOP;
	}
	return DIENUM_STOP;
}

BOOL CALLBACK CDirectInput::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;
	m_Instance = (LPDIDEVICEOBJECTINSTANCE)pdidoi;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	//�f�b�h�]�[���̐ݒ�
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	hr = m_lpDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

//----------------------------------------
//����������
//----------------------------------------
HRESULT CDirectInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, 
						  EnumJoysticksCallback,
							NULL, 
							DIEDFL_ATTACHEDONLY);

	// �������[�h��ݒ�
	if (m_lpDIDevice != nullptr)
	{
		if (FAILED(m_lpDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}
		m_lpDIDevice->GetCapabilities(&m_diDevCaps);
		m_lpDIDevice->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);
		m_lpDIDevice->Poll();
		m_lpDIDevice->Acquire();
	}
	return S_OK;

}
//----------------------------------------
//�I������
//----------------------------------------
void CDirectInput::Uninit(void)
{
	CInput::Uninit();
}
//----------------------------------------
//�X�V����
//----------------------------------------
void CDirectInput::Update(void)
{
	DIJOYSTATE2 aButtonState;
	int nCntKey;
	DWORD dwObj = NULL;
	if (m_lpDIDevice != nullptr)
	{
		m_Instance->dwSize = NULL;

		if (m_lpDIDevice->GetObjectInfo(m_Instance, dwObj, DIPH_BYID) != DIERR_INVALIDPARAM)
		{
			m_aButtonState.rglSlider[0] = m_lpDIDevice->GetObjectInfo(m_Instance, dwObj, DIPH_BYID);
			m_aButtonState.rglSlider[1] = m_lpDIDevice->GetObjectInfo(m_Instance, dwObj, DIPH_BYID);
		}

		//���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_lpDIDevice->GetDeviceState(sizeof(aButtonState), &aButtonState)))
		{
			for (nCntKey = 0; nCntKey < D_BUTTON_MAX;
				nCntKey++)
			{
				m_aButtonStateTrigger.rgbButtons[nCntKey] = (m_aButtonState.rgbButtons[nCntKey] ^ aButtonState.rgbButtons[nCntKey])&aButtonState.rgbButtons[nCntKey];
				m_aButtonStateRelease.rgbButtons[nCntKey] = (m_aButtonState.rgbButtons[nCntKey] ^ aButtonState.rgbButtons[nCntKey])& ~aButtonState.rgbButtons[nCntKey];
			}
			for (nCntKey = 0; nCntKey < MAX_CROSS_BOTTON;
				nCntKey++)
			{
				m_aButtonStateTrigger.rgdwPOV[nCntKey] = (m_aButtonState.rgdwPOV[nCntKey] ^ aButtonState.rgdwPOV[nCntKey])&aButtonState.rgdwPOV[nCntKey];
				m_aButtonStateRelease.rgdwPOV[nCntKey] = (m_aButtonState.rgdwPOV[nCntKey] ^ aButtonState.rgdwPOV[nCntKey])& ~aButtonState.rgdwPOV[nCntKey];
			}
			m_aButtonState = aButtonState;
		}
		else
		{
			m_lpDIDevice->Acquire();
		}

	}
}
bool CDirectInput::GetGamepadPress(int nButton)
{
	return(m_aButtonState.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CDirectInput::GetButtonTrigger(int nButton)
{
	return (m_aButtonStateTrigger.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CDirectInput::GetButtonRelease(int nButton)
{
	return (m_aButtonStateRelease.rgbButtons[nButton] & 0x80) ? true : false;
}

bool CDirectInput::GetClossButtonPress(int nButton)
{
	return(m_aButtonState.rgdwPOV[nButton] & 0x80) ? true : false;
}

bool CDirectInput::GetClossButtonTrigger(int nButton)
{
	return(m_aButtonStateTrigger.rgdwPOV[nButton] & 0x80) ? true : false;
}

bool CDirectInput::GetClossButtonRelease(int nButton)
{
	return false;
}
