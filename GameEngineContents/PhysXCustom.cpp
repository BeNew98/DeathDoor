#include "PreCompileHeader.h"
#include "PhysXCustom.h"
#include "Player.h"
#include "PhysXTestActor.h"
#include "PhysXTrigger.h"
#include "PhysXControllerComponent.h"

physx::PxFilterFlags CustomFilterShader
(
	physx::PxFilterObjectAttributes attributes0,
	physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags,
	const void* constantBlock,
	physx::PxU32 constantBlockSize
)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);

	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST | physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT
		| physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

	return physx::PxFilterFlag::eDEFAULT;
}

void CustomSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	//트리거에 충돌이 일어났을때 어떤 걸 할 것인지
	while (count--)
	{
		physx::PxTriggerPair& current = *pairs++;

		physx::PxShape* TriggerShape = current.triggerShape; // 트리거의 모양
		physx::PxShape* OtherShape = current.otherShape;

		//충돌한 본인 혹은 상대의 액터가 null이면 continue
		if (TriggerShape->userData == nullptr || OtherShape->userData == nullptr)
		{
			continue;
		}
		physx::PxFilterData TriggerFilterdata = TriggerShape->getSimulationFilterData();
		physx::PxFilterData OtherFilterdata = OtherShape->getSimulationFilterData();

		//둘 중 하나라도 충돌 필터 없으면 continue
		if (0 == TriggerFilterdata.word0 /* & static_cast<physx::PxU32>(PhysXFilterGroup::None)*/ ||
			0 == OtherFilterdata.word0 /*& static_cast<physx::PxU32>(PhysXFilterGroup::None)*/)
		{
			continue;
		}
		// 실제 데이터가 있는 경우
		// 필터 두개를 make_pair
		

		if (GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(TriggerFilterdata.word0), static_cast<UINT>(OtherFilterdata.word0)))||
			GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(OtherFilterdata.word0), static_cast<UINT>(TriggerFilterdata.word0)))
			) // 두개의 충돌을 체크한다면
		{
			std::atomic_uint filterbit = (static_cast<UINT>(TriggerFilterdata.word0) | static_cast<UINT>(OtherFilterdata.word0));
			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // 첫 충돌 했을 때
			{

				GameEngineActor* TestTrigger = reinterpret_cast<GameEngineActor*>(TriggerShape->userData);
				GameEngineActor* TestActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
				TestTrigger->isPhysXCollision |= filterbit;
				TestActor->isPhysXCollision |= filterbit;
				int a = 0;
			}

			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // 충돌이 끝날 때
			{

				GameEngineActor* TestTrigger = reinterpret_cast<GameEngineActor*>(TriggerShape->userData);
				GameEngineActor* TestActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
				TestTrigger->isPhysXCollision = ~(~TestTrigger->isPhysXCollision | filterbit);
				TestActor->isPhysXCollision = ~(~TestActor->isPhysXCollision | filterbit);
				int a = 0;
			}
		}
		
		// 두개의 충돌을 체크한다면
		// 

		//CheckPhysXCollision 에서 찾아보고
		// 
		//있으면  true/ false 변경
		// 

		//C26813  : 비트플래그로 사용된 enum끼리의 비교는 == 이 아닌 bitwise and(&)로 비교하는 것이 좋음
		//WARNING : resultFd.word0 == static_cast<physx::PxU32>(PhysXFilterGroup::Ground


		//if (TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::LeverTrigger) && // 트리거 필터 그룹 레버
		//	OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic))    // 충돌한놈의 필터그룹이 플레이어일때
		//{
		//	if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // 첫 충돌 했을 때
		//	{
		//
		//		PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
		//		PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
		//		int a = 0;
		//	}
		//
		//	if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) // 충돌 유지시 계속 들어옴
		//	{
		//
		//		PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
		//		PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
		//		int a = 0;
		//	}
		//
		//	if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // 충돌이 끝날 때
		//	{
		//
		//		PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
		//		PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
		//		int a = 0;
		//	}
		//}
	}
}


void CustomSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	// 접촉이 일어났을때 필터를 가져와서 어떻게 처리할것인지

	while (nbPairs--)
	{
		physx::PxContactPair current = *pairs++;

		// 액터가 가지고 있는 쉐이프를 모두 가져옴
		physx::PxShape* ContactShape = current.shapes[0];
		physx::PxShape* OtherShape = current.shapes[1];

		//충돌한 본인 혹은 상대의 액터가 null이면 continue
		if (ContactShape->userData == nullptr|| OtherShape->userData == nullptr)
		{
			continue;
		}
		physx::PxFilterData ContactFilterdata = ContactShape->getSimulationFilterData();
		physx::PxFilterData OtherFilterdata = OtherShape->getSimulationFilterData();

		if (0 == ContactFilterdata.word0 /* & static_cast<physx::PxU32>(PhysXFilterGroup::None)*/ ||
			0 == OtherFilterdata.word0 /*& static_cast<physx::PxU32>(PhysXFilterGroup::None)*/)
		{
			continue;
		}

		// 실제 데이터가 있는 경우
		// 필터 두개를 make_pair
		if (GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(ContactFilterdata.word0), static_cast<UINT>(OtherFilterdata.word0))) ||
			GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(OtherFilterdata.word0), static_cast<UINT>(ContactFilterdata.word0)))
			) // 두개의 충돌을 체크한다면
		{
			std::atomic_uint filterbit = (static_cast<UINT>(ContactFilterdata.word0) | static_cast<UINT>(OtherFilterdata.word0));
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // 첫 충돌 했을 때
			{

				GameEngineActor* ContactActor = reinterpret_cast<GameEngineActor*>(ContactShape->userData);
				GameEngineActor* OtherActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
				ContactActor->isPhysXCollision |= filterbit;
				OtherActor->isPhysXCollision |= filterbit;
				int a = 0;
			}

			//if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) //충돌이 유지되는동안 계속 들어옴
			//{
			//
			//	GameEngineActor* TestTrigger = reinterpret_cast<GameEngineActor*>(ContactShape->userData);
			//	GameEngineActor* TestActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
			//	int a = 0;
			//
			//}

			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // 충돌이 끝날 때
			{

				GameEngineActor* ContactActor = reinterpret_cast<GameEngineActor*>(ContactShape->userData);
				GameEngineActor* OtherActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
				ContactActor->isPhysXCollision = ~(~ContactActor->isPhysXCollision | filterbit);
				OtherActor->isPhysXCollision = ~(~OtherActor->isPhysXCollision | filterbit);
				int a = 0;
			}
		}

	}
}

physx::PxControllerBehaviorFlags CustomCctBehaviorCallback::getBehaviorFlags(const physx::PxShape& shape, const physx::PxActor& actor)
{

	if (m_pOwnerComp.lock() == nullptr)
	{
		return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}
	float4 PlayerGroundPos = m_pOwnerComp.lock()->GetWorldPosition();
	float4 CollPoint = float4::ZERO;
	if (true == m_pOwnerComp.lock()->RayCast(PlayerGroundPos, float4::DOWN, CollPoint, 30.0f))
	{
		return physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}
	return physx::PxControllerBehaviorFlag::eCCT_SLIDE;


}

physx::PxQueryHitType::Enum CustomQueryFilterCallback::preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
{
	if (m_pOwnerComp.lock() == nullptr)
	{
		return physx::PxQueryHitType::eBLOCK;
	}
	float4 PlayerGroundPos = m_pOwnerComp.lock()->GetWorldPosition();
	PlayerGroundPos.y += 5.f;
	float4 CollPoint = float4::ZERO;
	if (false == m_pOwnerComp.lock()->RayCast(PlayerGroundPos, float4::DOWN, CollPoint, 100.0f)&& shape->getGeometryType() == physx::PxBoxGeometry().getType())
	{
		return physx::PxQueryHitType::eNONE;
	}
	else if ( shape->getGeometryType() == physx::PxTriangleMeshGeometry().getType() && false == m_bReturnValue)
	{
		return physx::PxQueryHitType::eNONE;
	}
	else
	{
		return physx::PxQueryHitType::eBLOCK;
	}

}

physx::PxQueryHitType::Enum CustomQueryFilterCallback::postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit)
{
	return physx::PxQueryHitType::eBLOCK;
}
