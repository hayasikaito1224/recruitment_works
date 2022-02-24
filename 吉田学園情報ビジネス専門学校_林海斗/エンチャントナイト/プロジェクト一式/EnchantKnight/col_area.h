//=============================================================================
//
// �����蔻��G���A�̕\������
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
		D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	}FieldVtx;//���f���̊e���_�̕ۑ��p�\����

	CCol_Area(OBJTYPE nPriority = OBJTYPE_EFFECT);		//�R���X�g���N�^
	~CCol_Area();									//�f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCol_Area *Create(const D3DXVECTOR3 pos, const float fRotY, const float fSizeX, const float fAlpha,
		const float MaxGauge, const int nValueMax, const CTexture::Type texture);
	void SetPos(D3DXVECTOR3 pos);
	void MoveVtx();//���_�̈ړ�
	FieldVtx GetVtx(int nCnt) { return m_vtx[nCnt]; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetScale(void) { return m_size; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }

private:
	FieldVtx				m_vtx[AREA_VTX];			//�e���_�̏��i�[�\����

	CTexture::Type m_Tex;
	bool m_bDraw;
	bool m_bUninit;//��������
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pDevice = NULL;//�f�o�C�X�̃|�C���^
	D3DXCOLOR m_col;
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	float				m_fMaxGauge;	//gauge�̍ő吔
	float				m_fValueMax;	//�Q�[�W�̒l�̍ő吔
	float				m_fValue;		//�Q�[�W�̐��l

};

#endif // _COL_AREA_H_
