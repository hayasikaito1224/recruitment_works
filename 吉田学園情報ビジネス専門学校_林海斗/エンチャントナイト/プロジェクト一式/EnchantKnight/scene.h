//----------------------------------
//オブジェクトの処理
//----------------------------------
#ifndef _SCENE_H
#define _SCENE_H
#include "main.h"

class CScene
{

public:

	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_BG,
		OBJTYPE_FIELD,				//床
		OBJTYPE_WALL,				//壁
		OBJTYPE_SKY,				//床
		OBJTYPE_MODEL,				//モデル
		OBJTYPE_BLOCK,				//モデル
		OBJTYPE_WOOD,				//モデル
		OBJTYPE_PLAYER,				//プレイヤー
		OBJTYPE_ENEMY,				//敵
		OBJTYPE_ENEMY_BAUM,				//敵
		OBJTYPE_MAGIC,				//魔法
		OBJTYPE_EFFECT,				//エフェクト
		OBJTYPE_ITEM,					//UI
		OBJTYPE_POLYGON,
		OBJTYPE_MAP_UI,				//マップに表示するUI
		OBJTYPE_UI,					//UI
		OBJTYPE_PAUSE,				//PAUSE
		OBJTYPE_FADE,				//フェード
		OBJTYPE_MAX
	}OBJTYPE;
	CScene();
	CScene(OBJTYPE nPriority);
	virtual~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static void MapDraw(void);
	void SetObjType(OBJTYPE objType) { m_ObjType = objType; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	void SetLife(int nLife) { m_nLife = nLife; }
	void SetDamage(bool bDamage) { m_bDamage = bDamage; }
	void LifeSub(int nPow);
	int GetLife(void) { return m_nLife; }
	void SetPoint(int nPoint) { m_nPoint = nPoint; }
	int GetPoint(void) { return m_nPoint; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	CScene::OBJTYPE GetObjType(void) { return m_ObjType; }
	bool GetDamage(void) { return m_bDamage; }
	static CScene *GetScene(const int nPriority) { return m_pTop[nPriority]; }
	static CScene *GetSceneNext(CScene *pTop) { return pTop->m_pNext; }

	static void Delete(void);		//全てのインスタンスの破棄処理
	static void UpdateAll(void);	//全てのオブジェの更新	
	static void DrawAll(void);		//全てのオブジェの描画
	
private:
	D3DXVECTOR3				m_pos,m_oldpos;							//基準の位置
	D3DXVECTOR3				m_Scale;				//大きさ
	OBJTYPE					m_ObjType;				//オブジェクトタイプの種類
	static int				m_nNumAll[OBJTYPE_MAX];
	int						m_nID;						//格納先の番号
	int						m_nLife;
	int						m_nPoint;			//敵の点数
	int						m_nPriority;//優先順位
	bool					m_bDamage;					//ダメージ判定
	static CScene *m_pTop[OBJTYPE_MAX];						//先頭のオブジェクトのポインタ
	static CScene *m_pCur[OBJTYPE_MAX];						//現在（一番後ろ）のオブジェクトのポインタ
	CScene *m_pPrev;							//前のオブジェクトへのポインタ
	CScene *m_pNext;							//次のオブジェクトへのポインタ
	bool					m_bDath;
protected:
	void Release(void);//オブジェクト単体のインスタンス破棄

};


#endif // !_SCENE_H
