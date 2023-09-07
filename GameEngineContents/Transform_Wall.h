#pragma once
#include <GameEngineCore/GameEngineActor.h>


// ���� : ����ġ�� ����
class Transform_Wall : public GameEngineActor
{
public:
	// constrcuter destructer
	Transform_Wall();
	~Transform_Wall();

	// delete Function
	Transform_Wall(const Transform_Wall& _Other) = delete;
	Transform_Wall(Transform_Wall&& _Other) noexcept = delete;
	Transform_Wall& operator=(const Transform_Wall& _Other) = delete;
	Transform_Wall& operator=(Transform_Wall&& _Other) noexcept = delete;

	inline std::shared_ptr<class PhysXBoxComponent> GetPhysXComponent() const
	{
		return m_pComp;
	}

	// �������� ��� xyz�� �߿��� ������ ������ ���� ��� 1 �Է� 
	void Set_MeshRatio(float4& _Ratio);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void InitComponent();

	std::shared_ptr<class GameEngineFBXRenderer> m_pRenderer = nullptr;
	std::shared_ptr<class PhysXBoxComponent > m_pComp = nullptr;
};
