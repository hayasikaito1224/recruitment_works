//=============================================
//プレイヤーが操作するコマンド関係の処理
//=============================================
#ifndef _MAGICUI_H_
#define _MAGICUI_H_

#include "main.h"
#include "scene.h"
#include "texture.h"
class CPolygon;

class CMagicUI : CScene
{
public:

	typedef enum
	{
		Fire = 0,
		Ice,
		Heal,
		Magic_Max
	}Magic;

	CMagicUI(OBJTYPE nPriority = OBJTYPE_NONE);
	~CMagicUI();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CMagicUI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nTexType);
	void SetSelect(bool bSelect) { m_bSelect = bSelect; }
	void SetPush(bool bPush) { m_bPush = bPush; }
	void SetAlphaFade(bool bFade) { m_bAlphaFadeIn = bFade; }
	float GetMove() { return m_fMove; }
	void AlphaFade();//α値のフェード処理
	D3DXVECTOR3 GetPos() { return m_pos; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
private:
	CPolygon *m_pCommandBG;
	CPolygon *m_pMagic;
	CPolygon *m_pButton;

	D3DXVECTOR3 m_pos;
	bool		m_bSelect;//このコマンドが選択されているか
	bool		m_bPush;
	bool	m_bAlphaFadeIn;
	float		m_fMove;
	float		m_fColA;
	bool		m_bDraw;
	int m_nTexType;
	D3DXVECTOR3 m_size, m_InitPos;
	D3DXCOLOR m_col;
};

#endif // !_MAGICUI_H_

