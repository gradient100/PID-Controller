#ifndef PID_H
#define PID_H

#include <vector>

using namespace std;

enum TwiddleStatuses {TRYING_INCREASE, TRYING_DECREASE};

class PID {
public:
  /*
  * Errors
  */
  double _p_error;
  double _i_error;
  double _d_error;

  /*
  * Coefficients
  */ 
  double _Kp;
  double _Ki;
  double _Kd;

  int _step;
  int _paramIndex;

  float _bestCTE;
  float _runCTE;
  float _bestStep;

  double _bestP;
  double _bestI;
  double _bestD;

  TwiddleStatuses _twiddleStatus;

  bool _twiddleOn;
  
  int _stepsPerRun;

  vector <double> _p;
  vector <double> _dp;

  int _MAXERROR;
  int _crashStep;


  /*
  * Constructor
  */
  PID();

  PID(bool twiddleOn);

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  void twiddle(double cte);

  
  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  bool startRun();
  bool endRun();
  bool firstRun();

};

#endif /* PID_H */
