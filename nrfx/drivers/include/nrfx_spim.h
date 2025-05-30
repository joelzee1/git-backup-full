/*
 * Copyright (c) 2015 - 2023, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NRFX_SPIM_H__
#define NRFX_SPIM_H__

#include <nrfx.h>
#include <haly/nrfy_spim.h>
#include <haly/nrfy_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrfx_spim SPIM driver
 * @{
 * @ingroup nrf_spim
 * @brief   Serial Peripheral Interface Master with EasyDMA (SPIM) driver.
 */

/** @brief Data structure of the Serial Peripheral Interface Master with EasyDMA (SPIM) driver instance. */
typedef struct
{
    NRF_SPIM_Type * p_reg;        ///< Pointer to a structure with SPIM registers.
    uint8_t         drv_inst_idx; ///< Index of the driver instance. For internal use only.
} nrfx_spim_t;

#ifndef __NRFX_DOXYGEN__
enum {
    /* List all enabled driver instances (in the format NRFX_\<instance_name\>_INST_IDX). */
    NRFX_INSTANCE_ENUM_LIST(SPIM)
    NRFX_SPIM_ENABLED_COUNT
};
#endif

/** @brief Macro for creating an instance of the SPIM driver. */
#define NRFX_SPIM_INSTANCE(id)                               \
{                                                            \
    .p_reg        = NRFX_CONCAT_2(NRF_SPIM, id),             \
    .drv_inst_idx = NRFX_CONCAT_3(NRFX_SPIM, id, _INST_IDX), \
}

/** @brief Configuration structure of the SPIM driver instance. */
typedef struct
{
    uint32_t             sck_pin;        ///< SCK pin number.
    uint32_t             mosi_pin;       ///< MOSI pin number (optional).
                                         /**< Set to @ref NRF_SPIM_PIN_NOT_CONNECTED
                                          *   if this signal is not needed. */
    uint32_t             miso_pin;       ///< MISO pin number (optional).
                                         /**< Set to @ref NRF_SPIM_PIN_NOT_CONNECTED
                                          *   if this signal is not needed. */
    uint32_t             ss_pin;         ///< Slave Select pin number (optional).
                                         /**< Set to @ref NRF_SPIM_PIN_NOT_CONNECTED
                                          *   if this signal is not needed.
                                          *   @note Unlike the other fields that specify
                                          *   pin numbers, this one cannot be omitted
                                          *   when both GPIO configuration and pin
                                          *   selection are to be skipped but the signal
                                          *   is not controlled by hardware (the driver
                                          *   must then control it as a regular GPIO). */
    bool                 ss_active_high; ///< Polarity of the Slave Select pin during transmission.
    uint8_t              irq_priority;   ///< Interrupt priority.
    uint8_t              orc;            ///< Overrun character.
                                         /**< This character is used when all bytes from the TX buffer are sent,
                                          *   but the transfer continues due to RX. */
    uint32_t             frequency;      ///< SPIM frequency in Hz.
    nrf_spim_mode_t      mode;           ///< SPIM mode.
    nrf_spim_bit_order_t bit_order;      ///< SPIM bit order.
    nrf_gpio_pin_pull_t  miso_pull;      ///< MISO pull up configuration.
#if NRFX_CHECK(NRFX_SPIM_EXTENDED_ENABLED) || defined(__NRFX_DOXYGEN__)
    uint32_t             dcx_pin;        ///< D/CX pin number (optional).
    uint8_t              rx_delay;       ///< Sample delay for input serial data on MISO.
                                         /**< The value specifies the delay, in number of 64 MHz clock cycles
                                          *   (15.625 ns), from the the sampling edge of SCK (leading edge for
                                          *   CONFIG.CPHA = 0, trailing edge for CONFIG.CPHA = 1) until
                                          *   the input serial data is sampled. */
    bool                 use_hw_ss;      ///< Indication to use software or hardware controlled Slave Select pin.
    uint8_t              ss_duration;    ///< Slave Select duration before and after transmission.
                                         /**< Minimum duration between the edge of CSN and the edge of SCK.
                                          *   Also, minimum duration of CSN inactivity between transactions.
                                          *   The value is specified in number of 64 MHz clock cycles (15.625 ns).
                                          *   Supported only for hardware-controlled Slave Select. */
#endif
    bool                 skip_gpio_cfg;  ///< Skip GPIO configuration of pins.
                                         /**< When set to true, the driver does not modify
                                          *   any GPIO parameters of the used pins. Those
                                          *   parameters are supposed to be configured
                                          *   externally before the driver is initialized. */
    bool                 skip_psel_cfg;  ///< Skip pin selection configuration.
                                         /**< When set to true, the driver does not modify
                                          *   pin select registers in the peripheral.
                                          *   Those registers are supposed to be set up
                                          *   externally before the driver is initialized.
                                          *   @note When both GPIO configuration and pin
                                          *   selection are to be skipped, the structure
                                          *   fields that specify pins can be omitted,
                                          *   as they are ignored anyway. This does not
                                          *   apply to the @p ss_pin field, unless it is
                                          *   to be controlled by hardware.*/
} nrfx_spim_config_t;

