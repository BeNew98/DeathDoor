#include "ContentTransform.fx"
#include "ContentAnimation.fx"
#include "ContentLight.fx"
#include "ContentRenderBaseValue.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
    float4 BLENDWEIGHT : BLENDWEIGHT;
    int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    // w������ ���ְ�
    // viewport ��ı��� �����Ͷ��������� �����ش�.
    float4 POSITION : SV_POSITION;
    float4 VIEWPOSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
};


cbuffer ColorOption : register(b1)
{
    float4 MulColor;
    float4 AddColor;
};

cbuffer UVData : register(b2)
{
    float2 MulUV;
    float2 AddUV;
};

// ������� ���ؼ� ���� �ݻ縦 �����ϰ� ����
// �� ���� ����ϴ� ������ ���� �������� ������ �ߴ�.
// ������ ������ ���� �ǹ̰� ����.
// ���̶�� �÷��� ���س��� ���� �ȴ�.

// �װ� ���ؽ� ���̴������ϸ� �װ� �������� �ϸ� �÷����̵�
// �װ� ���ؽ� ���̴������ϸ� �װ� ��ν��̵�
// �װ� �ȼ� ���̴������ϸ� �װ� �����̵�

// �׷���ī�忡�� �̷����°�.
Output ContentAniMeshForward_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    float4 InputNormal = _Input.NORMAL;
    InputNormal.w = 0.0f;
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
       // SkinningNormal(InputNormal, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
        InputNormal.w = 0.0f;
    }
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    // ������� �ϱ� ���� �������̹Ƿ� �̳༮�� ������� �־�� �Ѵ�.
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(InputNormal, WorldView);
    
    return NewOutPut;
}

struct OutputTarget
{
    float4 CamForwardTarget : SV_Target0;
    float4 BlurTarget : SV_Target6;
};

Texture2D DiffuseTexture : register(t0);
Texture2D MaskTexture : register(t1);
SamplerState ENGINEBASE : register(s0);

OutputTarget ContentAniMeshForward_PS(Output _Input)
{
    OutputTarget PS_OutPut = (OutputTarget) 0.0f;
        
    float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, _Input.NORMAL, AllLight[0]);
    float4 SpacularRatio = CalSpacularLight(_Input.VIEWPOSITION, _Input.NORMAL, AllLight[0]);
    float4 AmbientRatio = CalAmbientLight(AllLight[0]);
    
    float4 MaskColor = MaskTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 MaskResultColor = (float4) 0.0f;
    
    float MaskAlpha = MaskColor.w;
    
    MaskResultColor = 2.0f * MaskColor * (DiffuseRatio + SpacularRatio + AmbientRatio);
    MaskResultColor += float4(0.8f, 0.0f, 0.5f, 1.0f);
    MaskResultColor.a = MaskAlpha;
    
    float4 DiffuseResultColor = (float4) 0.0f;
    float4 DiffuseColor = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    float DiffuseAlpha = DiffuseColor.w;
    DiffuseResultColor = DiffuseColor * (DiffuseRatio + SpacularRatio + AmbientRatio);
    DiffuseResultColor.a = DiffuseAlpha;
    
    //DiffuseResultColor = ceil(DiffuseResultColor * 10.0f) / 10.0f;
    
    PS_OutPut.CamForwardTarget = DiffuseResultColor;
    PS_OutPut.BlurTarget = MaskResultColor;
    
    return PS_OutPut;
}

