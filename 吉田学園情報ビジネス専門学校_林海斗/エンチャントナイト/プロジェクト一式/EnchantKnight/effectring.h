//=========================================
//ring��̃G�t�F�N�g�𔭐������鏈��
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
	�ʒu�̐ݒ�,
	�ړ���,
	�T�C�Y,
	�������Ȃ�X�s�[�h
	Texture
	*/
	static CEffect_Ring *Create(D3DXVECTOR3 pos,D3DXCOLOR col, float fDefSpeed, float fSizeSpeed, CTexture::Type type = CTexture::Text);
private:
	D3DXVECTOR3 m_MaxSize;
	float m_fSizeSpeed;//�傫���Ȃ鑬��
};


#endif // !_EFFECT_RING_H_

