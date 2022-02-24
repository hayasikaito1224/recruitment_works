//=============================================================================
//
// 当たり判定エリアの表示処理の背景
//
//=============================================================================
#ifndef _COL_AREA_BG_H_
#define _COL_AREA_BG_H_

#include "scene.h"
#include "texture.h"
#define AREA_VTX (4)
class CTexture;

class CCol_Area_Bg : public CScene
{

public:

	CCol_Area_Bg(OBJTYPE nPriority = OBJTYPE_EFFECT);		//コンストラクタ
	~CCol_Area_Bg();									//デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCol_Area_Bg *Create(const D3DXVECTOR3 pos, const float fRotY, const float fSizeX, const float fSizeZ,
		const float fAlpha, const int nValueMax, const CTexture::Type texture);

private:

	CTexture::Type m_Tex;
	bool m_bDraw;
	bool m_bUninit;//消去判定
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//頂点バッファへのポインタ
	LPDIRECT3DDEVICE9 m_pDevice = NULL;//デバイスのポインタ
	D3DXCOLOR m_col;
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	float				m_fMaxGauge;	//gaugeの最大数
	float				m_fValueMax;	//ゲージの値の最大数
	float				m_fValue;		//ゲージの数値

};

#endif // _COL_AREA_BG_H_
