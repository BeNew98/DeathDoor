#pragma once
#include <GameEngineCore/GameEngineActor.h>


// ���� :
class CullingManager : public GameEngineActor
{
public:
	// constrcuter destructer
	CullingManager();
	~CullingManager();

	// delete Function
	CullingManager(const CullingManager& _Other) = delete;
	CullingManager(CullingManager&& _Other) noexcept = delete;
	CullingManager& operator=(const CullingManager& _Other) = delete;
	CullingManager& operator=(CullingManager&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void Culling();

	void LinkTrigger();
	void On_FirstTrigger();

	void CheckLink();
	std::vector<std::shared_ptr<class CullingTrigger>> m_vCullingTriggers = std::vector<std::shared_ptr<class CullingTrigger>>();
	std::vector<std::shared_ptr<class CullingObject>> m_vCullingObjects = std::vector<std::shared_ptr<class CullingObject>>();
};

// ��Ģ
// 1. ù��°Ʈ������ �ø�������Ʈ�� ���ۺ��� On ó�� ���ش�. 



// �ø��� ������ ������ 
// 1. ��ŸƮ���� CullingManager �� ���� ( �������� CullingManager��.. ����ִ´�? statc? ) 
// 2. �ø��� ������ ������ Ȥ��,���͸� �ø� �Ŵ����� ����
// 3. �ø� �Ŵ����� nullptr�� �ƴ϶��. ������ Update ���� �ø��Ŵ����� ������Ʈ �Լ� ȣ��
// 4. �ø��Ŵ����� ������Ʈ �Լ������� �ø����� �Լ� ȣ��

// �³�? �ƴϸ�. �ø��Ŵ����� static, ���� ������ �׻� �����. ���� ������ � �����̳Ŀ� ���� �ٸ��� ���� 