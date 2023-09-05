#include "PrecompileHeader.h"
#include "PhysXManager.h"

PhysXManager::PhysXManager() 
{
}

PhysXManager::~PhysXManager() 
{
}

// ������ ���������� ����
void PhysXManager::Simulate(float _DeltaTime)
{
	if (nullptr == m_pPhysics|| nullptr == m_pScene)
	{
		return;
	}

	m_pScene->simulate(_DeltaTime);
	m_pScene->fetchResults(true);
}


// �ʱ�ȭ
void PhysXManager::Init()
{
	if (nullptr!=m_pPhysics)
	{
		return;
	}

	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
	if (!m_pFoundation)
	{
		MsgAssert("PxFoundation failed!");
	}

	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	if (!m_pDispatcher)
	{
		MsgAssert("Dispatcher failed!");
	}

	m_pSimulationEventCallback = new CustomSimulationEventCallback();

#ifdef _DEBUG
	m_pPvd = PxCreatePvd(*m_pFoundation);
	if (!m_pPvd)
	{
		MsgAssert("PxPvd failed!");
	}

	m_pTransport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);

	if (!m_pTransport)
	{
		MsgAssert("Transport failed!");
	}

	bool bConnect = m_pPvd->connect(*m_pTransport, physx::PxPvdInstrumentationFlag::eALL);

#endif
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), true, m_pPvd);

	if (!m_pPhysics)
	{
		MsgAssert("PxPhysics failed!");
	}

	

	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxCookingParams(m_pPhysics->getTolerancesScale()));
	if (!m_pCooking)
	{
		MsgAssert("PxCreateCooking failed!");
	}
}

void PhysXManager::CreateScene(const std::string_view& _Name)
{
	std::string sUpperName = GameEngineString::ToUpper(_Name);

	if (AllScene.end() != AllScene.find(sUpperName))
	{
		return;
		MsgAssert("�̹� �ش� ������ ���� ��������ϴ�." + sUpperName);
	}

	physx::PxScene* pNewScene = nullptr;
	physx::PxSceneDesc SceneDesc(m_pPhysics->getTolerancesScale());

	SceneDesc.gravity = physx::PxVec3(0.f, -SCENE_GRAVITY, 0.0f);
	SceneDesc.cpuDispatcher = m_pDispatcher;
	SceneDesc.simulationEventCallback = m_pSimulationEventCallback;
	SceneDesc.filterShader = CustomFilterShader;

	pNewScene = m_pPhysics->createScene(SceneDesc);

	if (!pNewScene)
	{
		MsgAssert("PxScene failed!");
	}

	m_pPvdClient = pNewScene->getScenePvdClient();
	if (m_pPvdClient)
	{
		m_pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		m_pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		m_pPvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	AllScene.emplace(sUpperName, pNewScene);
	ChangeScene(sUpperName);
}

void PhysXManager::ChangeScene(const std::string_view& _Name)
{
	std::string sUpperName = GameEngineString::ToUpper(_Name);

	if (AllScene.end() == AllScene.find(sUpperName))
	{
		m_pScene = nullptr;
		return;
	}
	m_pScene = AllScene[sUpperName];
}




// �޸� ����
void PhysXManager::Release()
{
	if (nullptr != m_pSimulationEventCallback)
	{
		delete m_pSimulationEventCallback;
		m_pSimulationEventCallback = nullptr;
	}

	std::map<std::string, physx::PxScene*>::iterator SceneStartIter = AllScene.begin();
	std::map<std::string, physx::PxScene*>::iterator SceneEndIter = AllScene.end();
	while (SceneStartIter!= SceneEndIter)
	{
		physx::PxScene* pScene = SceneStartIter->second;

		PX_RELEASE(pScene);

		++SceneStartIter;
	}

	PX_RELEASE(m_pDispatcher);
	PX_RELEASE(m_pPhysics);
	PX_RELEASE(m_pCooking);
	if (m_pPvd)
	{
		m_pPvd->release();
		m_pPvd = nullptr;
		PX_RELEASE(m_pTransport);
	}
	PX_RELEASE(m_pFoundation);
}
