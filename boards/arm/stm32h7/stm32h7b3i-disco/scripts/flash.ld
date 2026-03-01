/****************************************************************************
 * boards/arm/stm32h7/stm32h7b3i-disco/scripts/flash.ld
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

/* The STM32H7B3LI has 2048Kb of main FLASH memory. The flash memory is
 * partitioned into a User Flash memory and a System Flash memory.
 *
 *   1) User Flash memory:
 *
 *      Start address 0x0800:0000 to 0x081F:FFFF with 16 sectors, 128Kb each
 *
 *   2) System Flash memory:
 *
 *      Start address 0x1FF0:0000 to 0x1FF1:FFFF with 1 x 128Kb sector
 *
 *   3) User option bytes for user configuration, only in Bank 1.
 *
 * The STM32H7B3LI also has 1376Kb of SRAM:
 *
 *   1) TCM SRAMs:
 *      1.1) 128Kb of DTCM-RAM at 0x2000:0000
 *      1.2)  64Kb of ITCM-RAM at 0x0000:0000
 *
 *   2) AXI SRAM (D1 domain):
 *      2.1) 1024Kb of SRAM at 0x2400:0000
 *
 *   3) AHB SRAM (D2 domain):
 *      3.1) 128Kb of SRAM1 at 0x3000:0000
 *      3.2) 128Kb of SRAM2 at 0x3002:0000
 *
 *   4) AHB SRAM (D3 domain):
 *      4.1)  64Kb of SRAM4 at 0x3800:0000
 *      4.2)   4Kb of backup RAM at 0x3880:0000
 */

MEMORY
{
  itcm  (rwx) : ORIGIN = 0x00000000, LENGTH =   64K
  flash (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
  dtcm  (rwx) : ORIGIN = 0x20000000, LENGTH =  128K
  sram  (rwx) : ORIGIN = 0x24000000, LENGTH = 1024K
  sram1 (rwx) : ORIGIN = 0x30000000, LENGTH =  128K
  sram2 (rwx) : ORIGIN = 0x30020000, LENGTH =  128K
  sram4 (rwx) : ORIGIN = 0x38000000, LENGTH =   64K
  bbram (rwx) : ORIGIN = 0x38800000, LENGTH =    4K
}

OUTPUT_ARCH(arm)
EXTERN(_vectors)
ENTRY(_stext)
SECTIONS
{
    .text :
    {
        _stext = ABSOLUTE(.);
        *(.vectors)
        *(.text .text.*)
        *(.fixup)
        *(.gnu.warning)
        *(.rodata .rodata.*)
        *(.gnu.linkonce.t.*)
        *(.glue_7)
        *(.glue_7t)
        *(.got)
        *(.gcc_except_table)
        *(.gnu.linkonce.r.*)
        _etext = ABSOLUTE(.);
    } > flash

    .init_section :
    {
        _sinit = ABSOLUTE(.);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o) .ctors))
        _einit = ABSOLUTE(.);
    } > flash

    .ARM.extab :
    {
        *(.ARM.extab*)
    } > flash

    __exidx_start = ABSOLUTE(.);
    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > flash
    __exidx_end = ABSOLUTE(.);

    _eronly = ABSOLUTE(.);

    .data :
    {
        _sdata = ABSOLUTE(.);
        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        CONSTRUCTORS
        . = ALIGN(4);
        _edata = ABSOLUTE(.);
    } > sram AT > flash

    .bss :
    {
        _sbss = ABSOLUTE(.);
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = ABSOLUTE(.);
    } > sram

    /* Emit the D3 power domain section for locating BDMA data
     *
     * Static data with locate_data(".sram4") will be located
     * at start of SRAM4; the rest of SRAM4 will be added to the heap.
     */

    .sram4_reserve (NOLOAD) :
    {
        *(.sram4)
        . = ALIGN(4);
        _sram4_heap_start = ABSOLUTE(.);
    } > sram4

    /* Stabs debugging sections. */

    .stab 0 : { *(.stab) }
    .stabstr 0 : { *(.stabstr) }
    .stab.excl 0 : { *(.stab.excl) }
    .stab.exclstr 0 : { *(.stab.exclstr) }
    .stab.index 0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 : { *(.comment) }
    .debug_abbrev 0 : { *(.debug_abbrev) }
    .debug_info 0 : { *(.debug_info) }
    .debug_line 0 : { *(.debug_line) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_aranges 0 : { *(.debug_aranges) }
}
