//----------------------------------
//ポリゴンの処理
//----------------------------------
#ifndef _SCENE2D_H
#define _SCENE2D_H
#include "main.h"
#include "scene.h"
#include "texture.h"

//マクロ定義
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	float		rhw;//1.0fで固定
	D3DCOLOR	col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
} VERTEX_2D;
//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CScene2D : public CScene
{
public:
	CScene2D(OBJTYPE nPriority = OBJTYPE_NONE);
	~CScene2D();
	HRESULT Init(CTexture::Type type);
	HRESULT Init(void);

	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2D *Create(D3DXVECTOR3 pos, float fSize, CTexture::Type texture=CTexture::Text);
	void SetPos(D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3);//頂点座標の設定
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);		//頂点座標の設定
	void BindTexture(CTexture::Type type);
	void SetTex(int nAnim,int nPartU);						//テクスチャの設定
	void SetTex(float fSpeedX, float fSpeedY);						//テクスチャの設定
	void SetTex(int nType, int nPattern, float fSize);
	void SetCol(D3DXCOLOR col);						//テクスチャの設定

private:

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//テクスチャへのポインタ
	//VERTEX_2D				m_aVertex[4];					// 頂点情報を格納する
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//頂点バッファへのポインタ
	bool m_bUse;
	CTexture::Type			m_TexType;//テクスチャのタイプ
	D3DXVECTOR3 m_pos;
	D3DXCOLOR m_col;
};


#endif // !_SCENE2D_H
