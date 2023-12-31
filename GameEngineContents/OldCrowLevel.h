#pragma once
#include "ContentLevel.h"

// ���� :
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
	void Create_Light();
	void Create_Map();
	void Create_Player();
	void Create_OldCrow();

	void ClearCheck();

	void InitKey();



	void Set_PlayerStartPos();
	const float4 m_StartPos = float4{ 0, 1, 0 };

	std::shared_ptr<class Map_Emptyplain> m_pMap = nullptr;

	bool m_bIsClear = false;
	std::weak_ptr<class Boss_OldCrow> m_pBoss;

};
