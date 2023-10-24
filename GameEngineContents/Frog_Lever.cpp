#include "PrecompileHeader.h"
#include "Frog_Lever.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"


Frog_Lever::Frog_Lever()
{
}

Frog_Lever::~Frog_Lever()
{
}

void Frog_Lever::Start()
{
	InitComponent();
}

void Frog_Lever::Update(float _DeltaTime)
{
	bool b = CheckCollision(PhysXFilterGroup::PlayerDynamic);
	if (b == true)
	{
		int a = 0;
	}

	if (b == false)
	{
		int a = 0;
	}

	//조건을 IsDown, CheckCollision , m_Trigger의 null체크 
	if (m_TriggerFunc!= nullptr)
	{
		m_TriggerFunc();
	}
}

void Frog_Lever::InitComponent()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->GetTransform()->SetLocalScale(float4{ 100, 100, 100 });
	m_pRenderer->SetFBXMesh("LEVER_MESH.FBX", "ContentAniMeshDeffered");
	m_pRenderer->CreateFBXAnimation("Lever_Open", "LEVER_OPEN (1).FBX", { 1.f/30.f, false });
	


	m_pRenderer->ChangeAnimation("Lever_Open");

	float4 MeshScale = m_pRenderer->GetMeshScale();
	MeshScale *= 100.0f;

	m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
	m_pPhysXComponent->SetFilterData(PhysXFilterGroup::Obstacle);

	MeshScale.y = 10.f;
	m_pPhysXComponent->CreateSubShape(SubShapeType::BOX, MeshScale* 3.f,float4(0,50,0));
	m_pPhysXComponent->SetSubShapeFilter(PhysXFilterGroup::LeverTrigger);
	m_pPhysXComponent->AttachShape();
}
