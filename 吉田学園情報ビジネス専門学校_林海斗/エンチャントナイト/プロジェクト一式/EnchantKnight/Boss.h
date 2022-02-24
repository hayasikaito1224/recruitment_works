//=======================================
//�{�X�̏���
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
	//�ÓI�����o�[�֐�
	static CBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nType,int nLife);
	bool IsGimmickLock() { return m_bGimmickLock; }
	bool GetbBattle() { return m_bBattle; }
	CEnemy* GetEnemy() { return m_pEnemy; }

private:
	D3DXVECTOR3 m_pos;//�ʒu
	D3DXVECTOR3 m_rot;//����
	CEnemy* m_pEnemy;//�G�̊i�[
	int m_nEnemyType;//�G�̎��
	bool m_bGimmickLock;//�d�|���̃��b�N
	bool m_bBattle;//�퓬�����ǂ���
};


#endif // !_PLAYER_H_

