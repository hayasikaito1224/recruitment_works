//=============================================================================
// �����蔻��G���A�̕\�������̔w�i
//=============================================================================
#ifndef _COL_AREA_BG_H_
#define _COL_AREA_BG_H_

#include "scene.h"
#include "texture.h"
#define AREA_VTX (4)
class CTexture;

class CCol_Area_Bg : public CScene
{

public:

	CCol_Area_Bg(OBJTYPE nPriority = OBJTYPE_EFFECT);		//�R���X�g���N�^
	~CCol_Area_Bg();									//�f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCol_Area_Bg *Create(const D3DXVECTOR3 pos, const float fRotY, const float fSizeX, const float fSizeZ,
		const float fAlpha, const int nValueMax, const CTexture::Type texture);

private:

	CTexture::Type m_Tex;//�e�N�X�`���̎��
	bool m_bDraw;//�`�攻��
	bool m_bUninit;//��������
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pDevice = NULL;//�f�o�C�X�̃|�C���^
	D3DXCOLOR m_col;//�F
	D3DXMATRIX m_mtxWorld;//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;//�ʒu
	D3DXVECTOR3 m_rot;//����
	D3DXVECTOR3 m_size;//�傫��
	float				m_fMaxGauge;	//gauge�̍ő吔
	float				m_fValueMax;	//�Q�[�W�̒l�̍ő吔
	float				m_fValue;		//�Q�[�W�̐��l

};

#endif // _COL_AREA_BG_H_
