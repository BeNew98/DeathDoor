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

	void Set_CullingObject(std::shared_ptr<class CullingObject> _Obj, std::shared_ptr<class CullingObject> _Obj2);
	
	void On_CullingObject();
	void Off_CullingObject();


	inline bool IsActivate()
	{
		return m_bIsActivate;
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


	std::weak_ptr<class CullingObject> m_pCullingObject_1;
	std::weak_ptr<class CullingObject> m_pCullingObject_2;
};
