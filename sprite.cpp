/*==============================================================================

   Sprite 描画 [sprite.cpp]
														 Author : Zishan
														 Date   : 2025/06/12
--------------------------------------------------------------------------------

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include"DirectXTex.h"
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"
#include "sprite.h"
#include "texture.h"

#pragma comment(lib, "d3d11.lib")


static constexpr int NUM_VERTEX = 4; // Vertex


static ID3D11Buffer* g_pVertexBuffer = nullptr; // VertexBuffer
static ID3D11ShaderResourceView* g_pTexture = nullptr; // Texture

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;



// 頂点構造体
struct Vertex
{
	XMFLOAT3 position; // 頂点座標
	XMFLOAT4 color;
	XMFLOAT2 uv;//uv
};


void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// デバイスとデバイスコンテキストのチェック
	if (!pDevice || !pContext) {
		hal::dout << "Sprite_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);

}

void Sprite_Finalize(void)
{
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pVertexBuffer);
}

void Sprite_Begin()
{
	// 頂点シェーダーに変換行列を設定
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));
}



void Sprite_Draw(int texid, float dx, float dy, const DirectX::XMFLOAT4& color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	// Coordinate Position 
	/*v[0].position = {SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.5f, 0.0f};
	v[1].position = { SCREEN_WIDTH *0.4f, SCREEN_HEIGHT*0.5f, 0.0f };
	v[2].position = { SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT*0.7f, 0.0f };
	v[3].position = { SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT*0.7f, 0.0f };
	v[4].position = { SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.5f, 0.0f };
	v[5].position = { SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.5f, 0.0f };
	v[6].position = { SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 1.0f, 0.0f };
	v[7].position = { SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 1.0f, 0.0f };*/

	unsigned int dw = Texture_Width(texid);
	unsigned int dh = Texture_Height(texid);

	//Coordinate but variable lok pi tone loh ya dr ko pya
	v[0].position = { dx, dy, 0.0f };
	v[1].position = { dx + dw,dy,0.0f };
	v[2].position = { dx,dy + dh,0.0f };
	v[3].position = { dx + dw,dy + dh,0.0f };

	//RGB color (red,blue,green)
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	/*v[4].color = {1.0f,0.0f, 0.0f, 1.0f};
	v[5].color = { 0.0f, 1.0f, 0.0f, 1.0f };
	v[6].color = { 0.0f, 0.0f, 0.0f,1.0f };
	v[7].color = { 0.0f, 1.0f, 0.0f, 1.0f };*/

	// Kokosozai pone htl ka ta khu si ko pw ag lok ni
	/* // Kokosozai size = 512 * 512 pixels
		Each sprite = 32*32 pixels

	float u0 = 32.0f * 3.0f / 512.0f; // Sprite size / column * TexSize
	float v0 = 32.0f * 2.0f / 512.0f;  // Sprite size / row * TexSize
	float u1 = 32.0f * 4.0f / 512.0f; // (Sprite size / column + 1) * TexSize
	float v1 = 32.0f * 3.0f / 512.0f; // ( Sprite size / row +1) * TexSize

	v[0].uv = { u0, v0 }; // top left
	v[1].uv = { u1,v0 }; // top right
	v[2].uv = { u0, v1 }; // bottom left a
	v[3].uv = { u1, v1 }; // bottom right
	*/

	//uv
	v[0].uv = { 0.0f, 0.0f }; // top left
	v[1].uv = { 1.0f,0.0f }; // top right
	v[2].uv = { 0.0f, 1.0f }; // bottom left a
	v[3].uv = { 1.0f, 1.0f }; // bottom right


	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// World 変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //STRIPLIST 

	// Texture の設定
	Texture_SetTexture(texid);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float dx, float dy, float dh, float dw, const DirectX::XMFLOAT4& color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	//Coordinate but variable lok pi tone loh ya dr ko pya
	v[0].position = { dx, dy, 0.0f };
	v[1].position = { dx + dw,dy,0.0f };
	v[2].position = { dx,dy + dh,0.0f };
	v[3].position = { dx + dw,dy + dh,0.0f };

	//RGB color (red,blue,green)
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	//uv
	v[0].uv = { 0.0f, 0.0f }; // top left
	v[1].uv = { 1.0f,0.0f }; // top right
	v[2].uv = { 0.0f, 1.0f }; // bottom left a
	v[3].uv = { 1.0f, 1.0f }; // bottom right


	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// World 変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //STRIPLIST 

	// Texture の設定
	Texture_SetTexture(texid);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}