/**
 * @brief SPIM driver default configuration.
 *
 * This configuration sets up SPIM with the following options:
 * - SS pin active low
 * - over-run character set to 0xFF
 * - clock frequency: 4 MHz
 * - mode: 0 (SCK active high, sample on leading edge of the clock signal)
 * - MSB shifted out first
 * - MISO pull-up disabled
 *
 * @param[in] _pin_sck  SCK pin.
 * @param[in] _pin_mosi MOSI pin.
 * @param[in] _pin_miso MISO pin.
 * @param[in] _pin_ss   Slave select pin.
 */
#define NRFX_SPIM_DEFAULT_CONFIG(_pin_sck, _pin_mosi, _pin_miso, _pin_ss)                        \
{                                                                                                \
    .sck_pin        = _pin_sck,                                                                  \
    .mosi_pin       = _pin_mosi,                                                                 \
    .miso_pin       = _pin_miso,                                                                 \
    .ss_pin         = _pin_ss,                                                                   \
    .ss_active_high = false,                                                                     \
    .irq_priority   = NRFX_SPIM_DEFAULT_CONFIG_IRQ_PRIORITY,                                     \
    .orc            = 0xFF,                                                                      \
    .frequency      = NRFX_MHZ_TO_HZ(4),                                                         \
    .mode           = NRF_SPIM_MODE_0,                                                           \
    .bit_order      = NRF_SPIM_BIT_ORDER_MSB_FIRST,                                              \
    .miso_pull      = NRF_GPIO_PIN_NOPULL,                                                       \
    NRFX_COND_CODE_1(NRFX_SPIM_EXTENDED_ENABLED, (.use_hw_ss = false, .ss_duration = 0x02,), ()) \
    NRFX_COND_CODE_1(NRFX_SPIM_EXTENDED_ENABLED, (.rx_delay = 0x02,), ())                        \
    NRFX_COND_CODE_1(NRFX_SPIM_EXTENDED_ENABLED, (.dcx_pin = NRF_SPIM_PIN_NOT_CONNECTED,), ())   \
}

/**
 * @brief Macro for checking whether specified frequency can be achieved for a given SPIM instance.
 *
 * @note This macro uses a compile-time assertion.
 *
 * @param[in] id        Index of the specified SPIM instance.
 * @param[in] frequency Desired frequency value in Hz.
 */
#define NRFX_SPIM_FREQUENCY_STATIC_CHECK(id, frequency) \
         NRF_SPIM_FREQUENCY_STATIC_CHECK(NRF_SPIM_INST_GET(id), frequency)

/** @brief Flag indicating that TX buffer address will be incremented after transfer. */
#define NRFX_SPIM_FLAG_TX_POSTINC          (1UL << 0)

/** @brief Flag indicating that RX buffer address will be incremented after transfer. */
#define NRFX_SPIM_FLAG_RX_POSTINC          (1UL << 1)

/** @brief Flag indicating that the interrupt after each transfer will be suppressed, and the event handler will not be called. */
#define NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER (1UL << 2)

/** @brief Flag indicating that the transfer will be set up, but not started. */
#define NRFX_SPIM_FLAG_HOLD_XFER           (1UL << 3)

/** @brief Flag indicating that the transfer will be executed multiple times. */
#define NRFX_SPIM_FLAG_REPEATED_XFER       (1UL << 4)

/** @brief Single transfer descriptor structure. */
typedef nrfy_spim_xfer_desc_t nrfx_spim_xfer_desc_t;

/**
 * @brief Macro for setting up single transfer descriptor.
 *
 * This macro is for internal use only.
 */
