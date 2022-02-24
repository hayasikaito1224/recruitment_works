#ifndef _ENEMY_CRYSTAL_H_
#define _ENEMY_CRYSTAL_H_

#include "Enemy.h"

class CEnemy_Crystal : public CEnemy
{
public:
	typedef enum
	{
		FIRE = 0,
		BULLIZAD,
	}MAGIC;
	CEnemy_Crystal(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	~CEnemy_Crystal();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Colision();
	void AIAttack();
	void AIMove();
	void AddLife(int nPower, int nType);
	void AddLife(int nLife);//体力の増減
	//静的メンバー関数
	static CEnemy_Crystal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
	bool m_bDiscovery;//敵がプレイヤーを発見したか
	int m_nTimer;
	int m_nBreakTime;//攻撃したあとのクールタイム
	bool m_bBreakTime;//攻撃したあとのクールタイム
	D3DXCOLOR m_col;
};


#endif // !_PLAYER_H_

