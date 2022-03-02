//----------------------------------
//ギミック壁の処理
//----------------------------------
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"	
#include "gimmick_wall.h"

//==================================
//コンストラクタ
//==================================
CGimmickWall::CGimmickWall(OBJTYPE nPriority) :CScene3D(nPriority)
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
	{
		m_vtx[nCnt].rot = { 0.0f,0.0f,0.0f };

	}
	m_bUninit = false;
}
//==================================
//デストラクタ
//==================================
CGimmickWall::~CGimmickWall()
{

}

//------------------------------------------------------------------------------------------------
//インスタンス生成処理(位置、大きさ、角度、壁のナンバー、貼るテクスチャの種類)
//-------------------------------------------------------------------------------------------------
CGimmickWall *CGimmickWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nNumber, CTexture::Type Tex)
{
	//インスタンス生成
	CGimmickWall *pField = new CGimmickWall(OBJTYPE_GIMMICKWALL);

	pField->m_pos = pos;
	pField->m_rot.y = rot.y;
	pField->m_texType = Tex;
	pField->m_nNumber = nNumber;
	pField->m_size = size;
	if (pField != NULL)
	{
		pField->Init();
	}

	return pField;
}
////=============================================================================
// 壁の当たり判定
//=============================================================================
bool CGimmickWall::TestCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastpos, float fRadius)
{
	bool bHit = false;//当たったかどうかを返す
					  //現在のデータ
	D3DXVECTOR3 NorAsk;//法線
	D3DXVECTOR3 NorNormalization;//法線の正規化
	D3DXVECTOR3 VecNor;//法線ベクトル
	D3DXVECTOR3 VecVtx[MAX_GIMMICK_WALL_VTX];//頂点と頂点の間のベクトル
	D3DXVECTOR3 VecPair;//相手の位置から自分の位置までのベクトル
	D3DXVECTOR3 PairPos;//相手の位置
	D3DXVECTOR3 PairOldPos;//相手の前の位置
	D3DXVECTOR3 VecVtxWhile[MAX_GIMMICK_WALL_VTX];////頂点と頂点の間のベクトル
	D3DXVECTOR3 VecVtxPair[MAX_GIMMICK_WALL_VTX];//相手の位置から自分の頂点位置までのベクトル

										 //前のデータ
	D3DXVECTOR3 OldVecNor;//法線ベクトル
	D3DXVECTOR3 OldVecPair;//相手の位置から自分の位置までのベクトル
	NorNormalization = { 0.0f,0.0f,0.0f };
	NorAsk = { 0.0f,0.0f,0.0f };
	VecNor = { 0.0f,0.0f,0.0f };
	VecPair = { 0.0f,0.0f,0.0f };
	OldVecNor = { 0.0f,0.0f,0.0f };
	OldVecPair = { 0.0f,0.0f,0.0f };
	PairPos = *pPos;
	PairOldPos = *pLastpos;

	//ローカル変数の初期化
	for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
	{
		VecVtx[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxPair[nCnt] = { 0.0f,0.0f,0.0f };
		VecVtxWhile[nCnt] = { 0.0f,0.0f,0.0f };

	}
	//子のposを相手の幅分増やす
	//上
	m_vtx[0].vtx.x -= fRadius;
	m_vtx[0].vtx.z -= fRadius;

	m_vtx[1].vtx.x += fRadius;
	m_vtx[1].vtx.z -= fRadius;

	m_vtx[2].vtx.x -= fRadius;
	m_vtx[2].vtx.z -= fRadius;

	m_vtx[3].vtx.x += fRadius;
	m_vtx[3].vtx.z -= fRadius;

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//↑のやつをマトリックスに反映させる
	for (int nNumVtx = 0; nNumVtx < MAX_GIMMICK_WALL_VTX; nNumVtx++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス
		D3DXMATRIX mtxParent;//親のマトリックス

							 //各頂点のワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_vtx[nNumVtx].mtxWorld);
		////向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_vtx[nNumVtx].rot.y, m_vtx[nNumVtx].rot.x, m_vtx[nNumVtx].rot.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxRotModel);
		//位置を反映
		D3DXMatrixTranslation(&mtxTransModel, m_vtx[nNumVtx].vtx.x, m_vtx[nNumVtx].vtx.y, m_vtx[nNumVtx].vtx.z);
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld, &m_vtx[nNumVtx].mtxWorld, &mtxTransModel);
		mtxParent = m_mtxWorld;
		//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_vtx[nNumVtx].mtxWorld,
			&m_vtx[nNumVtx].mtxWorld,
			&m_mtxWorld);

		//各パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_vtx[nNumVtx].mtxWorld);
	}

	//頂点と頂点の間のベクトルを求める
	VecVtxWhile[0] = { m_vtx[1].mtxWorld._41 - m_vtx[0].mtxWorld._41,
		m_vtx[1].mtxWorld._42 - m_vtx[0].mtxWorld._42,
		m_vtx[1].mtxWorld._43 - m_vtx[0].mtxWorld._43 };

	VecVtxWhile[1] = { m_vtx[3].mtxWorld._41 - m_vtx[1].mtxWorld._41,
		m_vtx[3].mtxWorld._42 - m_vtx[1].mtxWorld._42,
		m_vtx[3].mtxWorld._43 - m_vtx[1].mtxWorld._43 };

	VecVtxWhile[2] = { m_vtx[2].mtxWorld._41 - m_vtx[3].mtxWorld._41,
		m_vtx[2].mtxWorld._42 - m_vtx[3].mtxWorld._42,
		m_vtx[2].mtxWorld._43 - m_vtx[3].mtxWorld._43 };

	VecVtxWhile[3] = { m_vtx[0].mtxWorld._41 - m_vtx[2].mtxWorld._41,
		m_vtx[0].mtxWorld._42 - m_vtx[2].mtxWorld._42,
		m_vtx[0].mtxWorld._43 - m_vtx[2].mtxWorld._43 };

	//相手の位置から自分の頂点位置までのベクトル
	VecVtxPair[0] = { PairPos.x - m_vtx[0].mtxWorld._41 ,
		PairPos.y - m_vtx[0].mtxWorld._42 ,
		PairPos.z - m_vtx[0].mtxWorld._43 };

	VecVtxPair[1] = { PairPos.x - m_vtx[1].mtxWorld._41 ,
		PairPos.y - m_vtx[1].mtxWorld._42 ,
		PairPos.z - m_vtx[1].mtxWorld._43 };

	VecVtxPair[2] = { PairPos.x - m_vtx[3].mtxWorld._41 ,
		PairPos.y - m_vtx[3].mtxWorld._42 ,
		PairPos.z - m_vtx[3].mtxWorld._43 };

	VecVtxPair[3] = { PairPos.x - m_vtx[2].mtxWorld._41 ,
		PairPos.y - m_vtx[2].mtxWorld._42 ,
		PairPos.z - m_vtx[2].mtxWorld._43 };

	float fVecClossX[MAX_GIMMICK_WALL_VTX];//外積
	float fVecClossZ[MAX_GIMMICK_WALL_VTX];//外積

								   //壁の外積を求める
	for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
	{
		fVecClossX[nCnt] = (VecVtxWhile[nCnt].z*VecVtxPair[nCnt].y) -
			(VecVtxPair[nCnt].z * VecVtxWhile[nCnt].y);

		fVecClossZ[nCnt] = (VecVtxWhile[nCnt].x*VecVtxPair[nCnt].y) -
			(VecVtxPair[nCnt].x * VecVtxWhile[nCnt].y);
	}


	//頂点０と頂点１のベクトル
	VecVtx[0] = { m_vtx[1].mtxWorld._41 - m_vtx[0].mtxWorld._41,
		m_vtx[1].mtxWorld._42 - m_vtx[0].mtxWorld._42,
		m_vtx[1].mtxWorld._43 - m_vtx[0].mtxWorld._43 };

	//頂点０と頂点2のベクトル
	VecVtx[1] = { m_vtx[2].mtxWorld._41 - m_vtx[0].mtxWorld._41,
		m_vtx[2].mtxWorld._42 - m_vtx[0].mtxWorld._42,
		m_vtx[2].mtxWorld._43 - m_vtx[0].mtxWorld._43 };

	//自分の法線を求める
	D3DXVec3Cross(&NorAsk, &VecVtx[0], &VecVtx[1]);

	//正規化
	D3DXVec3Normalize(&NorNormalization, &NorAsk);

	//********現在の位置のデータ********
	//相手の位置から自分の位置までのベクトルを求める
	VecPair = { PairPos.x - m_pos.x,0.0f,PairPos.z - m_pos.z };

	//内積を求める

	float fDot;//内積
	fDot = D3DXVec3Dot(&VecPair, &NorNormalization);

	//法線ベクトルを求める
	VecNor = NorNormalization*fDot;

	//*********前の位置のデータ**********
	//相手の位置から自分の位置までのベクトルを求める
	OldVecPair = { PairOldPos.x - m_pos.x,0.0f,PairOldPos.z - m_pos.z };

	//内積を求める
	float fOldDot;//内積
	fOldDot = D3DXVec3Dot(&OldVecPair, &NorNormalization);

	//法線ベクトルを求める
	OldVecNor = NorNormalization*fOldDot;

	//法線ベクトルの長さを求める
	float fLength = sqrtf((VecNor.x * VecNor.x) + (VecNor.z * VecNor.z));
	float fOldLength = sqrtf((OldVecNor.x * OldVecNor.x) + (OldVecNor.z * OldVecNor.z));

	//相手の過去の位置と現在の位置の差分を求める
	D3DXVECTOR3 MoveVec = PairPos - PairOldPos;
	D3DXVECTOR3 NorMoveVec = { MoveVec.x*-1,MoveVec.y*-1,MoveVec.z*-1 };


	//差分と法線の内積を求める
	float fNorMoveDot = D3DXVec3Dot(&NorMoveVec, &NorNormalization);

	//法線ベクトルを求める
	D3DXVECTOR3 NorMoveDotVec = { NorNormalization.x*fNorMoveDot ,
		NorNormalization.y*fNorMoveDot,
		NorNormalization.z*fNorMoveDot };
	if (NorNormalization.x <= -0.9f)
	{
		for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
		{
			fVecClossX[nCnt] *= -1.0f;
		}
	}
	if (NorNormalization.z >= 0.9f)
	{
		for (int nCnt = 0; nCnt < MAX_GIMMICK_WALL_VTX; nCnt++)
		{
			fVecClossZ[nCnt] *= -1.0f;
		}
	}
	//法線ベクトルが相手の半径以下になってる&&前の法線ベクトルが相手の半径以上だったら
	if (fVecClossX[0] < 0.00f && fVecClossX[1] < 0.00f && fVecClossX[2] < 0.00f && fVecClossX[3] < 0.00f ||
		fVecClossZ[0] < 0.00f && fVecClossZ[1] < 0.00f && fVecClossZ[2] < 0.00f && fVecClossZ[3] < 0.00f)
	{
		if (fLength <= fRadius && fOldLength >= -10.0f)
		{
			D3DXVECTOR3 push = NorMoveDotVec + NorMoveVec;
			//*pPos = *pLastpos;
			pPos->x += NorMoveDotVec.x;
			pPos->z += NorMoveDotVec.z;

			bHit = true;
		}

	}
	//上
	m_vtx[0].vtx.x -= -fRadius;
	m_vtx[0].vtx.z -= -fRadius;
	m_vtx[1].vtx.x += -fRadius;
	m_vtx[1].vtx.z -= -fRadius;
	m_vtx[2].vtx.x -= -fRadius;
	m_vtx[2].vtx.z -= -fRadius;
	m_vtx[3].vtx.x += -fRadius;
	m_vtx[3].vtx.z -= -fRadius;

	return bHit;
}
//==================================
//ギミックで閉じていた壁の開放処理
//==================================
void CGimmickWall::GimmickUnlock(int nNumber)
{
	//引数の数値とこのオブジェクトが持っている数値が同じなら
	if (nNumber == m_nNumber)
	{
		//削除
		m_bUninit = true;
	}
}

//==================================
//初期化
//==================================
HRESULT CGimmickWall::Init(void)
{
	CScene3D::Init(CScene3D::TYPE_WALL,m_texType);

	//左奥（上面）
	m_vtx[0].vtx = D3DXVECTOR3(
		-m_size.x,
		m_size.y,
		0.0f);

	//右奥（上面）
	m_vtx[1].vtx = D3DXVECTOR3(
		m_size.x,
		m_size.y,
		0.0f);

	//左前（上面）
	m_vtx[2].vtx = D3DXVECTOR3(
		-m_size.x,
		-m_size.y,
		0.0f);

	//右前（上面）
	m_vtx[3].vtx = D3DXVECTOR3(
		m_size.x,
		-m_size.y,
		0.0f);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGimmickWall::Uninit(void)
{

	CScene3D::Uninit();
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGimmickWall::Update(void)
{
	if (m_bUninit)
	{
		Uninit();
	}
}
//=======================================================================
//描画処理
//=======================================================================
void CGimmickWall::Draw()
{
	CScene3D::Draw();
}

