#include "PrecompileHeader.h"
#include "Bench.h"

#include "PhysXBoxComponent.h"

#include "ContentFBXRenderer.h"

Bench::Bench()
{
}

Bench::~Bench()
{
}

void Bench::Start()
{
	InitComponent();
}

void Bench::Update(float _DeltaTime)
{
}

void Bench::InitComponent()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("bench.fbx", "ContentMeshDeffered");
	m_pRenderer->Off();

	float4 MeshScale = m_pRenderer->GetMeshScale();
	MeshScale.x *= 1.2f;

	m_pComp = CreateComponent<PhysXBoxComponent>();
	m_pComp->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pComp->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
}
