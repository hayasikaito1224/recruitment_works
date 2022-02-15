//--------------------------------------
//タイトルの処理
//--------------------------------------
#ifndef _TITLE_H
#define _TITLE_H
#include "main.h"
#include "scene.h"
#define MAX_TITLE_POLYGON (5)
//前方宣言
class CBg;
class CPolygon;
class CSound;
class CTitle
{
public:
	typedef enum
	{
		GAME_START=0,
		OPERATION,
		MAX,
	}TYPE;
	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void NextMode(bool bNextMode) { m_bNextMode = bNextMode; }

private:
	CPolygon			*m_Polygon;
	int					m_nSelectType;//選択したタイプ
	bool				m_bNextMode;//次のモードに行くための
	bool				m_bOpra;//チュートリアルUIを選択しているか
};


#endif //_TITLE_H