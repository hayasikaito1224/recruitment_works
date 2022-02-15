//-------------------------------
//bumperのクラスの処理
//作成者：林海斗
//-------------------------------
#ifndef _BUMPER_H_
#define _BUMPER_H_
#include "scene.h"
class CModel_Spawner;

class CBumper : public CScene
{
public:
	CBumper(OBJTYPE nPriority = CScene::OBJTYPE_MODEL);
	~CBumper();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CBumper *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Knockback(D3DXVECTOR3& Playerpos, const D3DXVECTOR3& PlayerVec);//プレイヤーにノックバックさせる処理

private:
	CModel_Spawner *pModelSpawner;//バンパーのモデル
	D3DXVECTOR3	m_pos, m_rot;//動作関係
	float m_fKnockBackSpeed;//ノックバックのスピード
	bool m_bKnockBack;//ノックバックするかの判定
};
#endif