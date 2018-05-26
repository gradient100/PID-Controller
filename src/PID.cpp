#include <limits>
#include <cmath>
#include <iostream>
#include "PID.h"



using namespace std;


PID::PID() 
{
	_twiddleOn = false;
}

PID::PID(bool twiddleOn) 
{
	_twiddleOn = twiddleOn;
}


PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) 
{
	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;

	_p_error = 0;
	_i_error = 0;
	_d_error = 0;

	_p.push_back(_Kp);
	_p.push_back(_Ki);
	_p.push_back(_Kd);
	
	_dp.push_back(_Kp*0.1);
	_dp.push_back(_Ki*0.1);
	_dp.push_back(_Kd*0.1);

	_runCTE = 0;
	_bestCTE = numeric_limits<double>::max();
	_bestP = Kp;
	_bestI = Ki;
	_bestD = Kd;
	_bestStep = 0;
	_twiddleStatus = TRYING_INCREASE;
	_paramIndex = 0;
	_step =0;

	_stepsPerRun = 1500;

	_crashStep = -1;
	_MAXERROR = 700;

	
}	

void PID::twiddle(double cte)
{		
	cout << "Param Index : " << _paramIndex << "\t" << "Twiddle status : "<<_twiddleStatus << endl;
	

	
	if (endRun())
	{	
		if (_runCTE<=_bestCTE) // whatever params were, they were great! Reward!
		{
			_bestCTE = _runCTE;
			_bestStep = _step;
			_bestP = _Kp;
			_bestI = _Ki;
			_bestD = _Kd;

			if (!firstRun())
			{						
				_dp[_paramIndex] *=1.1;
				_paramIndex = ++_paramIndex%3;
				_p[_paramIndex] += _dp[_paramIndex];
				_twiddleStatus = TRYING_INCREASE;				
				
			}
			else
				_p[_paramIndex] += _dp[_paramIndex]; // after baseline determined, start twiddle!
		
		}
		else if (_twiddleStatus==TRYING_INCREASE) // just tried increasing param, but no luck, try decreasing now
		{
			_p[_paramIndex] -=   2* _dp[_paramIndex];
			_twiddleStatus = TRYING_DECREASE;

		}
		else if (_twiddleStatus==TRYING_DECREASE) // both in- and de-creasing params was worse.  Life is hard!
		{
			 _p[_paramIndex] += _dp[_paramIndex];
			 _dp[_paramIndex] *= 0.9;
             _paramIndex = ++_paramIndex%3;
             _p[_paramIndex] += _dp[_paramIndex];
             _twiddleStatus = TRYING_INCREASE;
		}

		// member vars need to record parameter updates
		_Kp = _p[0];
		_Ki = _p[1];
		_Kd = _p[2];	


		
	}



}

void PID::UpdateError(double cte) 
{
    

    _step++;
    if (firstRun()) 
    	_d_error = 0;
	else 
		_d_error = cte-_p_error;
	_p_error = cte;
	_i_error += cte;

	cout << "Step : " << _step << "\t" << "Run " << _step/_stepsPerRun +1 << ": " << _step%_stepsPerRun/(double)_stepsPerRun*100 << "%" << endl;
	cout << "P : " << _Kp << endl;
	cout << "I : " << _Ki << endl;
	cout << "D : " << _Kd << endl;

	if (startRun())
	{
		_runCTE = 0;
		_i_error = 0;
	}
	else 
		_runCTE += abs(cte);

	
	if (_twiddleOn)
		twiddle(cte);
	else
	{


			
		if (endRun())
			if (_runCTE<=_bestCTE) 
			{
				_bestCTE = _runCTE;	
				_bestStep = _step;			
			}
	}
	cout << "Best_CTE : " << _bestCTE << "\t" << "Best_Run : " << _bestStep/_stepsPerRun << "\t" 
		 << "p : " << _bestP <<  ",\t"  
		 << "i : " << _bestI << ",\t"
		 << "d : " << _bestD << endl;
	cout << "Run CTE : " << _runCTE << endl;

	
	if (_runCTE > _MAXERROR && _crashStep == -1)
			_crashStep = _step;
	if (_crashStep > 0)
		cout << "Car crashed at Step : " << _crashStep << "\t" << "Run : " << _crashStep/_stepsPerRun +1  <<endl;
	
	
	


}

double PID::TotalError() 
{
	return _Kp * _p_error + _Kd * _d_error + _Ki * _i_error;
}

bool PID::startRun()
{
	return (_step % _stepsPerRun == 1); 
}

bool PID::endRun()
{
	return (_step % _stepsPerRun == 0); 
}

bool PID::firstRun()
{
	return (_step == _stepsPerRun); 
}

