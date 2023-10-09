#pragma once
#include "ContentLevel.h"
// ���� :
class PlayerTestLevel : public ContentLevel
{
public:
	// constrcuter destructer
	PlayerTestLevel();
	~PlayerTestLevel();

	// delete Function
	PlayerTestLevel(const PlayerTestLevel& _Other) = delete;
	PlayerTestLevel(PlayerTestLevel&& _Other) noexcept = delete;
	PlayerTestLevel& operator=(const PlayerTestLevel& _Other) = delete;
	PlayerTestLevel& operator=(PlayerTestLevel&& _Other) noexcept = delete;

protected:
	
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:

};

