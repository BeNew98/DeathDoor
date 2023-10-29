#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class CullingTrigger : public GameEngineActor
{
public:
	// constrcuter destructer
	CullingTrigger();
	~CullingTrigger();

	// delete Function
	CullingTrigger(const CullingTrigger& _Other) = delete;
	CullingTrigger(CullingTrigger&& _Other) noexcept = delete;
	CullingTrigger& operator=(const CullingTrigger& _Other) = delete;
	CullingTrigger& operator=(CullingTrigger&& _Other) noexcept = delete;

	inline void TriggerOn()
	{
		m_bIsActivate = true;
	}

	inline void TriggerOff()
	{
		m_bIsActivate = false;
	}

	inline std::vector<int> Get_CullingObjectNumbers() const
	{
		return m_vCullingObj_Numbers;
	}

	void Set_CullingObjectNumber(int _Number0 , int _Number1, int _Number2, int _Number3 = INT_MAX);
	
	inline bool IsActivate()
	{
		return m_bIsActivate;
	}

	inline std::shared_ptr<class ContentFBXRenderer> GetRenderer()
	{
		return m_pRenderer;
	}
	
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void InitComponent();

	bool m_bIsActivate = false;

	std::shared_ptr<class ContentFBXRenderer> m_pRenderer = nullptr;
	std::shared_ptr<class PhysXBoxComponent> m_pPhysXComponent = nullptr;

	// Ʈ���Ű� ������Ʈ�� �����°� �ƴ�. Ʈ���Ű� Ȱ��ȭ �� �ø�������Ʈ�� �ε����ѹ��� �迭�ΰ�����. 
	std::vector<int> m_vCullingObj_Numbers = std::vector<int>();
};
