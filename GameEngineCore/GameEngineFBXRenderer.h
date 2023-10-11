#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineFBXMesh.h"

struct AnimStartFunc
{
	bool bStart = true;
	std::function<void()> pFunc = nullptr;
};

class FbxExAniData;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class GameEngineFBXRenderer;
class GameEngineFBXAnimationInfo : public std::enable_shared_from_this<GameEngineFBXAnimationInfo>
{
public:
	GameEngineFBXRenderer* ParentRenderer;
	// SetFBX ���� ������ �ִ� �Ž�
	std::shared_ptr<GameEngineFBXMesh> Mesh;
	// �ִϸ��̼��� ������ �ִ� FBX
	std::shared_ptr<GameEngineFBXAnimation> Aniamtion;
	FbxExAniData* FBXAnimationData;

	std::vector<float> FrameTime;
	float PlayTime = 0.0f;
	float CurFrameTime = 0.0f;
	float Inter = 0.1f;

	std::vector<unsigned int> Frames;
	UINT CurFrame = 0;
	UINT Start = -1;
	UINT End = -1;

	bool bOnceStart = true;
	bool bOnceEnd = true;
	bool Loop = true;
	bool EndValue = false;

	float BlendIn = 0.2f;
	float BlendOut = 0.2f;

	std::map<UINT, AnimStartFunc> StartFunc;

	void Init(std::shared_ptr<GameEngineFBXMesh> _Mesh, std::shared_ptr<GameEngineFBXAnimation> _Animation, const std::string_view& _Name, int _Index);
	void Reset();
	bool IsEnd();
	void Update(float _DeltaTime);

public:
	GameEngineFBXAnimationInfo()
		: Start(0)
		, End(0)
		, bOnceStart(false)
		, bOnceEnd(false)
	{
	}

	~GameEngineFBXAnimationInfo()
	{
	}
};

class AnimationCreateParams
{
public:
	float Inter = 0.0f;
	bool Loop = true;
	int Start = -1;
	int End = -1;
	std::vector<float> FrameTime = std::vector<float>();
	float BlendIn = 0.02f;
	float BlendOut = 0.02f;
};


struct AnimationBoneData
{
public:
	int Index = -1;
	float4 Scale;
	float4 RotQuaternion;
	float4 Pos;
	float4 RotEuler;
};

struct AttachTransformInfo
{
	int Index = -1;
	GameEngineTransform* Transform;
	float4 OffsetPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 OffsetRot = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4x4 OffsetMat;
};

// ���� :
class GameEngineFBXRenderer : public GameEngineRenderer
{
	friend class GameEngineFBXAnimationInfo;

public:
	// constrcuter destructer
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();

	// delete Function
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

	virtual void SetFBXMesh(const std::string& _Name, std::string _Material);
	virtual void SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex);
	virtual std::shared_ptr<GameEngineRenderUnit> SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex, size_t _SubSetIndex);

	void SetAnimationStartFunc(const std::string_view& _Name, UINT _Index, std::function<void()> _Func);

	bool IsAnimationEnd()
	{
		if (nullptr == CurAnimation)
		{
			MsgAssert("CurAnimation�� nullptr�Դϴ�");
		}
		return CurAnimation->IsEnd();
	}

	std::map<std::pair<size_t, size_t>, std::shared_ptr<GameEngineRenderUnit>> GetUnTexturedUnit()
	{
		return UnTexturedUnit;
	}
	inline std::shared_ptr<GameEngineFBXMesh> GetFBXMesh()
	{
		return FBXMesh;
	}

	void CreateFBXAnimation(const std::string& _AnimationFBXName, const AnimationCreateParams& _Params, int _Index = 0)
	{
		CreateFBXAnimation(_AnimationFBXName, _AnimationFBXName, _Params, _Index);
	}

	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, const AnimationCreateParams& _Params = {0.05f,true}, int _Index = 0);

	std::vector<std::vector<std::shared_ptr<GameEngineRenderUnit>>>& GetAllRenderUnit()
	{
		return Unit;
	}

	void Update(float _DeltaTime) override;

	void PauseSwtich();

	inline void PauseOn()
	{
		Pause = true;
	}
	inline void PauseOff()
	{
		Pause = false;
	}

	void ChangeAnimation(const std::string& _AnimationName, bool _Force = false, float _BlendTime = -1.0f);

	AnimationBoneData GetBoneData(std::string _Name);

	AnimationBoneData GetBoneData(int _Index)
	{
		return AnimationBoneDatas[_Index];
	}

	void SetAttachTransform(std::string_view _Name, GameEngineTransform* _Transform, float4 _OffsetPos = float4(0.0f, 0.0f, 0.0f, 1.0f), float4 _OffsetRot = float4(0.0f, 0.0f, 0.0f, 0.0f));

	void SetAttachTransform(int Index, GameEngineTransform* _Transform, float4 _OffsetPos = float4(0.0f, 0.0f, 0.0f, 1.0f), float4 _OffsetRot = float4(0.0f, 0.0f, 0.0f, 0.0f));

	void CalculateUnitPos();

	float4 GetMeshScale();
protected:
	// void Render(float _DeltaTime) override;

private:
	bool Pause = false;

	// �װ� �Ұ����ϴ�.
	// ��ó�� �������� ���� �Ž��Ϳ����� �����Ǿ� �ִ� �Ž����߸� �����ϴ�.
	float4 ResultMeshScale = float4::ZERO;
	std::shared_ptr<GameEngineFBXMesh> FBXMesh;
	std::map<std::pair<size_t, size_t>, std::shared_ptr<GameEngineRenderUnit>> UnTexturedUnit;

	// vertexbuffer1       indexbuffer          
	// 0���Ž���            0��° �����
	std::vector<         std::vector<std::shared_ptr<GameEngineRenderUnit>>> Unit;

	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>> Animations;
	std::shared_ptr<GameEngineFBXAnimationInfo> CurAnimation = nullptr;

	// ó������ �׳� ��� �ִٰ�
	// �ִϸ��̼��� �Ǵ¼���
	// Ȯ��Ǿ�� �Ѵ�.
	// ������ ��ŭ
	// ���ʿ� Ű�� �Ž� ���� x �������� ���� ������ �ȴ�.

	// Structure Buffer�� ��ũ�� �Ǵ� �༮.
	std::vector<float4x4> AnimationBoneMatrixs;

	std::vector<float4x4> AnimationBoneMatrixsNotOffset;

	float BlendTime = 0.0f; // 0.2
	float CurBlendTime = 0.0f; // 0.2

	void UpdateBlend(float _DeltaTime);

	std::vector <AnimationBoneData> PrevAnimationBoneDatas;

	std::vector<AnimationBoneData> AnimationBoneDatas;

	std::vector<AttachTransformInfo> AttachTransformValue;
};

