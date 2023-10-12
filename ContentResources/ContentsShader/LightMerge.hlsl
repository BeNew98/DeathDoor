struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    // �����Ͷ������� �� ��������
    // w���� ����  ����Ʈ ���ϰ� �ȼ� �������� �������� ������ ���� �����ž�.
    float4 Pos : SV_Position;
    float4 UV : TEXCOORD;
};

OutPut Merge_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    // ȭ�� ��ü ����
    return OutPutValue;
}

Texture2D DiffuseColor : register(t0);
Texture2D DiffuseLight : register(t1);
Texture2D SpecularLight : register(t2);
Texture2D AmbientLight : register(t3);

SamplerState WRAPSAMPLER : register(s0);

struct LightTarget
{
    float4 DiffuseLight : SV_Target0;
    float4 SpecularLight : SV_Target1;
    float4 AmbientLight : SV_Target2;
};

LightTarget Merge_PS(OutPut _Value)
{
    float4 DifColor = DiffuseColor.Sample(WRAPSAMPLER, _Value.UV.xy);
    float4 DifLight = DiffuseLight.Sample(WRAPSAMPLER, _Value.UV.xy);
    float4 SpcLight = SpecularLight.Sample(WRAPSAMPLER, _Value.UV.xy);
    float4 AmbLight = AmbientLight.Sample(WRAPSAMPLER, _Value.UV.xy);
    
    LightTarget OutPut = (LightTarget) 0.0f;
    
    if (DifLight.a <= 0.0f)
    {
        clip(-1);
    }
        
    OutPut.DiffuseLight = DifLight * DifColor;
    OutPut.SpecularLight = SpcLight * DifColor;
    OutPut.AmbientLight = AmbLight * DifColor;
    
    return OutPut;
}