#pragma once
#include "PhysXLevel.h"

// ���� : ���� �׽�Ʈ�� ����
class BossTestLevel : public PhysXLevel
{
public:
	// constrcuter destructer
	BossTestLevel();
	~BossTestLevel();

	// delete Function
	BossTestLevel(const BossTestLevel& _Other) = delete;
	BossTestLevel(BossTestLevel&& _Other) noexcept = delete;
	BossTestLevel& operator=(const BossTestLevel& _Other) = delete;
	BossTestLevel& operator=(BossTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;
	void LevelChangeEnd() override;


private:

};

