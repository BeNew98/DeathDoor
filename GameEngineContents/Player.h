#pragma once
#include <GameEngineBase/GameEngineNetObject.h>
#include "PlayerDefinition.h"
#include "FSMObjectBase.h"
#include "Ladder.h"
#include <queue>

// ���� :
class Player : public FSMObjectBase, public GameEngineNetObject
{
	friend class PlayerAttSkill;
public:

	static Player* MainPlayer;
	enum class PlayerSkill
	{
		ARROW,
		MAGIC,
		BOMB,
		HOOK,
		MAX,
	};

	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXControllerComponent> GetPhysXComponent() const
	{
		return m_pCapsuleComp;
	}

	float4 GetBonePos(const std::string_view& _BoneName);

	inline int GetPlayerHP()
	{
		return PlayerHP;
	}
	inline int GetSpellCost()
	{
		return SpellCost;
	}

	inline void AddSpellCost()
	{
		if (SpellCost >= 4)
		{
			return;
		}
		++SpellCost;
	}

	bool PlayerTestMode = true;


	inline PlayerSkill GetPlayerSkill()
	{
		return CurSkill;
	}

	inline int GetPlayerSkillInt()
	{
		return static_cast<int>(CurSkill);
	}

	void GetLadderData(class LadderData _Data)
	{
		sLadder = _Data;
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
		BASIC_ATT,		// ��Ŭ�� Slash_Light_L_new, Slash_Light_R_new
		ROLL,			// �����̽��� Roll, Roll_slash
		ROLL_ATT,		// �����̽���+��Ŭ�� Charge_slam_overhead, Roll_slash_end
		CHARGE_ATT,		// ��Ŭ�� Charge_slash_L, Charge_slash_R
		HIT,			// ���ݴ��� Hit_back, Hit_idle,Hit_Recover
		CLIMB,			// ��ٸ� Climbing_ladder, Climbing_ladder_down, Climbing_off_ladder_top
		LEVER,			// ������ ���� Push_Lever
		ITEM,			// �������� ���� GetItem
		DEAD,			// �ǰ����� ���� ��� Dead
		DROWN,			// �ͻ� Drown
		FALLING,			// ���̰� ���� ���� �� falling, Land
		MAX,
	};

	


	// Init
	std::atomic_int AnimationLoadCount = 0;
	void InitInputKey();
	void InitPlayerAnimation();
	void SetFSMFunc();

	// Component
		// Render
	std::shared_ptr<ContentFBXRenderer> Renderer = nullptr;
	std::shared_ptr<GameEngineComponent> BonePivot = nullptr;
		//physx
	std::shared_ptr<class PhysXControllerComponent> m_pCapsuleComp = nullptr;

		// Attack
	PlayerSkill CurSkill = PlayerSkill::ARROW;
	void SetSkill();
	bool isRightAttack = true;
	bool isChargeAttack = false;
	// ���̽����� 3Ÿ�� ���� ������ üũ��
	int AttackStack = 0;
	float StackDuration = 0.0f;
	
	// Direction
	float4 ForwardDir = float4::BACK; // �÷��̾ �ٶ󺸴� ����
	float4 MoveDir = float4::FORWARD; // �÷��̾ �������� ������ ����
	void DirectionUpdate(float _DeltaTime);
	float4 GetMousDirection();

	class LadderData sLadder = {};
	
	// State Controll
	float StateInputDelayTime = 0.0f;

	void CheckFalling();
	void CheckDirInput(float _DeltaTime);
	void CheckStateInput(float _DeltaTime);
	void CheckState(float _DeltaTime);

	void CheckClimbInput(float _DeltaTime);
	void MoveUpdate(float _MoveVec, float4 _Dir = float4::ZERONULL);	//MoveDir�� �ش��ϴ� ���� ó���ϱ� ������

	void DefaultPhysX();

	void CheckPlayerHit();

	//��ƼŬ
	void CreateDustParticle(float _Delta);
	float ParticleCount = 0.0f;

	// Attack
	std::shared_ptr< class AttackBase> AttackActor = nullptr;

	// WeaponRender
	std::shared_ptr<class PlayerBow> WeaponActor = nullptr;


	int PlayerHP = 4;
	int SpellCost = 4;

	// Level
	float4 CameraRot = float4::ZERO;

	// respone pos
	std::queue<float4> respawnPos;
	float PosInter = 0.0f;

};

