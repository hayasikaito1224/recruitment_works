//=============================================================================
//
// Polygon処理
//
//=============================================================================
#ifndef _COL_AREA_H_
#define _COL_AREA_H_

#include "scene2D.h"
#define AREA_VTX (4)
class CTexture;

class CCol_Area
{

public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//ワールドマトリックス
	}FieldVtx;//モデルの各頂点の保存用構造体

	CCol_Area();		//コンストラクタ
	~CCol_Area();									//デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCol_Area *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, CTexture::Type texture, D3DXCOLOR col = { 1.0,1.0,1.0,1.0 });
	void SetPos(D3DXVECTOR3 pos);
	FieldVtx GetVtx(int nCnt) { return m_vtx[nCnt]; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetScale(void) { return m_size; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }

private:
	FieldVtx				m_vtx[AREA_VTX];			//各頂点の情報格納構造体

	CTexture::Type m_Tex;
	bool m_bDraw;
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//頂点バッファへのポインタ
	LPDIRECT3DDEVICE9 m_pDevice = NULL;//デバイスのポインタ
	D3DXCOLOR m_col;
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;

};

#endif // _COL_AREA_H_
