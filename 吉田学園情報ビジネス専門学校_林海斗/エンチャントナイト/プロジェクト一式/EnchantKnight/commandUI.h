//=============================================
//プレイヤーが操作するコマンド関係の処理
//=============================================
#ifndef _COMMANDUI_H_
#define _COMMANDUI_H_

#include "main.h"
#include "scene2D.h"
#include "texture.h"

class CCommandUI : CScene2D
{
public:
	typedef enum
	{
		Co_Attack = 0,
		Co_Magic,
		Co_Enhance,
		Co_Max
	}CommandType;
	typedef enum
	{
		Fire = 0,
		Ice,
		Heal,
		Magic_Max
	}Magic;

	CCommandUI(OBJTYPE nPriority = OBJTYPE_NONE);
	~CCommandUI();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CCommandUI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,int nTexType);
	void SetSelect(bool bSelect) { m_bSelect = bSelect; }
	void SetPush(bool bPush) { m_bPush = bPush; }
	float GetMove() { return m_fMove; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
private:
	bool		m_bSelect;//このコマンドが選択されているか
	bool		m_bPush;
	float		m_fMove;
	float		m_fColA;
	bool		m_bDraw;
	D3DXVECTOR3 m_size,m_InitPos;
	D3DXCOLOR m_col;
};

#endif // !_COMMANDUI_H_

