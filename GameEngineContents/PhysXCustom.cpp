#include "PreCompileHeader.h"
#include "PhysXCustom.h"
#include "Player.h"
#include "PhysXTestActor.h"
#include "PhysXTrigger.h"

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

		physx::PxShape* TriggerShape = current.triggerShape;
		physx::PxShape* OtherShape = current.otherShape;

		//�浹�� ���� Ȥ�� ����� ���Ͱ� null�̸� continue
		if (TriggerShape->userData == nullptr || OtherShape->userData == nullptr)
		{
			continue;
		}
		physx::PxFilterData TriggerFilterdata = TriggerShape->getSimulationFilterData();
		physx::PxFilterData OtherFilterdata = OtherShape->getSimulationFilterData();

		//�� �� �ϳ��� �浹 ���� ������ continue
		if (TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::None) ||
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::None))
		{
			continue;
		}

		//C26813  : ��Ʈ�÷��׷� ���� enum������ �񱳴� == �� �ƴ� bitwise and(&)�� ���ϴ� ���� ����
		//WARNING : resultFd.word0 == static_cast<physx::PxU32>(PhysXFilterGroup::Ground

		if (TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle) && // Ʈ������ ���ͱ׷��� ��ֹ��϶�
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic))// �浹�ѳ��� ���ͱ׷��� �÷��̾��϶�
		{
			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // ù �浹 ���� ��
			{

				PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
				PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
				int a = 0;
			}

			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // �浹�� ���� ��
			{

				PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
				PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
				int a = 0;
			}
		}

		if (TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::LeverTrigger) && // Ʈ���� ���� �׷� ����
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic))    // �浹�ѳ��� ���ͱ׷��� �÷��̾��϶�
		{
			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // ù �浹 ���� ��
			{

				PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
				PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
				int a = 0;
			}

			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // �浹�� ���� ��
			{

				PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
				PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
				int a = 0;
			}
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

		//�浹�� ���� Ȥ�� ����� ���Ͱ� null�̸� continue
		if (tmpContactActor->userData == nullptr|| tmpOtherActor->userData == nullptr)
		{
			continue;
		}
		physx::PxFilterData ContactFilterdata = tmpContactActor->getSimulationFilterData();
		physx::PxFilterData OtherFilterdata = tmpOtherActor->getSimulationFilterData();

		//�� �� �ϳ��� �浹 ���� ������ continue
		if (ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::None)|| 
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::None))
		{
			continue;
		}

		if (ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic)&&// �浹�� ���� ���ͱ׷��� �÷��̾��϶�
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Ground))		  // �浹�� ���� ���ͱ׷��� ���϶�
		{
			
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) //�浹�� ���۵� ����
			{
				//userData�� void* �̱⿡ PhysXComponent�� ������ ������ �����͸� �־� ����. �װ� ĳ�����Ͽ� �����.
				PhysXTestActor* Test = reinterpret_cast<PhysXTestActor*>(tmpContactActor->userData);
				int a = 0;
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) //�浹�� �����Ǵµ��� ��� ����
			{

				int a = 0;
				
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) //�浹�� ������ ����. �浹 ����(eNOTIFY_TOUCH_PERSISTS)�� ������ ������ üũ�� �ȵ�
			{				
				PhysXTestActor* Test = reinterpret_cast<PhysXTestActor*>(tmpContactActor->userData);
				int a = 0;
			}
		}

		if (ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic) &&// �浹�� ���� ���ͱ׷��� �÷��̾��϶�
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle))			// �浹�� ���� ���ͱ׷��� ��ֹ��϶�	
		{

			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) //�浹�� ���۵� ����
			{
				//userData�� void* �̱⿡ PhysXComponent�� ������ ������ �����͸� �־� ����. �װ� ĳ�����Ͽ� �����.
				PhysXTrigger* Test = reinterpret_cast<PhysXTrigger*>(tmpOtherActor->userData);
				int a = 0;
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) //�浹�� �����Ǵµ��� ��� ����
			{

				int a = 0;

			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) //�浹�� ������ ����. �浹 ����(eNOTIFY_TOUCH_PERSISTS)�� ������ ������ üũ�� �ȵ�
			{
				PhysXTrigger* Test = reinterpret_cast<PhysXTrigger*>(tmpOtherActor->userData);
				int a = 0;
			}
		}

	}
}
