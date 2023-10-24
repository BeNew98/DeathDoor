#pragma once
#include "PhysXLevel.h"

enum class ContentLevelType
{
	NONE,
	OfficeLevel,
	FortressLevel,
	FrogBossLevel,
	OldCrowLevel,
};

class ContentLevel : public PhysXLevel
{
public:
	ContentLevel();
	~ContentLevel();

	ContentLevel(const ContentLevel& _Other) = delete;
	ContentLevel(ContentLevel&& _Other) noexcept = delete;
	ContentLevel& operator=(const ContentLevel& _Other) = delete;
	ContentLevel& operator=(ContentLevel&& _Other) noexcept = delete;

	inline ContentLevelType GetContentLevelType() const
	{
		return m_eLevelType;
	}

	inline void SetContentLevelType(ContentLevelType _Type)
	{
		if (_Type == ContentLevelType::NONE)
		{
			MsgAssert("콘텐츠레벨타입이 NONE 입니다. 각 레벨의 Start 에서 타입을 세팅하세요.");
			return;
		}

		m_eLevelType = _Type;
	}

protected:
	void LevelInit();
	void CreateUI();
	void SetPostPrecessEffect();
private:

	void CreateIMGUIDebugRenderTarget();

	ContentLevelType m_eLevelType = ContentLevelType::NONE;
};

