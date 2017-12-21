/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ETS_UART_INTR_ENABLE()  _xt_isr_unmask(1 << ETS_UART_INUM)
#define ETS_UART_INTR_DISABLE() _xt_isr_mask(1 << ETS_UART_INUM)
#define UART_INTR_MASK          0x1ff
#define UART_LINE_INV_MASK      (0x3f<<19)

typedef enum {
    UART_WordLength_5b = 0x0,
    UART_WordLength_6b = 0x1,
    UART_WordLength_7b = 0x2,
    UART_WordLength_8b = 0x3
} UART_WordLength;

typedef enum {
    UART_WORD_LENGTH_5b = 0x0,
    UART_WORD_LENGTH_6b = 0x1,
    UART_WORD_LENGTH_7b = 0x2,
    UART_WORD_LENGTH_8b = 0x3
} uart_word_length_e;

typedef enum {
    USART_StopBits_1   = 0x1,
    USART_StopBits_1_5 = 0x2,
    USART_StopBits_2   = 0x3,
} UART_StopBits;

typedef enum {
    UART_STOP_BITS_1   = 0x1,
    UART_STOP_BITS_1_5 = 0x2,
    UART_STOP_BITS_2   = 0x3,
} uart_stop_bits_e;

typedef enum {
    UART0 = 0x0,
    UART1 = 0x1,
} uart_port;

typedef enum {
    USART_Parity_None = 0x2,
    USART_Parity_Even = 0x0,
    USART_Parity_Odd  = 0x1
} UART_ParityMode;

typedef enum {
    UART_PARITY_NONE = 0x2,
    UART_PARITY_EVEN = 0x0,
    UART_PARITY_ODD  = 0x1
} uart_parity_mode_e;

typedef enum {
    PARITY_DIS = 0x0,
    PARITY_EN  = 0x2
} UartExistParity;

typedef enum {
    BIT_RATE_300     = 300,
    BIT_RATE_600     = 600,
    BIT_RATE_1200    = 1200,
    BIT_RATE_2400    = 2400,
    BIT_RATE_4800    = 4800,
    BIT_RATE_9600    = 9600,
    BIT_RATE_19200   = 19200,
    BIT_RATE_38400   = 38400,
    BIT_RATE_57600   = 57600,
    BIT_RATE_74880   = 74880,
    BIT_RATE_115200  = 115200,
    BIT_RATE_230400  = 230400,
    BIT_RATE_460800  = 460800,
    BIT_RATE_921600  = 921600,
    BIT_RATE_1843200 = 1843200,
    BIT_RATE_3686400 = 3686400,
} uart_baud_rate_e; //you can add any rate you need in this range

typedef enum {
    UART_HARDWARE_FLOW_CONTROL_NONE    = 0x0,
    UART_HARDWARE_FLOW_CONTROL_RTS     = 0x1,
    UART_HARDWARE_FLOW_CONTROL_CTS     = 0x2,
    UART_HARDWARE_FLOW_CONTROL_CTS_RTS = 0x3
} uart_hw_flow_ctrl_e;

typedef enum {
    UART_NONE_INVERSE = 0x0,
    UART_RXD_INVERSE  = UART_RXD_INV,
    UART_CTS_INVERSE  = UART_CTS_INV,
    UART_TXD_INVERSE  = UART_TXD_INV,
    UART_RTS_INVERSE  = UART_RTS_INV,
} uart_line_level_inverse_e;

typedef struct {
    uart_baud_rate_e    baud_rate;
    uart_word_length_e  data_bits;
    uart_parity_mode_e  parity;    // chip size in byte
    uart_stop_bits_e    stop_bits;
    uart_hw_flow_ctrl_e flow_ctrl;
    uint8               uart_rx_flow_thresh;
    uint32              uart_inverse_mask;
} uart_config_t;

typedef uart_config_t* uart_config_ptr;

typedef struct {
    uint32 uart_intr_en_mask;
    uint8  uart_rx_timeout_intr_thresh;
    uint8  uart_rx_fifo_full_intr_thresh;
    uint8  uart_tx_fifo_empty_intr_thresh;
} uart_intr_config_t;

