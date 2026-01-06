/**
 * @file dma_driver.hpp
 * @author KUSAKABE Yuki (yuqid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DMA_DRIVER_HPP_
#define DMA_DRIVER_HPP_

#include "gpdma.h"
#include "ringbufwithdma.hpp"

template <typename T, size_t N>
void SetupDma(const USART_TypeDef *USARTx, DMA_TypeDef *DMAx, uint32_t Channel,
              ringBufferWithDma<T, N> &ringbuff_with_dma) {
  LL_DMA_ConfigAddresses(
      DMAx, Channel,
      LL_USART_DMA_GetRegAddr(USARTx, LL_USART_DMA_REG_DATA_RECEIVE),
      // 転送先のアドレスを数値として渡す必要があるためreinterpret_castでキャストする
      reinterpret_cast<uint32_t>(ringbuff_with_dma.bufferPointer()));

  // Set DMA transfer size
  LL_DMA_SetBlkDataLength(DMAx, Channel, ringbuff_with_dma.bufferLength());

  /* Enable DMA transfer complete/error interrupts */
  // LL_DMA_EnableIT_TC(DMA1, Channel);
  LL_DMA_EnableIT_DTE(DMAx, Channel);

  LL_DMA_EnableChannel(DMAx, Channel);
}

template <typename T>
void SetupDma(uint32_t SrcAddress, DMA_TypeDef *DMAx, uint32_t Channel, T *buf,
              uint32_t size) {
  LL_DMA_ConfigAddresses(DMAx, Channel, SrcAddress,

                         reinterpret_cast<uint32_t>(buf));
  /* 転送先のアドレスを数値として渡す必要があるためreinterpret_castでキャストする*/
  /* Set DMA transfer size */

  LL_DMA_SetBlkDataLength(DMAx, Channel, size);

  /* Enable DMA transfer complete/error interrupts */
  // LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_DTE(DMAx, Channel);

  LL_DMA_EnableChannel(DMAx, Channel);
}

static inline volatile uint32_t &GetDmaCBR1(DMA_TypeDef *DMAx,
                                            uint32_t Channel) {
  uint32_t dma_base_addr = (uint32_t)DMAx;
  return ((DMA_Channel_TypeDef *)(dma_base_addr +
                                  LL_DMA_CH_OFFSET_TAB[Channel]))
      ->CBR1;
}

void LinkDmaUart4Rx(const DMA_TypeDef *DMAx, const uint32_t Channel,
                    const uint32_t dest_address,
                    const uint32_t blk_data_length);

void LinkDmaUart5Rx(const DMA_TypeDef *DMAx, const uint32_t Channel,
                    const uint32_t dest_address,
                    const uint32_t blk_data_length);

void LinkDmaADC12(const DMA_TypeDef *DMAx, const uint32_t Channel,
                  const uint32_t dest_address, const uint32_t blk_data_length);

/*
static inline void dma_nvic_setup(void) {

  NVIC_SetPriority(DMA1_Channel1_IRQn,
                   NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  NVIC_SetPriority(DMA1_Channel2_IRQn,
                   NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
  NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  NVIC_SetPriority(DMA1_Channel3_IRQn,
                   NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
  NVIC_EnableIRQ(DMA1_Channel3_IRQn);

  NVIC_SetPriority(DMA1_Channel4_IRQn,
                   NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}
*/
#endif /* DMA_DRIVER_HPP_ */