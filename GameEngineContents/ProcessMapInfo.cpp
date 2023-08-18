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
		ifs.read((char*)&NewActor.MeshType, sizeof(int));
		ifs.read((char*)&NewActor.LocRot, sizeof(float4::ZERO));
		ifs.read((char*)&NewActor.LocPos, sizeof(float4::ZERO));
		ifs.read((char*)&NewActor.ScaleRatio, sizeof(float));
		ifs.read((char*)&NewActor.FBXNameLen, sizeof(size_t));
		int Len = static_cast<int>(NewActor.FBXNameLen);
		if (Len <= 0)
		{
			break;
		}
		char* tmp1 = new char[Len + 1];
		ifs.read(tmp1, Len);
		tmp1[Len] = '\0';
		NewActor.FBXName = tmp1;

		ifs.read((char*)&NewActor.MeterialLen, sizeof(size_t));
		Len = static_cast<int>(NewActor.MeterialLen);
		if (Len <= 0)
		{
			break;
		}
		char* tmp2 = new char[Len + 1];
		ifs.read(tmp2, Len);
		tmp2[Len] = '\0';
		NewActor.MeterialName = tmp2;

		NewActorStructs.push_back(NewActor);
		delete[] tmp1;
		delete[] tmp2;
	}

	ifs.close();
	return NewActorStructs;
}


void ProcessMapInfo::WriteAllFile(GameEnginePath _Path, std::map<int, SponeMapActor> _AllStruct)
{
	std::ofstream ofs;
	ofs.open(_Path.GetFullPath(), std::ios::out);
	for (std::pair<int, SponeMapActor> _StructInfo : _AllStruct)
	{
		WriteFile(_Path, _StructInfo.second);
	}
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
	//ofs.write((char*)&_Value.ActorIndex, sizeof(int));
	ofs.write((char*)&_Value.MeshType, sizeof(int));
	ofs.write((char*)&_Value.LocRot, sizeof(float4::ZERO));
	ofs.write((char*)&_Value.LocPos, sizeof(float4::ZERO));
	ofs.write((char*)&_Value.ScaleRatio, sizeof(float));
	ofs.write((char*)&_Value.FBXNameLen, sizeof(size_t));
	ofs.write(_Value.FBXName.c_str(), _Value.FBXNameLen);
	ofs.write((char*)&_Value.MeterialLen, sizeof(size_t));
	ofs.write(_Value.MeterialName.c_str(), _Value.MeterialLen);

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
	int length = static_cast<int>(in.tellg());

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

	delete[] buf;
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
