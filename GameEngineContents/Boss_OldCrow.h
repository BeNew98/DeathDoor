#pragma once
#include <GameEngineContents/BossBase.h>

class Boss_OldCrow : public BossBase
{
public:
	// constrcuter destructer
	Boss_OldCrow();
	~Boss_OldCrow();

	// delete Function
	Boss_OldCrow(const Boss_OldCrow& _Other) = delete;
	Boss_OldCrow(Boss_OldCrow&& _Other) noexcept = delete;
	Boss_OldCrow& operator=(const Boss_OldCrow& _Other) = delete;
	Boss_OldCrow& operator=(Boss_OldCrow&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent()
	{
		return m_pCapsuleComp;
	}

protected:
	void InitAniamtion() override;
	void InitPattern() override;
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	enum class Boss_OldCrowState //������Ʈ ��
	{
		EMPTY,

		IDLE,

		//�뽬
		DASHSTART,
		DASH,
		TURN,

		//�罽
		MEGADASHPREP,
		MEGADASHPREPRANDOMPOS,
		MEGADASH,

		//���� �罽
		MEGADASH2PREP,
		MEGADASH2,

		//����
		JUMP,
		SLAM,
		SLAMIMPACT,

		//Egg
		EGG,

		//SmallCrow
		SCREAM,
		SCREAMMINI,
		
		//Death
		DEATHWHILERUNNING,

		DEATHWHILEUPRIGHT,
	};

	enum class Boss_OldCrowPattern //���� ��
	{
		PATTERN1, // �뽬, �뽬
		PATTERN2, // ����
		PATTERN3, // �罽, �罽, ����, �뽬
		PATTERN4, // �罽, �罽, ���ӻ罽, ����, �뽬
		PATTERN5, // SmallCrow ��ȯ
		PATTERN6, // EGG ��ȯ 
		PATTERNCOUNT //���� �� ���� (size)
	};

	std::shared_ptr<class ContentFBXRenderer> BossRender = nullptr;
	std::shared_ptr<class PhysXCapsuleComponent> m_pCapsuleComp = nullptr;

	//ü�� ����
	std::vector<std::shared_ptr<class Boss_OldCrowChain>> Chains;
	std::shared_ptr<class GameEngineComponent> ChainsPivot = nullptr;
	std::shared_ptr<Boss_OldCrowChain> GetChain();

	//Init
	void ChainsInit();

	//�������� ����
	void SetRandomPattern();    //������ ������ ���ؼ� ������
	void SetNextPatternState(); //������ ������ ������ ���� ������ ������

	//FSM Init
	void SetFSMFUNC();

	//FSM ���� ���Ǵ� ����
	float4 Dir = float4::ZERO;
	float4 CurrentDir = float4::ZERO;
	bool IsTurn = false;
	std::vector<std::vector<int>> UsingChainNumber;
	float4 ChainSize = float4::ZERO;
	float CurrentChainSpeed = 0.0f;

	float StateCalTime = 0.0f;
	float StateCalTime2 = 0.0f;

	// FSM ���� ���Ǵ� ���
	const float DashSpeed = 1000.0f;
	const float DashSpeed2 = DashSpeed * 0.75f;
	const float MegaDashSpeed = DashSpeed * 2.0f;
	const int ChainsCount = 10;
	const float ChainSpeed = 100.0f;

	//FSM���� ���Ǵ� �Լ�
	void TurnCheck(); //�뽬 �� ȸ�� ������Ʈ�� ������ ������ üũ
	void SetLerpDirection(float _DeltaTime); //������ ȸ���ϴ� (�뽬, ��) ������Ʈ���� ���
};

