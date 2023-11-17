#pragma once
#include "ContentLevel.h"

enum class PrevLevelType
{
	OldCrowLevel,
	FortressLevel,
	BossFrogLevel,
	ExplainLevel,
	None,
};

// M : NaviRender OnOff 스위치 

// 설명 :
class OfficeLevel : public ContentLevel
{
public:
	// constrcuter destructer
	OfficeLevel();
	~OfficeLevel();

	// delete Function
	OfficeLevel(const OfficeLevel& _Other) = delete;
	OfficeLevel(OfficeLevel&& _Other) noexcept = delete;
	OfficeLevel& operator=(const OfficeLevel& _Other) = delete;
	OfficeLevel& operator=(OfficeLevel&& _Other) noexcept = delete;
	
	void SetPrevLevelType(PrevLevelType _Type)
	{
		m_eType = _Type;
	}


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void InitKey();
	void KeyUpdate(float _DeltaTime);

	void LevelChangeStart() override;
	void LevelChangeEnd() override;


private:
	void Create_Light();
	void Create_Map();
	void Create_Player();
	
	void Set_PlayerStartPos();

	const float4 m_CameraPos = float4{ 2800 , 2500 , -6500 };
	const float4 m_CameraRot = float4{ 45.0f , 0 , 0 };

	float4 m_StartPos = float4 { 2610 , -740 , -5347 };

	const float4 m_f4FortressToOfficePos = float4{ 1201, 1256, 5077 };
	const float4 m_f4OldCrowToOfficePos = float4{ -1156, 1652, 6140 };
	const float4 m_f4ExplainToOfficePos = float4{ 1065, -750, -5321 };

	std::shared_ptr<class Map_Office> m_pMap = nullptr;

	void SetPointLight();
	void Create_TriggerObject();

	PrevLevelType m_eType = PrevLevelType::None;
	std::weak_ptr<class FadeEffect> m_pFadeEffect;

	class GameEngineSoundPlayer MainBGM;

	
};
