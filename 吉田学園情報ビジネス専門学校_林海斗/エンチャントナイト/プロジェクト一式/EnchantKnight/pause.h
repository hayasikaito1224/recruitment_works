//--------------------------------------
//PAUSEの処理
//--------------------------------------
#ifndef _PAUSE_H
#define _PAUSE_H
#include "main.h"
#include "scene.h"
#include "texture.h"

#define MAX_PAUSE (3)
#define PAUSE_INERVAL (100)
class CPolygon;
//前方宣言
class CPause : public CScene
{
public:
	typedef enum
	{
		FADE_IN = 0,
		FADE_OUT,
	}PAUSE_FADE;

	CPause(OBJTYPE nPriority = OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create(D3DXVECTOR3 pos);//インスタンス生成処理
private:
	void FadeInOut(int nFade);
	void SetPAUSE(int nType);							//PAUSEに使うポリゴンの生成
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_Speed;
	float			m_fAlpha;
	float			m_fBossPAUSEMove;
	int				m_nPAUSEType;
	bool			m_bUninit;
	bool			m_bNextMode;
	CPolygon		*m_pPolygon[MAX_PAUSE];
	CPolygon		*m_pCursor;
	CPolygon		*m_pBg;
	CPolygon		*m_pTutorialUI;	//操作説明UI

};


#endif //_PAUSE_H