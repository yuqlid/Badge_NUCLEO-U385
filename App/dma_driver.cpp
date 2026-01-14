/**
 * @file dma_driver.cpp
 * @author KUSAKABE Yuki (yuqid@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "dma_driver.hpp"

LL_DMA_LinkNodeTypeDef linknode_circular_usart1_rx = {0};

void LinkDmaUsart1Rx(const DMA_TypeDef *DMAx, const uint32_t Channel,
                    const uint32_t dest_address,
                    const uint32_t blk_data_length)
{
  LL_DMA_InitNodeTypeDef NodeConfig = {0};
  NodeConfig.DestAllocatedPort = LL_DMA_DEST_ALLOCATED_PORT0;
  NodeConfig.DestHWordExchange = LL_DMA_DEST_HALFWORD_PRESERVE;
  NodeConfig.DestByteExchange = LL_DMA_DEST_BYTE_PRESERVE;
  NodeConfig.DestBurstLength = 1;
  NodeConfig.DestIncMode = LL_DMA_DEST_INCREMENT;
  NodeConfig.DestDataWidth =
      LL_DMA_DEST_DATAWIDTH_BYTE; // UARTのデータレジスタは8bit幅
  NodeConfig.SrcAllocatedPort = LL_DMA_SRC_ALLOCATED_PORT0;
  NodeConfig.SrcByteExchange = LL_DMA_SRC_BYTE_PRESERVE;
  NodeConfig.DataAlignment = LL_DMA_DATA_ALIGN_ZEROPADD;
  NodeConfig.SrcBurstLength = 1;
  NodeConfig.SrcIncMode = LL_DMA_SRC_FIXED;
  NodeConfig.SrcDataWidth =
      LL_DMA_SRC_DATAWIDTH_BYTE; // UARTのデータレジスタは8bit幅
  NodeConfig.TransferEventMode = LL_DMA_TCEM_BLK_TRANSFER;
  NodeConfig.Mode = LL_DMA_NORMAL;
  NodeConfig.TriggerPolarity = LL_DMA_TRIG_POLARITY_MASKED;
  NodeConfig.BlkHWRequest = LL_DMA_HWREQUEST_SINGLEBURST;
  NodeConfig.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  NodeConfig.Request = LL_GPDMA1_REQUEST_USART1_RX;
  NodeConfig.UpdateRegisters =
      (LL_DMA_UPDATE_CTR1 | LL_DMA_UPDATE_CTR2 | LL_DMA_UPDATE_CBR1 |
       LL_DMA_UPDATE_CSAR | LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CTR3 |
       LL_DMA_UPDATE_CBR2 | LL_DMA_UPDATE_CLLR);
  NodeConfig.NodeType = LL_DMA_GPDMA_LINEAR_NODE;

  NodeConfig.BlkDataLength = blk_data_length;
  NodeConfig.DestAddress = dest_address;
  NodeConfig.SrcAddress =
      LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_RECEIVE);

  LL_DMA_CreateLinkNode(&NodeConfig, &linknode_circular_usart1_rx);

  LL_DMA_ConnectLinkNode(&linknode_circular_usart1_rx, LL_DMA_CLLR_OFFSET5,
                         &linknode_circular_usart1_rx, LL_DMA_CLLR_OFFSET5);

  LL_DMA_SetLinkedListBaseAddr(DMAx, Channel,
                               (uint32_t)&linknode_circular_usart1_rx);

  LL_DMA_ConfigLinkUpdate(
      DMAx, Channel,
      (LL_DMA_UPDATE_CTR1 | LL_DMA_UPDATE_CTR2 | LL_DMA_UPDATE_CBR1 |
       LL_DMA_UPDATE_CSAR | LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CTR3 |
       LL_DMA_UPDATE_CBR2 | LL_DMA_UPDATE_CLLR),
      (uint32_t)&linknode_circular_usart1_rx);
}

#ifdef __cplusplus
extern "C"
{
#endif

  void GPDMA1_Channel0_IRQHandler(void)
  {
    if (LL_DMA_IsActiveFlag_DTE(GPDMA1, LL_DMA_CHANNEL_0))
    {
      LL_DMA_ClearFlag_DTE(GPDMA1, LL_DMA_CHANNEL_0);
    }
  }

#ifdef __cplusplus
}
#endif
