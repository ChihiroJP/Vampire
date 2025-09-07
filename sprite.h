/*=======================================================================================


Sprite •\Ž¦ [sprite.h]

																	Author : Zishan
																	Date	: 2025/6/12
-----------------------------------------------------------------------------------------
=========================================================================================*/

#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>


void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);

void Sprite_Begin();

// Texture ‘S•\Ž¦
void Sprite_Draw(int texid, float dx, float dy, const DirectX::XMFLOAT4 & color = { 1.0f, 1.0f,1.0f,1.0f});

//Changing Texture size
void Sprite_Draw(int texid, float dx, float dy, float dh, float dw, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f,1.0f,1.0f });

// UV cut
void Sprite_Draw(int texid, float dx, float dy, int pix, int piy, int piw, int pih, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f,1.0f,1.0f }); // pi for pixels

// UV cut ( Can Change)
void Sprite_Draw(int texid, float dx, float dy, float dw, float dh, int pix, int piy, int piw, int pih, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f,1.0f,1.0f });

// UV cut ( Can Change) ‰ñ“]
void Sprite_Draw(int texid, float dx, float dy, float dw, float dh, int pix, int piy, int piw, int pih, float angle, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f,1.0f,1.0f });


#endif // SPRITE_H

