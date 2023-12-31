#include "../../EngineResources/Shader/Transform.fx"
#include "ContentFunction.fx"

struct Input
{
    float4 Pos : POSITION;
    float4 Normal : Normal;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    float4 Pos : SV_Position;
    float4 ViewPos : POSITION;
    float4 ViewNormal : NORMAL;
    float4 UV : TEXCOORD0;
    float4 ClipUV : TEXCOORD1;
};


OutPut ContentTexture_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
	
    _Value.Pos.w = 1.0f;
    OutPutValue.Pos = mul(_Value.Pos, WorldViewProjectionMatrix);
    OutPutValue.UV = _Value.UV;
    
    OutPutValue.ViewPos = mul(_Value.Pos, WorldView);
    OutPutValue.ViewNormal = mul(_Value.Normal, WorldView);
    
    return OutPutValue;
}

Texture2D DiffuseTexture : register(t0);
Texture2D NoiseTexture1 : register(t1);
Texture2D NoiseTexture2 : register(t2);
Texture2D AlphaTexture : register(t3);

SamplerState CLAMPSAMPLER : register(s0);
SamplerState WRAPSAMPLER : register(s1);

cbuffer MaskValue : register(b5)
{
    float2 DeltaTime;
    float Alpha;
    float Padding;
};

struct DefferedTarget
{
    float4 DiffuseColor : SV_Target6;
};

cbuffer DiffuseUV : register(b6)
{
    float2 DiffuseUV;
    float2 padding;
}

cbuffer DistortionData : register(b7)
{
    float2 Distortion1;
    float2 Distortion2;
    float DistortionScale;
    float DistortionBias;
}

DefferedTarget ContentTexture_PS(OutPut _Value)
{
    DefferedTarget OutPutTarget = (DefferedTarget) 0.0f;
    
    float4 Noise1 = NoiseTexture1.Sample(WRAPSAMPLER, _Value.UV.xy * 0.35f + DiffuseUV);
    float4 Noise2 = NoiseTexture2.Sample(WRAPSAMPLER, _Value.UV.xy * 0.35f);
            
    Noise1 = (Noise1 - 0.5f) * 2.0f;
    Noise2 = (Noise2 - 0.5f) * 2.0f;
    
    Noise1.xy = Noise1.xy * Distortion1;
    Noise2.xy = Noise2.xy * Distortion2;
    
    float4 FinalNoise = Noise1 + Noise2;
    float Perturb = ((1.0f - _Value.UV.y) * DistortionScale) + DistortionBias;
    
    float2 NoiseCoords = (FinalNoise.xy * Perturb) + _Value.UV.xy;
    
    float4 DustColor = AddColor;
    
    float4 AlphaColor = AlphaTexture.Sample(CLAMPSAMPLER, NoiseCoords.xy);
        
    if (1 - AlphaColor.a >= DeltaTime.x)
    {
        clip(-1);
    }
    
    if (1 - AlphaColor.a >= DeltaTime.y)
    {
        clip(-1);
    }
        
    float Alpha = 0.0f;
        
    if (AddColor.a >= 0.0f)
    {
        Alpha = DustColor.a;
        DustColor.rgb += AlphaColor.a;
    }
    else
    {
        Alpha = 1.0f;
    }
        
    DustColor = ceil(DustColor * 5.0f) / 5.0f;
    DustColor.a = Alpha;
        
    OutPutTarget.DiffuseColor = DustColor;
    return OutPutTarget;

    
}


   
    
    
    


