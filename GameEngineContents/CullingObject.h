#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class CullingObject : public GameEngineActor
{
public:
	// constrcuter destructer
	CullingObject();
	~CullingObject();

	// delete Function
	CullingObject(const CullingObject& _Other) = delete;
	CullingObject(CullingObject&& _Other) noexcept = delete;
	CullingObject& operator=(const CullingObject& _Other) = delete;
	CullingObject& operator=(CullingObject&& _Other) noexcept = delete;

	inline bool IsCulling() const
	{
		return m_bIsCulling;
	}

	void InitComponent(const std::string& _MeshName);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

	bool m_bIsCulling = true;
	
	std::shared_ptr<class ContentFBXRenderer> m_pRenderer = nullptr;
	std::shared_ptr<class PhysXBoxComponent> m_pPhysXComponent = nullptr;



	// Ʈ���� ���͸� �ϳ��ΰ�, ��ġ�Ѵ�. 
	// �÷��̾ �� Ʈ���ſ� �浹�ߴٸ�, �ش� Ʈ���ſ� ���� �Ǿ� �ִ� ��� Renderer�� On ? 
};
