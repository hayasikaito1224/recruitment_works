//----------------------------------
//ポリゴンの処理
//作成者　林海斗
//-----------------------------------
#include "model.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"
#include "xload.h"
#include "swordeffect.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CModel::CModel()
{
	memset(m_vtx, NULL, sizeof(m_vtx));
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);					//モデルの位置（オフセット）
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向き
	m_bDraw = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{
}
//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
void CModel::Init()
{
	CXload *pModelDataX = CManager::GetXload();
	CXload::ModelData ModelData;
	switch (m_nCreateType)
	{
	case TYPE_PLAYER:
		m_pModelDataX = pModelDataX->GetPlayerModelData(m_nModelType);
		break;
	case TYPE_ENEMY:
		m_pModelDataX = pModelDataX->GetEnemy01ModelData(m_nModelType);
		break;
	case TYPE_OBJECT:
		m_pModelDataX = pModelDataX->GetObjectData(m_nModelType);
		break;
	case TYPE_FRAMEENEMY:
		m_pModelDataX = pModelDataX->GetEnemy02ModelData(m_nModelType);
		break;
	case TYPE_CRYSTALENEMY:
		m_pModelDataX = pModelDataX->GetEnemy03ModelData(m_nModelType);
		break;

	}

	for (int nVtx = 0; nVtx < MAX_MODEL_VTX; nVtx++)
	{
		m_vtx[nVtx] = m_pModelDataX.m_vtx[nVtx];
	}

}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CModel::Uninit(void)
{
	memset(&m_pModelDataX, NULL, sizeof(m_pModelDataX));
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CModel::Update(void)
{


}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	//現在のマテリアルを取得
	D3DXMATRIX mtxRotModel, mtxTransModel,mtxScale;//計算用マトリックス
	D3DXMATRIX mtxParent;//親のマトリックス
	D3DMATERIAL9 Matdef;
						 //各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	////サイズ変更
	//D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	////向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);
	pDevice->GetMaterial(&Matdef);

	//書くパーツの親のマトリックスを設定
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		//最新のマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	if (m_bDraw == true)
	{

		//各パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//マテリアルデータ編のポインタを取得
		m_pMat = (D3DXMATERIAL*)m_pModelDataX.m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModelDataX.m_nNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);
			//テクスチャの設定
			pDevice->SetTexture(0, m_pModelDataX.m_pTexture[nCntMat]);
			//モデル（パーツ）の描画
			m_pModelDataX.m_pMesh->DrawSubset(nCntMat);


		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&Matdef);

	}
}
//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CModel::SetDiffuse(float DiffuseA)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
																	 //マテリアルデータ編のポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pModelDataX.m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelDataX.m_nNumMat; nCntMat++)
	{
		m_pMat[nCntMat].MatD3D.Diffuse.a = DiffuseA;
		//マテリアルの設定
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);
	}

}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int& nModelType,const int& nCreateType)
{
	//インスタンス生成
	CModel *pModel = new CModel;
	if (pModel != NULL)
	{
		pModel->m_nModelType = nModelType;
		pModel->m_nCreateType = nCreateType;
		pModel->Init();
	}
	pModel->m_pos = pos;
	pModel->m_rot = rot;
	pModel->m_layerpos = pos;
	pModel->m_layerrot = rot;


	return pModel;
}
