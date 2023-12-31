// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2017 Andes Technology Corporation
 * Rick Chen, Andes Technology Corporation <rick@andestech.com>
 * Wei Zheng, Picocom Technology Corporation <wei.zheng@picocom.com>
 */

/* CPU specific code */
#include <common.h>
#include <cpu_func.h>
#include <irq_func.h>
#include <asm/cache.h>
#include <asm/csr.h>


#define CSR_MCACHE_CTL	0x7ca
#define CSR_MMISC_CTL	0x7d0
#define CSR_MARCHID		0xf12

#define V5_MCACHE_CTL_IC_EN_OFFSET      0
#define V5_MCACHE_CTL_DC_EN_OFFSET      1
#define V5_MCACHE_CTL_DC_COHEN_OFFSET	19
#define V5_MCACHE_CTL_DC_COHSTA_OFFSET	20

#define V5_MCACHE_CTL_IC_EN     		(1UL << V5_MCACHE_CTL_IC_EN_OFFSET)
#define V5_MCACHE_CTL_DC_EN				(1UL << V5_MCACHE_CTL_DC_EN_OFFSET)
#define V5_MCACHE_CTL_DC_COHEN_EN       (1UL << V5_MCACHE_CTL_DC_COHEN_OFFSET)
#define V5_MCACHE_CTL_DC_COHSTA_EN      (1UL << V5_MCACHE_CTL_DC_COHSTA_OFFSET)


/*
 * cleanup_before_linux() is called just before we call linux
 * it prepares the processor for linux
 *
 * we disable interrupt and caches.
 */
int cleanup_before_linux(void)
{
	disable_interrupts();

	/* turn off I/D-cache */
#if !CONFIG_IS_ENABLED(RISCV_SMODE)
	cache_flush();
	icache_disable();
	dcache_disable();
#else
	invalidate_icache_all();
	if(!icache_status())
		icache_enable();

	if(!dcache_status())
		dcache_enable();
#endif

	return 0;
}


void harts_early_init(void)
{
#if 0
	if (CONFIG_IS_ENABLED(RISCV_MMODE)) {
		unsigned long long marchid = csr_read(CSR_MARCHID);
		unsigned long long mcache_ctl_val = csr_read(CSR_MCACHE_CTL);
		unsigned long long bitmap_mask = 0xFF;
		unsigned long long bitmap = marchid & bitmap_mask;

		if(bitmap == 0x45){
            printf("harts_early_init cache op?\n");
			if (!(mcache_ctl_val & V5_MCACHE_CTL_DC_COHEN_EN))
				mcache_ctl_val |= V5_MCACHE_CTL_DC_COHEN_EN;
			if (!(mcache_ctl_val & V5_MCACHE_CTL_IC_EN))
				mcache_ctl_val |= V5_MCACHE_CTL_IC_EN;
			if (!(mcache_ctl_val & V5_MCACHE_CTL_DC_EN))
				mcache_ctl_val |= V5_MCACHE_CTL_DC_EN;

			csr_write(CSR_MCACHE_CTL, mcache_ctl_val);

			/*
			* Check DC_COHEN_EN, if cannot write to mcache_ctl,
			* we assume this bitmap not support L2 CM
			*/
			mcache_ctl_val = csr_read(CSR_MCACHE_CTL);
			if ((mcache_ctl_val & V5_MCACHE_CTL_DC_COHEN_EN)) {
			/* Wait for DC_COHSTA bit be set */
				while (!(mcache_ctl_val & V5_MCACHE_CTL_DC_COHSTA_EN))
					mcache_ctl_val = csr_read(CSR_MCACHE_CTL);
			}
		}
	}
#endif
}
