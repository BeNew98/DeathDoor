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

		physx::PxShape* TriggerShape = current.triggerShape; // Ʈ������ ���
		physx::PxShape* OtherShape = current.otherShape;

		//�浹�� ���� Ȥ�� ����� ���Ͱ� null�̸� continue
		if (TriggerShape->userData == nullptr || OtherShape->userData == nullptr)
		{
			continue;
		}
		physx::PxFilterData TriggerFilterdata = TriggerShape->getSimulationFilterData();
		physx::PxFilterData OtherFilterdata = OtherShape->getSimulationFilterData();

		//�� �� �ϳ��� �浹 ���� ������ continue
		if (0 == TriggerFilterdata.word0 /* & static_cast<physx::PxU32>(PhysXFilterGroup::None)*/ ||
			0 == OtherFilterdata.word0 /*& static_cast<physx::PxU32>(PhysXFilterGroup::None)*/)
		{
			continue;
		}
		// ���� �����Ͱ� �ִ� ���
		// ���� �ΰ��� make_pair
		

		if (GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(TriggerFilterdata.word0), static_cast<UINT>(OtherFilterdata.word0)))||
			GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(OtherFilterdata.word0), static_cast<UINT>(TriggerFilterdata.word0)))
			) // �ΰ��� �浹�� üũ�Ѵٸ�
		{
			std::atomic_uint filterbit = (static_cast<UINT>(TriggerFilterdata.word0) | static_cast<UINT>(OtherFilterdata.word0));
			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // ù �浹 ���� ��
			{

				GameEngineActor* TestTrigger = reinterpret_cast<GameEngineActor*>(TriggerShape->userData);
				GameEngineActor* TestActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
				TestTrigger->isPhysXCollision |= filterbit;
				TestActor->isPhysXCollision |= filterbit;
				int a = 0;
			}

			if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // �浹�� ���� ��
			{

				GameEngineActor* TestTrigger = reinterpret_cast<GameEngineActor*>(TriggerShape->userData);
				GameEngineActor* TestActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
				TestTrigger->isPhysXCollision = ~(~TestTrigger->isPhysXCollision | filterbit);
				TestActor->isPhysXCollision = ~(~TestActor->isPhysXCollision | filterbit);
				int a = 0;
			}
		}
		
		// �ΰ��� �浹�� üũ�Ѵٸ�
		// 

		//CheckPhysXCollision ���� ã�ƺ���
		// 
		//������  true/ false ����
		// 

		//C26813  : ��Ʈ�÷��׷� ���� enum������ �񱳴� == �� �ƴ� bitwise and(&)�� ���ϴ� ���� ����
		//WARNING : resultFd.word0 == static_cast<physx::PxU32>(PhysXFilterGroup::Ground


		//if (TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::LeverTrigger) && // Ʈ���� ���� �׷� ����
		//	OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic))    // �浹�ѳ��� ���ͱ׷��� �÷��̾��϶�
		//{
		//	if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // ù �浹 ���� ��
		//	{
		//
		//		PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
		//		PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
		//		int a = 0;
		//	}
		//
		//	if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) // �浹 ������ ��� ����
		//	{
		//
		//		PhysXTrigger* TestTrigger = reinterpret_cast<PhysXTrigger*>(TriggerShape->userData);
		//		PhysXTestActor* TestActor = reinterpret_cast<PhysXTestActor*>(OtherShape->userData);
		//		int a = 0;
		//	}
		//
		//	if (current.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // �浹�� ���� ��
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
	// ������ �Ͼ���� ���͸� �����ͼ� ��� ó���Ұ�����

	while (nbPairs--)
	{
		physx::PxContactPair current = *pairs++;

		// ���Ͱ� ������ �ִ� �������� ��� ������
		physx::PxShape* ContactShape = current.shapes[0];
		physx::PxShape* OtherShape = current.shapes[1];

		//�浹�� ���� Ȥ�� ����� ���Ͱ� null�̸� continue
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
		// ���� �����Ͱ� �ִ� ���
		// ���� �ΰ��� make_pair


		if (GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(ContactFilterdata.word0), static_cast<UINT>(OtherFilterdata.word0))) ||
			GlobalValue::PhysXCollision.end() != GlobalValue::PhysXCollision.find(std::make_pair(static_cast<UINT>(OtherFilterdata.word0), static_cast<UINT>(ContactFilterdata.word0)))
			) // �ΰ��� �浹�� üũ�Ѵٸ�
		{
			std::atomic_uint filterbit = (static_cast<UINT>(ContactFilterdata.word0) | static_cast<UINT>(OtherFilterdata.word0));
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) // ù �浹 ���� ��
			{

				GameEngineActor* ContactActor = reinterpret_cast<GameEngineActor*>(ContactShape->userData);
				GameEngineActor* OtherActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
				ContactActor->isPhysXCollision |= filterbit;
				OtherActor->isPhysXCollision |= filterbit;
				int a = 0;
			}

			//if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) //�浹�� �����Ǵµ��� ��� ����
			//{
			//
			//	GameEngineActor* TestTrigger = reinterpret_cast<GameEngineActor*>(ContactShape->userData);
			//	GameEngineActor* TestActor = reinterpret_cast<GameEngineActor*>(OtherShape->userData);
			//	int a = 0;
			//
			//}

			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) // �浹�� ���� ��
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
