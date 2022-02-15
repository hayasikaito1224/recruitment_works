#ifndef _SWORDEFFECT_H_
#define _SWORDEFFECT_H_
#include "scene3D.h"
#include "texture.h"
#define MAX_SWORDEFFECT_VTX (4)
class CSwordEffect : public CScene3D
{
public:

	CSwordEffect(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CSwordEffect();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CSwordEffect *Create(D3DXVECTOR3 Startpos, D3DXVECTOR3 Endpos,D3DXVECTOR3 pos,int nNum_X, CTexture::Type Tex);
	D3DXVECTOR3 GetS_Pos(void) { return m_pos; }
	void SetMoveTex(float fSpeedX, float fSpeedY);
	void SetCol(float fAlpha);
	void SetSwordPos(D3DXVECTOR3 Startpos, D3DXVECTOR3 Endpos);
	void SetLastSwordPos(D3DXVECTOR3 Startpos, D3DXVECTOR3 Endpos);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void Drawtext();
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	D3DXVECTOR3 GetStartPos() { return VtxPos[0]; }
	D3DXVECTOR3 GetEndPos() { return VtxPos[1]; }

private:
	std::vector<D3DXVECTOR3> VtxPos;
	D3DXVECTOR3 m_size, m_LastStartpos,m_LastEndpos;										//�|���S���T�C�Y
	D3DXCOLOR m_col;
	int m_nCntLastPos;
	bool m_bDraw;
	float m_MoveTexX, m_MoveTexZ;
	int m_nMax_Vtx_X;		//���u���b�N�̐�
	int m_nMax_Vtx_Z;		//�c�u���b�N�̐�
	int m_nVtx_Num_Max;		//�S�̒��_�̐�
	int m_nIdx_Num_Max;		//�C���f�b�N�X�̐�
	int m_nPoly_Num_Max;		//�O�p�`�̃|���S���̐�
};


#endif // !_SWORDEFFECT_H_

