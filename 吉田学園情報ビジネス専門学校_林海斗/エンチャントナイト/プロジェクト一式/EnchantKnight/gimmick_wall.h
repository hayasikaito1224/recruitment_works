//----------------------------------
//�M�~�b�N�ǂ̏���
//----------------------------------
#ifndef _GIMMICK_WALL_H_
#define _GIMMICK_WALL_H_
#include "scene3D.h"
#include "texture.h"
#define MAX_GIMMICK_WALL_VTX (4)

class CGimmickWall : public CScene3D
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	}WallVtx;//���f���̊e���_�̕ۑ��p�\����

	CGimmickWall(OBJTYPE nPriority = OBJTYPE_GIMMICKWALL);
	~CGimmickWall();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CGimmickWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot,int nNumber,CTexture::Type Tex);
	WallVtx GetVtx(int nCnt) { return m_vtx[nCnt]; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	bool TestCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius);
	void GimmickUnlock(int nNumber);
private:
	CTexture::Type m_texType;//�e�N�X�`���̎��
	D3DXCOLOR m_col;
	WallVtx				m_vtx[MAX_GIMMICK_WALL_VTX];			//�e���_�̏��i�[�\����
	int m_nNumber;//�ǂ̔ԍ�
	bool m_bUninit;
};


#endif // !_GIMMICK_WALL_H_

