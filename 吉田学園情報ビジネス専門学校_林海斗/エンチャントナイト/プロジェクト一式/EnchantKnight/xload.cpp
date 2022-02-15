//----------------------------------
//ポリゴンの処理
//作成者　林海斗
//-----------------------------------
#include "xload.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"
std::vector<CXload::ModelData> CXload::m_PlayerModelData;
std::vector<CXload::ModelData> CXload::m_ObjectModelData;
std::vector<CXload::ModelData> CXload::m_Enemy01ModelData;

//=============================================================================
// コンストラクタ
//=============================================================================
CXload::CXload()
{
	m_PlayerModelData.clear();
	m_ObjectModelData.clear();
	m_Enemy01ModelData.clear();
}

//=============================================================================
// デストラクタ
//=============================================================================
CXload::~CXload()
{
}
//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CXload::Init(void)
{
	//モデルデータの読み込み
	Load();
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CXload::Uninit(void)
{
}


//=============================================================================
// モデルを読み込む処理
//=============================================================================
void CXload::Load(void)
{
	char sString[2][255];	// 読み込み用の変数
	int nCnt = 0;
	int nCnt2 = 0;
	int nCnt3 = 0;

	FILE *File = fopen("data/TEXT/AllModel.txt", "r");
	char sFileName[124];
	if (File != NULL)
	{
		while (true)
		{
			fscanf(File, "%s", &sString[0]);


			//プレイヤーモデルのファイルネームを読み込む
			if (strcmp(sString[0], "PLAYERMODEL_FILENAME") == 0)
			{
				LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
				D3DXMATERIAL *pMat;
				ModelData Securement = {};
				m_PlayerModelData.push_back(Securement);

				fscanf(File, "%s", &sString[1]);
				//モデル情報の読み込み
				fscanf(File, "%s", &sFileName);
				D3DXLoadMeshFromX(sFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_PlayerModelData[nCnt].m_pBuffMat,
					NULL,
					&m_PlayerModelData[nCnt].m_nNumMat,
					&m_PlayerModelData[nCnt].m_pMesh);

				//マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL *)m_PlayerModelData[nCnt].m_pBuffMat->GetBufferPointer();
				for (int nCntmat = 0; nCntmat < (int)m_PlayerModelData[nCnt].m_nNumMat; nCntmat++)
				{
					if (pMat[nCntmat].pTextureFilename != NULL)
					{
						LPDIRECT3DTEXTURE9 Securement;
						//テクスチャの読み込み
						D3DXCreateTextureFromFile
						(pDevice,
							pMat[nCntmat].pTextureFilename,
							&Securement);
						m_PlayerModelData[nCnt].m_pTexture.push_back(Securement);
					}
					else
					{
						m_PlayerModelData[nCnt].m_pTexture.push_back(NULL);

					}
				}
				//頂点数を取得
				m_PlayerModelData[nCnt].m_nNumVtx = m_PlayerModelData[nCnt].m_pMesh->GetNumVertices();
				//頂点フォーマットのサイズを取得
				m_PlayerModelData[nCnt].m_sizeFVF = D3DXGetFVFVertexSize(m_PlayerModelData[nCnt].m_pMesh->GetFVF());
				//頂点バッファをアンロック
				m_PlayerModelData[nCnt].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_PlayerModelData[nCnt].m_pVtxBuff);
				//モデルの大きさを測る
				for (int nCnt1 = 0; nCnt1 < m_PlayerModelData[nCnt].m_nNumVtx; nCnt1++)
				{
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_PlayerModelData[nCnt].m_pVtxBuff;//頂点座標の代入
																				  //xの最大値の比較
					if (m_PlayerModelData[nCnt].m_vtxMax.x >= vtx.x)
					{
						m_PlayerModelData[nCnt].m_vtxMax.x = vtx.x;
					}
					//ｘの最小値の比較
					else if (m_PlayerModelData[nCnt].m_vtxMin.x <= vtx.x)
					{
						m_PlayerModelData[nCnt].m_vtxMin.x = vtx.x;
					}
					//yの最大値の比較
					if (m_PlayerModelData[nCnt].m_vtxMax.y >= vtx.y)
					{
						m_PlayerModelData[nCnt].m_vtxMax.y = vtx.y;
					}
					//yの最小値の比較
					else if (m_PlayerModelData[nCnt].m_vtxMin.y <= vtx.y)
					{
						m_PlayerModelData[nCnt].m_vtxMin.y = vtx.y;
					}
					//zの最大値の比較
					if (m_PlayerModelData[nCnt].m_vtxMax.z >= vtx.z)
					{
						m_PlayerModelData[nCnt].m_vtxMax.z = vtx.z;
					}
					//zの最小値の比較
					else if (m_PlayerModelData[nCnt].m_vtxMin.z <= vtx.z)
					{
						m_PlayerModelData[nCnt].m_vtxMin.z = vtx.z;
					}
					m_PlayerModelData[nCnt].m_pVtxBuff += m_PlayerModelData[nCnt].m_sizeFVF;//頂点フォーマットのサイズ分ポインタを進める
				}
				//頂点バッファをアンロック
				m_PlayerModelData[nCnt].m_pMesh->UnlockVertexBuffer();
				//8つの頂点情報の保存
				//左奥（上面）
				m_PlayerModelData[nCnt].m_vtx[0].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//右奥（上面）
				m_PlayerModelData[nCnt].m_vtx[1].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//左前（上面）
				m_PlayerModelData[nCnt].m_vtx[2].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				//右前（上面）
				m_PlayerModelData[nCnt].m_vtx[3].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					m_PlayerModelData[nCnt].m_vtxMin.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				//左奥（下面）
				m_PlayerModelData[nCnt].m_vtx[4].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//右奥（下面）
				m_PlayerModelData[nCnt].m_vtx[5].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					m_PlayerModelData[nCnt].m_vtxMin.z);
				//左前（下面）
				m_PlayerModelData[nCnt].m_vtx[6].vtx = D3DXVECTOR3(
					-m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				//右前（下面）
				m_PlayerModelData[nCnt].m_vtx[7].vtx = D3DXVECTOR3(
					m_PlayerModelData[nCnt].m_vtxMin.x,
					-m_PlayerModelData[nCnt].m_vtxMax.y,
					-m_PlayerModelData[nCnt].m_vtxMin.z);
				nCnt++;
			}
			//敵モデルを読み込む
			if (strcmp(sString[0], "ENEMY01MODEL_FILENAME") == 0)
			{
				LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
				D3DXMATERIAL *pMat;
				ModelData Securement = {};
				m_Enemy01ModelData.push_back(Securement);

				fscanf(File, "%s", &sString[1]);

				//モデル情報の読み込み
				fscanf(File, "%s", &sFileName);
				D3DXLoadMeshFromX(sFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_Enemy01ModelData[nCnt3].m_pBuffMat,
					NULL,
					&m_Enemy01ModelData[nCnt3].m_nNumMat,
					&m_Enemy01ModelData[nCnt3].m_pMesh);

				//マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL *)m_Enemy01ModelData[nCnt3].m_pBuffMat->GetBufferPointer();
				for (int nCntmat = 0; nCntmat < (int)m_Enemy01ModelData[nCnt3].m_nNumMat; nCntmat++)
				{
					if (pMat[nCntmat].pTextureFilename != NULL)
					{
						LPDIRECT3DTEXTURE9 Securement;

						//テクスチャの読み込み
						D3DXCreateTextureFromFile
						(pDevice,
							pMat[nCntmat].pTextureFilename,
							&Securement);
						m_Enemy01ModelData[nCnt3].m_pTexture.push_back(Securement);

					}
					else
					{
						m_Enemy01ModelData[nCnt3].m_pTexture.push_back(NULL);
					}

				}
				//頂点数を取得
				m_Enemy01ModelData[nCnt3].m_nNumVtx = m_Enemy01ModelData[nCnt3].m_pMesh->GetNumVertices();
				//頂点フォーマットのサイズを取得
				m_Enemy01ModelData[nCnt3].m_sizeFVF = D3DXGetFVFVertexSize(m_Enemy01ModelData[nCnt3].m_pMesh->GetFVF());
				//頂点バッファをアンロック
				m_Enemy01ModelData[nCnt3].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_Enemy01ModelData[nCnt3].m_pVtxBuff);
				//モデルの大きさを測る
				for (int nCnt = 0; nCnt < m_Enemy01ModelData[nCnt3].m_nNumVtx; nCnt++)
				{
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_Enemy01ModelData[nCnt3].m_pVtxBuff;//頂点座標の代入
																						 //xの最大値の比較
					if (m_Enemy01ModelData[nCnt3].m_vtxMax.x >= vtx.x)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMax.x = vtx.x;
					}
					//ｘの最小値の比較
					else if (m_Enemy01ModelData[nCnt3].m_vtxMin.x <= vtx.x)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMin.x = vtx.x;
					}
					//yの最大値の比較
					if (m_Enemy01ModelData[nCnt3].m_vtxMax.y >= vtx.y)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMax.y = vtx.y;
					}
					//yの最小値の比較
					else if (m_Enemy01ModelData[nCnt3].m_vtxMin.y <= vtx.y)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMin.y = vtx.y;
					}
					//zの最大値の比較
					if (m_Enemy01ModelData[nCnt3].m_vtxMax.z >= vtx.z)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMax.z = vtx.z;
					}
					//zの最小値の比較
					else if (m_Enemy01ModelData[nCnt3].m_vtxMin.z <= vtx.z)
					{
						m_Enemy01ModelData[nCnt3].m_vtxMin.z = vtx.z;
					}
					m_Enemy01ModelData[nCnt3].m_pVtxBuff += m_Enemy01ModelData[nCnt3].m_sizeFVF;//頂点フォーマットのサイズ分ポインタを進める
				}
				//頂点バッファをアンロック
				m_Enemy01ModelData[nCnt3].m_pMesh->UnlockVertexBuffer();
				//8つの頂点情報の保存
				//左奥（上面）
				m_Enemy01ModelData[nCnt3].m_vtx[0].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//右奥（上面）
				m_Enemy01ModelData[nCnt3].m_vtx[1].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//左前（上面）
				m_Enemy01ModelData[nCnt3].m_vtx[2].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//右前（上面）
				m_Enemy01ModelData[nCnt3].m_vtx[3].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					m_Enemy01ModelData[nCnt3].m_vtxMin.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//左奥（下面）
				m_Enemy01ModelData[nCnt3].m_vtx[4].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//右奥（下面）
				m_Enemy01ModelData[nCnt3].m_vtx[5].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//左前（下面）
				m_Enemy01ModelData[nCnt3].m_vtx[6].vtx = D3DXVECTOR3(
					-m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				//右前（下面）
				m_Enemy01ModelData[nCnt3].m_vtx[7].vtx = D3DXVECTOR3(
					m_Enemy01ModelData[nCnt3].m_vtxMin.x,
					-m_Enemy01ModelData[nCnt3].m_vtxMax.y,
					-m_Enemy01ModelData[nCnt3].m_vtxMin.z);
				nCnt3++;
			}

			//ゲームに配置するモデルを読み込む
			if (strcmp(sString[0], "OBJECTMODEL_FILENAME") == 0)
			{
				LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
				D3DXMATERIAL *pMat;
				ModelData Securement = {};
				m_ObjectModelData.push_back(Securement);

				fscanf(File, "%s", &sString[1]);

				//モデル情報の読み込み
				fscanf(File, "%s", &sFileName);
				D3DXLoadMeshFromX(sFileName,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&m_ObjectModelData[nCnt2].m_pBuffMat,
					NULL,
					&m_ObjectModelData[nCnt2].m_nNumMat,
					&m_ObjectModelData[nCnt2].m_pMesh);

				//マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL *)m_ObjectModelData[nCnt2].m_pBuffMat->GetBufferPointer();
				for (int nCntmat = 0; nCntmat < (int)m_ObjectModelData[nCnt2].m_nNumMat; nCntmat++)
				{
					if (pMat[nCntmat].pTextureFilename != NULL)
					{
						LPDIRECT3DTEXTURE9 Securement;

						//テクスチャの読み込み
						D3DXCreateTextureFromFile
						(pDevice,
							pMat[nCntmat].pTextureFilename,
							&Securement);
						m_ObjectModelData[nCnt2].m_pTexture.push_back(Securement);

					}
					else
					{
						m_ObjectModelData[nCnt2].m_pTexture.push_back(NULL);
					}

				}
				//頂点数を取得
				m_ObjectModelData[nCnt2].m_nNumVtx = m_ObjectModelData[nCnt2].m_pMesh->GetNumVertices();
				//頂点フォーマットのサイズを取得
				m_ObjectModelData[nCnt2].m_sizeFVF = D3DXGetFVFVertexSize(m_ObjectModelData[nCnt2].m_pMesh->GetFVF());
				//頂点バッファをアンロック
				m_ObjectModelData[nCnt2].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_ObjectModelData[nCnt2].m_pVtxBuff);
				//モデルの大きさを測る
				for (int nCnt = 0; nCnt < m_ObjectModelData[nCnt2].m_nNumVtx; nCnt++)
				{
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_ObjectModelData[nCnt2].m_pVtxBuff;//頂点座標の代入
																				  //xの最大値の比較
					if (m_ObjectModelData[nCnt2].m_vtxMax.x >= vtx.x)
					{
						m_ObjectModelData[nCnt2].m_vtxMax.x = vtx.x;
					}
					//ｘの最小値の比較
					else if (m_ObjectModelData[nCnt2].m_vtxMin.x <= vtx.x)
					{
						m_ObjectModelData[nCnt2].m_vtxMin.x = vtx.x;
					}
					//yの最大値の比較
					if (m_ObjectModelData[nCnt2].m_vtxMax.y >= vtx.y)
					{
						m_ObjectModelData[nCnt2].m_vtxMax.y = vtx.y;
					}
					//yの最小値の比較
					else if (m_ObjectModelData[nCnt2].m_vtxMin.y <= vtx.y)
					{
						m_ObjectModelData[nCnt2].m_vtxMin.y = vtx.y;
					}
					//zの最大値の比較
					if (m_ObjectModelData[nCnt2].m_vtxMax.z >= vtx.z)
					{
						m_ObjectModelData[nCnt2].m_vtxMax.z = vtx.z;
					}
					//zの最小値の比較
					else if (m_ObjectModelData[nCnt2].m_vtxMin.z <= vtx.z)
					{
						m_ObjectModelData[nCnt2].m_vtxMin.z = vtx.z;
					}
					m_ObjectModelData[nCnt2].m_pVtxBuff += m_ObjectModelData[nCnt2].m_sizeFVF;//頂点フォーマットのサイズ分ポインタを進める
				}
				//頂点バッファをアンロック
				m_ObjectModelData[nCnt2].m_pMesh->UnlockVertexBuffer();
				//8つの頂点情報の保存
				//左奥（上面）
				m_ObjectModelData[nCnt2].m_vtx[0].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//右奥（上面）
				m_ObjectModelData[nCnt2].m_vtx[1].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//左前（上面）
				m_ObjectModelData[nCnt2].m_vtx[2].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				//右前（上面）
				m_ObjectModelData[nCnt2].m_vtx[3].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					m_ObjectModelData[nCnt2].m_vtxMin.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				//左奥（下面）
				m_ObjectModelData[nCnt2].m_vtx[4].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//右奥（下面）
				m_ObjectModelData[nCnt2].m_vtx[5].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					m_ObjectModelData[nCnt2].m_vtxMin.z);
				//左前（下面）
				m_ObjectModelData[nCnt2].m_vtx[6].vtx = D3DXVECTOR3(
					-m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				//右前（下面）
				m_ObjectModelData[nCnt2].m_vtx[7].vtx = D3DXVECTOR3(
					m_ObjectModelData[nCnt2].m_vtxMin.x,
					-m_ObjectModelData[nCnt2].m_vtxMax.y,
					-m_ObjectModelData[nCnt2].m_vtxMin.z);
				nCnt2++;
			}

			if (strcmp(sString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}

	}
	fclose(File);
}