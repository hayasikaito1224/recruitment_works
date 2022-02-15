#ifndef _COLLISION_H
#define _COLLISION_H
//----------------------------------------
//当たり判定の処理
//----------------------------------------
#include "scene.h"
class CModel_Spawner;
class CField;
class CEnemy;
class CPlayer;

class CCollision : public CScene
{
public:

	typedef enum
	{
		COLLISION_NONE = 0,
		COLLISION_UP,
		COLLISION_DOWN,
		COLLISION_MAX,
	}COLLISION_TYPE;

	CCollision(OBJTYPE nPriority=CScene::OBJTYPE_NONE);
	~CCollision();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static bool SetCollision(CScene *pHit_Side, CScene *pAttack_Side);
	bool CollisionField(CField *field, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius, float& fModelPos_Y);//設置物との当たり判定
	bool CollisionWeapon(CEnemy *Model, D3DXVECTOR3 pos,float fRadius);//設置物との当たり判定
	void CollisionObjectEnemy(CEnemy *Model, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius);//設置物との当たり判定
	void CollisionObjectPlayer(CPlayer *Model, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius);//設置物との当たり判定
	COLLISION_TYPE CollisionObjectModelShadow(CModel_Spawner *Model, D3DXVECTOR3 *pPos, float fRadius, float& fModelPos_Y);//設置物との当たり判定
	bool CollisionFieldShadow(CField *field, D3DXVECTOR3 *pPos, float fRadius, float& fModelPos_Y);//設置物との当たり判定
	void TestCollision(void);
	COLLISION_TYPE CollisionObjectModel(CModel_Spawner *Model,D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius,float& fModelPos_Y);//設置物との当たり判定
	bool CollisionAttack(const D3DXVECTOR3& AttackPos, const D3DXVECTOR3& Hitpos, const D3DXVECTOR3& HeadPos, float fRadius);//設置物との当たり判定

private:
};


#endif // !_COLLISION_H

