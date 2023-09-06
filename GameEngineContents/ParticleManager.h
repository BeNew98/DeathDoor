#pragma once
#include "ParticleRenderer.h"
#include "ParticleStruct.h"

#include <GameEngineCore/GameEngineActor.h>

class ParticleManager : public GameEngineActor
{

public:

	ParticleManager();
	~ParticleManager();

	ParticleManager(const ParticleManager& _Other) = delete;
	ParticleManager(ParticleManager&& _Other) noexcept = delete;
	ParticleManager& operator=(const ParticleManager& _Other) = delete;
	ParticleManager& operator=(ParticleManager&& _Other) noexcept = delete;

	void SetMeshName(const std::string_view _Name)
	{
		MeshName = _Name;
	}

	void ParticleOn()
	{
		isOn = true;
	}

	void ParticleOff()
	{
		isOn = false;
	}

	void SetDisposable()
	{
		isDisposable = true;
	}

protected:
	float EmitCount = 0.0f;
	//������ƼŬ����
	int CurParticle = 0;
	//���ݱ��� ������� �� ����
	int ParticleCount = 0;

	bool isOn = false;
	bool isDisposable = false;
	bool isFull = false;

	std::string_view MeshName;
	std::vector<std::shared_ptr<ParticleRenderer>> ParticleList;
private:
};

