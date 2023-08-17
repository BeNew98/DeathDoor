#include "PreCompileHeader.h"
#include "ProcessMapInfo.h"
#include <fstream>
ProcessMapInfo* ProcessMapInfo::RWProcessInst = nullptr;

ProcessMapInfo::ProcessMapInfo()
{
	RWProcessInst = this;
}

ProcessMapInfo::~ProcessMapInfo()
{
}

std::vector<SponeMapActor> ProcessMapInfo::OpenFile(GameEnginePath _Path)
{
	std::vector<SponeMapActor> NewActorStructs;

	std::ifstream ifs;
	ifs.open(_Path.GetFullPath(), std::ios_base::in | std::ios::binary);
	if (ifs.fail())
	{
		MsgAssert("������ �дµ� �����Ͽ����ϴ�");
		ifs.close();
		return NewActorStructs;
	}



	while (!ifs.eof()) {
		SponeMapActor NewActor;
		ifs.read((char*)&NewActor.ActorIndex, sizeof(int));
		ifs.read((char*)&NewActor.ActorType, sizeof(int));
		ifs.read((char*)&NewActor.ActorOrder, sizeof(int));
		ifs.read((char*)&NewActor.LocScale, sizeof(float4::ZERO));
		ifs.read((char*)&NewActor.LocRot, sizeof(float4::ZERO));
		ifs.read((char*)&NewActor.LocPos, sizeof(float4::ZERO));
		ifs.read((char*)&NewActor.ScaleRatio, sizeof(float));
		ifs.read((char*)&NewActor.IsMoveable, sizeof(bool));
		ifs.read((char*)&NewActor.FBXNameLen, sizeof(size_t));

		int Len = static_cast<int>(NewActor.FBXNameLen);
		if (Len <= 0)
		{
			break;
		}
		char* temp = new char[Len + 1];
		ifs.read(temp, Len);
		temp[Len] = '\0';
		NewActor.FBXName = temp;

		NewActorStructs.push_back(NewActor);
		delete[] temp;
	}

	ifs.close();
	return NewActorStructs;
}


void ProcessMapInfo::WriteFile(GameEnginePath _Path, const SponeMapActor& _Value)
{
	std::ofstream ofs;
	ofs.open(_Path.GetFullPath(), std::ios::out | std::ios::app | std::ios::binary);
	if (ofs.fail())
	{
		MsgAssert("������ ���µ� �����Ͽ����ϴ�");
		ofs.close();
		return;
	}
	ofs.write((char*)&_Value.ActorIndex, sizeof(int));
	ofs.write((char*)&_Value.ActorType, sizeof(int));
	ofs.write((char*)&_Value.ActorOrder, sizeof(int));
	ofs.write((char*)&_Value.LocScale, sizeof(float4::ZERO));
	ofs.write((char*)&_Value.LocRot, sizeof(float4::ZERO));
	ofs.write((char*)&_Value.LocPos, sizeof(float4::ZERO));
	ofs.write((char*)&_Value.ScaleRatio, sizeof(float));
	ofs.write((char*)&_Value.IsMoveable, sizeof(bool));
	ofs.write((char*)&_Value.FBXNameLen, sizeof(size_t));
	ofs.write(_Value.FBXName.c_str(), _Value.FBXNameLen);

	//ofs.write((char*)&_Value, sizeof(struct SponeMapActor));
	ofs.close();
	return;
}

void ProcessMapInfo::CpyAndClear(GameEnginePath _Path)
{

	std::ifstream in;
	in.open(_Path.GetFullPath(), std::ios_base::in | std::ios::binary);
	if (in.fail())
	{
		MsgAssert("������ �дµ� �����Ͽ����ϴ�");
		in.close();
		return;
	}
	// ������ ��ü ũ�⸦ Ȯ���Ѵ�
	in.seekg(0, std::ios::end);
	int length = in.tellg();

	// ������ ��ü ũ�⸸ŭ �޸𸮿� �ε��Ѵ�
	in.seekg(0, std::ios::beg);
	char* buf = new char[length];
	in.read(buf, length);

	in.close();
	std::ofstream clearf;
	clearf.open(_Path.GetFullPath(), std::ios_base::out);
	clearf.close();

	// �޸𸮿� ����� ���� �����͸� �ٸ� ���Ͽ� �����Ѵ�
	std::ofstream out;
	std::string NewPath = GameEnginePath::GetFolderPath(_Path.GetFullPath()) + "Cpy" + _Path.GetFileName();
	out.open(NewPath, std::ios_base::out | std::ios::binary);
	if (out.fail())
	{
		MsgAssert("������ ���µ� �����Ͽ����ϴ�");
		out.close();
		return;
	}
	out.write(buf, length);

	out.close();
}

void ProcessMapInfo::CreatPathFile(GameEnginePath _Path)
{
	if (false == _Path.IsExists())
	{
		std::ofstream ofs;
		ofs.open(_Path.GetFullPath(), std::ios::out);
		ofs.close();
		return;
	}
}