void Sprite_Draw(int texid, float dx, float dy, int pix, int piy, int piw, int pih, const DirectX::XMFLOAT4& color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { dx      , dy      , 0.0f };
	v[1].position = { dx + piw, dy      ,0.0f };
	v[2].position = { dx      , dy + pih,0.0f };
	v[3].position = { dx + piw, dy + pih,0.0f };

	//RGB color (red,blue,green)
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	float tw = Texture_Width(texid);
	float th = Texture_Height(texid);

	float u0 = pix / tw;
	float v0 = piy / th;
	float u1 = (pix + piw) / tw;
	float v1 = (piy + pih) / th;

	/*v[0].uv = {u0, v0}; // top left
	v[1].uv = { u1,v0 }; // top right
	v[2].uv = { u0, v1 }; // bottom left a
	v[3].uv = { u1, v1 }; // bottom right*/


	//uv
	v[0].uv = { u0, v0 }; // top left
	v[1].uv = { u1, v0 }; // top right
	v[2].uv = { u0, v1 }; // bottom left a
	v[3].uv = { u1, v1 }; // bottom right


	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// World 変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //STRIPLIST 

	// Texture の設定
	Texture_SetTexture(texid);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}



void Sprite_Draw(int texid, float dx, float dy, float dw, float dh, int pix, int piy, int piw, int pih, const DirectX::XMFLOAT4& color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { dx,      dy, 0.0f };
	v[1].position = { dx + dw, dy,0.0f };
	v[2].position = { dx,      dy + dh,0.0f };
	v[3].position = { dx + dw, dy + dh,0.0f };

	//RGB color (red,blue,green)
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	float tw = Texture_Width(texid);
	float th = Texture_Height(texid);

	float u0 = pix / tw;
	float v0 = piy / th;
	float u1 = (pix + piw) / tw;
	float v1 = (piy + pih) / th;

	/*v[0].uv = {u0, v0}; // top left
	v[1].uv = { u1,v0 }; // top right
	v[2].uv = { u0, v1 }; // bottom left a
	v[3].uv = { u1, v1 }; // bottom right*/


	//uv
	v[0].uv = { u0,v0 }; // top left
	v[1].uv = { u1,v0 }; // top right
	v[2].uv = { u0,v1 }; // bottom left a
	v[3].uv = { u1,v1 }; // bottom right

	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// World 変換行列を設定
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //STRIPLIST 

	// Texture の設定
	Texture_SetTexture(texid);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(int texid, float dx, float dy, float dw, float dh, int pix, int piy, int piw, int pih, float angle, const DirectX::XMFLOAT4& color)
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { -0.5f,-0.5f, 0.0f };
	v[1].position = {+0.5f, -0.5f,0.0f };
	v[2].position = { -0.5f, +0.5f,0.0f };
	v[3].position = { +0.5f, +0.5f,0.0f };

	//RGB color (red,blue,green)
	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	float tw = Texture_Width(texid);
	float th = Texture_Height(texid);

	float u0 = pix / tw;
	float v0 = piy / th;
	float u1 = (pix + piw) / tw;
	float v1 = (piy + pih) / th;

	hal::dout << u0 << " " << u1 << " " <<  v0 << " " << v1 << std::endl;

	/*v[0].uv = {u0, v0}; // top left
	v[1].uv = { u1,v0 }; // top right
	v[2].uv = { u0, v1 }; // bottom left a
	v[3].uv = { u1, v1 }; // bottom right*/


	//uv
	v[0].uv = { u0,v0 }; // top left
	v[1].uv = { u1,v0 }; // top right
	v[2].uv = { u0, v1 }; // bottom left a
	v[3].uv = { u1,v1 }; // bottom right


	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// World 変換行列を設定
	//Shader_SetWorldMatrix(XMMatrixIdentity());

	// 回転行列を shader に設定
	XMMATRIX scale = XMMatrixScaling(dw, dh, 1.0f);
	XMMATRIX rotation = XMMatrixRotationZ(angle);
	XMMATRIX translation = XMMatrixTranslation(dx, dy, 0.0f);

	Shader_SetWorldMatrix(scale * rotation * translation);

	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //STRIPLIST 

	// Texture の設定
	Texture_SetTexture(texid);

	// ポリゴン描画命令発行
	g_pContext->Draw(NUM_VERTEX, 0);
}