typedef uart_intr_config_t* uart_intr_config_ptr;

//=======================================

/** \defgroup Driver_APIs Driver APIs
  * @brief Driver APIs
  */

/** @addtogroup Driver_APIs
  * @{
  */

/** \defgroup UART_Driver_APIs UART Driver APIs
  * @brief UART driver APIs
  */

/** @addtogroup UART_Driver_APIs
  * @{
  */

/**  
  * @brief   Wait uart tx fifo empty, do not use it if tx flow control enabled.
  * 
  * @param   uart_port uart_no:UART0 or UART1
  *  
  * @return  null
  */
void uart_wait_tx_fifo_empty(uart_port uart_no); //do not use if tx flow control enabled
/**  
  * @brief   Clear uart tx fifo and rx fifo.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  *  
  * @return  null
  */
void uart_reset_fifo(uart_port uart_no);

/**  
  * @brief  Clear uart interrupt flags.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   uint32 clr_mask : To clear the interrupt bits
  *  
  * @return  null
  */
void UART_ClearIntrStatus(uart_port uart_no, uint32 clr_mask);

/**  
  * @brief   Enable uart interrupts .
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   uint32 ena_mask : To enable the interrupt bits
  *  
  * @return  null
  */
void UART_SetIntrEna(uart_port uart_no, uint32 ena_mask);

/**  
  * @brief   Register an application-specific interrupt handler for Uarts interrupts.
  * 
  * @param   void *fn : interrupt handler for Uart interrupts.
  * @param   void *arg : interrupt handler's arg.
  *  
  * @return  null
  */
void uart_intr_handler_register(void *fn, void *arg);

/**  
  * @brief   Config from which serial output printf function.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  *  
  * @return  null
  */
void uart_set_print_port(uart_port uart_no);
/**  
  * @brief   Config Common parameters of serial ports.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   UART_ConfigTypeDef *pUARTConfig : parameters structure
  *  
  * @return  null
  */
void uart_config(uart_port uart_no, uart_config_ptr config);
/**  
  * @brief   Config types of uarts.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   UART_IntrConfTypeDef *pUARTIntrConf : parameters structure
  *  
  * @return  null
  */
void uart_intr_config(uart_port uart_no,  uart_intr_config_ptr intr_config);

/**  
  * @brief   Config the length of the uart communication data bits.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   uart_word_length_e len : the length of the uart communication data bits
  *  
  * @return  null
  */
void UART_SetWordLength(uart_port uart_no, uart_word_length_e len);

/**  
  * @brief   Config the length of the uart communication stop bits.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   uart_stop_bits_e bit_num : the length uart communication stop bits
  *  
  * @return  null
  */
void UART_SetStopBits(uart_port uart_no, uart_stop_bits_e bit_num);

/**  
  * @brief   Configure whether to open the parity.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   uart_parity_mode_e Parity_mode : the enum of uart parity configuration
  *  
  * @return  null
  */
void UART_SetParity(uart_port uart_no, uart_parity_mode_e Parity_mode) ;

/**  
  * @brief   Configure the Baud rate.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   uint32 baud_rate : the Baud rate
  *  
  * @return  null
  */
void uart_set_baud_rate(uart_port uart_no, uint32 baud_rate);

/**  
  * @brief   Configure Hardware flow control.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   UART_HwFlowCtrl flow_ctrl : Hardware flow control mode
  * @param   uint8 rx_thresh : threshold of Hardware flow control
  *  
  * @return  null
  */
void uart_set_flow_ctrl(uart_port uart_no, uart_hw_flow_ctrl_e flow_ctrl, uint8 rx_thresh);
/**  
  * @brief   Configure trigging signal of uarts.
  * 
  * @param   uart_port uart_no : UART0 or UART1
  * @param   UART_LineLevelInverse inverse_mask : Choose need to flip the IO
  *  
  * @return  null
  */
void UART_SetLineInverse(uart_port uart_no, uart_line_level_inverse_e inverse_mask) ;

/**  
  * @brief   uart_init.
  * @param   null
  * @return  null
  */
void uart_init(void);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif




#endif
