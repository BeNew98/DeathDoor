#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <functional>
#include <map>

// ���� :
class FSMObjectBase : public GameEngineActor
{
public:
	// constrcuter destructer
	FSMObjectBase();
	~FSMObjectBase();

	// delete Function
	FSMObjectBase(const FSMObjectBase& _Other) = delete;
	FSMObjectBase(FSMObjectBase&& _Other) noexcept = delete;
	FSMObjectBase& operator=(const FSMObjectBase& _Other) = delete;
	FSMObjectBase& operator=(FSMObjectBase&& _Other) noexcept = delete;

protected:

	// state
	template<typename Enumclass>
	inline void SetNextState(Enumclass _Num) //���� State ����
	{
		NextState = static_cast<int>(_Num);
	}

	template<typename Enumclass> // ���� State Get
	inline Enumclass GetCurState()
	{
		return static_cast<Enumclass>(CurState);
	}

	template<typename Enumclass>
	inline void InitFSM(Enumclass _Num) // FSM Function ����
	{
		for (int i = 0; i < static_cast<int>(_Num); ++i)
		{
			StateFunctionParameter NewStatePara;
			FSMFunc[i] = NewStatePara;
		}
	}

	// FSM Function ����
	template<typename Enumclass>
	inline void SetFSM(Enumclass _Num, std::function<void()> _Start, std::function<void(float _Delta)> _Update, std::function<void()> _End)
	{
		SetFSM(static_cast<int>(_Num), _Start, _Update, _End);
	}

	inline void SetFSM(int _Num, std::function<void()> _Start, std::function<void(float _Delta)> _Update, std::function<void()> _End)
	{
		FSMFunc[_Num].Start = _Start;
		FSMFunc[_Num].Update = _Update;
		FSMFunc[_Num].End = _End;

	}

	inline void SetChangeFSMCallBack(std::function<void()> _Func) // FSM�� �ٲ� �� ���� ���������� ����
	{
		ChangeFSMCallBack = _Func;
	}

	void Update(float _DeltaTime) override;
private:
	class StateFunctionParameter
	{
	public:
		std::function<void()> Start = nullptr;
		std::function<void(float _Delta)> Update = nullptr;
		std::function<void()> End = nullptr;
	};

	std::map<int, StateFunctionParameter> FSMFunc;
	std::function<void()> ChangeFSMCallBack = nullptr;
	int CurState = 0;
	int NextState = 0;

};

