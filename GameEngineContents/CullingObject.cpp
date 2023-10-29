#include "PrecompileHeader.h"
#include "CullingObject.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"

CullingObject::CullingObject()
{
}

CullingObject::~CullingObject()
{
}

void CullingObject::Start()
{
}

void CullingObject::Update(float _DeltaTime)
{
}

// ��� Start �� �ƴ϶�, CreateActor ���Ŀ� InitComponent �� ���� ȣ���Ͽ� �ʱ�ȭ �Ѵ�. 
void CullingObject::InitComponent(const std::string& _MeshName)
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh(_MeshName, "ContentMeshDeffered");
	m_pRenderer->Off();

	float4 MeshScale = m_pRenderer->GetMeshScale();

	/*m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
	m_pPhysXComponent->SetTrigger();
	m_pPhysXComponent->SetFilterData(PhysXFilterGroup::CullingObject);
	
	m_pPhysXComponent->SetPositionSetFromParentFlag(true);*/
}

