
#ifndef GRANTLEE_TEST_UTILS_H
#define GRANTLEE_TEST_UTILS_H

#include <memory>

template<typename T, typename... Args>
std::unique_ptr<T> testMakeUnique(Args&&... a)
{
  return std::unique_ptr<T>(new T(std::forward<Args>(a)...));
}

#endif
