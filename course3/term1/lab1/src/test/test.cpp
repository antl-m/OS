#include <iostream>
#include <type_traits>
#include <csignal>
#include "trialfuncs.hpp"
#include "Calculator.h"

using namespace os::lab1::compfuncs;

static Calculator<bool, int> BoolCalc;
static Calculator<int, int> IntCalc;
static Calculator<double, int> DoubleCalc;

template <typename T, typename U>
void ProcessComputationResult(
    Calculator<T, U> & _Calc,
    typename Calculator<T, U>::ParamT _X
  )
{
  typename Calculator<T, U>::ValueT Result = _Calc.Compute(_X);
  if (_Calc.WasExceptionThrown())
  {
    std::cout << "Exception was thrown: " << _Calc.GetErrorString()
              << "\nCalculated result is: " << Result << std::endl;
  }
  else if (_Calc.WasCancelConfirmed())
  {
    std::cout << "Cancelation was confirmed.\nCalculated result is: " << Result << std::endl;
  }
  else
  {
    std::cout << "Result of calculations: " <<  Result << std::endl;
  }
  std::cout << "Time spent: " << _Calc.GetComputationTime() << "ms.\n" << std::endl;
  _Calc.Reset();
}

void BoolCancelCb(int)
{
  BoolCalc.OnCancelConfirmed();
  std::signal(SIGINT, &BoolCancelCb);
}

void IntCancelCb(int)
{
  IntCalc.OnCancelConfirmed();
  std::signal(SIGINT, &IntCancelCb);
}

void DoubleCancelCb(int)
{
  DoubleCalc.OnCancelConfirmed();
  std::signal(SIGINT, &DoubleCancelCb);
}



int main()
{
  std::signal(SIGINT, &BoolCancelCb);
  std::cout << "Calculation for op_group::AND:" << std::endl;
  BoolCalc.AddFunction(&trial_f<AND>);
  BoolCalc.AddFunction(&trial_g<AND>);
  BoolCalc.SetBinaryOp([](bool f, bool g){ return f && g; });
  std::cout << "Case 0:" << std::endl;
  ProcessComputationResult(BoolCalc, 0);
  std::cout << "Case 1:" << std::endl;
  ProcessComputationResult(BoolCalc, 1);

  BoolCalc.Clear();
  std::cout << "Calculation for op_group::OR:" << std::endl;
  BoolCalc.AddFunction(&trial_f<OR>);
  BoolCalc.AddFunction(&trial_g<OR>);
  BoolCalc.SetBinaryOp([](bool f, bool g){ return f || g; });
  std::cout << "Case 0:" << std::endl;
  ProcessComputationResult(BoolCalc, 0);
  std::cout << "Case 1:" << std::endl;
  ProcessComputationResult(BoolCalc, 1);

  std::signal(SIGINT, &IntCancelCb);
  std::cout << "Calculation for op_group::INT_SUM:" << std::endl;
  IntCalc.AddFunction(&trial_f<INT_SUM>);
  IntCalc.AddFunction(&trial_g<INT_SUM>);
  IntCalc.SetBinaryOp([](int f, int g){ return f + g; });
  std::cout << "Case 0:" << std::endl;
  ProcessComputationResult(IntCalc, 0);
  std::cout << "Case 1:" << std::endl;
  ProcessComputationResult(IntCalc, 1);

  std::signal(SIGINT, &DoubleCancelCb);
  std::cout << "Calculation for op_group::DOUBLE_MULT:" << std::endl;
  DoubleCalc.AddFunction(&trial_f<DOUBLE_MULT>);
  DoubleCalc.AddFunction(&trial_g<DOUBLE_MULT>);
  DoubleCalc.SetBinaryOp([](double f, double g){ return f * g; });
  std::cout << "Case 0:" << std::endl;
  ProcessComputationResult(DoubleCalc, 0);
  std::cout << "Case 1:" << std::endl;
  ProcessComputationResult(DoubleCalc, 1);
}
