//=============================================================================
//
// Polygon����
//
//=============================================================================
#ifndef _COL_AREA_H_
#define _COL_AREA_H_

#include "scene2D.h"
#define AREA_VTX (4)
class CTexture;

class CCol_Area
{

public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	}FieldVtx;//���f���̊e���_�̕ۑ��p�\����

	CCol_Area();		//�R���X�g���N�^
	~CCol_Area();									//�f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCol_Area *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, CTexture::Type texture, D3DXCOLOR col = { 1.0,1.0,1.0,1.0 });
	void SetPos(D3DXVECTOR3 pos);
	FieldVtx GetVtx(int nCnt) { return m_vtx[nCnt]; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetScale(void) { return m_size; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }

private:
	FieldVtx				m_vtx[AREA_VTX];			//�e���_�̏��i�[�\����

	CTexture::Type m_Tex;
	bool m_bDraw;
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pDevice = NULL;//�f�o�C�X�̃|�C���^
	D3DXCOLOR m_col;
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;

};

#endif // _COL_AREA_H_
