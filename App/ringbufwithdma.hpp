/**
 * @file ringbufwithdma.hpp
 * @author KUSAKABE Yuki (yuqid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef RINGBUFFERWITHDMA_HPP_
#define RINGBUFFERWITHDMA_HPP_

#include <array>

template <typename T, std::size_t N>
class ringBufferWithDma {
  static_assert((N & (N - 1)) == 0, "Capacity must be a power of two.");

 private:
  std::array<T, N> buffer;
  std::size_t tail;
  std::size_t size;
  volatile uint32_t& pdmacounter;  // DMAのカウンタレジスタの参照

 public:
  ringBufferWithDma(volatile uint32_t& cntdr)
      : buffer{}, tail(0), size(0), pdmacounter(cntdr) {}

  bool isEmpty() { return getAvailableDataSize() == 0; }

  bool isFull() { return getAvailableDataSize() == N; }

  constexpr std::size_t bufferLength() const { return N; }

  /**
   * @brief Get the Size object
   *
   * @return std::size_t
   */
  std::size_t getAvailableDataSize() {
    std::size_t head = ((N - (pdmacounter & 0xFFFF)) & (N - 1));
    if (tail < head) {
      size = head - tail;
    } else if (tail > head) {
      size = N - (tail - head);
    } else {
      size = 0;
    }
    return size;
  }

  /**
   * @brief buffer先頭アドレスを返す。
   *
   * @return uint8_t*
   */
  T* bufferPointer(void) { return buffer.data(); }

  T dequeue(void) {
    // コンパイル時に配列の大きさが2のn乗が決定しているためビット演算でtailの位置を更新
    T item = buffer.at(tail++);
    tail &= (N - 1);
    return item;
  }
};

#endif /* RINGBUFFERWITHDMA_HPP_ */