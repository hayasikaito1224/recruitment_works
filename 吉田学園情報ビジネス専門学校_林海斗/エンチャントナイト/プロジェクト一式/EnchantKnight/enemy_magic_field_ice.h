//===============================================-
//CP消費スキル（フリージングショット）の処理
//===============================================
#ifndef _ENEMY_MAGIC_FIELD_ICE_H_
#define _ENEMY_MAGIC_FIELD_ICE_H_
#include "magic.h"
#define ENEMY_MAGIC_FIELD_ICE_MP (20)//MP消費量
#define SKILL02_COLLISIONVTX (4)
#define ENEMY_MAGIC_FIELD_ICE_SIZE_Z (30)//氷のサイズ
#define ENEMY_MAGIC_FIELD_ICE_SIZE_X (30)//氷のサイズ

//class C_Magic_Ice;

class CModel;
class CCol_Area;
class C_Enemy_Magic_Field_Ice : public C_Magic
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//ワールドマトリックス
	}Vtx;//各頂点の保存用構造体

	C_Enemy_Magic_Field_Ice(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Enemy_Magic_Field_Ice();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Enemy_Magic_Field_Ice *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const float fSpeed,const int nMaxCnt);
	bool Collision(D3DXVECTOR3 *pPos,float fRadius);
private:
	int m_nTimer, m_nMaxTime;//ショット間隔系
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス
	bool m_bAttack;
	D3DXVECTOR3 ModelRot;
	int m_nCnt;//今出している氷の塊の個数
	int m_nMaxCnt;//出す氷の塊の最大数
	float m_fSpeed;//球が出る速度
	Vtx m_vtx[SKILL02_COLLISIONVTX];
	int m_nAttackTimer;
	CCol_Area *m_pArea;
	//C_Magic_Ice *m_pIce;

};
#endif // !_ENEMY_MAGIC_FIELD_ICE_H_
