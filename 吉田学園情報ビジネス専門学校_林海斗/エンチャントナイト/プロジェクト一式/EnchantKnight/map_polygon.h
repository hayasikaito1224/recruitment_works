//=============================================================================
//
// Map_Polygon処理
// Author : 林海斗
//
//=============================================================================
#ifndef _MAP_POLYGON_H_
#define _MAP_POLYGON_H_

#include "scene3D.h"
class CTexture;

class CMap_Polygon : public CScene3D
{
public:

	CMap_Polygon(OBJTYPE nPriority = OBJTYPE_MAP_UI);		//コンストラクタ
	~CMap_Polygon();									//デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMap_Polygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, CTexture::Type texture);
	void SetPos(const float PosX,const float PosZ);
	void SetRot(const float RotY) { m_rot.y = RotY; }
	void SetbDraw(bool bDraw) {m_bDraw = bDraw;}
	void BindTexture(CTexture::Type type);
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }

	void SetCol(D3DXCOLOR col);						//テクスチャの設定

private:

	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Scale;
	bool m_bDraw;
	CTexture::Type m_Tex;

};

#endif // _MAP_POLYGON_H_
