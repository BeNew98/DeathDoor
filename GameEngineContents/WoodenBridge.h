#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class WoodenBridge : public GameEngineActor
{
public:
	// constrcuter destructer
	WoodenBridge();
	~WoodenBridge();

	// delete Function
	WoodenBridge(const WoodenBridge& _Other) = delete;
	WoodenBridge(WoodenBridge&& _Other) noexcept = delete;
	WoodenBridge& operator=(const WoodenBridge& _Other) = delete;
	WoodenBridge& operator=(WoodenBridge&& _Other) noexcept = delete;

	inline std::shared_ptr<class ContentFBXRenderer> GetRender()
	{
		return m_pRenderer;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void InitComponent();

	std::shared_ptr<class ContentFBXRenderer> m_pRenderer = nullptr;
};
