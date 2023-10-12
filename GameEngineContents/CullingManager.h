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

	void SetCullingObject(std::vector<std::vector<std::shared_ptr<GameEngineActor>>>& _Objects);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void Culling();

	// ���⼭ �ø��ҳ���� �ڷᱸ���� ����, �ø��Լ����� �ø�..���ϴµ�.. ��� �����ҰŴ�.. 
	// ���ӿ������͸� �����ϴ� 2�����迭�� �����. �� ������ 0���ε������� CullingTrigger �� ����
	// �ø� Ʈ������ 
};



// �ø��� ������ ������ 
// 1. ��ŸƮ���� CullingManager �� ���� ( �������� CullingManager��.. ����ִ´�? statc? ) 
// 2. �ø��� ������ ������ Ȥ��,���͸� �ø� �Ŵ����� ����
// 3. �ø� �Ŵ����� nullptr�� �ƴ϶��. ������ Update ���� �ø��Ŵ����� ������Ʈ �Լ� ȣ��
// 4. �ø��Ŵ����� ������Ʈ �Լ������� �ø����� �Լ� ȣ��

// �³�? �ƴϸ�. �ø��Ŵ����� static, ���� ������ �׻� �����. ���� ������ � �����̳Ŀ� ���� �ٸ��� ���� 