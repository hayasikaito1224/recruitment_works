//===============================================-
//炎魔法のクラス
//===============================================
#ifndef _ENEMY_MAGIC_FIRE_H_
#define _ENEMY_MAGIC_FIRE_H_
#include "magic.h"

//前方宣言
class CModel;
class CPlayer;
class CShadow;

class C_Enemy_Magic_Fire : public C_Magic
{
public:
	C_Enemy_Magic_Fire(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Enemy_Magic_Fire();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static C_Enemy_Magic_Fire *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,const bool bBezier = false,
		const float Shotrot = 0.0f);
	void BezierCurve();//ベジェ曲線の処理
private:
	CShadow *m_pShadow;//影

	CModel *m_pFireModel;//ファイアーボール用のポリゴン.
	D3DXVECTOR3 ModelRot;
	D3DXMATRIX m_mtxWorld;											//ワールドマトリックス
	D3DXVECTOR3 m_StartingPoint;//始点
	D3DXVECTOR3 m_EndPoint;//終点
	D3DXVECTOR3 m_ControlPoint;//制御点
	float  m_nTime;//始点から終点まで進む時間
	float  m_nTimeAddSpeed;//始点から終点まで進む時間
	CPlayer *m_pPlayer;
	bool m_bAttack;
	bool m_bBezier;//ベジェ曲線のオンオフ(true=オン)
};
#endif // !_ENEMY_MAGIC_FIRE_H_
