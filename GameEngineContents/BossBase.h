#pragma once
#include <GameEngineContents/EnemyBase.h>

// ���� :
class BossBase : public EnemyBase
{
public:
	// constrcuter destructer
	BossBase();
	~BossBase();

	// delete Function
	BossBase(const BossBase& _Other) = delete;
	BossBase(BossBase&& _Other) noexcept = delete;
	BossBase& operator=(const BossBase& _Other) = delete;
	BossBase& operator=(BossBase&& _Other) noexcept = delete;

protected:
	//virtual void InitAniamtion() {};
	virtual void InitPattern() {};

	std::map<short, std::vector<short>> Patterns;

	short PatternNum = 0; //���� �������� ���� ��ȣ
	short CurrentPatternNum = 0; //���� ��ȣ�� ���° ���� ����������

private:

};

