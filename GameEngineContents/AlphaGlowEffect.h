#pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>

class AlphaGlowEffect : public GameEnginePostProcess
{

public:

	AlphaGlowEffect();
	~AlphaGlowEffect();

	AlphaGlowEffect(const AlphaGlowEffect& _Other) = delete;
	AlphaGlowEffect(AlphaGlowEffect&& _Other) noexcept = delete;
	AlphaGlowEffect& operator=(const AlphaGlowEffect& _Other) = delete;
	AlphaGlowEffect& operator=(AlphaGlowEffect&& _Other) noexcept = delete;

	void SetLevel(std::weak_ptr<GameEngineLevel> _Level)
	{
		EffectedLevel = _Level;
	}

	std::shared_ptr<GameEngineRenderTarget> DetectMaskTarget = nullptr;
	std::shared_ptr<GameEngineRenderTarget> BlurTarget = nullptr;
	std::shared_ptr<GameEngineRenderTarget> DoubleBlurTarget = nullptr;

	//x�� DiffuseColor, y�� DiffuseLight, z�� SpecularLight, w�� AmbientLight
	void SetIntensity(float4 _Intensity)
	{
		Intensity.x = _Intensity.x;
		Intensity.y = _Intensity.y;
		Intensity.z = _Intensity.z;
		Intensity.w = _Intensity.w;
	}

	void CreateTarget(float4 _BlurSize);

	void Init(std::weak_ptr<GameEngineLevel> _Level, float4 _Intensity, float4 _BlurSize)
	{
		SetLevel(_Level);
		SetIntensity(_Intensity);
		CreateTarget(_BlurSize);
	}

	//x,y �� �⺻�� z,w �� �����
	void SetBlurScale(float4 _Scale)
	{
		BlurSize = { _Scale.x, _Scale.y };
		DoubleBlurSize = { _Scale.z, _Scale.w };
	}

	void DoubleBlurOn()
	{
		isDoubleBlur = true;
	}

	void DoubleBlurOff()
	{
		isDoubleBlur = false;
	}

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;
private:

	std::weak_ptr<GameEngineLevel> EffectedLevel;

	GameEngineRenderUnit DetectMaskUnit;
	GameEngineRenderUnit BlurMergeUnit;
	GameEngineRenderUnit BlurUnit;
	GameEngineRenderUnit DoubleBlurUnit;
	GameEngineRenderUnit ColorMerge;

	float4 Intensity = { 1.0f, 1.0f, 1.0f, 1.0f };
	float4 BlurSize = { 1600.0f, 900.0f };
	float4 DoubleBlurSize = { 1600.0f, 900.0f };

	bool isDoubleBlur = true;
};

