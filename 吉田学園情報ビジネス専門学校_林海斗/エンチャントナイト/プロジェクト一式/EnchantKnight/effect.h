//=============================================================================
// エフェクトの処理
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "scene.h"
#include "texture.h"

class CEffect : public CScene
{
public:
	typedef enum
	{
		VectorDelete = 0,//ベクトルの長さで消える
		TimeDelete		//時間で消える
	}DeleteType;
	CEffect(OBJTYPE nPriority = OBJTYPE_EFFECT);
	~CEffect();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetCol_E(D3DXCOLOR col);
	void Setpos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);//頂点情報設定
	void SetAng(float Ang) { m_fAng = Ang; }
	void SetCulling(bool bCulling) { m_bCulling = bCulling; }
	void bOnBill(bool bOnBillbord) { m_bOnBillbord = bOnBillbord; }//ビルボードを有効にするかしないか
	void AddColA();
	/*
	 位置の設定,
	 移動量,
	 サイズ,
	 小さくなるスピード
	 Texture
	*/
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 m_move, D3DXVECTOR3 size, D3DXCOLOR col,bool bGravity,
		float fGravity, float fDefSpeed, bool bIsBill,CTexture::Type type = CTexture::Text,float fAng = 0.0f,
		bool bScele=false,bool bScaleX=false,bool bDef = false);
	
private:
	float m_fGravity;//重力
	bool m_bGravity;
	DeleteType m_DeleteType;//エフェクトが消えるタイプ
	float		m_fDeleteTime;
	float		m_fDeleteVec;
	float		m_fDeleteMaxTime;
	float		m_fDeleteMaxVec;
	float		m_fDefSpeedA;
	float		m_fAddA;
	bool m_bDefScale;//小さくなる判定
protected:
	D3DXVECTOR3 m_pos,m_rot;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_initsize;

	D3DXCOLOR m_col;//色
	float m_fScaleSpeed;//サイズを小さくする変数
	int m_nLife;//寿命
	float m_fRand;
	float m_fAng;
	CTexture::Type m_TexType;
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;									//ワールドマトリックス	
	D3DXMATRIX m_mtxView;									//ビューマトリックス
	bool		m_bUninit;//消去判定
	bool		m_bOnBillbord;
	bool		m_bCulling;
	bool		m_bAddColA;
	bool		m_bScale;
	bool		m_bScaleX;

};


#endif // !_EFFECT_H_

