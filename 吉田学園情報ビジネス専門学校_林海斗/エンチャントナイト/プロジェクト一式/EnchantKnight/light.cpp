//----------------------------------
//ポリゴンの処理
//-----------------------------------
#include "light.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"
int CLight::m_nCntLight = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{
	//生成数のカウント
	m_nId = m_nCntLight;
	//カウント増加
	m_nCntLight++;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{
	m_nCntLight--;
}
//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CLight::Init(D3DXCOLOR Diffuse, D3DXVECTOR3 vecDir)
{
	//ライトをクリアする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	D3DXVECTOR3 r_vecDir;//設定用のベクトル
	pDevice = CManager::GetRenderer()->GetDevice();
	//ライトの種類を設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	//ライトの拡散光
	m_light.Diffuse = Diffuse;
	//ライトの方向を設定
	r_vecDir = vecDir;
	D3DXVec3Normalize(&r_vecDir, &r_vecDir);//正規化する
	m_light.Direction = r_vecDir;
	//ライトの設定
	pDevice->SetLight(m_nId, &m_light);
	//ライトを有効にする
	pDevice->LightEnable(m_nId, TRUE);


	return S_OK;
}

