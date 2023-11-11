#pragma once
#include <GameEngineContents/BossBase.h>
#include "Boss_OldCrowDefinition.cpp"


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


protected:
	void InitPattern() override;
	void SetFSMFUNC() override;
	void InitAnimation() override;
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	enum class Boss_OldCrowState //������Ʈ ��
	{
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

		MAX,
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

	//ü�� ����
	std::vector<std::shared_ptr<class Boss_OldCrowChain>> Chains;     //���Ͽ� ���Ǵ� ü���� ���ִ� ����
	std::vector<std::shared_ptr<class GameEngineActor>> ChainsPivots; //ü���� �Ǻ� ����
	std::vector<std::shared_ptr<class GameEngineComponent>> MegaDash2PatternTransforms1; //�ް��뽬2 ���Ͽ� ���� Ʈ������1
	std::vector<std::shared_ptr<class GameEngineComponent>> MegaDash2PatternTransforms2; //�ް��뽬2 ���Ͽ� ���� Ʈ������2
	std::shared_ptr<class GameEngineActor> MegaDash2PatternTransformPivot;

	//Init
	void ChainsInit();

	//�������� ����
	void SetRandomPattern();    //������ ������ ���ؼ� ������
	void SetNextPatternState(); //������ ������ ������ ���� ������ ������

	//FSM ���� ���Ǵ� ����
	float4 Dir = float4::ZERO;  //��ǥ Dir
	float4 CurrentDir = float4::ZERO; //���� Dir

	bool IsTurn = false; //ȸ�� ����
	bool IsDeath = false;

	float CurrentChainSpeed = 10.0f;
	float StateCalTime = 0.0f;
	float StateCalTime2 = 0.0f;
	float StateCalFloat = 0.0f;
	float4 TargetPos = float4::ZERO;
	float4 StartPos = float4::ZERO;
	float4 JumpDir = float4::ZERO;
	float4 SlamDir = float4::ZERO;
	int MegaDash2PatternCount = 0;
	int MegaDash2PatternNumber = 0;  
	float CurrentSpeed = 0.0f;
	bool StateCalBool = false;

	//FSM���� ���Ǵ� �Լ�
	void TurnCheck(); //�뽬 �� ȸ�� ������Ʈ�� ������ ������ üũ
	void SetLerpDirection(float _DeltaTime); //������ ȸ���ϴ� (�뽬, ��) ������Ʈ���� ���
	void SetDirection(); //��� Dir�� ȸ��
	void SettingChainPatternPivot(); //���� �罽 ���� ������ ����� Pivot�� ������
	void SetMegaDashRandomPos(); //������ ��ġ�� �ڷ���Ʈ�ϴ� �ް��뽬 ���� ��

	float4 GetRandomPos(float _Value);

	void GetDamaged();		//�ǰ� üũ
	float DamagedTime = 0.0f; //Ư�� �ð����� ������ ���� �ʰԲ� 

	void SetDeathState();

	void CreateCrowHead();

	//Particle
	void CreateFeatherParticle();
	float FeatherCount = 0.0f;

	void CreateDustParticle();
	float DustCount = 0.0f;

	float4 StartColor = { 0.956f, 0.286f, 0.372f };
	float4 EndColor = { 0.0f, 0.0f, 0.0f };
	float LerpRatio = 0.0f;
};

