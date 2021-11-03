#ifndef OSLAB1_CONDITIONVARIABLE_H
#define OSLAB1_CONDITIONVARIABLE_H

#include <condition_variable>

class ConditionVariable
{
public:

  void SetCounter(
      const int _Counter
    )
  {
    m_Counter = _Counter;
  }

  void Decrease()
  {
    int Count = --m_Counter;
    if (Count <= 0)
      m_Cv.notify_all();
  }

  void Free()
  {
    m_Counter = 0;
    m_Cv.notify_all();
  }

  void Wait()
  {
    std::mutex Mutex;
    std::unique_lock Lock(Mutex);
    m_Cv.wait(Lock);
  }

  std::cv_status WaitFor(
      const int _Sec
    )
  {
    std::mutex Mutex;
    std::unique_lock Lock(Mutex);
    return m_Cv.wait_for(Lock, std::chrono::seconds(_Sec));
  }

private:

  std::atomic<int> m_Counter = 0;
  std::condition_variable m_Cv;
};

#endif //OSLAB1_CONDITIONVARIABLE_H
