#ifndef _state_machine_
#define _state_machine_

class StateMachine
{
public:

	void ModeSet(int mode)
	{
		modeSet = mode;
	}
	
	void Run();
	
	int GetMode()
	{	return state; }
	
	void SetFaultState(bool b)
	{
		Abnormal = b;
	}
	int GetFaultState()
	{
		return Abnormal;
	}
protected:
	int 	modeSet;
	int 	state;
	int 	outputMode;
	bool	Abnormal;
};

#endif
