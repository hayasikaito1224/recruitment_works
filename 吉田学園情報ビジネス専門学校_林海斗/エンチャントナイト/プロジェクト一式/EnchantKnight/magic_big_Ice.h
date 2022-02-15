//===============================================-
//でっかい氷魔法のクラス
//===============================================
#ifndef _MAGIC_BIG_ICE_H_
#define _MAGIC_BIG_ICE_H_
#include "magic.h"
#define ICE_MP (20)//MP消費量

class CModel;
class C_Magic_Big_Ice : public C_Magic
{
public:
	C_Magic_Big_Ice(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic_Big_Ice();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Magic_Big_Ice *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,bool bRotX, float m_fSpeed);

private:
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス
	bool m_bAttack;
	float	m_fSpeed;
	D3DXVECTOR3 ModelRot;
	CModel *m_pModel;
};
#endif // !_MAGIC_BIG_ICE_H_
