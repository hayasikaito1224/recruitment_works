//=============================================================================
//
// 当たり判定エリアの表示処理
//
//=============================================================================
#ifndef _COL_AREA_H_
#define _COL_AREA_H_

#include "scene.h"
#include "texture.h"
#define AREA_VTX (4)
class CTexture;

class CCol_Area : public CScene
{

public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//ワールドマトリックス
	}FieldVtx;//モデルの各頂点の保存用構造体

	CCol_Area(OBJTYPE nPriority = OBJTYPE_EFFECT);		//コンストラクタ
	~CCol_Area();									//デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCol_Area *Create(const D3DXVECTOR3 pos, const float fRotY, const float fSizeX, const float fAlpha,
		const float MaxGauge, const int nValueMax, const CTexture::Type texture);
	void SetPos(D3DXVECTOR3 pos);
	void MoveVtx();//頂点の移動
	FieldVtx GetVtx(int nCnt) { return m_vtx[nCnt]; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetScale(void) { return m_size; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }

private:
	FieldVtx				m_vtx[AREA_VTX];			//各頂点の情報格納構造体

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

#endif // _COL_AREA_H_
