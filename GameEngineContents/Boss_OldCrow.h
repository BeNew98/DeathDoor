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

protected:
	void InitAniamtion() override;
	void InitPattern() override;
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	enum class Boss_OldCrowState //������Ʈ ��
	{
		IDLE,

		//�뽬
		STARTDASH,
		DASH,
		TURN,

		//�罽
		MEGADASHPREP,
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
		PATTERNCOUNT //���� �� ����
	};

	//std::map<Boss_OldCrowState, BossStateParameter> StateMap;
	std::shared_ptr<class GameEngineFBXRenderer> BossRender = nullptr;
	std::shared_ptr<class PhysXCapsuleComponent> m_pCapsuleComp = nullptr;

	void SetFSMFUNC();
	
};

