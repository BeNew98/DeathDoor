#pragma once
#include "ContentLevel.h"

enum class TestMapType
{
	Office,
	OldCrowFloor,
	None,
};


// Ό³Έν :
class MapTestLevel : public ContentLevel
{
public:
	// constrcuter destructer
	MapTestLevel();
	~MapTestLevel();

	// delete Function
	MapTestLevel(const MapTestLevel& _Other) = delete;
	MapTestLevel(MapTestLevel&& _Other) noexcept = delete;
	MapTestLevel& operator=(const MapTestLevel& _Other) = delete;
	MapTestLevel& operator=(MapTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:
	void InitTestLevel();

	inline void SetTestLevelType(TestMapType _Type)
	{
		m_Type = _Type;
	}

	TestMapType m_Type = TestMapType::None;
};
