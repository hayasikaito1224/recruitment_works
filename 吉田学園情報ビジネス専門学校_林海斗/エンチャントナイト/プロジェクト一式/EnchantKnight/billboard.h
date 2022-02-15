#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_
#include "main.h"
#include "scene.h"
#include "texture.h"

class CBillboard : public CScene
{
public:
	CBillboard(OBJTYPE nPriority = OBJTYPE_MAGIC);
	~CBillboard();
	HRESULT Init(CTexture::Type type = CTexture::Text);
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void BindTexture(CTexture::Type type);			//画像の設定
	void Setpos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);//頂点情報設定
	void SetColor(D3DXCOLOR col);
	static CBillboard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CTexture::Type type);

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;									//ワールドマトリックス	
	D3DXMATRIX m_mtxView;									//ビューマトリックス
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;										//ポリゴンサイズ
	D3DXCOLOR m_col;

};


#endif // !_BILLBOARD_H_

