#include "Calculator.h"

#include <iostream>
#include <csignal>
#include <chrono>

using namespace std::chrono_literals;

Calculator<int> Calc;

void CancelCb(int)
{
  return Calc.OnCancelConfirmed();
}

int main(int argc, char ** argv)
{

  Calc.AddFunction([](int x){ std::this_thread::sleep_for(3s);  return x + 1; });
  Calc.AddFunction([](int x){ std::this_thread::sleep_for(2s); return x + 2; });
  Calc.AddFunction([](int x){ std::this_thread::sleep_for(1s); return x + 3; });
  Calc.AddFunction([](int x){ std::this_thread::sleep_for(5s); return x + 4; });
  Calc.SetBinaryOp([](int x, int y){ return x * y; });

  signal(SIGINT, &CancelCb);

  if (argc != 2)
  {
    std::cout << "Invalid input" << std::endl;
    return 0;
  }

  std::stringstream InputStream(argv[1]);

  int x = 0;
  if (InputStream >> x && InputStream.eof())
  {
    int Result = Calc.Compute(x);
    if (Calc.WasExceptionThrown())
    {
      std::cout << "Exception was thrown: " << Calc.GetErrorString()
                << "\nCalculated result is: " << Result << std::endl;
    }
    else if (Calc.WasCancelConfirmed())
    {
      std::cout << "Cancelation was confirmed.\nCalculated result is: " << Result << std::endl;
    }
    else
    {
      std::cout << "Result of calculations: " <<  Result << std::endl;
    }
    std::cout << "Time spent: " << Calc.GetComputationTime() << "ms." << std::endl;
  }
  else
    std::cout << "Invalid input" << std::endl;

  return 0;
}
