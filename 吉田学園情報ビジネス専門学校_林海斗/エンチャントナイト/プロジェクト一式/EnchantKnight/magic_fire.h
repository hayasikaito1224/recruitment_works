//===============================================-
//炎魔法のクラス
//===============================================
#ifndef _MAGIC_FIRE_H_
#define _MAGIC_FIRE_H_
#include "magic.h"
#define FIRE_MP (8)//MP消費量

//前方宣言
class CModel;

class C_Magic_Fire : public C_Magic
{
public:
	C_Magic_Fire(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic_Fire();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Magic_Fire *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:
	CModel *m_pFireModel;//ファイアーボール用のポリゴン.
	D3DXVECTOR3 ModelRot;
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス

	bool m_bAttack;
};
#endif // !_MAGIC_FIRE_H_
