//===============================================-
//炎魔法のクラス
//===============================================
#ifndef _ENEMY_MAGIC_ICE_H_
#define _ENEMY_MAGIC_ICE_H_
#include "magic.h"


//前方宣言
class CModel;
class CPlayer;
class C_Enemy_Magic_Ice : public C_Magic
{
public:
	C_Enemy_Magic_Ice(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Enemy_Magic_Ice();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Enemy_Magic_Ice *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
private:
	CModel *m_pIceModel;//アイスボール用のモデル.
	D3DXVECTOR3 ModelRot;
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス
	CPlayer *m_pPlayer;
	bool m_bAttack;
};
#endif // !_ENEMY_MAGIC_ICE_H_
