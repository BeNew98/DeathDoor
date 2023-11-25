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

// ������� ���ؼ� ���� �ݻ縦 �����ϰ� ����
// �� ���� ����ϴ� ������ ���� �������� ������ �ߴ�.
// ������ ������ ���� �ǹ̰� ����.
// ���̶�� �÷��� ���س��� ���� �ȴ�.

// �װ� ���ؽ� ���̴������ϸ� �װ� �������� �ϸ� �÷����̵�
// �װ� ���ؽ� ���̴������ϸ� �װ� ��ν��̵�
// �װ� �ȼ� ���̴������ϸ� �װ� �����̵�

// �׷���ī�忡�� �̷����°�.
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

cbuffer isEffect : register(b5)
{
    bool isHdr;
    bool3 Padding;
};

float4 ToneMapping_ACES(float4 _Color)
{
    return saturate((_Color * (2.51f * _Color + 0.03f)) / (_Color * (2.43f * _Color + 0.59f) + 0.14f));
}

float4 DeferredCalLight_PS(Output _Input) : SV_Target0
{
    LightOutPut NewOutPut = (LightOutPut) 0;
    
    // ���� => ���� => �� => �������� => ����Ʈ
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
    
    // ����� ����Ʈ�� ������ ���̶��
    if (DiffuseRatio.x > 0.0f)
    {
        float4 WorldViewPos = Position;
        WorldViewPos.a = 1.0f;
        // ���������ϹǷ�
        // �׸��ڵ� �����ؾ����� �Ǵ��Ѵ�.
        // ��� world 
        float4 WorldPos = mul(float4(WorldViewPos.xyz, 1.0f), LData.CameraViewInverseMatrix);
        WorldPos.w = 1.0f;
        
        // ���� ���������� ���������� �ٲ��.
        float4 LightPos = mul(WorldPos, LData.LightViewProjectionMatrix);
        
    // worldviewprojection 
    // �� �������� �װ� -1~1������ �����Դϱ�?
    // w�� ������������ z���� ������ ���� ���� �˴ϴ�.
        float3 LightProjection = LightPos.xyz / LightPos.w;
    // ��� ���� -1~1������ ���� �˴ϴ�.
        
        float2 ShadowUV = float2(LightProjection.x * 0.5f + 0.5f, LightProjection.y * -0.5f + 0.5f);
        float fShadowDepth = ShadowTex.Sample(POINTWRAP, float2(ShadowUV.x, ShadowUV.y)).r;
        
    // ���� �ܰ��� �ణ ��Ƴ��� 
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
    
    if (isHdr == true)
    {
        float Alpha = ResultColor.a;
        ResultColor = ToneMapping_ACES(ResultColor);
        ResultColor.a = Alpha;
    }
    
    return ResultColor;
}

