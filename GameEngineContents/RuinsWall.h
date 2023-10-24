#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class RuinsWall : public GameEngineActor
{
public:
	// constrcuter destructer
	RuinsWall();
	~RuinsWall();

	// delete Function
	RuinsWall(const RuinsWall& _Other) = delete;
	RuinsWall(RuinsWall&& _Other) noexcept = delete;
	RuinsWall& operator=(const RuinsWall& _Other) = delete;
	RuinsWall& operator=(RuinsWall&& _Other) noexcept = delete;

	inline std::shared_ptr<class PhysXBoxComponent> GetPhysXComponent() const
	{
		return m_pPhysXComponent;
	}

	inline std::shared_ptr<class ContentFBXRenderer> GetRenderer() const
	{
		return m_pRenderer;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void InitComponent();

	std::shared_ptr<class ContentFBXRenderer> m_pRenderer = nullptr;
	std::shared_ptr<class PhysXBoxComponent> m_pPhysXComponent = nullptr;
};