#define NRFX_SPIM_SINGLE_XFER(p_tx, tx_len, p_rx, rx_len) \
    {                                                     \
    .p_tx_buffer = (uint8_t const *)(p_tx),               \
    .tx_length = (tx_len),                                \
    .p_rx_buffer = (p_rx),                                \
    .rx_length = (rx_len),                                \
    }

/** @brief Macro for setting the duplex TX RX transfer. */
#define NRFX_SPIM_XFER_TRX(p_tx_buf, tx_length, p_rx_buf, rx_length) \
        NRFX_SPIM_SINGLE_XFER(p_tx_buf, tx_length, p_rx_buf, rx_length)

/** @brief Macro for setting the TX transfer. */
#define NRFX_SPIM_XFER_TX(p_buf, length) \
        NRFX_SPIM_SINGLE_XFER(p_buf, length, NULL, 0)

/** @brief Macro for setting the RX transfer. */
#define NRFX_SPIM_XFER_RX(p_buf, length) \
        NRFX_SPIM_SINGLE_XFER(NULL, 0, p_buf, length)

/**
 * @brief SPIM master driver event types, passed to the handler routine provided
 *        during initialization.
 */
typedef enum
{
    NRFX_SPIM_EVENT_DONE, ///< Transfer done.
} nrfx_spim_evt_type_t;

/** @brief SPIM event description with transmission details. */
typedef struct
{
    nrfx_spim_evt_type_t  type;      ///< Event type.
    nrfx_spim_xfer_desc_t xfer_desc; ///< Transfer details.
} nrfx_spim_evt_t;

/** @brief SPIM driver event handler type. */
typedef void (* nrfx_spim_evt_handler_t)(nrfx_spim_evt_t const * p_event,
                                         void *                  p_context);

/**
 * @brief Function for initializing the SPIM driver instance.
 *
 * This function configures and enables the specified peripheral.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] p_config   Pointer to the structure with the initial configuration.
 *                       NULL if configuration is to be skipped and will be done later
 *                       using @ref nrfx_spim_reconfigure.
 * @param[in] handler    Event handler provided by the user. If NULL, transfers
 *                       will be performed in blocking mode.
 * @param[in] p_context  Context passed to event handler.
 *
 * @warning On nRF5340, 32 MHz setting for SPIM4 peripheral instance is supported
 *          only on the dedicated pins with @ref NRF_GPIO_PIN_SEL_PERIPHERAL configuration.
 *          See the chapter <a href=@nRF5340pinAssignmentsURL>Pin assignments</a> in the Product Specification.
 *
 * @retval NRFX_SUCCESS             Initialization was successful.
 * @retval NRFX_ERROR_INVALID_STATE The driver was already initialized.
 * @retval NRFX_ERROR_BUSY          Some other peripheral with the same
 *                                  instance ID is already in use. This is
 *                                  possible only if @ref nrfx_prs module
 *                                  is enabled.
 * @retval NRFX_ERROR_NOT_SUPPORTED Requested configuration is not supported
 *                                  by the SPIM instance.
 * @retval NRFX_ERROR_INVALID_PARAM Requested frequency is not available on the specified driver instance or pins.
 * @retval NRFX_ERROR_FORBIDDEN     Software-controlled Slave Select and hardware-controlled Slave Select
                                    cannot be active at the same time.
 */
nrfx_err_t nrfx_spim_init(nrfx_spim_t const *        p_instance,
                          nrfx_spim_config_t const * p_config,
                          nrfx_spim_evt_handler_t    handler,
                          void *                     p_context);

/**
 * @brief Function for reconfiguring the SPIM driver instance.
 *
 * @note This function can not be called during transmission.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] p_config   Pointer to the structure with the configuration.
 *
 * @retval NRFX_SUCCESS             Reconfiguration was successful.
 * @retval NRFX_ERROR_BUSY          The driver is during transfer.
 * @retval NRFX_ERROR_INVALID_STATE The driver is uninitialized.
 * @retval NRFX_ERROR_NOT_SUPPORTED Requested configuration is not supported
 *                                  by the SPIM instance.
 * @retval NRFX_ERROR_INVALID_PARAM Requested frequency is not available on the specified driver instance or pins.
 * @retval NRFX_ERROR_FORBIDDEN     Software-controlled Slave Select and hardware-controlled Slave Select
                                    cannot be active at the same time.
 */
nrfx_err_t nrfx_spim_reconfigure(nrfx_spim_t const *        p_instance,
                                 nrfx_spim_config_t const * p_config);

