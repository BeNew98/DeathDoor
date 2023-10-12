#pragma once
#include "PhysXLevel.h"

enum class ContentLevelType
{
	NONE,
	OfficeLevel,
	FortressLevel,
	FrogBossLevel,
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
			MsgAssert("����������Ÿ���� NONE �Դϴ�. �� ������ Start ���� Ÿ���� �����ϼ���.");
			return;
		}

		m_eLevelType = _Type;
	}

protected:
	void CreateUI();
	void SetPostPrecessEffect();
private:

	ContentLevelType m_eLevelType = ContentLevelType::NONE;
};

