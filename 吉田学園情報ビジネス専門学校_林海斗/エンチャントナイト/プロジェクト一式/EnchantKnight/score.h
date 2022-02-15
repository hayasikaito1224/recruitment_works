#ifndef _SCORE_H_
#define _SCORE_H_
#include "scene.h"
class CScene2D;
#define MAX_SCORE (3)//最大描画枚数


class CScore : public CScene
{
public:
	CScore(OBJTYPE nPriority = OBJTYPE_NONE);
	~CScore();
	static CScore*Create(D3DXVECTOR3 pos, float fsize);
	HRESULT Init(D3DXVECTOR3 pos, float fsize);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScore(void);//スコアの描画設定
	static void AddScore(int AddScore) { m_nScore += AddScore; };//スコアの加算
private:
	CScene2D *m_apScene2D[MAX_SCORE];
	int m_nAddScore;//スコア加算用変数
	static int m_nScore;//現在のスコア
};
#endif