/**
 * @brief Function for uninitializing the SPIM driver instance.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 */
void nrfx_spim_uninit(nrfx_spim_t const * p_instance);

/**
 * @brief Function for starting the SPIM data transfer.
 *
 * Additional options are provided using the @c flags parameter:
 *
 * - @ref NRFX_SPIM_FLAG_TX_POSTINC and @ref NRFX_SPIM_FLAG_RX_POSTINC -
 *   Post-incrementation of buffer addresses.
 * - @ref NRFX_SPIM_FLAG_HOLD_XFER - Driver is not starting the transfer. Use this
 *   flag if the transfer is triggered externally by PPI. Use
 *   @ref nrfx_spim_start_task_address_get to get the address of the start task.
 *   Chip select must be configured to @ref NRF_SPIM_PIN_NOT_CONNECTED and managed outside the driver.
 *   If you do not expect more transfers, you should call @ref nrfx_spim_abort to inform the driver
 *   that the peripheral can be put into a low power state.
 * - @ref NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER - No user event handler after transfer
 *   completion. This also means no interrupt at the end of the transfer.
 *   If @ref NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER is used, the driver does not set the instance into
 *   busy state, so you must ensure that the next transfers are set up when SPIM is not active.
 *   Additionally, you should call @ref nrfx_spim_abort to inform the driver that no more transfers will occur.
 *   @ref nrfx_spim_end_event_address_get function can be used to detect end of transfer. Option can
 *   be used together with @ref NRFX_SPIM_FLAG_REPEATED_XFER to prepare a sequence of SPI transfers
 *   without interruptions. If you do not expect more transfers, you should call @ref nrfx_spim_abort
 *   to inform the driver that the peripheral can be put into a low power state.
 * - @ref NRFX_SPIM_FLAG_REPEATED_XFER - Prepare for repeated transfers. You can set
 *   up a number of transfers that will be triggered externally (for example by PPI). An example is
 *   a TXRX transfer with the options @ref NRFX_SPIM_FLAG_RX_POSTINC,
 *   @ref NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER, and @ref NRFX_SPIM_FLAG_REPEATED_XFER. After the
 *   transfer is set up, a set of transfers can be triggered by PPI that will read, for example,
 *   the same register of an external component and put it into a RAM buffer without any interrupts.
 *   @ref nrfx_spim_end_event_address_get can be used to get the address of the END event, which can
 *   be used to count the number of transfers. If @ref NRFX_SPIM_FLAG_REPEATED_XFER is used,
 *   the driver does not set the instance into busy state, so you must ensure that the next
 *   transfers are set up when SPIM is not active. If you do not expect more transfers, you should call
 *   @ref nrfx_spim_abort to inform the driver that the peripheral can be put into a low power state.
 *
 * @note Peripherals using EasyDMA (including SPIM) require the transfer buffers
 *       to be placed in the Data RAM region. If this condition is not met,
 *       this function will fail with the error code NRFX_ERROR_INVALID_ADDR.
 *
 * @param p_instance  Pointer to the driver instance structure.
 * @param p_xfer_desc Pointer to the transfer descriptor.
 * @param flags       Transfer options (0 for default settings).
 *
 * @retval NRFX_SUCCESS             The procedure is successful.
 * @retval NRFX_ERROR_BUSY          The driver is not ready for a new transfer.
 * @retval NRFX_ERROR_NOT_SUPPORTED The provided parameters are not supported.
 * @retval NRFX_ERROR_INVALID_ADDR  The provided buffers are not placed in the Data
 *                                  RAM region.
 */
nrfx_err_t nrfx_spim_xfer(nrfx_spim_t const *           p_instance,
                          nrfx_spim_xfer_desc_t const * p_xfer_desc,
                          uint32_t                      flags);

