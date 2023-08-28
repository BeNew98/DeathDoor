#include "PreCompileHeader.h"
#include "PhysXCustom.h"

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
	// let triggers through
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		return physx::PxFilterFlag::eDEFAULT;
	}

	//generate contacts for all that were not filtered above
	pairFlags = physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_TOUCH_LOST |
		physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | physx::PxPairFlag::eSOLVE_CONTACT;

	return physx::PxFilterFlag::eDEFAULT;
}

void CustomSimulationEventCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	//Ʈ���ſ� �浹�� �Ͼ���� � �� �� ������
	while (count--)
	{
		physx::PxTriggerPair& current = *pairs++;

		// ���Ͱ� ������ �ִ� �������� ��� ������
		physx::PxRigidActor& tmpOtherActor = *current.otherActor;
		physx::PxFilterData OtherFilterdata = current.otherShape->getSimulationFilterData();
		physx::PxFilterData TriggerFilterdata = current.triggerShape->getSimulationFilterData();
		physx::PxU16 tmpnbShape = current.otherActor->getNbShapes();

		//C26813  : ��Ʈ�÷��׷� ���� enum������ �񱳴� == �� �ƴ� bitwise and(&)�� ���ϴ� ���� ����
		//WARNING : resultFd.word0 == static_cast<physx::PxU32>(PhysXFilterGroup::Ground

		if (OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle) &&
			TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerFace) &&
			current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			
			
		}

	}
}


void CustomSimulationEventCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	// ������ �Ͼ���� ���͸� �����ͼ� ��� ó���Ұ�����

	while (nbPairs--)
	{
		physx::PxContactPair current = *pairs++;
		// ���Ͱ� ������ �ִ� �������� ��� ������
		physx::PxShape* tmpContactActor = current.shapes[0];
		physx::PxShape* tmpOtherActor = current.shapes[1];
		physx::PxFilterData OtherFilterdata = tmpOtherActor->getSimulationFilterData();
		physx::PxFilterData ContactFilterdata = tmpContactActor->getSimulationFilterData();
		if (OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Ground) &&
			ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic))
		{

			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				int a = 0;
				
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				int a = 0;
			}

		}
		if (OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic) &&
			ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle) &&
			current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			int a = 0;
		}

		if (OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle) &&
			ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic) &&
			current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			int a = 0;
		}

	}
}
