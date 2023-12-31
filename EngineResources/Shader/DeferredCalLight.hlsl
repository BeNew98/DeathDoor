#include "Light.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct Output
{
    float4 POSITION : SV_Position;
    float4 TEXCOORD : TEXCOORD;
};

// 빛계산을 통해서 빛의 반사를 구현하고 나면
// 그 빛을 계산하는 시점에 따라서 예전에는 구분을 했다.
// 이제는 구분이 거의 의미가 없다.
// 빛이라는 컬러를 구해내는 것이 된다.

// 그걸 버텍스 쉐이더에서하면 그걸 점단위로 하면 플랫쉐이딩
// 그걸 버텍스 쉐이더에서하면 그걸 고로쉐이딩
// 그걸 픽셀 쉐이더에서하면 그걸 퐁쉐이딩

// 그래픽카드에서 이뤄지는것.
Output DeferredCalLight_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.POSITION = _Input.POSITION;
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    return NewOutPut;
}

struct LightOutPut
{
    float4 DifLight : SV_Target0;
    float4 SpcLight : SV_Target1;
    float4 AmbLight : SV_Target2;
    float4 ShadowTest : SV_Target3;
    float4 PointLight : SV_Target4;
};

Texture2D PositionTex : register(t0);
Texture2D NormalTex : register(t1);
Texture2D ShadowTex : register(t2);
Texture2D DiffuseColor : register(t3);
SamplerState POINTWRAP : register(s0);

float4 ToneMapping_ACES(float4 _Color)
{
    return saturate((_Color * (2.51f * _Color + 0.03f)) / (_Color * (2.43f * _Color + 0.59f) + 0.14f));
}

float4 DeferredCalLight_PS(Output _Input) : SV_Target0
{
    LightOutPut NewOutPut = (LightOutPut) 0;
    
    // 로컬 => 월드 => 뷰 => 프로젝션 => 뷰포트
    // Position
    float4 Position = PositionTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    
    if (0 >= Position.a)
    {
        clip(-1);
    }
    
    float4 Normal = NormalTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    Normal = normalize(Normal);
    
    float4 DiffuseRatio = (float4) 0.0f;
    float4 SpacularRatio = (float4) 0.0f;
    float4 AmbientRatio = (float4) 0.0f;
    float4 PointLight = (float4) 0.0f;
    
    for (int Index = 0; Index < PointLightNum; ++Index)
    {
        PointLight += CalPointLight(Position, Normal, PointLights[Index]);
    }

    LightData LData = AllLight[LightCount];
    
    DiffuseRatio += CalDiffuseLight(Position, Normal, LData);
    SpacularRatio += CalSpacularLight(Position, Normal, LData);;
    AmbientRatio += CalAmbientLight(LData);
    
    // 디뷰즈 라이트가 비춰진 곳이라면
    if (DiffuseRatio.x > 0.0f)
    {
        float4 WorldViewPos = Position;
        WorldViewPos.a = 1.0f;
        // 빛이존재하므로
        // 그림자도 존재해야할지 판단한다.
        // 어느 world 
        float4 WorldPos = mul(float4(WorldViewPos.xyz, 1.0f), LData.CameraViewInverseMatrix);
        WorldPos.w = 1.0f;
        
        // 빛을 기준으로한 포지션으로 바꿨다.
        float4 LightPos = mul(WorldPos, LData.LightViewProjectionMatrix);
        
    // worldviewprojection 
    // 이 곱해지면 그건 -1~1사이의 공간입니까?
    // w에 곱해지기전의 z값을 보관해 놓은 값이 됩니다.
        float3 LightProjection = LightPos.xyz / LightPos.w;
    // 모든 값은 -1~1사이의 값이 됩니다.
        
        float2 ShadowUV = float2(LightProjection.x * 0.5f + 0.5f, LightProjection.y * -0.5f + 0.5f);
        float fShadowDepth = ShadowTex.Sample(POINTWRAP, float2(ShadowUV.x, ShadowUV.y)).r;
        
    // 가장 외각을 약간 깎아내서 
        if (
            fShadowDepth >= 0.0f
            && 0.001f < ShadowUV.x && 0.999f > ShadowUV.x
            && 0.001f < ShadowUV.y && 0.999f > ShadowUV.y
            && LightProjection.z >= (fShadowDepth + 0.001f)
            )
        {
            NewOutPut.ShadowTest.x = 1.0f;
            NewOutPut.ShadowTest.a = 1.0f;
        }
    }

    if (0 != NewOutPut.ShadowTest.x)
    {
        DiffuseRatio = 0.2f / NewOutPut.ShadowTest.x;
        SpacularRatio = 0.2f / NewOutPut.ShadowTest.x;
    }
    
    float4 DifColor = DiffuseColor.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    
    float4 ResultColor = (float4) 0.0f;
    
    if (DifColor.a)
    {
    //      0.1f
        ResultColor.xyz = DifColor.xyz * (PointLight.xyz + DiffuseRatio.xyz + SpacularRatio.xyz + AmbientRatio.xyz);
        ResultColor.a = saturate(DifColor.a + (PointLight.w + DiffuseRatio.w + SpacularRatio.w + AmbientRatio.w));
        ResultColor.a = 1.0f;
    }
    else
    {
        ResultColor.xyz = (PointLight.xyz + DiffuseRatio.xyz + SpacularRatio.xyz + AmbientRatio.xyz);
        ResultColor.a = saturate(ResultColor.x);
    }
    
    return ResultColor;
}

