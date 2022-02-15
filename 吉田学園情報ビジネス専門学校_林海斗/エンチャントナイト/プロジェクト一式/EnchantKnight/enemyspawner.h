//=======================================
//敵のスポナーの処理
//=======================================
#ifndef _ENEMY_SPAWNER_H_
#define _ENEMY_SPAWNER_H_
#include "scene.h"
class CEnemy;
class CEnemy_Spawner : public CScene
{
public:

	CEnemy_Spawner(OBJTYPE nPriority = CScene::OBJTYPE_ENEMY);
	~CEnemy_Spawner();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	//静的メンバー関数
	static CEnemy_Spawner *Create(D3DXVECTOR3 pos,float fAreaX, float fAreaZ,int nMaxEnemy);
	bool IsGimmickLock() { return m_bGimmickLock; }
	CEnemy* GetEnemy(int nNum) { return m_pEnemy[nNum]; }
private:
	D3DXVECTOR3 m_pos;
	std::vector<CEnemy*> m_pEnemy;//敵の格納
	int m_nNumEnemy;//敵の数
	float m_fAreaX;
	float m_fAreaZ;
	bool m_bGimmickLock;//仕掛けのロック
};


#endif // !_PLAYER_H_

