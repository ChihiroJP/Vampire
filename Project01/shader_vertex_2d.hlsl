/*==============================================================================

   2D�`��p���_�V�F�[�_�[ [shader_vertex_2d.hlsl]
														 Author : Youhei Sato
														 Date   : 2025/05/15
--------------------------------------------------------------------------------

==============================================================================*/

// �萔�o�b�t�@
float4x4 mtx;

struct VS_IN
{
    float4 posL: POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
VS_OUT main(VS_IN vi)
{

    VS_OUT vo;
    
    vo.posH = mul(vi.posL, mtx);
    vo.color = vi.color;
    vo.uv = vi.uv;
    
    return vo;
}