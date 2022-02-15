//=========================================
//ring状のエフェクトを発生させる処理
//=========================================
#ifndef _EFFECT_RING_H_
#define _EFFECT_RING_H_
#include "effect.h"

class CEffect_Ring : public CEffect
{
public:
	CEffect_Ring(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CEffect_Ring();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetSize(D3DXVECTOR3 size);

	/*
	位置の設定,
	移動量,
	サイズ,
	小さくなるスピード
	Texture
	*/
	static CEffect_Ring *Create(D3DXVECTOR3 pos,D3DXCOLOR col, float fDefSpeed, float fSizeSpeed, CTexture::Type type = CTexture::Text);
private:
	D3DXVECTOR3 m_MaxSize;
	float m_fSizeSpeed;//大きくなる速さ
};


#endif // !_EFFECT_RING_H_

