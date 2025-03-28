/****************************************************************************
 * boards/arm/stm32/olimex-stm32-e407/src/olimex-stm32-e407.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __BOARDS_ARM_STM32_OLIMEX_STM32_E407_SRC_OLIMEX_STM32_E407_H
#define __BOARDS_ARM_STM32_OLIMEX_STM32_E407_SRC_OLIMEX_STM32_E407_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>
#include <stdint.h>
#include <arch/stm32/chip.h>

#include "stm32.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration from stm32f4discovery */

/* Assume that we have everything */

#define HAVE_USBDEV     1
#define HAVE_USBHOST    1
#define HAVE_USBMONITOR 1
#define HAVE_SDIO       1
#define HAVE_RTC_DRIVER 1
#define HAVE_NETMONITOR 1

/* Can't support USB host or device features if USB OTG FS is not enabled */

#ifndef CONFIG_STM32_OTGFS
#  undef HAVE_USBDEV
#  undef HAVE_USBHOST
#endif

/* Can't support USB device if USB device is not enabled */

#ifndef CONFIG_USBDEV
#  undef HAVE_USBDEV
#endif

/* Can't support USB host is USB host is not enabled */

#ifndef CONFIG_USBHOST
#  undef HAVE_USBHOST
#endif

/* Check if we should enable the USB monitor before starting NSH */

#ifndef CONFIG_USBMONITOR
#  undef HAVE_USBMONITOR
#endif

#ifndef HAVE_USBDEV
#  undef CONFIG_USBDEV_TRACE
#endif

#ifndef HAVE_USBHOST
#  undef CONFIG_USBHOST_TRACE
#endif

#if !defined(CONFIG_USBDEV_TRACE) && !defined(CONFIG_USBHOST_TRACE)
#  undef HAVE_USBMONITOR
#endif

/* NSH Network monitor  */

#if !defined(CONFIG_NET) || !defined(CONFIG_STM32_EMACMAC)
#  undef HAVE_NETMONITOR
#endif

#if !defined(CONFIG_NSH_NETINIT_THREAD) || !defined(CONFIG_ARCH_PHY_INTERRUPT) || \
    !defined(CONFIG_NETDEV_PHY_IOCTL) || !defined(CONFIG_NET_UDP)
#  undef HAVE_NETMONITOR
#endif

/* procfs File System */

#ifdef CONFIG_FS_PROCFS
#  ifdef CONFIG_NSH_PROC_MOUNTPOINT
#    define STM32_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#  else
#    define STM32_PROCFS_MOUNTPOINT "/proc"
#  endif
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Olimex-STM32-E407 GPIOs **************************************************/

/* LEDs */

#define GPIO_LED_STATUS   (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz|\
                           GPIO_OUTPUT_CLEAR|GPIO_PORTC|GPIO_PIN13)

/* Pin for MRF24J40 radio */

#define GPIO_MRF24J40_INT (GPIO_INPUT|GPIO_FLOAT|\
                           GPIO_EXTI|GPIO_PORTG|GPIO_PIN12) /* PG12 */
#define GPIO_MRF24J40_RST (GPIO_OUTPUT|GPIO_OPENDRAIN|GPIO_SPEED_50MHz|\
                           GPIO_OUTPUT_CLEAR|GPIO_PORTG|GPIO_PIN15) /* PG15 */

/* SPI chip selects */

#define GPIO_MRF24J40_CS  (GPIO_OUTPUT|GPIO_SPEED_50MHz|\
                           GPIO_OUTPUT_SET|GPIO_PORTA|GPIO_PIN4) /* PA4 */

/* BUTTONS -- NOTE that all have EXTI interrupts configured */

#define MIN_IRQBUTTON     BUTTON_BUT
#define MAX_IRQBUTTON     BUTTON_BUT
#define NUM_IRQBUTTONS    1

#define GPIO_BTN_BUT      (GPIO_INPUT|GPIO_FLOAT|GPIO_EXTI|GPIO_PORTA|GPIO_PIN0)

/* USB OTG FS - USB-A connector
 *
 * PC4  OTG_FS_VBUS VBUS sensing
 * PB0  OTG_FS_PowerSwitchOn
 * PB1  OTG_FS_Overcurrent
 */

#define GPIO_OTGFS_VBUS   (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|\
                           GPIO_OPENDRAIN|GPIO_PORTA|GPIO_PIN9)
#define GPIO_OTGFS_PWRON  (GPIO_OUTPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|\
                           GPIO_PUSHPULL|GPIO_PORTB|GPIO_PIN0)

#ifdef CONFIG_USBHOST
#  define GPIO_OTGFS_OVER (GPIO_INPUT|GPIO_EXTI|GPIO_FLOAT|\
                           GPIO_SPEED_100MHz|GPIO_PUSHPULL|\
                           GPIO_PORTB|GPIO_PIN1)

