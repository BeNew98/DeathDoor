#include "ContentTransform.fx"
#include "ContentAnimation.fx"
#include "ContentLight.fx"
#include "ContentRenderBaseValue.fx"
#include "ContentFunction.fx"

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
    float4 POSITION : SV_POSITION;
    float4 VIEWPOSITION : POSITION;
    float4 WORLDPOSITION : POSITION1;
    float4 WVPPOSITION : POSITION5;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
};


Output ContentAniMeshDeferred_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    float4 InputNormal = _Input.NORMAL;
    InputNormal.w = 0.0f;
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
        InputNormal.w = 0.0f;
    }
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    NewOutPut.WVPPOSITION = NewOutPut.POSITION;
    
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    NewOutPut.NORMAL = mul(InputNormal, WorldView);
    
    NewOutPut.WORLDPOSITION = mul(InputPos, WorldMatrix);
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D MaskTexture : register(t1);
Texture2D CrackTexture : register(t2);

SamplerState ENGINEBASE : register(s0);

struct DeferredOutPut
{
    float4 DifTarget : SV_Target1;
    float4 PosTarget : SV_Target2;
    float4 NorTarget : SV_Target3;
    float4 BlurTarget : SV_Target7;
};

cbuffer BlurColor : register(b1)
{
    float4 BlurColor;
}


DeferredOutPut ContentAniMeshDeferred_PS(Output _Input)
{
    if (_Input.WORLDPOSITION.y < -50.0F)
    {
        clip(-1);
    }
    
    DeferredOutPut NewOutPut = (DeferredOutPut) 0;
    
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    float4 MaskColor = (float4) 0.0f;
    
    //Crack
    if (UV_MaskingValue > 0.0f && _Input.TEXCOORD.x <= UV_MaskingValue && _Input.TEXCOORD.y <= UV_MaskingValue)
    {
        MaskColor = CrackTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
        
        float3 f3_BlurColor = BlurColor.rgb;
        
        if (MaskColor.a > 0.0f)
        {
            NewOutPut.BlurTarget = float4(f3_BlurColor, 1.0f);
            Color = NewOutPut.BlurTarget;
            
            NewOutPut.BlurTarget = pow(NewOutPut.BlurTarget, 2.2f);
        }
    }
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    //Fade
    if (Delta > 0.0f)
    {
        NewOutPut.BlurTarget *= Fading(MaskTexture, ENGINEBASE, _Input.TEXCOORD.xy);
        Color *= Fading(MaskTexture, ENGINEBASE, _Input.TEXCOORD.xy);
    }
    
    NewOutPut.DifTarget = pow(Color, 2.2f);
    NewOutPut.PosTarget = _Input.VIEWPOSITION;
    _Input.NORMAL.a = 1.0f;
    NewOutPut.NorTarget = _Input.NORMAL;
    
    return NewOutPut;
}

