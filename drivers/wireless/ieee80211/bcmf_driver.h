/****************************************************************************
 * drivers/wireless/ieee80211/bcmf_driver.h
 *
 *   Copyright (C) 2017 Gregory Nutt. All rights reserved.
 *   Author: Simon Piriou <spiriou31@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __DRIVERS_WIRELESS_IEEE80211_BCMF_DRIVER_H
#define __DRIVERS_WIRELESS_IEEE80211_BCMF_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <semaphore.h>

#define BCMF_STATUS_BUS_UP (1<<0) /* Chip is flashed and running */
#define BCMF_STATUS_READY  (1<<1) /* Chip is ready to receive requests */

#define BCMF_STATUS_SLEEP  (1<<2) /* Chip is in low power mode */
#define BCMF_STATUS_WAIT_CONTROL (1<<3) /* Waiting for control response */

struct bcmf_bus_dev_s;
struct bcmf_frame_s;

/* This structure contains the unique state of the Broadcom FullMAC driver */

struct bcmf_dev_s
{
  FAR struct bcmf_bus_dev_s *bus;  /* Bus interface structure */

  // FIXME use mutex instead of semaphore
  sem_t control_mutex;         /* Cannot handle multiple control requests */
  sem_t control_timeout;       /* Semaphore to wait for control frame rsp */
  uint16_t control_reqid;      /* Current control request id */
  uint16_t control_rxdata_len; /* Received control frame out buffer length */
  uint8_t *control_rxdata;     /* Received control frame out buffer */
  uint32_t control_status;     /* Last received frame status */

  uint8_t mac_addr[6];         /* Current mac address */
};

/* Default bus interface structure */

struct bcmf_bus_dev_s {
  void (*stop)(FAR struct bcmf_dev_s *priv);
  int (*txframe)(FAR struct bcmf_dev_s *priv, struct bcmf_frame_s *frame);

  /* Frame buffer allocation primitive
   * len     - requested payload length
   * control - true if control frame else false
   * block   - true to block until free frame is available
   */
  struct bcmf_frame_s* (*allocate_frame)(FAR struct bcmf_dev_s *priv,
          unsigned int len, bool control, bool block);
};

/* bcmf frame definition */

struct bcmf_frame_s {
  uint8_t *base;    /* Frame base buffer used by low level layer (SDIO) */
  uint8_t *data;    /* Payload data (Control, data and event messages) */
  unsigned int len; /* Frame buffer size */
};

/* Notify driver frame is available */

void bcmf_notify_rxframe(FAR struct bcmf_dev_s *priv,
                         struct bcmf_frame_s *frame);

/* Notify driver bus is ready */

int brcmf_bus_start(FAR struct bcmf_dev_s *priv);

#endif /* __DRIVERS_WIRELESS_IEEE80211_BCMF_DRIVER_H */
