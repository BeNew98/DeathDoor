#pragma once
#include "BossFrog.h"

// ���� : ������ �԰��ִ� ������ ������ ���� �ִ� �� 3���� ��� ������ ���� ������� �Ѿ��.

class BossFrogMain : public BossFrog
{
public:
	// constrcuter destructer
	BossFrogMain();
	~BossFrogMain();

	// delete Function
	BossFrogMain(const BossFrogMain& _Other) = delete;
	BossFrogMain(BossFrogMain&& _Other) noexcept = delete;
	BossFrogMain& operator=(const BossFrogMain& _Other) = delete;
	BossFrogMain& operator=(BossFrogMain&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void InitAnimation() override;
	void SetFSMFUNC() override;

private:


	// intro to jump = idle to jump
	// idle to jump
	// swim to jump

	const float IdleStateDuration = 3.0f;

	enum class BossFrogMainState
	{
		IDLE, //IDLE
		INTRO, //POGO_START ��ȿ�� ��
		SWIM, // SWIM_EDIT
		DAMAGED, // DAMEGED_LOOP
		IDLE_JUMP_START,	//Idle to jump
		SWIM_JUMP_START,	//swim to jump
		JUMP_LOOP,
		JUMP_END,// SMASH,	
		MAX,
	};

	int JumpCount = 0;
};

