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

#include <espressif/esp_common.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <driver/uart/uart.h>

enum {
    UART_EVENT_RX_CHAR,
    UART_EVENT_MAX
};

typedef struct _os_event_ {
    uint32 event;
    uint32 param;
} os_event_t;

xTaskHandle xUartTaskHandle;
xQueueHandle xQueueUart;

LOCAL STATUS
uart_tx_one_char(uint8 uart, uint8 TxChar)
{
    while (true) {
        uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(uart)) & (UART_TXFIFO_CNT << UART_TXFIFO_CNT_S);

        if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126) {
            break;
        }
    }

    WRITE_PERI_REG(UART_FIFO(uart) , TxChar);
    return OK;
}

LOCAL void
uart1_write_char(char c)
{
    if (c == '\n') {
        uart_tx_one_char(UART1, '\r');
        uart_tx_one_char(UART1, '\n');
    } else if (c == '\r') {
    } else {
        uart_tx_one_char(UART1, c);
    }
}

LOCAL void
uart0_write_char(char c)
{
    if (c == '\n') {
        uart_tx_one_char(UART0, '\r');
        uart_tx_one_char(UART0, '\n');
    } else if (c == '\r') {
    } else {
        uart_tx_one_char(UART0, c);
    }
}

LOCAL void
uart_rx_intr_handler_ssc(void *arg)
{
    /* uart0 and uart1 intr combine togther, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
      * uart1 and uart0 respectively
      */
    os_event_t e;
    portBASE_TYPE xHigherPriorityTaskWoken;

    uint8 RcvChar;
    uint8 uart_no = 0;

    if (UART_RXFIFO_FULL_INT_ST != (READ_PERI_REG(UART_INT_ST(uart_no)) & UART_RXFIFO_FULL_INT_ST)) {
        return;
    }

    RcvChar = READ_PERI_REG(UART_FIFO(uart_no)) & 0xFF;

    WRITE_PERI_REG(UART_INT_CLR(uart_no), UART_RXFIFO_FULL_INT_CLR);

    e.event = UART_EVENT_RX_CHAR;
    e.param = RcvChar;

    xQueueSendFromISR(xQueueUart, (void *)&e, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

LOCAL void
uart_task(void *pvParameters)
{
    os_event_t e;

    for (;;) {
        if (xQueueReceive(xQueueUart, (void *)&e, (portTickType)portMAX_DELAY)) {
            switch (e.event) {
                case UART_EVENT_RX_CHAR:
                    printf("%c", e.param);
                    break;

                default:
                    break;
            }
        }
    }

    vTaskDelete(NULL);
}

//=================================================================

void
UART_SetWordLength(UART_Port uart_no, UART_WordLength len)
{
    SET_PERI_REG_BITS(UART_CONF0(uart_no), UART_BIT_NUM, len, UART_BIT_NUM_S);
}

void
UART_SetStopBits(UART_Port uart_no, uart_stop_bits_e bit_num)
{
    SET_PERI_REG_BITS(UART_CONF0(uart_no), UART_STOP_BIT_NUM, bit_num, UART_STOP_BIT_NUM_S);
}

void
UART_SetLineInverse(UART_Port uart_no, uart_line_level_inverse_e inverse_mask)
{
    CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_LINE_INV_MASK);
    SET_PERI_REG_MASK(UART_CONF0(uart_no), inverse_mask);
}

void
UART_SetParity(UART_Port uart_no, uart_parity_mode_e Parity_mode)
{
    CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_PARITY | UART_PARITY_EN);

    if (Parity_mode == USART_Parity_None) {
    } else {
        SET_PERI_REG_MASK(UART_CONF0(uart_no), Parity_mode | UART_PARITY_EN);
    }
}


void uart_set_baud_rate(UART_Port uart_no, uint32 baud_rate)
{
    uart_div_modify(uart_no, UART_CLK_FREQ / baud_rate);
}

//only when USART_HardwareFlowControl_RTS is set , will the rx_thresh value be set.

