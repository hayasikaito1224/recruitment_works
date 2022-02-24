//----------------------------------
//オブジェクトの処理
//----------------------------------
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
//------------------------------------------------------------------------------
//静的メンバ変数宣言
//-----------------------------------------------------------------------------
int CScene::m_nNumAll[OBJTYPE_MAX] = {};
CScene *CScene::m_pCur[OBJTYPE_MAX] = {};
CScene *CScene::m_pTop[OBJTYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene()
{
}
CScene::CScene(OBJTYPE nPriority)
{
	//トップがNULLだったら先頭オブジェクト
	if (m_pTop[nPriority] == NULL)
	{
		m_pTop[nPriority] = this;
		m_pCur[nPriority] = this;
		m_pPrev = NULL;
		m_pNext = NULL;
		m_bDamage = false;

	}
	//二回目
	//m_pCurがNULLだったら現在のオブジェクト
	else
	{
		m_pCur[nPriority]->m_pNext = this;//前のオブジェクトの次は自分
		m_pPrev = m_pCur[nPriority];	//自分の前のオブジェクトを代入
		m_pCur[nPriority] = this;		//現在のオブジェクトを自分にする
		m_pNext = NULL;
		m_bDamage = false;

	}
	m_bDath = false;
	m_nPriority = nPriority;
}

//=============================================================================
// デストラクタ
//=============================================================================

CScene::~CScene()
{
	//ココのオブジェクトが生成されていたら
	if (this == m_pTop[m_nPriority] && this == m_pCur[m_nPriority])
	{
		m_pTop[m_nPriority] = NULL;
		m_pCur[m_nPriority] = NULL;
		m_pNext = NULL;
		m_pPrev = NULL;
	}
	else if (this == m_pTop[m_nPriority])
	{
		m_pNext->m_pPrev = NULL;
		m_pTop[m_nPriority] = m_pNext;

	}
	else if (this == m_pCur[m_nPriority])
	{
		m_pPrev->m_pNext = NULL;
		m_pCur[m_nPriority] = m_pPrev;

	}
	else
	{
		m_pPrev->m_pNext = m_pNext;
		m_pNext->m_pPrev = m_pPrev;
	}


}
//=============================================================================
//全てのオブジェの更新	
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		if (CManager::GetPause() == false)
		{
			if (m_pTop[nObj] != NULL)
			{
				CScene *m_pUpdate = m_pTop[nObj];				//次に更新するオブジェクトの格納
																//m_pUpdateにNULLが入るまで
				while (m_pUpdate != NULL)
				{
					m_pUpdate->Update();
					m_pUpdate = m_pUpdate->m_pNext;
				}
			}
		}
		else
		{
			if (nObj == OBJTYPE_PAUSE || nObj == OBJTYPE_UI || nObj == OBJTYPE_FADE)
			{
				if (m_pTop[nObj] != NULL)
				{
					CScene *m_pUpdate = m_pTop[nObj];				//次に更新するオブジェクトの格納
																	//m_pUpdateにNULLが入るまで
					while (m_pUpdate != NULL)
					{
						if (m_pUpdate->m_bDath == false)
						{
							m_pUpdate->Update();
						}
							m_pUpdate = m_pUpdate->m_pNext;

					}
				}
			}
		}
	}

	//死亡フラグ判定
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		CScene *m_pUpdate = m_pTop[nObj];				//次に更新するオブジェクトの格納
		CScene *m_pDeleteUpdate = NULL;				//次に更新するオブジェクトの格納
		//オブジェクトがNULLになるまで
		while (m_pUpdate)
		{
			//次のデータが消えないように保存
			m_pDeleteUpdate = m_pUpdate->m_pNext;
			//死亡フラグが立っていたら
			if (m_pUpdate->m_bDath == true)
			{
				//消す
				delete m_pUpdate;
				m_pUpdate = NULL;
			}
			//次のオブジェクトにする
			m_pUpdate = m_pDeleteUpdate;

		}
	}

}
//=============================================================================
//全てのオブジェの描画
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		if (nObj != OBJTYPE_MAP_UI)
		{
			if (m_pTop[nObj] != NULL)
			{
				CScene *m_pDraw = m_pTop[nObj];				//次に更新するオブジェクトの格納
															//m_pUpdateにNULLが入るまで
				while (m_pDraw)
				{
					if (m_pDraw->m_bDath == false)
					{
						m_pDraw->Draw();
					}
					m_pDraw = m_pDraw->m_pNext;

				}
			}

		}
	}
	//死亡フラグ判定
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		CScene *m_pDraw = m_pTop[nObj];				//次に更新するオブジェクトの格納
		CScene *m_pDeleteDraw = NULL;				//次に更新するオブジェクトの格納

		while (m_pDraw)
		{
			//次のデータが消えないように保存

			m_pDeleteDraw = m_pDraw->m_pNext;
			//死亡フラグが立っていたら
			if (m_pDraw->m_bDath == true)
			{
				//消す
				delete m_pDraw;
				m_pDraw = NULL;
			}
			//次のオブジェクトにする
			m_pDraw = m_pDeleteDraw;

		}
	}

}
//=============================================================================
//マップに表示したいオブジェの描画
//=============================================================================
void CScene::MapDraw(void)
{
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		if (nObj != OBJTYPE_SKY && nObj != OBJTYPE_PLAYER && nObj != OBJTYPE_EFFECT&&
			nObj != OBJTYPE_MAGIC && nObj!= OBJTYPE_ENEMY && nObj != OBJTYPE_WALL&&
			nObj != OBJTYPE_SHADOW)
		{
			if (m_pTop[nObj] != NULL)
			{
				CScene *m_pDraw = m_pTop[nObj];				//次に更新するオブジェクトの格納
															//m_pUpdateにNULLが入るまで
				while (m_pDraw)
				{
					if (m_pDraw->m_bDath == false)
					{
						m_pDraw->Draw();
					}
					m_pDraw = m_pDraw->m_pNext;

				}
			}

		}
	}
	//死亡フラグ判定
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		CScene *m_pDraw = m_pTop[nObj];				//次に更新するオブジェクトの格納
		CScene *m_pDeleteDraw = NULL;				//次に更新するオブジェクトの格納

		while (m_pDraw)
		{
			//次のデータが消えないように保存

			m_pDeleteDraw = m_pDraw->m_pNext;
			//死亡フラグが立っていたら
			if (m_pDraw->m_bDath == true)
			{
				//消す
				delete m_pDraw;
				m_pDraw = NULL;
			}
			//次のオブジェクトにする
			m_pDraw = m_pDeleteDraw;

		}
	}

}

//全てのインスタンスの破棄処理
void CScene::Delete(void)
{
	for (int nObj = 0; nObj < OBJTYPE_MAX; nObj++)
	{
		CScene *pScene;
		pScene = m_pTop[nObj];
		//最後のオブジェクトの次のNextがNULLでなかったら
		//無限ループ
		if (nObj != OBJTYPE_FADE)
		{
			while (pScene != NULL)
			{
				//トップの次のオブジェクトを格納
				CScene *pDeletePrev = pScene->m_pNext;
				//トップの情報を消す
				delete pScene;
				pScene = NULL;
				//トップの情報を次のオブジェクトにする
				pScene = pDeletePrev;
			}

		}
	}
}
//オブジェクト単体のインスタンス破棄
void CScene::Release(void)
{
	m_bDath = true;

}

void CScene::LifeSub(int nPow)
{
	m_nLife -= nPow; 
}
