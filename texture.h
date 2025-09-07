/*==============================================================================

   Texture ä«óù[texture.h]
														 Author : Zishan
														 Date   : 2025/06/13
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>


void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Texture_Finialize(void);

// Texture Load

// Returning : ä«óùî‘çÜ. ì«Ç›çûÇﬂÇ»Ç©Ç¡ÇΩèÍçáÅ[ÇP.
int Texture_Load(const wchar_t* p_FileName);

void Texture_A11Release();

void Texture_SetTexture(int texid);

unsigned int Texture_Width(int texid);
unsigned int Texture_Height(int texid);


#endif // Texture.h
#pragma once