#if NRFX_CHECK(NRFX_SPIM_EXTENDED_ENABLED) || defined(__NRFX_DOXYGEN__)
/**
 * @brief Function for starting the SPIM data transfer with DCX control.
 *
 * See @ref nrfx_spim_xfer for description of additional options of transfer
 * provided by the @c flags parameter.
 *
 * @note Peripherals that use EasyDMA (including SPIM) require the transfer buffers
 *       to be placed in the Data RAM region. If this condition is not met,
 *       this function will fail with the error code NRFX_ERROR_INVALID_ADDR.
 *
 * @param p_instance  Pointer to the driver instance structure.
 * @param p_xfer_desc Pointer to the transfer descriptor.
 * @param flags       Transfer options (0 for default settings).
 * @param cmd_length  Length of the command bytes preceding the data
 *                    bytes. The DCX line will be low during transmission
 *                    of command bytes and high during transmission of data bytes.
 *                    Maximum value available for dividing the transmitted bytes
 *                    into command bytes and data bytes is @ref NRF_SPIM_DCX_CNT_ALL_CMD - 1.
 *                    The @ref NRF_SPIM_DCX_CNT_ALL_CMD value passed as the
 *                    @c cmd_length parameter causes all transmitted bytes
 *                    to be marked as command bytes.
 *
 * @retval NRFX_SUCCESS             The procedure is successful.
 * @retval NRFX_ERROR_BUSY          The driver is not ready for a new transfer.
 * @retval NRFX_ERROR_NOT_SUPPORTED The provided parameters are not supported.
 * @retval NRFX_ERROR_INVALID_ADDR  The provided buffers are not placed in the Data
 *                                  RAM region.
 */
nrfx_err_t nrfx_spim_xfer_dcx(nrfx_spim_t const *           p_instance,
                              nrfx_spim_xfer_desc_t const * p_xfer_desc,
                              uint32_t                      flags,
                              uint8_t                       cmd_length);
#endif

/**
 * @brief Function for returning the address of a SPIM start task.
 *
 * This function is to be used if @ref nrfx_spim_xfer was called with the flag @ref NRFX_SPIM_FLAG_HOLD_XFER.
 * In that case, the transfer is not started by the driver, but it must be started externally by PPI.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 *
 * @return Start task address.
 */
NRFX_STATIC_INLINE uint32_t nrfx_spim_start_task_address_get(nrfx_spim_t const * p_instance);

/**
 * @brief Function for returning the address of a END SPIM event.
 *
 * The END event can be used to detect the end of a transfer
 * if the @ref NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER option is used.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 *
 * @return END event address.
 */
NRFX_STATIC_INLINE uint32_t nrfx_spim_end_event_address_get(nrfx_spim_t const * p_instance);

/**
 * @brief Function for aborting ongoing transfer.
 *
 * @note You should call the function if the first transfer has been started with one or more
 *       of the following options: @ref NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER,
 *       @ref NRFX_SPIM_FLAG_HOLD_XFER, and @ref NRFX_SPIM_FLAG_REPEATED_XFER. When you do not
 *       expect more transfers, use this function so that the driver can put the peripheral into
 *       a low power state.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 */
void nrfx_spim_abort(nrfx_spim_t const * p_instance);

/**
 * @brief Macro returning SPIM interrupt handler.
 *
 * param[in] idx SPIM index.
 *
 * @return Interrupt handler.
 */
#define NRFX_SPIM_INST_HANDLER_GET(idx) NRFX_CONCAT_3(nrfx_spim_, idx, _irq_handler)

#ifndef NRFX_DECLARE_ONLY
NRFX_STATIC_INLINE uint32_t nrfx_spim_start_task_address_get(nrfx_spim_t const * p_instance)
{
    return nrfy_spim_task_address_get(p_instance->p_reg, NRF_SPIM_TASK_START);
}

NRFX_STATIC_INLINE uint32_t nrfx_spim_end_event_address_get(nrfx_spim_t const * p_instance)
{
    return nrfy_spim_event_address_get(p_instance->p_reg, NRF_SPIM_EVENT_END);
}
#endif // NRFX_DECLARE_ONLY
/** @} */

/*
 * Declare interrupt handlers for all enabled driver instances in the following format:
 * nrfx_\<periph_name\>_\<idx\>_irq_handler (for example, nrfx_spim_0_irq_handler).
 *
 * A specific interrupt handler for the driver instance can be retrieved by using
 * the NRFX_SPIM_INST_HANDLER_GET macro.
 *
 * Here is a sample of using the NRFX_SPIM_INST_HANDLER_GET macro to directly map
 * an interrupt handler in a Zephyr application:
 *
 * IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_SPIM_INST_GET(\<instance_index\>)), \<priority\>,
 *                    NRFX_SPIM_INST_HANDLER_GET(\<instance_index\>), 0);
 */
NRFX_INSTANCE_IRQ_HANDLERS_DECLARE(SPIM, spim)


#ifdef __cplusplus
}
#endif

#endif // NRFX_SPIM_H__
