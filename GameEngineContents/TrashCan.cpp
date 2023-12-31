#include "PrecompileHeader.h"
#include "TrashCan.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"

TrashCan::TrashCan()
{
}

TrashCan::~TrashCan()
{
}

void TrashCan::Start()
{
	InitComponent();
}

void TrashCan::Update(float _DeltaTime)
{
}

void TrashCan::InitComponent()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("TrashCan.fbx", "ContentMeshDeffered");
	m_pRenderer->Off();

	float4 MeshScale = m_pRenderer->GetMeshScale();
	// MeshScale *= 2.6f;

	m_pComp = CreateComponent<PhysXBoxComponent>();
	m_pComp->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pComp->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
}
