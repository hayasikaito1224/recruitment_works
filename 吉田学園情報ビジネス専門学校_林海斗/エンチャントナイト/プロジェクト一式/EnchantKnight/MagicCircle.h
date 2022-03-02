//=========================================
//���@�w�̏���
//=========================================
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
	void SizeAdd();//�T�C�Y���傫���Ȃ鏈��
	void SizeDef();//�T�C�Y���������Ȃ鏈��
	void ColSizeAdd();//�T�C�Y���傫�����邾���̏���
	void SetSize();//�T�C�Y�̐ݒ�
	void SetCol(D3DXCOLOR col) { m_col = col; }//�F�̐ݒ�
	void SetbSizeAdd(bool bSizeAdd, float fSizeSpeed = 1.5f)//�T�C�Y�ύX�ݒ�̏���
	{ m_bSizeAdd = bSizeAdd;
	m_fScaleSpeed = fSizeSpeed; }
	void SetbSizeDef(bool bSizeDef) { m_bSizeDef = bSizeDef; }//�T�C�Y�ύX�ݒ�̏���
	void SetbEraseStart(bool bStart, float fSizeSpeed = 1.5f)//���@�w�����̏���
	{ m_bEraseStart = bStart;
	m_fScaleSpeed = fSizeSpeed;
	}
	/*
	�ʒu�̐ݒ�,
	�T�C�Y,
	������܂ł̎���
	true�ɂ�����o�������玞�Ԃ�����������鏈���ɂȂ�
	Texture
	*/
	static CMagicCircle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fsize, float fMaxTime, bool bRot,
		bool bEraseStart, D3DXCOLOR col, CTexture::Type type = CTexture::Text,const float fSpeed = 1.5f);

private:
	CShadow *pShadow;//�e
	CParticle *m_pParticle;//�p�[�e�B�N��
	bool m_bSizeAdd,m_bSizeDef,m_bColDef;//�T�C�Y��傫�����邩�ǂ����̔���
	float m_fMaxSize;//�ő�̑傫��
	float m_fRotSpeed;//��]���鑬�x
	float m_fEraseTime;//������܂ł̃J�E���g
	float m_fEraseMaxTime;//������܂ł̃J�E���g
	bool m_bErase, m_bEraseStart, m_bEraseSizeStart;//�����܂ł̏����𓮍삷�邩�̔���
	bool m_bUninit;//�I������
	bool m_bRotX;//�c��]������]��
};


#endif // !_MAGICCIRCLE_H_

