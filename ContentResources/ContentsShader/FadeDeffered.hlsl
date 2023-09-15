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
    float4 TEXCOORD : TEXCOORD;
    float4 VIEWPOSITION : POSITION;
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


cbuffer FadeInfo : register(b3)
{
    float Delta;
    float3 FadeColor;
};

// ������� ���ؼ� ���� �ݻ縦 �����ϰ� ����
// �� ���� ����ϴ� ������ ���� �������� ������ �ߴ�.
// ������ ������ ���� �ǹ̰� ����.
// ���̶�� �÷��� ���س��� ���� �ȴ�.

// �װ� ���ؽ� ���̴������ϸ� �װ� �������� �ϸ� �÷����̵�
// �װ� ���ؽ� ���̴������ϸ� �װ� ��ν��̵�
// �װ� �ȼ� ���̴������ϸ� �װ� �����̵�

// �׷���ī�忡�� �̷����°�.
Output FadeDeferred_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    float4 InputNormal = _Input.NORMAL;
    InputNormal.w = 0.0f;
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(InputNormal, WorldView);
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D FilterTexture : register(t1);

SamplerState ENGINEBASE : register(s0);

struct DeferredOutPut
{
    // float4 CamForwardTarget : SV_Target0;
    float4 DifTarget : SV_Target1;
    float4 PosTarget : SV_Target2;
    float4 NorTarget : SV_Target3;
};

DeferredOutPut FadeDeferred_PS(Output _Input)
{  
    DeferredOutPut OutPut;
    
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    Color *= MulColor;
    Color += AddColor;
    
    float4 FilterColor = FilterTexture.Sample(ENGINEBASE,_Input.TEXCOORD.xy);
    
    if (FilterColor.r <= Delta)
    {
        clip(-1);
    }
    
    //float UVsize = 1.0f / 512.0f;
    //
    //float2 BaseUV = _Input.TEXCOORD.xy;
    //float2 StartUV = float2(BaseUV.x - 2.0f * UVsize, BaseUV.y - 2.0f * UVsize);
    //float2 CurUV = StartUV;
    //
    //for (int i = 0; i < 5; i++)
    //{
    //    for (int j = 0; j < 5; j++)
    //    {
    //        float4 CurFliterColor = FilterTexture.Sample(ENGINEBASE, CurUV);
    //        
    //        if (CurFliterColor.r <= Delta)
    //        {
    //            Color.rgb = float3(0.0f, 1.0f, 1.0f);
    //            break;
    //        }
    //
    //        CurUV += float2(UVsize, 0);
    //    }
    //
    //    CurUV = float2(StartUV.x , StartUV.y + (i + 1) * UVsize);
    //}
    
    float2 UVsize = (1.0f / 1600.0f, 1.0f / 900.0f);
    
    float2 BaseUV = _Input.TEXCOORD.xy;
    float2 StartUV = float2(BaseUV.x - 2.0f * UVsize.x, BaseUV.y - 2.0f * UVsize.y);
    float2 CurUV = StartUV;
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            float4 CurFliterColor = FilterTexture.Sample(ENGINEBASE, CurUV);
            
            if (CurFliterColor.r <= Delta)
            {
                Color.rgb = float3(1.0f, 0.0f, 1.0f);
                break;
            }
    
            CurUV += float2(UVsize.x, 0);
        }
    
        CurUV = float2(StartUV.x , StartUV.y + (i + 1) * UVsize.y);
    }
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    OutPut.DifTarget = Color;
    
    OutPut.NorTarget = _Input.NORMAL;
    OutPut.NorTarget.a = 1.0f;
    
    OutPut.PosTarget = _Input.VIEWPOSITION;
    
    return OutPut;
}

