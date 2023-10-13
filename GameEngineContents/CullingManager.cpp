#include "PrecompileHeader.h"
#include "CullingManager.h"

#include "FortressLevel.h"
#include "OfficeLevel.h"

#include "Map_Fortress.h"

#include "CullingObject.h"
#include "CullingTrigger.h"

#include "Player.h"

CullingManager::CullingManager()
{
}

CullingManager::~CullingManager()
{
}

void CullingManager::Start()
{
	ContentLevel* CurContentLevel = dynamic_cast<ContentLevel*>(GetLevel());
	if (nullptr == CurContentLevel)
	{
		MsgAssert("������ ���̳��� ĳ��Ʈ�� �����߽��ϴ�. ����");
		return;
	}

	ContentLevelType LevelType = CurContentLevel->GetContentLevelType();
	switch (LevelType)
	{
	case ContentLevelType::NONE:
	{
		MsgAssert("����������Ÿ���� NONE �Դϴ�. Level �� Start ���� Ÿ���� �������ּ���.");
		break;
	}
	case ContentLevelType::OfficeLevel:
	{

		break;
	}
	case ContentLevelType::FortressLevel:
	{
		std::shared_ptr<FortressLevel> CurLevel = CurContentLevel->DynamicThis<FortressLevel>();
		std::shared_ptr<Map_Fortress> CurMap = CurLevel->GetMap();

		m_vCullingTriggers = CurMap->GetCullingTrigger();
		m_vCullingObjects = CurMap->GetCullingObject();

		// ���⼭ Ʈ���ſ� ������Ʈ�� ��ũ��Ų��. 
		LinkTrigger();

		break;
	}
	case ContentLevelType::FrogBossLevel:
		break;
	default:
		break;
	}
}

void CullingManager::Update(float _DeltaTime)
{
	if (0.3f >= GetLiveTime())
	{
		On_FirstTrigger();

		size_t TriggerSize = m_vCullingTriggers.size();
		for (size_t i = 0; i < TriggerSize; ++i)
		{
			// ������Ʈ �������� �̷��� ������� 
			m_vCullingTriggers[i]->isPhysXCollision = 0;
		}

		
		return;
	}

	Culling();
}

void CullingManager::Culling()
{
	

	float4 PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
	
	size_t TriggerSize = m_vCullingTriggers.size();

	// ���⼭ Ʈ���� ��üũ�� ����
	for (size_t i = 0; i < TriggerSize; ++i)
	{
		// �÷��̾�� �浹�߰� 
		if (true == m_vCullingTriggers[i]->CheckCollision(PhysXFilterGroup::PlayerDynamic))
		{
			// Ʈ���Ű� �ߵ����� �ʾҴٸ� 
			if (false == m_vCullingTriggers[i]->IsActivate())
			{
				m_vCullingTriggers[i]->TriggerOn();
				m_vCullingTriggers[i]->On_CullingObject();

				// �׸��� �ٷ� �� Ʈ������ ù��° �ø� ������Ʈ�� Off ó��
				if (i > 0)
				{
					m_vCullingTriggers[i - 1]->Off_CullingObject();
				}
			}
			
		}

		// ������Ʈ �������� �̷��� ������� 
		m_vCullingTriggers[i]->isPhysXCollision = 0;
	}
}


void CullingManager::LinkTrigger()
{
	// ������Ʈ�� Ʈ���Ÿ� ��ũ�Ѵ�.
	size_t TriggersSize = m_vCullingTriggers.size();
	size_t ObjectsSize = m_vCullingObjects.size();

	if (TriggersSize <= static_cast<size_t>(0) || ObjectsSize <= static_cast<size_t>(0))
	{
		MsgAssert("������ ����� 0 �Դϴ�. Ʈ���ſ� ������Ʈ�� �ڷᱸ���� ����Ǿ����� Ȯ���ϼ���.");
		return;
	}

	// Ʈ���� �ϳ��� �ø� ������Ʈ�� ��ũ�ϰ� 
	// ex ) 1�� Ʈ���� : 1,2 �� �ø�������Ʈ , 2��Ʈ���� : 2,3 �� �ø�������Ʈ
	// Ʈ���� �۵��� ��ũ�Ǿ��ִ� �ø�������Ʈ�� �������� ON ��Ų��. 
	for (size_t i = 0; i < TriggersSize; ++i)
	{
		m_vCullingTriggers[i]->Set_CullingObject(m_vCullingObjects[i], m_vCullingObjects[i + 1]);
	}

	CheckLink();
}

void CullingManager::On_FirstTrigger()
{
	m_vCullingTriggers[0]->TriggerOn();
	m_vCullingTriggers[0]->On_CullingObject();
}

void CullingManager::CheckLink()
{
	std::vector<std::shared_ptr<CullingObject>>::iterator StartIter = m_vCullingObjects.begin();
	std::vector<std::shared_ptr<CullingObject>>::iterator EndIter = m_vCullingObjects.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		// �ϳ��� false ��� ����
		if (false == (*StartIter)->IsLink())
		{
			MsgAssert("Ʈ���ſ��� ��ũ�� ���������� ������� �ʾҽ��ϴ�.");
			return;
		}
	}
}

// �����ؾߵɰ�? 
// �ø��Ŵ������� ������ �ִ� Ʈ���ŵ��� �浹�˻縦 �����Ѵ�. 