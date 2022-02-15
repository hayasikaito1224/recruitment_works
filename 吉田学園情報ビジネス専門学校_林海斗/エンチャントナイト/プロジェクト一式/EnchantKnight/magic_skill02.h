//===============================================-
//CP消費スキル（フリージングショット）の処理
//===============================================
#ifndef _MAGIC_SKILL02_H_
#define _MAGIC_SKILL02_H_
#include "magic.h"
#define MAGIC_SKILL02_MP (20)//MP消費量

//class C_Magic_Ice;

class CModel;

class C_Magic_Skill02 : public C_Magic
{
public:
	C_Magic_Skill02(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic_Skill02();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Magic_Skill02 *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:
	int m_nTimer, m_nMaxTime;//ショット間隔系
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス
	bool m_bAttack;
	D3DXVECTOR3 ModelRot;
	//C_Magic_Ice *m_pIce;

};
#endif // !_MAGIC_SKILL02_H_
