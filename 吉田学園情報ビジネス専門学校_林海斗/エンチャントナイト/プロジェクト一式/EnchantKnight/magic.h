//===============================================-
//魔法に関するベースクラス
//===============================================
#ifndef _MAGIC_H_
#define _MAGIC_H_
#include "main.h"
#include "scene.h"
class C_Magic : public CScene
{
public:
	C_Magic(OBJTYPE nPriority = CScene::OBJTYPE_MAGIC);
	~C_Magic();

	virtual HRESULT Init()=0;
	virtual void Uninit()=0;
	virtual void Update()=0;
	virtual void Draw()=0;

	int Sub(int& nMP) { return nMP - m_ConsumingMP; }//魔力の消費処理(プレイヤーが持っている魔力ポイントを引数に入れる)

private:
protected:
	D3DXVECTOR3 m_pos, m_move, m_col,m_rot,m_MoveSpeed;
	bool		m_bUninit;//消去判定
	int			m_nDeleteTimer;
	int			m_ConsumingMP;//各魔法の属性ごとに持っている魔力の消費ポイント
	int			m_nPower;//攻撃力

};
#endif // !_MAGIC_H_
