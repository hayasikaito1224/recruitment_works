//=======================================
//�G�̃X�|�i�[�̏���
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
	//�ÓI�����o�[�֐�
	static CEnemy_Spawner *Create(D3DXVECTOR3 pos,float fAreaX, float fAreaZ,int nMaxEnemy);
	bool IsGimmickLock() { return m_bGimmickLock; }
	CEnemy* GetEnemy(int nNum) { return m_pEnemy[nNum]; }
private:
	D3DXVECTOR3 m_pos;
	std::vector<CEnemy*> m_pEnemy;//�G�̊i�[
	int m_nNumEnemy;//�G�̐�
	float m_fAreaX;
	float m_fAreaZ;
	bool m_bGimmickLock;//�d�|���̃��b�N
};


#endif // !_PLAYER_H_

