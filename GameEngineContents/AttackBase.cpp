#include "PreCompileHeader.h"
#include "AttackBase.h"

#include "ContentFBXRenderer.h"

#include "PhysXCapsuleComponent.h"
#include "PhysXBoxComponent.h"
#include "PhysXSphereComponent.h"

AttackBase::AttackBase()
{
}

AttackBase::~AttackBase()
{
}


void AttackBase::Start()
{

}

void AttackBase::Update(float _DeltaTime)
{
	if (false == isShoot)
	{
		return;
	}
	if (true == CheckCollision(PhysXFilterGroup::MonsterDynamic) || GetLiveTime() > FireTime + 20.0f)
	{
		Death();
		return;
	}
	PhysXComp->GetDynamic()->setLinearVelocity({ 0,0,0 });
	PhysXComp->SetMoveSpeed(Dir * ShootSpeed);
	isPhysXCollision = 0;

}


void AttackBase::SetTrans(const float4& _Dir, const float4& _Pos)
{
	if (true == _Dir.IsZero())
	{
		MsgAssert("Zero dir�� �Է��� �� �����ϴ�");
		return;
	}

	if (nullptr == PhysXComp)
	{
		MsgAssert("PhysX Component�� �������� �ʾҽ��ϴ�.");
		return;
	}
	float4 CalRot = float4::ZERO;
	CalRot.y = float4::GetAngleVectorToVectorDeg360(float4::RIGHT, _Dir);
	PhysXComp->GetDynamic()->setGlobalPose(float4::PhysXTransformReturn(-CalRot, _Pos));
	Dir = _Dir;
	FireTime = GetLiveTime();
}
