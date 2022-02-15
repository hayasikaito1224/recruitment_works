//----------------------------------
//モデルを配置するための処理
//----------------------------------
#ifndef _MODEL_SPAWNER_H
#define _MODEL_SPAWNER_H
#include "main.h"
#include "scene.h"
#include "texture.h"
class CModel;

//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CModel_Spawner : public CScene
{
public:
	CModel_Spawner(OBJTYPE nPriority = OBJTYPE_MODEL);
	~CModel_Spawner();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CModel_Spawner *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,const int nCreateModel);//rotは角度を入れて
	CModel *GetModel(void) { return m_pModel; }			//モデル情報の取得
	void SetRot(float fRot) {m_rot.y = fRot;}
	void SetSRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSPos(D3DXVECTOR3 pos) { m_pos = pos; }

	void SetBoolRot(bool bRot) { m_MoveRot = bRot; }
private:
	float m_fRot;
	bool m_MoveRot;//回転するモデルになるかの判定
	D3DXVECTOR3				m_pos;					//モデルの位置（オフセット）
	D3DXVECTOR3				m_rot;					//向き
	D3DXMATRIX				m_mtxWorld;				//ワールドマトリックス
	D3DXMATERIAL			*m_pMat;						//マテリアルへのポインタ
	CModel					*m_pModel;
};


#endif // !_MODEL_SPAWNER_H
