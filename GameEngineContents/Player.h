#pragma once
#include <GameEngineBase/GameEngineNetObject.h>
#include "PlayerDefinition.h"
#include "FSMObjectBase.h"


// ���� :
class Player : public FSMObjectBase, public GameEngineNetObject
{
public:
	static Player* MainPlayer;
public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent() const
	{
		return m_pCapsuleComp;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

private:

	// Struct
	enum class PlayerState
	{
		IDLE,			// Idle_0, Idle_1
		WALK,			// Walk, Run
		SKILL,			// ��Ŭ�� Arrow, Arrow_bomb, Arrow_magic, Hookshot, Hookshot_fly
		HOOK_FLY,		// Hookshot_fly
		BASE_ATT,		// ��Ŭ�� Slash_Light_L_new, Slash_Light_R_new
		ROLL,			// �����̽��� Roll, Roll_slash
		ROLL_ATT,		// �����̽���+��Ŭ�� Charge_slam_overhead, Roll_slash_end
		CHARGE_ATT,		// ��Ŭ�� Charge_slash_L, Charge_slash_R
		HIT,			// ���ݴ��� Hit_back, Hit_idle,Hit_Recover
		CLIMB,			// ��ٸ� Climbing_ladder, Climbing_ladder_down, Climbing_off_ladder_top
		LEVER,			// ������ ���� Push_Lever
		ITEM,			// �������� ���� GetItem
		DEAD,			// �ǰ����� ���� ��� Dead
		DROWN,			// �ͻ� Drown
		FALLING,		// ���� Falling
		FLY,			// ���̰� ���� ���� �� FLy, Land
		MAX,
	};

	enum class PlayerSkill
	{
		ARROW,
		MAGIC,
		BOMB,
		HOOK,
		MAX,
	};




	// Init
	std::atomic_int AnimationLoadCount = 0;
	void InitInputKey();
	void InitPlayerAnimatioin();
	void SetFSMFunc();

	// Render
	std::shared_ptr<GameEngineFBXRenderer> Renderer = nullptr;


		// Attack
	PlayerSkill CurSkill = PlayerSkill::ARROW;
	void SetSkill();
	bool isRightAttack = true;

	float StateDuration = 2.0f;
	bool StateChecker = false;
	
	// Direction
	float4 NextForwardDir = float4::FORWARD; // �÷��̾ ��ȭ �� ����
	float4 ForwardDir = float4::FORWARD; // �÷��̾ �ٶ󺸴� ����
	float4 MoveDir = float4::FORWARD; // �÷��̾ �������� ������ ����
	void DirectionUpdate(float _DeltaTime);
	float4 GetMousDirection();
	 
	
	// input & move
	float StateInputDelayTime = 0.0f;
	void CheckInput(float _DeltaTime);
	void CheckClimbInput(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);

	float MoveSpeed = 500.0f;
	bool mButton = false;


	//physx
	std::shared_ptr<class PhysXCapsuleComponent> m_pCapsuleComp = nullptr;
	void DefaultPhysX();


};

