// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2019 Picocom Technology Corporation
 * Wei Zheng, Picocom Technology Corporation <wei.zheng@picocom.com>
 */

#include <asm/csr.h>
#include <asm/sbi.h>
#include <common.h>
#include <flash.h>
#include <image.h>
#include <init.h>
#include <net.h>
#if defined(CONFIG_FTMAC100) && !defined(CONFIG_DM_ETH)
#include <netdev.h>
#endif
#include <linux/io.h>
#include <faraday/ftsmc020.h>
#include <fdtdec.h>
#include <dm.h>
#include <spl.h>
#include <mtd.h>
#include <nand.h>


DECLARE_GLOBAL_DATA_PTR;

extern phys_addr_t prior_stage_fdt_address;
/*
 * Miscellaneous platform dependent initializations
 */

int misc_init_r(void)
{
	char cpu[] = "pc805";
	long csr_marchid = 0;
	u16 mask64 = 0x8000;
#if CONFIG_IS_ENABLED(RISCV_SMODE)
	csr_marchid = sbi_get_marchid();
#elif CONFIG_IS_ENABLED(RISCV_MMODE)
	csr_marchid = csr_read(CSR_MARCHID);
#endif
	snprintf(cpu, strlen(cpu)+1, "%x", (u16)csr_marchid);
	if(csr_marchid & mask64){
		cpu[0] = cpu[1];
		cpu[1] = 'x';
	}

	return env_set("cpu", cpu);
}

int board_init(void)
{
	//gd->bd->bi_boot_params = PHYS_SDRAM_0 + 0x400;

	return 0;
}

int dram_init(void)
{
	return fdtdec_setup_mem_size_base();
}

int dram_init_banksize(void)
{
	return fdtdec_setup_memory_banksize();
}

#if defined(CONFIG_FTMAC100) && !defined(CONFIG_DM_ETH)
int board_eth_init(struct bd_info *bd)
{
	return ftmac100_initialize(bd);
}
#endif

ulong board_flash_get_legacy(ulong base, int banknum, flash_info_t *info)
{
	return 0;
}

#if 0
void *board_fdt_blob_setup(void)
{
	return (void *)CONFIG_SYS_FDT_BASE;
}
#endif

int smc_init(void)
{
	int node = -1;
	const char *compat = "andestech,atfsmc020";
	void *blob = (void *)gd->fdt_blob;
	fdt_addr_t addr;
	struct ftsmc020_bank *regs;

	node = fdt_node_offset_by_compatible(blob, -1, compat);
	if (node < 0)
		return -FDT_ERR_NOTFOUND;

	addr = fdtdec_get_addr_size_auto_noparent(blob, node,
		"reg", 0, NULL, false);

	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	regs = (struct ftsmc020_bank *)addr;
	regs->cr &= ~FTSMC020_BANK_WPROT;

	return 0;
}

#ifdef CONFIG_V5L2_CACHE
static void v5l2_init(void)
{
	struct udevice *dev;

	uclass_get_device(UCLASS_CACHE, 0, &dev);
}
#endif

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef CONFIG_V5L2_CACHE
	v5l2_init();
#endif
	return 0;
}
#endif

#ifdef CONFIG_BOARD_EARLY_INIT_F
int board_early_init_f(void)
{
	smc_init();
#ifdef CONFIG_V5L2_CACHE
	v5l2_init();
#endif

	return 0;
}
#endif

/*
 * Main initialization routine
 */
void board_nand_init(void)
{
	struct mtd_info *mtd;
	struct udevice *dev;
	int ret;

	ret = uclass_get_device_by_driver(UCLASS_MTD,
					  DM_GET_DRIVER(spinand), &dev);
	if (ret && ret != -ENODEV) {
		pr_err("Failed to initialize %s. (error %d)\n", "spinand", ret);
		return;
	}

	mtd = dev_get_uclass_priv(dev);
	nand_register(0, mtd);

	nand_curr_device = 0;
}
