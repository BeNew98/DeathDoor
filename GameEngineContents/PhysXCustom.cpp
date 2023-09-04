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
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

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
			TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic) &&
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
		if (current.contactPatches == 0)
		{
			continue;
		}
		// ���Ͱ� ������ �ִ� �������� ��� ������
		physx::PxShape* tmpContactActor = current.shapes[0];
		physx::PxShape* tmpOtherActor = current.shapes[1];
		physx::PxFilterData OtherFilterdata = tmpOtherActor->getSimulationFilterData();
		physx::PxFilterData ContactFilterdata = tmpContactActor->getSimulationFilterData();
		if (ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic)&&// �÷��̾�
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Ground))  //��			
		{
			
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) //�浹�� ���۵� ����
			{
				physx::PxRigidDynamic* pPlayer = static_cast<physx::PxRigidDynamic*>(tmpContactActor->getActor());
				int a = 0;
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) //�浹�� �����Ǵµ��� ��� ����
			{
				int a = 0;
				
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) //�浹�� ������ ����
			{
				int a = 0;
			}
		}
		if (ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic) &&// �÷��̾�
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle))  //��ֹ�		
		{
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) //�浹�� ���۵� ����
			{
				int a = 0;
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) //�浹�� �����Ǵµ��� ��� ����
			{
				int a = 0;

			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) //�浹�� ������ ����
			{
				int a = 0;
			}
		}

	}
}
