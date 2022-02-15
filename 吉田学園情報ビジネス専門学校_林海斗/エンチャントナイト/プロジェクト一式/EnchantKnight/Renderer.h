//--------------------------
//Rendering�N���X
//----------------------------
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"
class CCamera;

class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCamera *GetCamera(void) { return m_pCamera[0]; }
	static CCamera *GetMapCamera(void) { return m_pCamera[1]; }
	LPDIRECT3DDEVICE9 GetDevice(void);
	LPD3DXFONT GetFont(void) { return m_pFont; }
private:
#ifdef _DEBUG
	void DrawFPS(void);
#endif
	static CCamera			*m_pCamera[2];

	LPDIRECT3D9				m_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)

	LPD3DXFONT				m_pFont = NULL;			// �t�H���g�ւ̃|�C���^


};


#endif
