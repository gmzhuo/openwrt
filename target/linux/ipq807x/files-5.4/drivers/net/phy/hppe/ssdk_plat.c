/*
 * Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*qca808x_start*/
#include <linux/io.h>
#include "sw.h"
#include "sw.h"
#include "ssdk_init.h"
#include "fal_init.h"
#include "init/ssdk_plat.h"
#include "hppe_init.h"
#define QCA_MAX_PHY_NUM 4

struct qca_phy_priv *qca_phy_priv_global[QCA_MAX_PHY_NUM];
a_uint32_t m_used_dev_id = 0;

a_uint32_t add_qca_phy_dev(struct qca_phy_priv *priv)
{
	a_uint32_t tempid;

	if(m_used_dev_id >= QCA_MAX_PHY_NUM)
		return -1;

	tempid = m_used_dev_id++;
	qca_phy_priv_global[tempid] = priv;

	return tempid;
}

sw_error_t
qca_switch_reg_read(a_uint32_t dev_id, a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
	uint32_t reg_val = 0;

	if (len != sizeof (a_uint32_t))
        return SW_BAD_LEN;

	if ((reg_addr%4)!= 0)
	return SW_BAD_PARAM;

	reg_val = readl(qca_phy_priv_global[dev_id]->hw_addr + reg_addr);

	aos_mem_copy(reg_data, &reg_val, sizeof (a_uint32_t));
	return 0;
}

sw_error_t
qca_switch_reg_write(a_uint32_t dev_id, a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
	uint32_t reg_val = 0;
	if (len != sizeof (a_uint32_t))
        return SW_BAD_LEN;

	if ((reg_addr%4)!= 0)
	return SW_BAD_PARAM;

	aos_mem_copy(&reg_val, reg_data, sizeof (a_uint32_t));
	writel(reg_val, qca_phy_priv_global[dev_id]->hw_addr + reg_addr);
	return 0;
}

sw_error_t
qca_psgmii_reg_read(a_uint32_t dev_id, a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
	uint32_t reg_val = 0;

	if (len != sizeof (a_uint32_t))
        return SW_BAD_LEN;

	if((reg_addr%4)!=0)
	return SW_BAD_PARAM;

	if (qca_phy_priv_global[dev_id]->psgmii_hw_addr == NULL)
		return SW_NOT_SUPPORTED;

	reg_val = readl(qca_phy_priv_global[dev_id]->psgmii_hw_addr + reg_addr);

	aos_mem_copy(reg_data, &reg_val, sizeof (a_uint32_t));
	return 0;
}

sw_error_t
qca_psgmii_reg_write(a_uint32_t dev_id, a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
	uint32_t reg_val = 0;
	if (len != sizeof (a_uint32_t))
        return SW_BAD_LEN;

	if((reg_addr%4)!=0)
	return SW_BAD_PARAM;

	if (qca_phy_priv_global[dev_id]->psgmii_hw_addr == NULL)
		return SW_NOT_SUPPORTED;

	aos_mem_copy(&reg_val, reg_data, sizeof (a_uint32_t));
	writel(reg_val, qca_phy_priv_global[dev_id]->psgmii_hw_addr + reg_addr);
	return 0;
}

sw_error_t
qca_uniphy_reg_read(a_uint32_t dev_id, a_uint32_t uniphy_index,
				a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
	uint32_t reg_val = 0;
	void __iomem *hppe_uniphy_base = NULL;
	a_uint32_t reg_addr1, reg_addr2;

	if (len != sizeof (a_uint32_t))
        return SW_BAD_LEN;

	if (SSDK_UNIPHY_INSTANCE0 == uniphy_index)
		hppe_uniphy_base = qca_phy_priv_global[dev_id]->uniphy_hw_addr;
	else if (SSDK_UNIPHY_INSTANCE1 == uniphy_index)
		hppe_uniphy_base = qca_phy_priv_global[dev_id]->uniphy_hw_addr + HPPE_UNIPHY_BASE1;

	else if (SSDK_UNIPHY_INSTANCE2 == uniphy_index)
		hppe_uniphy_base = qca_phy_priv_global[dev_id]->uniphy_hw_addr + HPPE_UNIPHY_BASE2;
	else
		return SW_BAD_PARAM;

	if ( reg_addr > HPPE_UNIPHY_MAX_DIRECT_ACCESS_REG)
	{
		// uniphy reg indireclty access
		reg_addr1 = (reg_addr & 0xffffff) >> 8;
		writel(reg_addr1, hppe_uniphy_base + HPPE_UNIPHY_INDIRECT_REG_ADDR);

		reg_addr2 = reg_addr & HPPE_UNIPHY_INDIRECT_LOW_ADDR;
		reg_addr = (HPPE_UNIPHY_INDIRECT_DATA << 10) | (reg_addr2 << 2);

		reg_val = readl(hppe_uniphy_base + reg_addr);
		aos_mem_copy(reg_data, &reg_val, sizeof (a_uint32_t));
	}
	else
	{	// uniphy reg directly access
		reg_val = readl(hppe_uniphy_base + reg_addr);
		aos_mem_copy(reg_data, &reg_val, sizeof (a_uint32_t));
	}
	return 0;
}

sw_error_t
qca_uniphy_reg_write(a_uint32_t dev_id, a_uint32_t uniphy_index,
				a_uint32_t reg_addr, a_uint8_t * reg_data, a_uint32_t len)
{
	void __iomem *hppe_uniphy_base = NULL;
	a_uint32_t reg_addr1, reg_addr2;
	uint32_t reg_val = 0;

	if (len != sizeof (a_uint32_t))
        return SW_BAD_LEN;

	if (SSDK_UNIPHY_INSTANCE0 == uniphy_index)
		hppe_uniphy_base = qca_phy_priv_global[dev_id]->uniphy_hw_addr;
	else if (SSDK_UNIPHY_INSTANCE1 == uniphy_index)
		hppe_uniphy_base = qca_phy_priv_global[dev_id]->uniphy_hw_addr + HPPE_UNIPHY_BASE1;

	else if (SSDK_UNIPHY_INSTANCE2 == uniphy_index)
		hppe_uniphy_base = qca_phy_priv_global[dev_id]->uniphy_hw_addr + HPPE_UNIPHY_BASE2;
	else
		return SW_BAD_PARAM;

	if ( reg_addr > HPPE_UNIPHY_MAX_DIRECT_ACCESS_REG)
	{
		// uniphy reg indireclty access
		reg_addr1 = (reg_addr & 0xffffff) >> 8;
		writel(reg_addr1, hppe_uniphy_base + HPPE_UNIPHY_INDIRECT_REG_ADDR);

		reg_addr2 = reg_addr & HPPE_UNIPHY_INDIRECT_LOW_ADDR;
		reg_addr = (HPPE_UNIPHY_INDIRECT_DATA << 10) | (reg_addr2 << 2);
		aos_mem_copy(&reg_val, reg_data, sizeof (a_uint32_t));
		writel(reg_val, hppe_uniphy_base + reg_addr);
	}
	else
	{	// uniphy reg directly access
		aos_mem_copy(&reg_val, reg_data, sizeof (a_uint32_t));
		writel(reg_val, hppe_uniphy_base + reg_addr);
	}
	return 0;
}


