#pragma once
#include "ParticleRenderer.h"
#include "ParticleStruct.h"

#include <GameEngineCore/GameEngineActor.h>

/*
	DynamicParticleManager -> ���� ��ġ�� ��� ���ϴ� ��ƼŬ
	(�����̴� �÷��̾ ����ٴѴٰų� ���)
	�׷��� ��� ��ġ������ ����� Ʈ�������� �����ͷ� ����

	StaticParticleManager -> ������ ���� ���� �ڸ����� �Ѹ��� �ֵ�
	(���� ����Ŭ�̶�簡 ���)
	��� ��ġ������ ������ ����
*/

class ParticleManager : public GameEngineActor
{

public:

	ParticleManager();
	~ParticleManager();

	ParticleManager(const ParticleManager& _Other) = delete;
	ParticleManager(ParticleManager&& _Other) noexcept = delete;
	ParticleManager& operator=(const ParticleManager& _Other) = delete;
	ParticleManager& operator=(ParticleManager&& _Other) noexcept = delete;

	//��ƼŬ �Ž� ����
	void SetMeshName(const std::string_view _Name)
	{
		MeshName = _Name;
	}

	//��ƼŬ ������ �Ѵ� ��
	void ParticleOn()
	{
		isOn = true;
	}

	//��ƼŬ ������ �Ͻ������� ���ߴ� �� 
	void ParticleOff()
	{
		isOn = false;
	}

	//��ȸ������ ����
	//Start���� �� ������ �Ѱ�, ���� ����(MaxParticle) ��ŭ ������ �Ϸ�Ǹ� ���̻� �������� �ʰ� ���Ͱ� Death��.
	void SetDisposable()
	{
		isDisposable = true;
	}

protected:
	//���� ���ο��� ����ð��� ��� �뵵
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

