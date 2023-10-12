#include "PreCompileHeader.h"
#include "FSMObjectBase.h"

FSMObjectBase::FSMObjectBase() 
{
}

FSMObjectBase::~FSMObjectBase() 
{
}

void FSMObjectBase::Update(float _DeltaTime)
{
	if (true == FSMFunc.empty())
	{
		return;
	}

	if (CurState != NextState || true == ChangeStateForce)  // state�� ����Ǵ� ���
	{
		if (FSMFunc.end() == FSMFunc.find(NextState))
		{
			MsgAssert("State�� �ش��ϴ� func�� �������� �ʾҽ��ϴ�");
		}

		if (nullptr != FSMFunc[CurState].End) // �� FSM�� End Function
		{
			FSMFunc[CurState].End();
		}

		if (nullptr != ChangeFSMCallBack) // FSM�� ����� �� �ϰ������� �����ϴ� ��
		{
			ChangeFSMCallBack();
		}

		if (nullptr != FSMFunc[NextState].Start) // �ٲ� FSM�� Start Function
		{
			StateDuration = 0.0f;
			StateChecker = false;

			FSMFunc[NextState].Start();
		}
		CurState = NextState;
		ChangeStateForce = false;
	}
	if (nullptr != FSMFunc[CurState].Update) // FSM Update
	{
		StateDuration += _DeltaTime;
		FSMFunc[CurState].Update(_DeltaTime);
	}
	isPhysXCollision = 0;
}


