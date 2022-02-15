//===============================================-
//氷魔法のクラス
//===============================================
#ifndef _MAGIC_ICE_H_
#define _MAGIC_ICE_H_
#include "magic.h"
#define ICE_MP (20)//MP消費量

class CModel;
class C_Magic_Ice : public C_Magic
{
public:
	C_Magic_Ice(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic_Ice();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Magic_Ice *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float m_fSpeed);

private:
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス
	bool m_bAttack;
	float	m_fSpeed;
	D3DXVECTOR3 ModelRot;
	CModel *m_pModel;
};
#endif // !_MAGIC_ICE_H_
