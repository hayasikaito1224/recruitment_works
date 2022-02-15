//フェードの処理

#ifndef _FADE_H_
#define _FADE_H_
#include "scene.h"
#include "texture.h"
#include "manager.h"
class CScene2D;
class CBg;

class CFade : public CScene
{
public:
	//フェードの状態
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX,
	} FADE;
	CFade(OBJTYPE nPriority = OBJTYPE_FADE);
	~CFade();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetFade(CManager::MODE modeNext);
	FADE GetFade(void);
	static CFade *Create(CTexture::Type type,CManager::MODE modeNext);//インスタンス生成処理
private:
	CTexture::Type				m_Type;
	static CBg							*m_pBg;
	static FADE					m_fade;			//フェードの状態
	static CManager::MODE		m_modeNext;		//次の画面（モード）
	static float				m_colorAlpha;	//フェードの色

};
#endif // !_FADE_H_

