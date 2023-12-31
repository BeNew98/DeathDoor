#include "PrecompileHeader.h"
#include "MiniDesk.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"

MiniDesk::MiniDesk()
{
}

MiniDesk::~MiniDesk()
{
	
}

void MiniDesk::Start()
{
	InitComponent();
}

void MiniDesk::Update(float _DeltaTime)
{
}

void MiniDesk::InitComponent()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("desk.fbx", "ContentMeshDeffered");
	m_pRenderer->Off();

	float4 MeshScale = m_pRenderer->GetMeshScale();
	MeshScale.z *= 0.7f;
	// MeshScale *= 0.75f;
	// MeshScale *= 2.2f;

	m_pComp = CreateComponent<PhysXBoxComponent>();
	m_pComp->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pComp->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
}
