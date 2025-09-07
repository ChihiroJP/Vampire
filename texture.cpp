/*==============================================================================

   Texture 管理[texture.cpp]
														 Author : Zishan
														 Date   : 2025/06/13
--------------------------------------------------------------------------------

==============================================================================*/

#include "texture.h"
#include "direct3d.h"
#include <string>
#include "DirectXTex.h"
using namespace DirectX;

static constexpr int  TEXTURE_MAX = 256;

struct Texture
{
	std::wstring filename;
	ID3D11ShaderResourceView* pTexture;
	unsigned int width;
	unsigned int height;
};

static Texture g_Texture[TEXTURE_MAX]{};
static int g_SetTextureIndex = -1;

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (Texture& t : g_Texture) {
		t.pTexture = nullptr;
	}
	g_SetTextureIndex = -1;

	// Device & Context 保存
	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Texture_Finialize(void)
{
	Texture_A11Release();
}

int Texture_Load(const wchar_t* pFileName)
{
	// The File which is already read don't read again
	for (int i = 0; i < TEXTURE_MAX; i++) {
		if (g_Texture[i].filename == pFileName) {
			return i;
		}
	}

	// 空いている管理領域を探す
	for (int i = 0; i < TEXTURE_MAX; i++) {

		if (g_Texture[i].pTexture) continue; // Still Using

		//Load Texture
		TexMetadata metadata;
		ScratchImage image;

		HRESULT hr = LoadFromWICFile(pFileName, WIC_FLAGS_NONE, &metadata, image);


		if (FAILED(hr)) {
			MessageBox(nullptr, "Failed to Load Texture", "Error", MB_OK | MB_ICONERROR);
			return -1;
		}

		g_Texture[i].filename = pFileName;
		g_Texture[i].width = (unsigned int)metadata.width;
		g_Texture[i].height = (unsigned int)metadata.height;

		hr = CreateShaderResourceView(g_pDevice,
			image.GetImages(), image.GetImageCount(), metadata, &g_Texture[i].pTexture);

		if (FAILED(hr)) {
			MessageBox(nullptr, "Failed to create shader resource view", "Error", MB_OK | MB_ICONERROR);
			return -1;
		}
		return i;
	}
}
void Texture_A11Release()
{
	for (Texture& t : g_Texture) {
		t.filename.clear();
		SAFE_RELEASE(t.pTexture);
	}
}

void Texture_SetTexture(int texid)
{
	if (texid < 0) return;

	//if (g_SetTextureIndex == texid) return;

	g_SetTextureIndex = texid;

	// Texture 
	g_pContext->PSSetShaderResources(0, 1, &g_Texture[texid].pTexture);
}

unsigned int Texture_Width(int texid)
{
	if (texid < 0) return 0;

	return g_Texture[texid].width;
}

unsigned int Texture_Height(int texid)
{
	if (texid < 0) return 0;

	return g_Texture[texid].height;
}
