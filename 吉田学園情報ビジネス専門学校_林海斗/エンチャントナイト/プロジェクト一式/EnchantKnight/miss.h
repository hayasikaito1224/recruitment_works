//--------------------------------------
//MISSの処理
//--------------------------------------
#ifndef _MISS_H
#define _MISS_H
#include "main.h"
#include "scene.h"
#include "texture.h"

#define MAX_MISS_POLYGON (4)
#define MAX_MISS_TYPE (2)

class CPolygon;

//前方宣言
class CMiss : public CScene
{
public:
	CMiss(OBJTYPE nPriority = OBJTYPE_UI);
	~CMiss();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMiss *Create(D3DXVECTOR3 pos);//インスタンス生成処理
private:
	CPolygon	*m_pMiss[MAX_MISS_POLYGON];
	CPolygon	*m_pBg;

	D3DXVECTOR3 m_pos;
	float		m_fMove;
	float		m_fAlpha,m_fAlpha2;
	int			m_nTimer;
	int			m_nType;
	bool		m_bSelectOn;	//選択画面を表示
	bool		m_bUninit;
	bool		m_bNextMode;
};


#endif //_MISS_H