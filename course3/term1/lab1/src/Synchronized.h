#ifndef SYNCHRONIZED_H
#define SYNCHRONIZED_H

#include <mutex>

template <typename T>
struct Synchronized
{
  T Value = 0;
  std::mutex Mutex;
};

#endif //SYNCHRONIZED_H
