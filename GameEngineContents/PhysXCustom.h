#pragma once


physx::PxFilterFlags CustomFilterShader
(
	physx::PxFilterObjectAttributes attributes0,
	physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1,
	physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags,
	const void* constantBlock,
	physx::PxU32 constantBlockSize
);

// callback �̺�Ʈ�� �������� Ŭ����
class CustomSimulationEventCallback : public physx::PxSimulationEventCallback, public PhysXDefault
{
public:
	void SetPlayer(std::shared_ptr<class Player> _Player)
	{
		// �÷��̾��� ������ �� ���̳����� �����Ѵ�.
	}

	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;

	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {};
	void onWake(physx::PxActor** actors, physx::PxU32 count) override {};
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override {};
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
	void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {};


private:
};





class CustomErrorCallback : public physx::PxErrorCallback
{
private:
	void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
	{
		std::string Code = "Code : " + std::to_string(code)
			+ "\nMsg : " + message
			+ "\nFile : " + file
			+ "\nLine : " + std::to_string(line);
		MsgAssert(Code);
	}
};