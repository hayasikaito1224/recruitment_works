//----------------------------------
//ポリゴンの処理
//----------------------------------
#ifndef _SCENE3D_H
#define _SCENE3D_H
#include "main.h"
#include "scene.h"
#include "texture.h"

//マクロ定義
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	D3DXVECTOR3 nor;//法線ベクトル
	D3DCOLOR	col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
} VERTEX_3D;
//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CScene3D : public CScene
{
public:
	typedef enum
	{
		TYPE_FIELD = 0,
		TYPE_WALL,
		TYPE_MAX
	}TYPE;
	CScene3D(OBJTYPE nPriority = OBJTYPE_NONE);
	~CScene3D();
	HRESULT Init(const int nPolygonType,CTexture::Type type= CTexture::Text);
	HRESULT Init(void);

	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);		//頂点座標の設定
	void BindTexture(CTexture::Type type);
	void SetTex(int nAnim, int nPartU);						//テクスチャの設定
	void SetTex(float fSpeedX, float fSpeedY);				//テクスチャの設定
	void SetCol(D3DXCOLOR col);								//テクスチャの設定
	static CScene3D *Create(D3DXVECTOR3 size);

private:

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//頂点バッファへのポインタ
	LPDIRECT3DDEVICE9 m_pDevice = NULL;//デバイスのポインタ

	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;

};


#endif // !_SCENE3D_H
