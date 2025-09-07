/*==============================================================================

   �|���S���`�� [polygon.cpp]
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

// ���ӁI�������ŊO������ݒ肳�����́BRelease�s�v�B
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;



// ���_�\����
struct Vertex
{
	XMFLOAT3 position; // ���_���W
	XMFLOAT4 color;
	XMFLOAT2 uv;//uv
};


void Polygon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̃`�F�b�N
	if (!pDevice || !pContext) {
		hal::dout << "Polygon_Initialize() : �^����ꂽ�f�o�C�X���R���e�L�X�g���s���ł�" << std::endl;
		return;
	}

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̕ۑ�
	g_pDevice = pDevice;
	g_pContext = pContext;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
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
	// �V�F�[�_�[��`��p�C�v���C���ɐݒ�
	Shader_Begin();

	// ���_�o�b�t�@�����b�N����
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// ���_�o�b�t�@�ւ̉��z�|�C���^���擾
	Vertex* v = (Vertex*)msr.pData;

	// ���_������������
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	constexpr float x = 50.0f;
	constexpr float y = 50.0f;
	constexpr float w = 500.0f;
	constexpr float h = 500.0f;

	// Coordinate Position 
	/*v[0].position = {SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.5f, 0.0f};
	v[1].position = { SCREEN_WIDTH *0.4f, SCREEN_HEIGHT*0.5f, 0.0f };
	v[2].position = { SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT*0.7f, 0.0f };
	v[3].position = { SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT*0.7f, 0.0f };
	v[4].position = { SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.5f, 0.0f };
	v[5].position = { SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.5f, 0.0f };
	v[6].position = { SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 1.0f, 0.0f };
	v[7].position = { SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 1.0f, 0.0f };*/

	//Coordinate but variable lok pi tone loh ya dr ko pya
	v[0].position = { x, y, 0.0f };
	v[1].position = { x + w,y,0.0f };
	v[2].position ={ x,y + h,0.0f };
	v[3].position = { x + w,y + h,0.0f };

	//RGB color (red,blue,green)
	v[0].color = { 1.0f,0.0f, 0.0f, 1.0f };
	v[1].color = {0.0f, 1.0f, 0.0f, 1.0f };
	v[2].color = { 0.0f, 0.0f, 0.0f,1.0f };
	v[3].color = { 0.0f, 1.0f, 0.0f, 1.0f };

	v[4].color = { 1.0f,0.0f, 0.0f, 1.0f };
	v[5].color = { 0.0f, 1.0f, 0.0f, 1.0f };
	v[6].color = { 0.0f, 0.0f, 0.0f,1.0f };
	v[7].color = { 0.0f, 1.0f, 0.0f, 1.0f };

	//uv
	v[0].uv = { 0.0f, 0.0f };
	v[1].uv = { 1.0f, 0.0f };
	v[2].uv = { 0.0f, 1.0f };
	v[3].uv = { 1.0f, 1.0f };


	// ���_�o�b�t�@�̃��b�N������
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// ���_�V�F�[�_�[�ɕϊ��s���ݒ�
	Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 0.1f));

	// �v���~�e�B�u�g�|���W�ݒ�
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); //STRIPLIST 

	// Texture 
	g_pContext -> PSSetShaderResources(0, 1, &g_pTexture);

	// �|���S���`�施�ߔ��s
	g_pContext->Draw(NUM_VERTEX, 0);
}
