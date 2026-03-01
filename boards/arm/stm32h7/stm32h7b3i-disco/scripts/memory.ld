/****************************************************************************
 * boards/arm/stm32h7/stm32h7b3i-disco/scripts/memory.ld
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

/* STM32H7B3LI Memory Layout:
 *
 * FLASH: 2048KB total
 * SRAM:  1376KB total
 *   - 64KB ITCM-RAM at 0x00000000
 *   - 128KB DTCM-RAM at 0x20000000
 *   - 1024KB AXI SRAM at 0x24000000
 *   - 128KB SRAM1 at 0x30000000
 *   - 128KB SRAM2 at 0x30020000
 *   - 64KB SRAM4 at 0x38000000
 */

MEMORY
{
  /* ITCM boot address */

  itcm  (rwx) : ORIGIN = 0x00000000, LENGTH = 64K

  /* 2048Kb FLASH */

  kflash (rx) : ORIGIN = 0x08000000, LENGTH = 128K
  uflash (rx) : ORIGIN = 0x08020000, LENGTH = 128K
  xflash (rx) : ORIGIN = 0x08040000, LENGTH = 1792K

  /* 256Kb SRAM1+SRAM2 (D2 domain) */

  ksram (rwx) : ORIGIN = 0x30000000, LENGTH = 16K
  usram (rwx) : ORIGIN = 0x30004000, LENGTH = 16K
  xsram (rwx) : ORIGIN = 0x30008000, LENGTH = 256K - 32K

  /* 1024Kb of contiguous AXI SRAM (D1 domain) */

  sram (rwx)  : ORIGIN = 0x24000000, LENGTH = 1024K

  /* DTCM SRAM */

  dtcm  (rwx) : ORIGIN = 0x20000000, LENGTH = 128K

  /* SRAM4 (D3 domain) and backup RAM */

  sram4 (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
  bbram (rwx) : ORIGIN = 0x38800000, LENGTH = 4K
}