void uart_set_flow_ctrl(UART_Port uart_no, uart_hw_flow_ctrl_e flow_ctrl, uint8 rx_thresh)
{
    if (flow_ctrl & UART_HARDWARE_FLOW_CONTROL_RTS) {
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_U0RTS);
        SET_PERI_REG_BITS(UART_CONF1(uart_no), UART_RX_FLOW_THRHD, rx_thresh, UART_RX_FLOW_THRHD_S);
        SET_PERI_REG_MASK(UART_CONF1(uart_no), UART_RX_FLOW_EN);
    } else {
        CLEAR_PERI_REG_MASK(UART_CONF1(uart_no), UART_RX_FLOW_EN);
    }

    if (flow_ctrl & UART_HARDWARE_FLOW_CONTROL_CTS) {
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_UART0_CTS);
        SET_PERI_REG_MASK(UART_CONF0(uart_no), UART_TX_FLOW_EN);
    } else {
        CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_TX_FLOW_EN);
    }
}

//  Do not use if tx flow control enabled
void uart_wait_tx_fifo_empty(UART_Port uart_no)
{
    while (READ_PERI_REG(UART_STATUS(uart_no)) & (UART_TXFIFO_CNT << UART_TXFIFO_CNT_S));
}

void uart_reset_fifo(UART_Port uart_no)
{
    SET_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);    
}

void
UART_ClearIntrStatus(UART_Port uart_no, uint32 clr_mask)
{
    WRITE_PERI_REG(UART_INT_CLR(uart_no), clr_mask);
}

void
UART_SetIntrEna(UART_Port uart_no, uint32 ena_mask)
{
    SET_PERI_REG_MASK(UART_INT_ENA(uart_no), ena_mask);
}

void uart_intr_handler_register(void *fn, void *arg)
{
    _xt_isr_attach(ETS_UART_INUM, fn, arg);    
}

void uart_set_print_port(UART_Port uart_no)
{
    if (uart_no == 1) 
    {
        os_install_putc1(uart1_write_char);
    } 
    else 
    {
        os_install_putc1(uart0_write_char);
    }
}


void uart_intr_config(UART_Port uart_no,  uart_intr_config_ptr intr_config)
{
    uint32 reg_val = 0;
    UART_ClearIntrStatus(uart_no, UART_INTR_MASK);
    reg_val = READ_PERI_REG(UART_CONF1(uart_no)) & ((UART_RX_FLOW_THRHD << UART_RX_FLOW_THRHD_S) | UART_RX_FLOW_EN) ;

    reg_val |= ((intr_config->uart_intr_en_mask & UART_RXFIFO_TOUT_INT_ENA) ?
                ((((intr_config->uart_rx_timeout_intr_thresh) & UART_RX_TOUT_THRHD) << UART_RX_TOUT_THRHD_S) | UART_RX_TOUT_EN) : 0);

    reg_val |= ((intr_config->uart_intr_en_mask & UART_RXFIFO_FULL_INT_ENA) ?
                (((intr_config->uart_rx_fifo_full_intr_thresh) & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S) : 0);

    reg_val |= ((intr_config->uart_intr_en_mask & UART_TXFIFO_EMPTY_INT_ENA) ?
                (((intr_config->uart_tx_fifo_empty_intr_thresh) & UART_TXFIFO_EMPTY_THRHD) << UART_TXFIFO_EMPTY_THRHD_S) : 0);

    WRITE_PERI_REG(UART_CONF1(uart_no), reg_val);
    CLEAR_PERI_REG_MASK(UART_INT_ENA(uart_no), UART_INTR_MASK);
    SET_PERI_REG_MASK(UART_INT_ENA(uart_no), intr_config->uart_intr_en_mask);    
}

LOCAL void
uart0_rx_intr_handler(void *para)
{
    /* uart0 and uart1 intr combine togther, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
    * uart1 and uart0 respectively
    */
    uint8 RcvChar;
    uint8 uart_no = UART0;//UartDev.buff_uart_no;
    uint8 fifo_len = 0;
    uint8 buf_idx = 0;
    uint8 fifo_tmp[128] = {0};

    uint32 uart_intr_status = READ_PERI_REG(UART_INT_ST(uart_no)) ;

    while (uart_intr_status != 0x0) 
    {
        if (UART_FRM_ERR_INT_ST == (uart_intr_status & UART_FRM_ERR_INT_ST)) {
            //printf("FRM_ERR\r\n");
            WRITE_PERI_REG(UART_INT_CLR(uart_no), UART_FRM_ERR_INT_CLR);
        } 
        else if (UART_RXFIFO_FULL_INT_ST == (uart_intr_status & UART_RXFIFO_FULL_INT_ST)) 
        {
            printf("full\r\n");
            fifo_len = (READ_PERI_REG(UART_STATUS(UART0)) >> UART_RXFIFO_CNT_S)&UART_RXFIFO_CNT;
            buf_idx = 0;

            while (buf_idx < fifo_len) {
                uart_tx_one_char(UART0, READ_PERI_REG(UART_FIFO(UART0)) & 0xFF);
                buf_idx++;
            }

            WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);
        } 
        else if (UART_RXFIFO_TOUT_INT_ST == (uart_intr_status & UART_RXFIFO_TOUT_INT_ST)) 
        {
            printf("tout\r\n");
            fifo_len = (READ_PERI_REG(UART_STATUS(UART0)) >> UART_RXFIFO_CNT_S) & UART_RXFIFO_CNT;
            buf_idx = 0;

            while (buf_idx < fifo_len) {
                uart_tx_one_char(UART0, READ_PERI_REG(UART_FIFO(UART0)) & 0xFF);
                buf_idx++;
            }

            WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_TOUT_INT_CLR);
        } 
        else if (UART_TXFIFO_EMPTY_INT_ST == (uart_intr_status & UART_TXFIFO_EMPTY_INT_ST)) 
        {
            printf("empty\n\r");
            WRITE_PERI_REG(UART_INT_CLR(uart_no), UART_TXFIFO_EMPTY_INT_CLR);
            CLEAR_PERI_REG_MASK(UART_INT_ENA(UART0), UART_TXFIFO_EMPTY_INT_ENA);
        } 
        else 
        {
            //skip
        }

        uart_intr_status = READ_PERI_REG(UART_INT_ST(uart_no)) ;
    }
}

