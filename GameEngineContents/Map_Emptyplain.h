#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class Map_Emptyplain : public GameEngineActor
{
public:
	// constrcuter destructer
	Map_Emptyplain();
	~Map_Emptyplain();

	// delete Function
	Map_Emptyplain(const Map_Emptyplain& _Other) = delete;
	Map_Emptyplain(Map_Emptyplain&& _Other) noexcept = delete;
	Map_Emptyplain& operator=(const Map_Emptyplain& _Other) = delete;
	Map_Emptyplain& operator=(Map_Emptyplain&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void InitComponent();

	std::shared_ptr<class PhysXBoxComponent> m_pComp = nullptr;
	std::shared_ptr<GameEngineFBXRenderer> m_pRenderer = nullptr;
	std::shared_ptr<GameEngineFBXRenderer> m_pNaviRenderer = nullptr;
};
