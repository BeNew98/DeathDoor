#include "PrecompileHeader.h"
#include "CullingManager.h"

#include "FortressLevel.h"
#include "OfficeLevel.h"

#include "Map_Fortress.h"

CullingManager::CullingManager()
{
}

CullingManager::~CullingManager()
{
}

// ���ڷ� ~~ �� �ް� 
void CullingManager::SetCullingObject()
{
	
}

// ��ŸƮ���� ���� ������ ���� �ø��� �ʿ��� ������Ʈ�� �޾ƿͼ� ����, �����Ѵ�... 
// ��� �����Ұ��� 
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

		// �̷��� �����ͼ�
		// CurMap->GetCullingObject();
		// SetCullingObject(); <-- ���ڳ־ ȣ���ؼ� �Լ��ȿ��� �ø��ҳ༮���� �������ش�. 
		// �׸��� Culling �Լ����� �������� ��¼����¼��
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
	Culling();
}

void CullingManager::Culling()
{
	int a = 0;
}
