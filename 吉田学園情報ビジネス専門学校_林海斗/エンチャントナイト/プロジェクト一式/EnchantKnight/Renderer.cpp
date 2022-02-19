//--------------------------
//描画処理をするクラス
//----------------------------
#include "Renderer.h"
#include "input.h"
#include "scene.h"
#include "scene2D.h"
#include "camera.h"
#include "manager.h"
#include "game.h"

static bool s_bOnece = false;
CCamera		*CRenderer::m_pCamera[2];

//=============================================================================
// コンストラクタ
//=============================================================================

CRenderer::CRenderer()
{
}
//=============================================================================
// デストラクタ
//=============================================================================

CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;				//アンチエイリアスの設定
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

																// デバイスの生成
																// ディスプレイアダプタを表すためのデバイスを作成
																// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}
	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングを行わない
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	////フォグの処理
	//float Start = 0.0f;    // For linear mode
	//float End = 2000.0f;
	//float Density = 0.03f;   // For exponential modes
	//						 // Enable fog blending.
	//m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//// Set the fog color.
	//m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(255, 0, 0, 0));

	//// Set fog parameters.
	//m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	//m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	//m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	//m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));

																			// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

																			// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 28, 5, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	// カメラの生成
	if (m_pCamera[0] == NULL && m_pCamera[1] == NULL)
	{
		m_pCamera[0] = new CCamera;
		m_pCamera[0]->Init();
		m_pCamera[0]->SetViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		m_pCamera[0]->SetAngleView(PLAYER_CAMERA_ANGLE_OF_VIEW);

		m_pCamera[1] = new CCamera;
		m_pCamera[1]->Init();
		m_pCamera[1]->SetViewport(0, 0, 200, 200, 0);
		m_pCamera[1]->SetAngleView(MAP_ANGLE_OF_VIEW);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{


	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}


	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	CScene::UpdateAll();
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{

		// カメラの更新
		m_pCamera[nCnt]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		D3DVIEWPORT9 CurrentViewPort;
		m_pD3DDevice->GetViewport(&CurrentViewPort);
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_pD3DDevice->SetViewport(&m_pCamera[nCnt]->GetViewport());
			// バックバッファ＆Ｚバッファのクリア
			m_pD3DDevice->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
				D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

			// カメラの設定
			m_pCamera[nCnt]->SetCamera();
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{
				LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ
				D3DXVECTOR3 Pos, Rot;
				Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				D3DXMATRIX world_matrix, trans_matrix, rot_matrix;
				D3DXMatrixIdentity(&world_matrix);

				D3DXMatrixRotationYawPitchRoll(&rot_matrix, Rot.x, Rot.y, Rot.z);

				//D3DXMatrixMultiply(&world_matrix, &world_matrix, &rot_matrix);

				D3DXMatrixTranslation(&trans_matrix, Pos.x, Pos.y, Pos.z);

				//D3DXMatrixMultiply(&world_matrix, &world_matrix, &trans_matrix);


				pDevice->SetTransform(D3DTS_WORLD, &world_matrix);
				pGame->Draw(&world_matrix);
			}

			if (nCnt == 0)
			{
				//フォグの処理
				float Start = 0.0f;    // For linear mode
				float End = 3000.0f;
										 // Enable fog blending.
				m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

				// Set the fog color.
				m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(10, 10, 10, 15));

				// Set fog parameters.
				m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
				m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
				m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));

				//オブジェクトの描画
				CScene::DrawAll();

			}
			if (nCnt == 1)
			{
				m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

				CScene::MapDraw();
			}
#ifdef _DEBUG
			// FPS表示
			DrawFPS();
#endif
		}
		m_pD3DDevice->SetViewport(&CurrentViewPort);

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];
	int nCountFps = GetFps();
	wsprintf(str, "FPS:%d\n", nCountFps);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif
//----------------------------------------------------------------------------
//デバイスの取得処理
//----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}