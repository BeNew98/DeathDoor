#pragma once

enum class OrderNum
{
	TEST
};


enum class PhysXFilterGroup
{
	Player = (1 << 0),		// �÷��̾�
	Ground = (1 << 1),		// �ٴ�
	Obstacle = (1 << 2),	// �浹ü(HitCallback�� ȣ���� �뵵)
	PlayerFace = (1 << 3),	// �浹ü(HitCallback�� ȣ���� �뵵)
	PlayerDynamic = (1 << 4),		// �÷��̾� ���̳����� ����
	PlayerHead = (1 << 5),		// �÷��̾� �Ӹ� ���̳����� ����
	GroundTrigger = (1 << 6),		// �� Ʈ����
};