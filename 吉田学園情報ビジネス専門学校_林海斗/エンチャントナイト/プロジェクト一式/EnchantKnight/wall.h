#ifndef _WALL_H_
#define _WALL_H_
#include "scene3D.h"
#include "texture.h"
#define MAX_WALL_VTX (4)

class CWall : public CScene3D
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	}WallVtx;//���f���̊e���_�̕ۑ��p�\����
	typedef struct
	{
		D3DXVECTOR3 m_size, m_InitSize;//���b�V���̃T�C�Y
		int m_nMax_Vtx_X;		//���u���b�N�̐�
		int m_nMax_Vtx_Y;		//�c�u���b�N�̐�

	}WallData;//���f���̊e���_�̕ۑ��p�\����

	CWall(OBJTYPE nPriority = OBJTYPE_WALL);
	~CWall();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nNum_X, int nNum_Y, CTexture::Type Tex);
	WallVtx GetVtx(int nCnt) { return m_vtx[nCnt]; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetSize(D3DXVECTOR3 size);//�����f�[�^�������f
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius);
	bool TestCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius);
	void SetMoveTex(float fSpeedX, float fSpeedY);
	void SetCol(float fAlpha);
	WallData GetWallData() { return m_WallData; }
private:
	WallData m_WallData;
	D3DXCOLOR m_col;
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuffWall;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WallVtx				m_vtx[MAX_WALL_VTX];			//�e���_�̏��i�[�\����
	D3DXVECTOR3				m_vtxMin, m_vtxMax;				//���f���̍ŏ��l�A�ő�l
	float m_MoveTexX, m_MoveTexY;
	int m_nVtx_Num_Max;		//�S�̒��_�̐�
	int m_nIdx_Num_Max;		//�C���f�b�N�X�̐�
	int m_nPoly_Num_Max;		//�O�p�`�̃|���S���̐�
	float m_fHalfWidth;		//�����̃T�C�Y
	float m_fHalfDepth;		//�����̃T�C�Y
};


#endif // !_WALL_H_

