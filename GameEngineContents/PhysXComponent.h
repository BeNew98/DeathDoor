#pragma once
#include <GameEngineCore/GameEngineComponent.h>
#include "PhysXDefault.h"

// ���� :
class PhysXComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXComponent();
	~PhysXComponent();

	// delete Function
	PhysXComponent(const PhysXComponent& _Other) = delete;
	PhysXComponent(PhysXComponent&& _Other) noexcept = delete;
	PhysXComponent& operator=(const PhysXComponent& _Other) = delete;
	PhysXComponent& operator=(PhysXComponent&& _Other) noexcept = delete;

	virtual void CreatePhysXActors(float4 _GeoMetryScale = float4(2.0f, 2.0f, 2.0f), float4 _GeoMetryRot = float4::ZERO, bool _Static = false) {};
	virtual void CreatePhysXActors(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO, bool _Static = false) {};


protected:

private:

};

