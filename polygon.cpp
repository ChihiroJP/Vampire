/*==============================================================================

   ポリゴン描画 [polygon.cpp]
														 Author : Youhei Sato
														 Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include"DirectXTex.h"
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"

#pragma comment(lib, "d3d11.lib")


static constexpr int NUM_VERTEX = 8; // Vertex


static ID3D11Buffer* g_pVertexBuffer = nullptr; // VertexBuffer
static ID3D11ShaderResourceView* g_pTexture = nullptr; // Texture

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static constexpr int NUM_SEGMENTS = 64;
static int g_NumVertex = NUM_SEGMENTS + 2;
static float g_Radius = 300.0f;
static float g_Cx = 1000.0f;
static float g_Cy = 500.0f;

// 頂点構造体
struct Vertex
{
	XMFLOAT3 position; // 頂点座標
	XMFLOAT4 color;
	XMFLOAT2 texcoord;//uv
};


void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// デバイスとデバイスコンテキストのチェック
	if (!pDevice || !pContext) {
		hal::dout << "Polygon_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;

	// Calculating number points
	g_NumVertex = NUM_SEGMENTS + 2;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * g_NumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);

	//Load Texture
	TexMetadata metadata;
	ScratchImage image;

	LoadFromWICFile(L"Zero two texture.png", WIC_FLAGS_NONE, &metadata, image);
	HRESULT hr = CreateShaderResourceView(g_pDevice,
		image.GetImages(), image.GetImageCount(), metadata, &g_pTexture);

	if (FAILED(hr)) {
		MessageBox(nullptr, "Failed to Load Texture", "Error", MB_OK | MB_ICONERROR);
	}

}

void Polygon_Finalize(void)
{
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pVertexBuffer);
}

void Polygon_Draw(void)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	const float rad = XM_2PI / (g_NumVertex - 2);

	v[0].position = { g_Cx, g_Cy, 0.0f };
	v[0].color = { 1,1,1,1 };
	v[0].texcoord = { 0.5f, 0.5f };

	for (int i = 0; i <=  g_NumVertex - 2; i++) {
		float angle = rad * i;

		v[i + 1].position = { cosf(angle) * g_Radius + g_Cx, sinf(angle) * g_Radius + g_Cy, 0.0f };
		v[i + 1].color ={ 1.0f, 1.0f, 1.0f, 1.0f };
		v[i + 1].texcoord = { 0.5f + 0.5f * cosf(angle), 0.5f + 0.5f * sinf(angle)};
	}

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// 頂点シェーダーに変換行列を設定
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 0.1f));

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);//STRIPLIST 

	// Texture 
	// g_pContext -> PSSetShaderResources(0, 1, &g_pTexture);

	// ポリゴン描画命令発行
	g_pContext->Draw(g_NumVertex, 0);
}
