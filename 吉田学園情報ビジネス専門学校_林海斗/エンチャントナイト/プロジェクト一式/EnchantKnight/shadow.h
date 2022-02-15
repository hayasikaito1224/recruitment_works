//----------------------------------
//ポリゴンの処理
//----------------------------------
#ifndef _SHADOW_H
#define _SHADOW_H
#include "main.h"
#include "texture.h"
#include "Scene3D.h"
#define SHADOWGRAVITY (50.0)
//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CShadow : public CScene3D
{
public:
	CShadow(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CShadow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShadow *Create(D3DXVECTOR3 pos, float fSize, CTexture::Type texture = CTexture::Text);
	void SetPos(const float fposX, const float fposZ, D3DXVECTOR3 scale);
	void SetPosY(float fposY) { m_pos.y = fposY; }
	void SetGraviSp(float data) { m_fGravitySpeed = data; }
	void SetGravi(float data) { m_fGravity = data; }
	void SetCol(D3DXCOLOR col);						//テクスチャの設定
	void BindTexture(CTexture::Type type);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
private:
	D3DXVECTOR3 m_LastPos;
	CTexture::Type m_Tex;
	float	m_fGravity;//重力
	float	m_fGravitySpeed;//重力

};


#endif // !_SHADOW_H
