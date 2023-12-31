#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class PhysXTestPlane :public GameEngineActor
{
public:
	// constrcuter destructer
	PhysXTestPlane();
	~PhysXTestPlane();

	// delete Function
	PhysXTestPlane(const PhysXTestPlane& _Other) = delete;
	PhysXTestPlane(PhysXTestPlane&& _Other) noexcept = delete;
	PhysXTestPlane& operator=(const PhysXTestPlane& _Other) = delete;
	PhysXTestPlane& operator=(PhysXTestPlane&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXBoxComponent>GetComp()
	{
		return m_pBoxComp;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class PhysXBoxComponent> m_pBoxComp = nullptr;
	static int Count;
};

