#ifndef OSLAB1_CALCULATOR_H
#define OSLAB1_CALCULATOR_H

#include "Synchronized.h"

#include <latch>
#include <functional>

template<typename T>
class Calculator
{
public:

  using FunctionT = std::function<T(T)>;
  using BinaryOpT = std::function<T(T, T)>;

  T Compute(
      T x
    )
  {
    Synchronized<T> Result;
    m_LatchCount = m_Functions.size();
    m_WaitLatch.emplace(m_LatchCount);
    bool IsFirstComputed = true;

    auto StartPoint = std::chrono::steady_clock::now();
    for (const auto & Function : m_Functions)
    {
      std::thread Thread([&]{
        try
        {
          T PartialResult = Function(x);
          std::lock_guard Guard(Result.Mutex);
          Result.Value = (IsFirstComputed ? PartialResult : m_BinaryOp(Result.Value, PartialResult));
          IsFirstComputed = false;
          --m_LatchCount;
          m_WaitLatch->count_down();
        }
        catch (const std::exception & e)
        {
          std::lock_guard Guard(m_WasExceptionThrown.Mutex);
          if (!m_WasExceptionThrown.Value)
          {
            m_WasExceptionThrown.Value = true;
            m_ExceptionStr = e.what();
            m_WaitLatch->count_down(m_LatchCount);
          }
        }
      });
      Thread.detach();
    }
    m_WaitLatch->wait();
    auto Diff = std::chrono::steady_clock::now() - StartPoint;
    m_Time = std::chrono::duration_cast<std::chrono::milliseconds>(Diff).count();

    return Result.Value;
  }

  void AddFunction(
      const FunctionT & _Function
    )
  {
    m_Functions.push_back(_Function);
  }

  void SetBinaryOp(
      const BinaryOpT & _BinaryOp
    )
  {
    m_BinaryOp = _BinaryOp;
  }

  std::size_t GetComputationTime() const
  {
    return m_Time;
  }

  void OnCancelConfirmed()
  {
    std::lock_guard Guard(m_WasCancelConfirmed.Mutex);
    if (!m_WasCancelConfirmed.Value)
    {
      m_WasCancelConfirmed.Value = true;
      m_WaitLatch->count_down(m_LatchCount);
    }
  }

  bool WasCancelConfirmed() const
  {
    return m_WasCancelConfirmed.Value;
  }

  bool WasExceptionThrown() const
  {
    return m_WasExceptionThrown.Value;
  }

  const std::string & GetErrorString() const
  {
    return m_ExceptionStr;
  }

private:
  std::optional<std::latch> m_WaitLatch;
  std::atomic_ptrdiff_t m_LatchCount;
  std::vector<FunctionT> m_Functions;
  BinaryOpT m_BinaryOp;
  Synchronized<bool> m_WasCancelConfirmed;
  Synchronized<bool> m_WasExceptionThrown;
  std::string m_ExceptionStr;
  long long m_Time = 0;
};

#endif //OSLAB1_CALCULATOR_H
