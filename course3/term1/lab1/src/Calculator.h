#ifndef OSLAB1_CALCULATOR_H
#define OSLAB1_CALCULATOR_H

#include "Synchronized.h"
#include "ConditionVariable.h"
#include "compfuncs.hpp"

#include <latch>
#include <functional>

template<typename T, typename U = T>
class Calculator
{
public:

  using ParamT = U;
  using ValueT = typename os::lab1::compfuncs::op_group_type_traits<T>::value_type;
  using ResultT = typename os::lab1::compfuncs::op_group_type_traits<T>::result_type;

  using FunctionT = std::function<ResultT(ParamT)>;
  using BinaryOpT = std::function<ValueT(ValueT, ValueT)>;

  ValueT Compute(
      ParamT x
    )
  {
    Synchronized<T> Result;
    m_ConditionVariable.SetCounter(m_Functions.size());
    bool IsFirstComputed = true;

    auto StartPoint = std::chrono::steady_clock::now();
    for (const auto & Function : m_Functions)
    {
      std::thread Thread([&]{
        try
        {
          ResultT PartialResult = Function(x);
          if (PartialResult.index() != 2)
          {
            std::stringstream ss;
            ss << PartialResult;
            throw std::runtime_error(ss.str());
          }
          std::lock_guard Guard(Result.Mutex);
          Result.Value = (IsFirstComputed ? std::get<2>(PartialResult) : m_BinaryOp(Result.Value, std::get<2>(PartialResult)));
          IsFirstComputed = false;
          m_ConditionVariable.Decrease();
        }
        catch (const std::exception & e)
        {
          std::lock_guard Guard(m_WasExceptionThrown.Mutex);
          if (!m_WasExceptionThrown.Value)
          {
            m_WasExceptionThrown.Value = true;
            m_ExceptionStr = e.what();
            m_ConditionVariable.Free();
          }
        }
      });
      Thread.detach();
    }
    m_ConditionVariable.Wait();
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
      m_ConditionVariable.Free();
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

  void Clear()
  {
    m_Functions.clear();
    m_BinaryOp = {};
    Reset();
  }

  void Reset()
  {
    m_ConditionVariable.SetCounter(0);
    m_WasCancelConfirmed.Value = false;
    m_WasExceptionThrown.Value = false;
    m_ExceptionStr.clear();
    m_Time = 0;
  }

private:
  ConditionVariable m_ConditionVariable;
  std::vector<FunctionT> m_Functions;
  BinaryOpT m_BinaryOp;
  Synchronized<bool> m_WasCancelConfirmed;
  Synchronized<bool> m_WasExceptionThrown;
  std::string m_ExceptionStr;
  long long m_Time = 0;
};

#endif //OSLAB1_CALCULATOR_H
