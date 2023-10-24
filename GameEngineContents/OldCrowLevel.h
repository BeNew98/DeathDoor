#pragma once
#include "ContentLevel.h"

// Ό³Έν :
class OldCrowLevel : public ContentLevel
{
public:
	// constrcuter destructer
	OldCrowLevel();
	~OldCrowLevel();

	// delete Function
	OldCrowLevel(const OldCrowLevel& _Other) = delete;
	OldCrowLevel(OldCrowLevel&& _Other) noexcept = delete;
	OldCrowLevel& operator=(const OldCrowLevel& _Other) = delete;
	OldCrowLevel& operator=(OldCrowLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	void InitKey();

	const float4 m_CameraPos = float4{ 0, 2000, -1400 };
	const float4 m_CameraRot = float4{ 55, 0, 0 };

	void Set_PlayerStartPos();
	const float4 m_StartPos = float4{ 0, 1, 0 };

	std::shared_ptr<class Map_Emptyplain> m_pMap = nullptr;
};
