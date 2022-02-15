//----------------------------------
//ポリゴンの処理
//-----------------------------------
#include "model_spawner.h"
#include "input.h"
#include "Renderer.h"
#include "manager.h"
#include "texture.h"
#include "keyboard.h"
#include "model.h"


//=============================================================================
// コンストラクタ
//=============================================================================
CModel_Spawner::CModel_Spawner(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pModel = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel_Spawner::~CModel_Spawner()
{

}
//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CModel_Spawner::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}
//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CModel_Spawner::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CModel_Spawner::Update(void)
{
	m_fRot -= 0.01f;
	if (m_fRot <= -D3DX_PI)
	{
		m_fRot = D3DX_PI;
	}
	//if (m_MoveRot == true)
	//{
	//	m_pModel->SetRot(D3DXVECTOR3(0.0f, m_fRot, 0.0f));
	//}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CModel_Spawner::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

																	 //現在のマテリアルを取得
	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
	D3DXMATRIX mtxParent;//親のマトリックス
	//各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	////向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);
	//位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);
	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}
}
//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CModel_Spawner *CModel_Spawner::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,const int nCreateModel)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//インスタンス生成
	CModel_Spawner *pSpawner = new CModel_Spawner();
	if (pSpawner != NULL)
	{
		pSpawner->Init();
		pSpawner->m_pos = pos;

	}
	//角度をラジアンに変換
	float rot_x = D3DXToRadian(rot.x);
	float rot_y = D3DXToRadian(rot.y);
	float rot_z = D3DXToRadian(rot.z);

	if (pSpawner->m_pModel == NULL)
	{
		//モデルの生成
		pSpawner->m_pModel = CModel::Create({0.0f,0.0f,0.0f}, D3DXVECTOR3(0.0f, 0.0f, 0.0f), nCreateModel, CModel::TYPE_OBJECT);
		//親モデルをNULLに設定
		pSpawner->m_pModel->SetParent(NULL);
	}

	return pSpawner;
}