void uart_config(UART_Port uart_no, uart_config_ptr config)
{
    if (uart_no == UART1)
    {
        // configure UART1
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);
    }
    else
    {
        PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_U0RXD);
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);        
    }
    
    uart_set_flow_ctrl(uart_no, config->flow_ctrl, config->uart_rx_flow_thresh);
    uart_set_baud_rate(uart_no, config->baud_rate);
    
    WRITE_PERI_REG(UART_CONF0(uart_no),
                   ((config->parity == UART_PARITY_NONE) ? 0x0 : (UART_PARITY_EN | 
                                                                    config->parity))
                   | (config->stop_bits << UART_STOP_BIT_NUM_S)
                   | (config->data_bits << UART_BIT_NUM_S)
                   | ((config->flow_ctrl &  UART_HARDWARE_FLOW_CONTROL_CTS) 
                                                            ? UART_TX_FLOW_EN : 0x0)
                   | config->uart_inverse_mask);
    
    uart_reset_fifo(uart_no);
}

void uart_init(void)
{
    uart_wait_tx_fifo_empty(UART0);
    uart_wait_tx_fifo_empty(UART1);
    
    uart_config_t config;
    config.baud_rate = BIT_RATE_9600;
    config.data_bits = UART_WORD_LENGTH_8b;
    config.parity = UART_PARITY_NONE;
    config.stop_bits = UART_STOP_BITS_1;
    config.flow_ctrl = UART_HARDWARE_FLOW_CONTROL_NONE;
    config.uart_rx_flow_thresh = 120;
    config.uart_inverse_mask = UART_NONE_INVERSE;
    uart_config(UART0, &config);
    
    uart_intr_config_t uart_intr;
//     uart_intr.uart_intr_en_mask = UART_RXFIFO_TOUT_INT_ENA | UART_FRM_ERR_INT_ENA |
//                                   UART_RXFIFO_FULL_INT_ENA | UART_TXFIFO_EMPTY_INT_ENA;
    uart_intr.uart_intr_en_mask = UART_FRM_ERR_INT_ENA |
                                  UART_RXFIFO_FULL_INT_ENA | UART_TXFIFO_EMPTY_INT_ENA;                                  
    uart_intr.uart_rx_fifo_full_intr_thresh = 10;
    uart_intr.uart_rx_timeout_intr_thresh = 2;
    uart_intr.uart_tx_fifo_empty_intr_thresh = 20;
    uart_intr_config(UART0, &uart_intr);
    
    uart_set_print_port(UART0);
    uart_intr_handler_register(uart0_rx_intr_handler, NULL);
    ETS_UART_INTR_ENABLE();
}
