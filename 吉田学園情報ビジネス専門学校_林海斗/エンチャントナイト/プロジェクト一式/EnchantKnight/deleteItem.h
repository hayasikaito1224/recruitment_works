#ifndef _DELETEITEM_H
#define _DELETEITEM_H
#include "scene.h"
#include "texture.h"
#define DELETEITEM_CNT_LIMIT (20)
#define DELETEITEM_POINT (20)

class CDeleteItem
{
public:
	CDeleteItem();
	~CDeleteItem();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ItemSuction(void);								//アイテムが引き寄せられる関数
	void SetTex(D3DXVECTOR2 tex, D3DXVECTOR2 tex1, D3DXVECTOR2 tex2, D3DXVECTOR2 tex3);
	void SetCol(D3DXCOLOR col);							//カラーを変える
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }
	void SetPos(D3DXVECTOR3 pos);						//カラーを変える
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//位置の取得
	D3DXVECTOR3 GetScale(void) { return m_Scale; }		//大きさの取得
	static CDeleteItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, CTexture::Type type = CTexture::Text, float fAngle = 0.0f);//インスタンス生成処理
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;			//頂点バッファへのポインタ
	D3DXVECTOR3					m_Scale;				//大きさ
	D3DXVECTOR3					m_pos;					//基準の位置
	CTexture::Type				m_Type;					//表示するテクスチャのタイプ
	int							m_nTimer;				//タイマー
	float						m_fAngle;				//角度
	float						m_fSpeed;				//引き寄せられる速度
	float						m_fSucRadius;			//引き寄せの範囲
	bool						m_bDoSuction;			//引き寄せ状態か
	bool						m_bUninit;				//消去判定
};


#endif // !_DELETEITEM_H

