#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class PinActor : public GameEngineActor
{
public:
	// constrcuter destructer
	PinActor();
	~PinActor();

	// delete Function
	PinActor(const PinActor& _Other) = delete;
	PinActor(PinActor&& _Other) noexcept = delete;
	PinActor& operator=(const PinActor& _Other) = delete;
	PinActor& operator=(PinActor&& _Other) noexcept = delete;

protected:

private:
	
};

