//
// Created by Jared McFarland on 1/8/24.
//
#pragma once

#ifndef OFXCRVSBUFFEREDCONTAINER_H
#define OFXCRVSBUFFEREDCONTAINER_H

#include "ofMain.h"

namespace ofxCrvs {

template <typename T> class BufferedContainer {
public:
  BufferedContainer() : bufferedCache({T(), T()}) {}
  explicit BufferedContainer(T value) : bufferedCache({value, value}) {}

  const typename T::value_type &operator[](const std::size_t index) const {
    const auto &currentCache =
        bufferedCache[currentCacheIndex.load(std::memory_order_acquire)];
    return currentCache[index % currentCache.size()];
  }

  const T &get() const {
    return bufferedCache[currentCacheIndex.load(std::memory_order_acquire)];
  }

  void set(T value) {
    const int cacheIdx =
        currentCacheIndex.load(std::memory_order_acquire) == 0 ? 1 : 0;
    bufferedCache[cacheIdx] = value;
    currentCacheIndex.store(cacheIdx, std::memory_order_release);
  }

  std::size_t size() const {
    const auto &currentCache =
        bufferedCache[currentCacheIndex.load(std::memory_order_acquire)];
    return currentCache.size();
  }

private:
  std::array<T, 2> bufferedCache;
  std::atomic<int> currentCacheIndex{0};
};

} // namespace ofxCrvs

#endif // OFXCRVSBUFFEREDCONTAINER_H
