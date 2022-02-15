#ifndef _FIELD_H_
#define _FIELD_H_
#include "scene3D.h"
#include "texture.h"
#define MAX_FIELD_VTX (4)
class CField : public CScene3D
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//ワールドマトリックス
	}FieldVtx;//モデルの各頂点の保存用構造体
	typedef struct
	{
		D3DXVECTOR3 m_size,m_InitSize;//メッシュのサイズ
		int m_nMax_Vtx_X;		//横ブロックの数
		int m_nMax_Vtx_Z;		//縦ブロックの数

	}FieldData;//モデルの各頂点の保存用構造体

	CField(OBJTYPE nPriority = OBJTYPE_FIELD);
	~CField();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot,int nNum_X,int nNum_Z, CTexture::Type Tex);
	FieldVtx GetVtx(int nCnt) { return m_vtx[nCnt]; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetSize(D3DXVECTOR3 size);//内部データだけ反映
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }


	void SetMoveTex(float fSpeedX, float fSpeedY); 
	void SetCol(float fAlpha);
	FieldData GetFieldData() { return m_FieldData; }
private:
	FieldData m_FieldData;
	D3DXCOLOR m_col;
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuffWall;	//インデックスバッファへのポインタ
	FieldVtx				m_vtx[MAX_FIELD_VTX];			//各頂点の情報格納構造体
	D3DXVECTOR3				m_vtxMin, m_vtxMax;				//モデルの最小値、最大値
	float m_MoveTexX, m_MoveTexZ;
	int m_nVtx_Num_Max;		//全体頂点の数
	int m_nIdx_Num_Max;		//インデックスの数
	int m_nPoly_Num_Max;		//三角形のポリゴンの数
	float m_fHalfWidth;		//半分のサイズ
	float m_fHalfDepth;		//半分のサイズ
};


#endif // !_FIELD_H_

