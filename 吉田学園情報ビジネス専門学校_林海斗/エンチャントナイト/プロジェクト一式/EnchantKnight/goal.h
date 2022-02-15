#ifndef _GOAL_H_
#define _GOAL_H_
#include "billboard.h"
#include "texture.h"

class CGoal : public CBillboard
{
public:
	CGoal(OBJTYPE nPriority = OBJTYPE_ITEM);
	~CGoal();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CGoal *Create(D3DXVECTOR3 pos,D3DXVECTOR3 size, CTexture::Type Tex);
	CBillboard *GetBill(void) { return m_pBillboard; }
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;										//ポリゴンサイズ
	D3DXCOLOR m_col;
	CBillboard	*m_pBillboard;
};
#endif // !_GOAL_H_

