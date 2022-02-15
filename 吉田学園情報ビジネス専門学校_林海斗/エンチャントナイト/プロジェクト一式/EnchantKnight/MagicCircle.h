#ifndef _MAGICCIRCLE_H_
#define _MAGICCIRCLE_H_
#include "effect.h"
#include "texture.h"
class CParticle;
class CShadow;
class CMagicCircle : public CEffect
{
public:
	CMagicCircle(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CMagicCircle();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SizeAdd();
	void SizeDef();
	void ColSizeAdd();
	void SetSize();
	void SetCol(D3DXCOLOR col) { m_col = col; }
	void SetbSizeAdd(bool bSizeAdd) { m_bSizeAdd = bSizeAdd; }
	void SetbSizeDef(bool bSizeDef) { m_bSizeDef = bSizeDef; }

	/*
	�ʒu�̐ݒ�,
	�T�C�Y,
	������܂ł̎���
	true�ɂ�����o�������玞�Ԃ�����������鏈���ɂȂ�
	Texture
	*/

	static CMagicCircle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fsize, float fMaxTime, bool bRot, bool bEraseStart, D3DXCOLOR col, CTexture::Type type = CTexture::Text);

private:
	CShadow *pShadow;
	CParticle *m_pParticle;
	bool m_bSizeAdd,m_bSizeDef,m_bColDef;//�T�C�Y��傫�����邩�ǂ����̔���
	float m_fMaxSize;//�ő�̑傫��
	float m_fRotSpeed;
	float m_fEraseTime;//������܂ł̃J�E���g
	float m_fEraseMaxTime;//������܂ł̃J�E���g
	bool m_bErase, m_bEraseStart, m_bEraseSizeStart;//�����܂ł̏����𓮍삷�邩�̔���
	bool m_bUninit;
	bool m_bEffect;
	bool m_bRotX;//�c��]������]��
};


#endif // !_MAGICCIRCLE_H_

