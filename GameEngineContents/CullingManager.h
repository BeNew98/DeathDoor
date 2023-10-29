#pragma once
#include <GameEngineCore/GameEngineActor.h>


class ContentLevel;

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

	template<typename EnumType>
	void LinkTrigger(EnumType _LevelType);
	void On_FirstTrigger();

	void CheckLink();
	
	inline void Set_ActiveTrigger_Index(int _Index);

	int m_iCurTrigger_Idx = 0;
	
	inline void CullingObjIdxClear()
	{
		m_iCurCullingObj_Idx0 = -1;
		m_iCurCullingObj_Idx1 = -1;
		m_iCurCullingObj_Idx2 = -1;
		m_iCurCullingObj_Idx3 = -1;
	}

	void Off_Trigger();

	int m_iCurCullingObj_Idx0 = -1;
	int m_iCurCullingObj_Idx1 = -1;
	int m_iCurCullingObj_Idx2 = -1;
	int m_iCurCullingObj_Idx3 = -1;


	std::vector<std::shared_ptr<class CullingTrigger>> m_vCullingTriggers = std::vector<std::shared_ptr<class CullingTrigger>>();
	std::vector<std::shared_ptr<class CullingObject>> m_vCullingObjects = std::vector<std::shared_ptr<class CullingObject>>();
};

// 

// ��Ģ
// 1. ù��°Ʈ������ �ø�������Ʈ�� ���ۺ��� On ó�� ���ش�. 
// 2. �������� Ʈ���Ŵ� � ������Ʈ�� on ��ų������ �ϵ��ڵ����� �ۼ� 



// �ø��� ������ ������ 
// 1. ��ŸƮ���� CullingManager �� ���� ( �������� CullingManager��.. ����ִ´�? statc? ) 
// 2. �ø��� ������ ������ Ȥ��,���͸� �ø� �Ŵ����� ����
// 3. �ø� �Ŵ����� nullptr�� �ƴ϶��. ������ Update ���� �ø��Ŵ����� ������Ʈ �Լ� ȣ��
// 4. �ø��Ŵ����� ������Ʈ �Լ������� �ø����� �Լ� ȣ��

// �³�? �ƴϸ�. �ø��Ŵ����� static, ���� ������ �׻� �����. ���� ������ � �����̳Ŀ� ���� �ٸ��� ���� 