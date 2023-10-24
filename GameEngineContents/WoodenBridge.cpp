#include "PrecompileHeader.h"
#include "WoodenBridge.h"

#include "ContentFBXRenderer.h"

WoodenBridge::WoodenBridge()
{
}

WoodenBridge::~WoodenBridge()
{
}

void WoodenBridge::Start()
{
	InitComponent();
}

void WoodenBridge::Update(float _DeltaTime)
{
}

void WoodenBridge::InitComponent()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("WoodenBridge.fbx", "ContentMeshDeffered");
}
