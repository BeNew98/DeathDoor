#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class Map_NaviMesh : public GameEngineActor
{
public:
	static Map_NaviMesh* NaviMesh;
	// constrcuter destructer
	Map_NaviMesh();
	~Map_NaviMesh();

	// delete Function
	Map_NaviMesh(const Map_NaviMesh& _Other) = delete;
	Map_NaviMesh(Map_NaviMesh&& _Other) noexcept = delete;
	Map_NaviMesh& operator=(const Map_NaviMesh& _Other) = delete;
	Map_NaviMesh& operator=(Map_NaviMesh&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXTriangleComponent>GetPhysXComp()
	{
		return m_pNaviComp;
	}
protected:
	virtual void Start();
	virtual void Update(float _DeltaTime);

private:
	std::shared_ptr<class ContentFBXRenderer> m_pNaviRenderer = nullptr;
	std::shared_ptr<class ContentFBXRenderer> m_pWallRenderer = nullptr;
	std::shared_ptr<class PhysXTriangleComponent> m_pNaviComp = nullptr;


};

