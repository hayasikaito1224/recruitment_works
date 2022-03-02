//=========================================
//魔法陣の処理
//=========================================
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
	void SizeAdd();//サイズが大きくなる処理
	void SizeDef();//サイズが小さくなる処理
	void ColSizeAdd();//サイズが大きくするだけの処理
	void SetSize();//サイズの設定
	void SetCol(D3DXCOLOR col) { m_col = col; }//色の設定
	void SetbSizeAdd(bool bSizeAdd, float fSizeSpeed = 1.5f)//サイズ変更設定の処理
	{ m_bSizeAdd = bSizeAdd;
	m_fScaleSpeed = fSizeSpeed; }
	void SetbSizeDef(bool bSizeDef) { m_bSizeDef = bSizeDef; }//サイズ変更設定の処理
	void SetbEraseStart(bool bStart, float fSizeSpeed = 1.5f)//魔法陣消去の処理
	{ m_bEraseStart = bStart;
	m_fScaleSpeed = fSizeSpeed;
	}
	/*
	位置の設定,
	サイズ,
	消えるまでの時間
	trueにしたら出現したら時間たったら消える処理になる
	Texture
	*/
	static CMagicCircle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fsize, float fMaxTime, bool bRot,
		bool bEraseStart, D3DXCOLOR col, CTexture::Type type = CTexture::Text,const float fSpeed = 1.5f);

private:
	CShadow *pShadow;//影
	CParticle *m_pParticle;//パーティクル
	bool m_bSizeAdd,m_bSizeDef,m_bColDef;//サイズを大きくするかどうかの判定
	float m_fMaxSize;//最大の大きさ
	float m_fRotSpeed;//回転する速度
	float m_fEraseTime;//消えるまでのカウント
	float m_fEraseMaxTime;//消えるまでのカウント
	bool m_bErase, m_bEraseStart, m_bEraseSizeStart;//消すまでの処理を動作するかの判定
	bool m_bUninit;//終了判定
	bool m_bRotX;//縦回転か横回転か
};


#endif // !_MAGICCIRCLE_H_

