#ifndef SYNCHRONIZED_H
#define SYNCHRONIZED_H

#include <mutex>

template <typename T>
struct Synchronized
{
  T Value{};
  std::mutex Mutex;
};

#endif //SYNCHRONIZED_H
