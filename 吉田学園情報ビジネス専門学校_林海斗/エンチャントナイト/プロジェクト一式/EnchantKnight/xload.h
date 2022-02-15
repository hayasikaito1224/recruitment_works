//----------------------------------
//モデルの処理
//作成者：林　海斗
//----------------------------------
#ifndef _XLOAD_H
#define _XLOAD_H
#include "main.h"
#include "scene.h"
#include "texture.h"
#define MAX_XLOAD_TEX (10)
#define MAX_XLOAD_VTX (8)						//モデルの頂点数

//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CXload
{
public:
	typedef struct
	{
		D3DXVECTOR3 vtx, rot;
		D3DXMATRIX mtxWorld;							//ワールドマトリックス
	}ModelVtx;//モデルの各頂点の保存用構造体
	typedef struct
	{
		LPD3DXMESH				m_pMesh;						//メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER			m_pBuffMat;						//マテリアル（材質情報）へのポインタ
		DWORD					m_nNumMat;						//マテリアルの数
		D3DXVECTOR3				m_vtxMin, m_vtxMax;				//モデルの最小値、最大値
		std::vector<LPDIRECT3DTEXTURE9>		m_pTexture;		//テクスチャ
		int						m_nNumVtx;						//頂点の数
		DWORD					m_sizeFVF;						//頂点フォーマットのサイズ
		BYTE					*m_pVtxBuff;					//頂点バッファのポインタ
		ModelVtx				m_vtx[MAX_XLOAD_VTX];			//各頂点の情報格納構造体

	}ModelData;
	CXload();
	~CXload();
	void Init(void);
	void Uninit(void);
	static void Load(void);
	static ModelData GetPlayerModelData(int nType) { return m_PlayerModelData[nType]; }
	static ModelData GetEnemy01ModelData(int nType) { return m_Enemy01ModelData[nType]; }

	static ModelData GetObjectData(int nType){ return m_ObjectModelData[nType]; }
private:
	static vector<ModelData>		m_PlayerModelData;//モデル情報の構造体
	static vector<ModelData>		m_Enemy01ModelData;//モデル情報の構造体

	static vector<ModelData>		m_ObjectModelData;//モデル情報の構造体

};


#endif // !_XLOAD_H
