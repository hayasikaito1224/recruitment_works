#ifndef _BG_H
#define _BG_H
#include "scene.h"
#include "texture.h"
class CScene2D;
class CBg : public CScene
{
public:
	CBg(OBJTYPE nPriority = OBJTYPE_BG);
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCol(D3DXCOLOR col);
	static CBg *Create(CTexture::Type type = CTexture::Text, OBJTYPE nPriority = OBJTYPE_BG,bool bScrol = true);//インスタンス生成処理
private:
	CTexture::Type				m_Type;
	float						m_fSpeed;
	OBJTYPE						m_nPriority;
	CScene2D					*m_pScene2D;
	bool						m_bScroll;
};


#endif // !_BG_H

