/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2019 Picocom Technology Corporation
 * Wei Zheng, Picocom Technology Corporation <wei.zheng@picocom.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

#ifdef CONFIG_SPL
#define CONFIG_SPL_MAX_SIZE		    0x0001C000

#if 0
#define CONFIG_SPL_BSS_START_ADDR   0x10320000
#define CONFIG_SPL_BSS_MAX_SIZE     0x00008000
#define CONFIG_SPL_STACK            0x10330000
#define CONFIG_SYS_SPL_MALLOC_START 0x10350000
#define CONFIG_SYS_SPL_MALLOC_SIZE  0x00010000
#define CONFIG_MALLOC_F_ADDR        0x10340000
#else
#define CONFIG_SPL_BSS_START_ADDR   0x10120000
#define CONFIG_SPL_BSS_MAX_SIZE     0x00008000
#define CONFIG_SPL_STACK            0x10130000
#define CONFIG_SYS_SPL_MALLOC_START 0x10150000
#define CONFIG_SYS_SPL_MALLOC_SIZE  0x00010000
#define CONFIG_MALLOC_F_ADDR        0x10140000
#endif

#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_TEXT_BASE + SZ_1M)

#ifdef CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME		"u-boot.itb"
#endif

#else

#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_TEXT_BASE + SZ_1M)
//#define CONFIG_SYS_MALLOC_LEN       SZ_1M

#endif

/*
 * CPU and Board Configuration Options
 */

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_CBSIZE	1024	/* Console I/O Buffer Size */

/*
 * Print Buffer Size
 */
#define CONFIG_SYS_PBSIZE	\
	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/*
 * max number of command args
 */
#define CONFIG_SYS_MAXARGS	16

/*
 * Boot Argument Buffer Size
 */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE

/*
 * Size of malloc() pool
 * 512kB is suggested, (CONFIG_ENV_SIZE + 128 * 1024) was not enough
 */
#define CONFIG_SYS_MALLOC_LEN   (512 << 10)
//#define CONFIG_SYS_NONCACHED_MEMORY	SZ_1M

/* DT blob (fdt) address */
//#define CONFIG_SYS_FDT_BASE		0x800f0000

#if 0
/*
 * Physical Memory Map
 */
#define PHYS_SDRAM_0	0x00000000		/* SDRAM Bank #1 */
#define PHYS_SDRAM_1	\
	(PHYS_SDRAM_0 + PHYS_SDRAM_0_SIZE)	/* SDRAM Bank #2 */
#define PHYS_SDRAM_0_SIZE	0x20000000	/* 512 MB */
#define PHYS_SDRAM_1_SIZE	0x20000000	/* 512 MB */
#define CONFIG_SYS_SDRAM_BASE	PHYS_SDRAM_0
#endif

/*
 * Serial console configuration
 */
//#define CONFIG_SYS_NS16550_SERIAL
#ifndef CONFIG_DM_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	-4
#endif
#define CONFIG_SYS_NS16550_CLK		325000000


#if 0
/* Init Stack Pointer */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x10000000 - \
					GENERATED_GBL_DATA_SIZE)

/*
 * Load address and memory test area should agree with
 * arch/riscv/config.mk. Be careful not to overwrite U-Boot itself.
 */
#define CONFIG_SYS_LOAD_ADDR		0x100000	/* SDRAM */
#endif

/*
 * memtest works on 512 MB in DRAM
 */

/*
 * FLASH and environment organization
 */

/* use CFI framework */

#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#define CONFIG_SYS_CFI_FLASH_STATUS_POLL

/* support JEDEC */
#ifdef CONFIG_CFI_FLASH
#define CONFIG_SYS_MAX_FLASH_BANKS_DETECT	1
#endif/* Do not use CONFIG_FLASH_CFI_LEGACY to detect on board flash */
#define PHYS_FLASH_1			0x88000000	/* BANK 0 */
#define CONFIG_SYS_FLASH_BASE		PHYS_FLASH_1
#define CONFIG_SYS_FLASH_BANKS_LIST	{ PHYS_FLASH_1, }
#define CONFIG_SYS_MONITOR_BASE		PHYS_FLASH_1

#define CONFIG_SYS_FLASH_ERASE_TOUT	120000	/* TO for Flash Erase (ms) */
#define CONFIG_SYS_FLASH_WRITE_TOUT	500	/* TO for Flash Write (ms) */

/* max number of memory banks */
/*
 * There are 4 banks supported for this Controller,
 * but we have only 1 bank connected to flash on board
*/
#ifndef CONFIG_SYS_MAX_FLASH_BANKS_DETECT
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#endif
#define CONFIG_SYS_FLASH_BANKS_SIZES {0x4000000}

/* max number of sectors on one chip */
#define CONFIG_FLASH_SECTOR_SIZE	(0x10000*2)
#define CONFIG_SYS_MAX_FLASH_SECT	512

/* environments */

/* SPI FLASH */

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 16 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */

/* Initial Memory map for Linux*/
#define CONFIG_SYS_BOOTMAPSZ	(64 << 20)
/* Increase max gunzip size */
#define CONFIG_SYS_BOOTM_LEN	(64 << 20)

/* When we use RAM as ENV */

#if 0
/* Enable distro boot */
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(DHCP, dhcp, na)
#include <config_distro_bootcmd.h>
#endif

#define CONFIG_EXTRA_ENV_SETTINGS	\
				"opensbi_addr_r=0x80000000\0" \
				"kernel_addr_r=0x80400000\0" \
				"pxefile_addr_r=0x01f00000\0" \
				"scriptaddr=0x01f00000\0" \
				"fdt_addr_r=0x82200000\0" \
				"ramdisk_addr_r=0x02800000\0" \
				"fdt_high=0xffffffffffffffff\0"
				//"ramdisk_addr_r=0x02800000\0"
				//BOOTENV

#endif /* __CONFIG_H */

#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_SPL_NAND_RAW_ONLY
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x100000
#define CONFIG_SYS_NAND_U_BOOT_SIZE	0xc0000