#else
#  define GPIO_OTGFS_OVER (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|\
                           GPIO_PUSHPULL|GPIO_PORTB|GPIO_PIN1)
#endif

/* USB OTG HS - miniUSB connector
 *
 * PB13  OTG_HS_VBUS VBUS sensing (also connected to the green LED)
 * PA8  OTG_HS_PowerSwitchOn
 * PF11  OTG_HS_Overcurrent
 */

#define GPIO_OTGHS_VBUS \
  (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_OPENDRAIN|GPIO_PORTB| \
   GPIO_PIN13)
#define GPIO_OTGHS_PWRON \
  (GPIO_OUTPUT|GPIO_OUTPUT_SET|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_PUSHPULL| \
   GPIO_PORTA|GPIO_PIN8)

#ifdef CONFIG_USBHOST
#  define GPIO_OTGHS_OVER \
  (GPIO_INPUT|GPIO_EXTI|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_PUSHPULL| \
   GPIO_PORTF|GPIO_PIN11)

#else
#  define GPIO_OTGHS_OVER \
  (GPIO_INPUT|GPIO_FLOAT|GPIO_SPEED_100MHz|GPIO_PUSHPULL|GPIO_PORTF| \
  GPIO_PIN11)
#endif

/* LAN8710 works with LAN8720 driver
 *
 * ---------- ------------- ------------
 * PIO        SIGNAL        Comments
 * ---------- ------------- ------------
 * PG11       TXEN
 * PG13       TXD0
 * PG14       TXD1
 * PC4        RXD0/MODE0
 * PC5        RXD1/MODE1
 * PA7        CRS_DIV/MODE2
 * PA2        MDIO
 * PC1        MDC
 * PA3        NINT/REFCLK0
 * PG6        NRST
 * ---------- ------------- ------------
 */

#if defined(CONFIG_STM32_ETHMAC)
#  define GPIO_EMAC_NINT  (GPIO_INPUT|GPIO_PULLUP|GPIO_EXTI| \
                           GPIO_PORTA|GPIO_PIN3)
#  define GPIO_EMAC_NRST  (GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_50MHz| \
                           GPIO_OUTPUT_SET|GPIO_PORTG|GPIO_PIN6)
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y :
 *     Called from board_late_initialize().
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y && CONFIG_BOARDCTL=y :
 *     Called from the NSH library
 *
 ****************************************************************************/

int stm32_bringup(void);

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called from stm32_usbinitialize very early in initialization to setup
 *   USB-related GPIO pins for the Olimex-STM32-H405 board.
 *
 ****************************************************************************/

#if defined(CONFIG_STM32_OTGFS) || defined(CONFIG_STM32_OTGHS)
void weak_function stm32_usbinitialize(void);
#endif

/****************************************************************************
 * Name: stm32_adc_setup
 *
 * Description:
 *   Initialize ADC and register the ADC driver.
 *
 ****************************************************************************/

#ifdef CONFIG_ADC
int stm32_adc_setup(void);
#endif

/****************************************************************************
 * Name: stm32_sdio_initialize
 *
 * Description:
 *   Initialize SDIO-based MMC/SD card support
 *
 ****************************************************************************/

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && defined(CONFIG_STM32_SDIO)
int stm32_sdio_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_can_setup
 *
 * Description:
 *  Initialize CAN and register the CAN device
 *
 ****************************************************************************/

#ifdef CONFIG_CAN
int stm32_can_setup(void);
#endif

/****************************************************************************
 * Name: stm32_dac_setup
 *
 * Description:
 *   Initialize and register the DAC0 of the microcontroller.
 *
 * Input parameters:
 *   devpath - The full path to the driver to register. E.g., "/dev/dac0"
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

#if defined(CONFIG_DAC)
int stm32_dac_setup(void);
#endif

/****************************************************************************
 * Name: stm32_timer_driver_setup
 *
 * Description:
 *   Configure the timer driver.
 *
 * Input Parameters:
 *   devpath - The full path to the timer device.  This should be of the
 *             form /dev/timer0
 *   timer   - The timer's number.
 *
 * Returned Values:
 *   Zero (OK) is returned on success; A negated errno value is returned
 *   to indicate the nature of any failure.
 *
 ****************************************************************************/

#ifdef CONFIG_TIMER
int stm32_timer_driver_setup(const char *devpath, int timer);
#endif

/****************************************************************************
 * Name: stm32_mrf24j40_initialize
 *
 * Description:
 *   Initialize the MRF24J40 device.
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

#ifdef CONFIG_IEEE802154_MRF24J40
int stm32_mrf24j40_initialize(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __BOARDS_ARM_STM32_OLIMEX_STM32_E407_SRC_OLIMEX_STM32_E407_H */
