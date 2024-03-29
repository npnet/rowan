/*
 * Copyright (c) 2015, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
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
#include <arch_helpers.h>
#include <mcucfg.h>
#include <mmio.h>
#include <mt8167_def.h>
#include <mt_cpuxgpt.h>

uint64_t	normal_time_base;

static void write_cpuxgpt(unsigned int reg_index, unsigned int value)
{
	mmio_write_32((uintptr_t)&mt8167_mcucfg->xgpt_idx, reg_index);
	mmio_write_32((uintptr_t)&mt8167_mcucfg->xgpt_ctl, value);
}

static void cpuxgpt_set_init_cnt(unsigned int countH, unsigned int countL)
{
	write_cpuxgpt(INDEX_CNT_H_INIT, countH);
	/* update count when countL programmed */
	write_cpuxgpt(INDEX_CNT_L_INIT, countL);
}

void generic_timer_backup(void)
{
	uint64_t cval;

	cval = read_cntpct_el0();
	cpuxgpt_set_init_cnt((uint32_t)(cval >> 32),
			       (uint32_t)(cval & 0xffffffff));
}

uint64_t atf_sched_clock(void)
{
	uint64_t cval;

	cval = ((read_cntpct_el0() * 1000) / SYS_COUNTER_FREQ_IN_MHZ);
	return cval;
}

