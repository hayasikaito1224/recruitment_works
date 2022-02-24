//=======================================
//ボスの処理
//=======================================
#ifndef _BOSS_H_
#define _BOSS_H_
#include "scene.h"
class CEnemy;
class CBoss : public CScene
{
public:
	enum EnemyType
	{
		Type_Purple = 0,
		Type_Max
	}; 

	CBoss(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	~CBoss();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	//静的メンバー関数
	static CBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType,int nLife);
	bool IsGimmickLock() { return m_bGimmickLock; }
	bool GetbBattle() { return m_bBattle; }
	CEnemy* GetEnemy() { return m_pEnemy; }

private:
	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	CEnemy* m_pEnemy;//敵の格納
	int m_nEnemyType;//敵の種類
	bool m_bGimmickLock;//仕掛けのロック
	bool m_bBattle;//戦闘中かどうか
};


#endif // !_PLAYER_H_

