#pragma once

#include <GameEngineCore/GameEngineComponent.h>
#include "PhysXDefault.h"
struct sTriangle
{
    UINT ID = -1;
	UINT NearID[3];
	float4 CenterPos = float4::ZERONULL;
	float Cost = -1;
	float Heuristic = -1;
	float Value = -1;

	struct compare
	{
		bool operator()(sTriangle a, sTriangle b);
	};
};


// ���� :
class PhysXTriangleComponent : public GameEngineComponent, public PhysXDefault
{
	friend class compare;

public:
	// constrcuter destructer
	PhysXTriangleComponent();
	~PhysXTriangleComponent();

	// delete Function
	PhysXTriangleComponent(const PhysXTriangleComponent& _Other) = delete;
	PhysXTriangleComponent(PhysXTriangleComponent&& _Other) noexcept = delete;
	PhysXTriangleComponent& operator=(const PhysXTriangleComponent& _Other) = delete;
	PhysXTriangleComponent& operator=(PhysXTriangleComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(const std::string& _MeshName,bool _InverseIndex = true, float _Ratio = 1.f, float4 _GeoMetryRot = { 0.0f, 0.0f });
	physx::PxTriangleMesh* GetTriMesh()
	{
		return TriMesh;
	}
	void SetNavigation()
	{
		UINT iTriangleSize = TriMesh->getNbTriangles();
		for (UINT i = 0; i < iTriangleSize; i++)
		{
			sTriangle Temp;
			Temp.ID = i;
			Temp.NearID;
			physx::PxTriangle TriangleTemp;
			physx::PxU32 VertexIndex[3];
			physx::PxMeshQuery::getTriangle(TriMesh, m_pRigidStatic->getGlobalPose(), i, TriangleTemp, VertexIndex, Temp.NearID);
			float4 Vertex0 = float4(TriangleTemp.verts[0].x, TriangleTemp.verts[0].y, TriangleTemp.verts[0].z);
			float4 Vertex1 = float4(TriangleTemp.verts[1].x, TriangleTemp.verts[1].y, TriangleTemp.verts[1].z);
			float4 Vertex2 = float4(TriangleTemp.verts[2].x, TriangleTemp.verts[2].y, TriangleTemp.verts[2].z);
			float4 Result = float4(
				((Vertex0.x + Vertex1.x + Vertex2.x) / 3.f),
				((Vertex0.y + Vertex1.y + Vertex2.y) / 3.f),
				((Vertex0.z + Vertex1.z + Vertex2.z) / 3.f));
			Temp.CenterPos = Result;
			vec_TriangleNav.push_back(Temp);
		}
		int a = 0;
	}
	bool FindRoad(float4 _Start, float4 _End);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void CustomFBXLoad(const std::string& _MeshName,float _Ratio, bool _InverseIndex);

private:
	// Phys���� ������ �ʿ��� ����
	physx::PxCooking* m_pCooking = nullptr;
	bool m_bInit = false;

	std::vector<std::vector<physx::PxVec3>> VertexVec;
	std::vector<std::vector<physx::PxU32>> IndexVec;
	std::shared_ptr<class GameEngineFBXMesh> Mesh;
	physx::PxTriangleMesh* TriMesh = nullptr;

    std::vector<sTriangle> vec_TriangleNav;
	std::vector<sTriangle> vec_ResultRoad;

};
//
//#include <iostream>
//#include <vector>
//#include <queue>
//#include <cmath>
//#include <algorithm>
//
//using namespace std;
//
//// NavMesh�� Cell�� ��Ÿ���� ����ü
//struct Cell {
//    int id; // Cell�� ���� ��ȣ
//    vector<int> vertices; // Cell�� ���ؽ����� �ε���
//    vector<int> neighbors; // Cell�� �̿� Cell���� �ε���
//    double cost; // Cell�� �̵� ���
//    double heuristic; // Cell�� ������������ ���� ���
//    double f; // Cell�� �� ��� (cost + heuristic)
//    int parent; // Cell�� �θ� Cell�� �ε���
//};
//
//// NavMesh�� ��Ÿ���� Ŭ����
//class NavMesh 
//{
//private:
//    vector<Cell> cells; // Cell���� ���
//    vector<pair<double, double>> vertices; // ���ؽ����� ��ǥ ���
//    int start; // �������� ���� Cell�� �ε���
//    int goal; // �������� ���� Cell�� �ε���
//    vector<int> path; // �ִ� ��θ� ������ ����
//
//public:
//    // NavMesh�� ������
//    NavMesh(vector<Cell> cells, vector<pair<double, double>> vertices, pair<double, double> start, pair<double, double> goal) {
//        this->cells = cells;
//        this->vertices = vertices;
//        this->start = findCell(start); // �������� ���� Cell�� ã�´�
//        this->goal = findCell(goal); // �������� ���� Cell�� ã�´�
//        this->path.clear(); // ��θ� ����
//    }
//
//    // �־��� ��ǥ�� ���� Cell�� �ε����� ��ȯ�ϴ� �Լ�
//    int findCell(pair<double, double> point) {
//        for (int i = 0; i < cells.size(); i++) { // ��� Cell�� ���� �ݺ��Ѵ�
//            bool inside = true; // point�� Cell �ȿ� �ִ��� ����
//            for (int j = 0; j < cells[i].vertices.size(); j++) { // Cell�� ��� ���ؽ��� ���� �ݺ��Ѵ�
//                int k = (j + 1) % cells[i].vertices.size(); // ���� ���ؽ��� �ε���
//                // point�� Cell�� ������ �̷�� �� ���ؽ��� �������� ������ ���Ѵ�
//                double cross = (point.first - vertices[cells[i].vertices[j]].first) * (vertices[cells[i].vertices[k]].second - vertices[cells[i].vertices[j]].second)
//                    - (point.second - vertices[cells[i].vertices[j]].second) * (vertices[cells[i].vertices[k]].first - vertices[cells[i].vertices[j]].first);
//                if (cross < 0) { // ������ ������� point�� Cell�� �ܺο� �ִ�
//                    inside = false;
//                    break;
//                }
//            }
//            if (inside) return i; // point�� Cell �ȿ� �ִٸ� �ش� Cell�� �ε����� ��ȯ�Ѵ�
//        }
//        return -1; // point�� � Cell���� ������ �ʴ´ٸ� -1�� ��ȯ�Ѵ�
//    }
//
//    // �� Cell ������ �Ÿ��� ��ȯ�ϴ� �Լ�
//    double getDistance(int a, int b) {
//        // �� Cell�� �߽����� ��ǥ�� ���Ѵ�
//        double ax = 0, ay = 0, bx = 0, by = 0;
//        for (int i = 0; i < cells[a].vertices.size(); i++) {
//            ax += vertices[cells[a].vertices[i]].first;
//            ay += vertices[cells[a].vertices[i]].second;
//        }
//        ax /= cells[a].vertices.size();
//        ay /= cells[a].vertices.size();
//        for (int i = 0; i < cells[b].vertices.size(); i++) {
//            bx += vertices[cells[b].vertices[i]].first;
//            by += vertices[cells[b].vertices[i]].second;
//        }
//        bx /= cells[b].vertices.size();
//        by /= cells[b].vertices.size();
//        // �� �߽��� ������ ��Ŭ���� �Ÿ��� ��ȯ�Ѵ�
//        return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
//    }
//
//    // A* �˰����� �̿��Ͽ� �ִ� ��θ� ã�� �Լ�
//    void findPath() 
//    {
//        // �켱���� ť�� �̿��Ͽ� ����� ���� Cell���� �湮�Ѵ�
//        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
//        // ���� Cell�� ���� �θ� �ʱ�ȭ�Ѵ�
//        cells[start].cost = 0;
//        cells[start].heuristic = getDistance(start, goal);
//        cells[start].f = cells[start].cost + cells[start].heuristic;
//        cells[start].parent = -1;
//        // ���� Cell�� �켱���� ť�� �ִ´�
//        pq.push({ cells[start].f, start });
//        // �켱���� ť�� �� ������ �ݺ��Ѵ�
//        while (!pq.empty()) {
//            // �켱���� ť���� ���� ����� ���� Cell�� ������
//            int last = pq.top().second;
//            pq.pop();
//            // ���� Cell�� �����ߴٸ� �ݺ��� �����Ѵ�
//            if (last == goal) break;
//            // ���� Cell�� ��� �̿� Cell�� ���� �ݺ��Ѵ�
//            for (int near : cells[last].neighbors) {
//                // �̿� Cell�� ���ο� ����� ����Ѵ�
//                double newCost = cells[last].cost + getDistance(last, next);
//                // �̿� Cell�� ����� ���ο� ��뺸�� ũ�ų� �ʱⰪ�̶�� �����Ѵ�
//                if (cells[near].cost > newCost || cells[near].cost == -1) {
//                    cells[near].cost = newCost;
//                    cells[near].heuristic = getDistance(near, goal);
//                    cells[near].f = cells[near].cost + cells[near].heuristic;
//                    cells[near].parent = current;
//                    // �̿� Cell�� �켱���� ť�� �ִ´�
//                    pq.push({ cells[next].f, next });
//                }
//            }
//        }
//        // ���� Cell���� �������Ͽ� ��θ� ���Ѵ�
//        int current = goal;
//        while (current != -1) {
//            path.push_back(current);
//            current = cells[current].parent;
//        }
//        // ��θ� �����´�
//        reverse(path.begin(), path.end());
//    }
//
//    // �ִ� ��θ� ����ϴ� �Լ�
//    void printPath() {
//        cout << "The shortest path is:" << endl;
//        for (int i = 0; i < path.size(); i++) {
//            cout << "Cell " << cells[path[i]].id;
//            if (i < path.size() - 1) cout << " -> ";
//            else cout << endl;
//        }
//    }
//};
//
//// ���� �Լ�
//int main() {
//    // NavMesh�� Cell���� �����Ѵ�
//    vector<Cell> cells;
//    cells.push_back({ 1, {0, 1, 2}, {1, 3}, -1, -1, -1, -1 }); // Cell 1
//    cells.push_back({ 2, {0, 2, 3}, {0, 2, 4}, -1, -1, -1, -1 }); // Cell 2
//    cells.push_back({ 3, {2, 3, 4}, {1, 3, 5}, -1, -1, -1, -1 }); // Cell 3
//    cells.push_back({ 4, {1, 2, 4, 5}, {0, 2, 4, 6}, -1, -1, -1, -1 }); // Cell 4
//    cells.push_back({ 5, {3, 4, 6}, {1, 3, 5, 7}, -1, -1, -1, -1 }); // Cell 5
//    cells.push_back({ 6, {4, 5, 7}, {2, 4, 6, 8}, -1, -1, -1, -1 }); // Cell 6
//    cells.push_back({ 7, {2, 4, 5, 8}, {3, 5, 7, 9}, -1, -1, -1, -1 }); // Cell 7
//    cells.push_back({ 8, {5, 6, 9}, {4, 6, 8, 10}, -1, -1, -1, -1 }); // Cell 8
//    cells.push_back({ 9, {4, 5, 6, 9}, {5, 7, 9, 11}, -1, -1, -1, -1 }); // Cell 9
//    cells.push_back({ 10, {6, 7, 10}, {6, 8, 10, 12}, -1, -1, -1, -1 }); // Cell 10
//    cells.push_back({ 11, {5, 6, 7, 10}, {7, 9, 11, 13}, -1, -1, -1, -1 }); // Cell 11
//    cells.push_back({ 12, {7, 8, 11}, {8, 10, 12, 14}, -1, -1, -1, -1 }); // Cell 12
//    cells.push_back({ 13, {6, 7, 8, 11}, {9, 11, 13, 15}, -1, -1, -1, -1 }); // Cell 13
//    cells.push_back({ 14, {8, 9, 12}, {10, 12, 14, 16}, -1, -1, -1, -1 }); // Cell 14
//    cells.push_back({ 15, {7, 8, 9, 12}, {11, 13, 15, 17}, -1, -1, -1, -1 }); // Cell 15
//    cells.push_back({ 16, {9, 10, 13}, {12, 14, 16, 18}, -1, -1, -1, -1 }); // Cell 16
//    cells.push_back({ 17, {8, 9, 10, 13}, {13, 15, 17, 19}, -1, -1, -1, -1 }); // Cell 17
//    cells.push_back({ 18, {10, 11, 14}, {14, 16, 18}, -1, -1, -1, -1 }); // Cell 18
//    cells.push_back({ 19, {9, 10, 11, 14}, {15, 17, 19}, -1, -1, -1, -1 }); // Cell 19
//
//    // NavMesh�� ���ؽ����� ��ǥ�� �����Ѵ�
//    vector<pair<double, double>> vertices;
//    vertices.push_back({ 0, 0 }); // Vertex 0
//    vertices.push_back({ 0, 5 }); // Vertex 1
//    vertices.push_back({ 5, 0 }); // Vertex 2
//    vertices.push_back({ 5, 5 }); // Vertex 3
//    vertices.push_back({ 10, 0 }); // Vertex 4
//    vertices.push_back({ 10, 5 }); // Vertex 5
//    vertices.push_back({ 15, 0 }); // Vertex 6
//    vertices.push_back({ 15, 5 }); // Vertex 7
//    vertices.push_back({ 20, 0 }); // Vertex 8
//    vertices.push_back({ 20, 5 }); // Vertex 9
//    vertices.push_back({ 25, 0 }); // Vertex 10
//    vertices.push_back({ 25, 5 }); // Vertex 11
//
//    // �������� �������� ��ǥ�� �����Ѵ�
//    pair<double, double> start = { 2, 2 }; // ������
//    pair<double, double> goal = { 23, 3 }; // ������
//
//    // NavMesh ��ü�� �����Ѵ�
//    NavMesh navmesh(cells, vertices, start, goal);
//
//    // �ִ� ��θ� ã�´�
//    navmesh.findPath();
//
//    // �ִ� ��θ� ����Ѵ�
//    navmesh.printPath();
//
//    return 0;
//}
