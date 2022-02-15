#ifndef _MAGICCIRCLE_H_
#define _MAGICCIRCLE_H_
#include "effect.h"
#include "texture.h"
class CParticle;
class CShadow;
class CMagicCircle : public CEffect
{
public:
	CMagicCircle(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CMagicCircle();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SizeAdd();
	void SizeDef();
	void ColSizeAdd();
	void SetSize();
	void SetCol(D3DXCOLOR col) { m_col = col; }
	void SetbSizeAdd(bool bSizeAdd) { m_bSizeAdd = bSizeAdd; }
	void SetbSizeDef(bool bSizeDef) { m_bSizeDef = bSizeDef; }

	/*
	位置の設定,
	サイズ,
	消えるまでの時間
	trueにしたら出現したら時間たったら消える処理になる
	Texture
	*/

	static CMagicCircle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fsize, float fMaxTime, bool bRot, bool bEraseStart, D3DXCOLOR col, CTexture::Type type = CTexture::Text);

private:
	CShadow *pShadow;
	CParticle *m_pParticle;
	bool m_bSizeAdd,m_bSizeDef,m_bColDef;//サイズを大きくするかどうかの判定
	float m_fMaxSize;//最大の大きさ
	float m_fRotSpeed;
	float m_fEraseTime;//消えるまでのカウント
	float m_fEraseMaxTime;//消えるまでのカウント
	bool m_bErase, m_bEraseStart, m_bEraseSizeStart;//消すまでの処理を動作するかの判定
	bool m_bUninit;
	bool m_bEffect;
	bool m_bRotX;//縦回転か横回転か
};


#endif // !_MAGICCIRCLE_H_

