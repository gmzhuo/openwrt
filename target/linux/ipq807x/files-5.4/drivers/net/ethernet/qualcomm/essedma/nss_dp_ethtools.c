/*
 **************************************************************************
 * Copyright (c) 2017-2019, The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **************************************************************************
 */

#include <linux/ethtool.h>
#include <linux/phy.h>
#include <linux/version.h>
#include "nss_dp_dev.h"
#ifdef NSS_FAL_SUPPORT
#include "fal/fal_port_ctrl.h"
#endif

/*
 * nss_dp_get_ethtool_stats()
 */
static void nss_dp_get_ethtool_stats(struct net_device *netdev,
				struct ethtool_stats *stats, uint64_t *data)
{
#ifdef NSS_FAL_SUPPORT
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);

	dp_priv->gmac_hal_ops->getethtoolstats(dp_priv->gmac_hal_ctx, data);
#endif
}

/*
 * nss_dp_get_strset_count()
 */
static int32_t nss_dp_get_strset_count(struct net_device *netdev, int32_t sset)
{
#ifdef NSS_FAL_SUPPORT
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);

	return dp_priv->gmac_hal_ops->getssetcount(dp_priv->gmac_hal_ctx, sset);
#else
	return 0;
#endif
}

/*
 * nss_dp_get_strings()
 */
static void nss_dp_get_strings(struct net_device *netdev, uint32_t stringset,
			uint8_t *data)
{
#ifdef NSS_FAL_SUPPORT
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);

	dp_priv->gmac_hal_ops->getstrings(dp_priv->gmac_hal_ctx, stringset,
					  data);
#endif
}

/*
 * nss_dp_get_settings()
 */
static int32_t nss_dp_get_settings(struct net_device *netdev,
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5,4,0))
				   struct ethtool_link_ksettings *cmd)
#else
				   struct ethtool_cmd *cmd)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5,4,0))
	return phy_ethtool_get_link_ksettings(netdev, cmd);
#else
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);

	/*
	 * If there is a PHY attached, get the status from Kernel helper
	 */
	if (dp_priv->phydev)
		return phy_ethtool_gset(dp_priv->phydev, cmd);

	return -EIO;
#endif
}

/*
 * nss_dp_set_settings()
 */
static int nss_dp_set_settings(struct net_device *netdev,
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5,4,0))
				  const struct ethtool_link_ksettings *cmd)
#else
				   struct ethtool_cmd *cmd)
#endif
{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(5,4,0))
	return phy_ethtool_set_link_ksettings(netdev, cmd);
#else
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);

	if (!dp_priv->phydev)
		return -EIO;

	return phy_ethtool_sset(dp_priv->phydev, cmd);
#endif
}

/*
 * nss_dp_get_pauseparam()
 */
static void nss_dp_get_pauseparam(struct net_device *netdev,
				     struct ethtool_pauseparam *pause)
{
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);

	pause->rx_pause = dp_priv->pause & FLOW_CTRL_RX ? 1 : 0;
	pause->tx_pause = dp_priv->pause & FLOW_CTRL_TX ? 1 : 0;
	pause->autoneg = AUTONEG_ENABLE;
}

/*
 * nss_dp_set_pauseparam()
 */
static int32_t nss_dp_set_pauseparam(struct net_device *netdev,
				     struct ethtool_pauseparam *pause)
{
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);

	/* set flow control settings */
	dp_priv->pause = 0;
	if (pause->rx_pause)
		dp_priv->pause |= FLOW_CTRL_RX;

	if (pause->tx_pause)
		dp_priv->pause |= FLOW_CTRL_TX;

	if (!dp_priv->phydev)
		return 0;

#if (LINUX_VERSION_CODE > KERNEL_VERSION(5,4,0))
	linkmode_clear_bit(ETHTOOL_LINK_MODE_Pause_BIT, 
								dp_priv->phydev->advertising);
	linkmode_clear_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, 
								dp_priv->phydev->advertising);

	if (pause->rx_pause) {
		linkmode_set_bit(ETHTOOL_LINK_MODE_Pause_BIT, dp_priv->phydev->advertising);
		linkmode_set_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, dp_priv->phydev->advertising);
	}

	if (pause->tx_pause) {
		linkmode_set_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, dp_priv->phydev->advertising);
	}

#else
	/* Update flow control advertisment */
	dp_priv->phydev->advertising &=
				~(ADVERTISED_Pause | ADVERTISED_Asym_Pause);

	if (pause->rx_pause)
		dp_priv->phydev->advertising |=
				(ADVERTISED_Pause | ADVERTISED_Asym_Pause);

	if (pause->tx_pause)
		dp_priv->phydev->advertising |= ADVERTISED_Asym_Pause;
#endif

	genphy_config_aneg(dp_priv->phydev);

	return 0;
}

/*
 * nss_dp_fal_to_ethtool_linkmode_xlate()
 *	Translate linkmode from FAL type to ethtool type.
 */
static inline void nss_dp_fal_to_ethtool_linkmode_xlate(uint32_t *xlate_to, uint32_t *xlate_from)
{
#ifdef NSS_FAL_SUPPORT
	uint32_t pos;

	while (*xlate_from) {
		pos = ffs(*xlate_from);
		switch (1 << (pos - 1)) {
		case FAL_PHY_EEE_10BASE_T:
			*xlate_to |= SUPPORTED_10baseT_Full;
			break;

		case FAL_PHY_EEE_100BASE_T:
			*xlate_to |= SUPPORTED_100baseT_Full;
			break;

		case FAL_PHY_EEE_1000BASE_T:
			*xlate_to |= SUPPORTED_1000baseT_Full;
			break;

		case FAL_PHY_EEE_2500BASE_T:
			*xlate_to |= SUPPORTED_2500baseX_Full;
			break;

		case FAL_PHY_EEE_5000BASE_T:
			/*
			 * Ethtool does not support enumeration for 5G.
			 */
			break;

		case FAL_PHY_EEE_10000BASE_T:
			*xlate_to |= SUPPORTED_10000baseT_Full;
			break;
		}

		*xlate_from &= (~(1 << (pos - 1)));
	}
#else
	*xlate_to |= SUPPORTED_10baseT_Full;
	*xlate_to |= SUPPORTED_100baseT_Full;
	*xlate_to |= SUPPORTED_1000baseT_Full;
#endif
}

/*
 * nss_dp_get_eee()
 *	Get EEE settings.
 */
static int32_t nss_dp_get_eee(struct net_device *netdev, struct ethtool_eee *eee)
{
#ifdef NSS_FAL_SUPPORT
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);
	fal_port_eee_cfg_t port_eee_cfg;
	uint32_t port_id;
	sw_error_t ret;

	memset(&port_eee_cfg, 0, sizeof(fal_port_eee_cfg_t));
	port_id = dp_priv->macid;
	ret = fal_port_interface_eee_cfg_get(NSS_DP_ACL_DEV_ID, port_id, &port_eee_cfg);
	if (ret != SW_OK) {
		netdev_dbg(netdev, "Could not fetch EEE settings err = %d\n", ret);
		return -EIO;
	}

	/*
	 * Translate the FAL linkmode types to ethtool linkmode types.
	 */
	nss_dp_fal_to_ethtool_linkmode_xlate(&eee->supported, &port_eee_cfg.capability);
	nss_dp_fal_to_ethtool_linkmode_xlate(&eee->advertised, &port_eee_cfg.advertisement);
	nss_dp_fal_to_ethtool_linkmode_xlate(&eee->lp_advertised, &port_eee_cfg.link_partner_advertisement);
	eee->eee_enabled = port_eee_cfg.enable;
	eee->eee_active = port_eee_cfg.eee_status;
	eee->tx_lpi_enabled = port_eee_cfg.lpi_tx_enable;
	eee->tx_lpi_timer = port_eee_cfg.lpi_sleep_timer;
#else
	eee->eee_enabled = 0;
        eee->eee_active = 0;
        eee->tx_lpi_enabled = 0;
        //eee->tx_lpi_timer = 
#endif
	return 0;
}

/*
 * nss_dp_set_eee()
 *	Set EEE settings.
 */
static int32_t nss_dp_set_eee(struct net_device *netdev, struct ethtool_eee *eee)
{
#ifdef NSS_FAL_SUPPORT
	struct nss_dp_dev *dp_priv = (struct nss_dp_dev *)netdev_priv(netdev);
	fal_port_eee_cfg_t port_eee_cfg, port_eee_cur_cfg;
	uint32_t port_id, pos;
	sw_error_t ret;

	memset(&port_eee_cfg, 0, sizeof(fal_port_eee_cfg_t));
	memset(&port_eee_cur_cfg, 0, sizeof(fal_port_eee_cfg_t));
	port_id = dp_priv->macid;

	/*
	 * Get current EEE configuration.
	 */
	ret = fal_port_interface_eee_cfg_get(NSS_DP_ACL_DEV_ID, port_id, &port_eee_cur_cfg);
	if (ret != SW_OK) {
		netdev_dbg(netdev, "Could not fetch EEE settings err = %d\n", ret);
		return -EIO;
	}

	port_eee_cfg.enable = eee->eee_enabled;

	/*
	 * Translate the ethtool speed types to FAL speed types.
	 */
	while (eee->advertised) {
		pos = ffs(eee->advertised);
		switch (1 << (pos - 1)) {
		case ADVERTISED_10baseT_Full:
			if (port_eee_cur_cfg.capability & FAL_PHY_EEE_10BASE_T) {
				port_eee_cfg.advertisement |= FAL_PHY_EEE_10BASE_T;
				break;
			}

			netdev_dbg(netdev, "Advertised value 10baseT_Full is not supported\n");
			return -EIO;

		case ADVERTISED_100baseT_Full:
			if (port_eee_cur_cfg.capability & FAL_PHY_EEE_100BASE_T) {
				port_eee_cfg.advertisement |= FAL_PHY_EEE_100BASE_T;
				break;
			}

			netdev_dbg(netdev, "Advertised value 100baseT_Full is not supported\n");
			return -EIO;

		case ADVERTISED_1000baseT_Full:
			if (port_eee_cur_cfg.capability & FAL_PHY_EEE_1000BASE_T) {
				port_eee_cfg.advertisement |= FAL_PHY_EEE_1000BASE_T;
				break;
			}

			netdev_dbg(netdev, "Advertised value 1000baseT_Full is not supported\n");
			return -EIO;

		case ADVERTISED_2500baseX_Full:
			if (port_eee_cur_cfg.capability & FAL_PHY_EEE_2500BASE_T) {
				port_eee_cfg.advertisement |= FAL_PHY_EEE_2500BASE_T;
				break;
			}

			netdev_dbg(netdev, "Advertised value 2500baseX_Full is not supported\n");
			return -EIO;

		case ADVERTISED_10000baseT_Full:
			if (port_eee_cur_cfg.capability & FAL_PHY_EEE_10000BASE_T) {
				port_eee_cfg.advertisement |= FAL_PHY_EEE_10000BASE_T;
				break;
			}

			netdev_dbg(netdev, "Advertised value 10000baseT_Full is not supported\n");
			return -EIO;

		default:
			netdev_dbg(netdev, "Advertised value is not supported\n");
			return -EIO;
		}

		eee->advertised &= (~(1 << (pos - 1)));
	}

	port_eee_cfg.lpi_tx_enable = eee->tx_lpi_enabled;
	port_eee_cfg.lpi_sleep_timer = eee->tx_lpi_timer;
	ret = fal_port_interface_eee_cfg_set(NSS_DP_ACL_DEV_ID, port_id, &port_eee_cfg);
	if (ret != SW_OK) {
		netdev_dbg(netdev, "Could not configure EEE err = %d\n", ret);
		return -EIO;
	}
#endif
	return 0;
}

/*
 * Ethtool operations
 */
struct ethtool_ops nss_dp_ethtool_ops = {
	.get_strings = &nss_dp_get_strings,
	.get_sset_count = &nss_dp_get_strset_count,
	.get_ethtool_stats = &nss_dp_get_ethtool_stats,
	.get_link = &ethtool_op_get_link,
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5,4,0))
	.get_settings = &nss_dp_get_settings,
	.set_settings = &nss_dp_set_settings,
#else
	.get_link_ksettings = &nss_dp_get_settings,
	.set_link_ksettings = &nss_dp_set_settings,
#endif
	.get_pauseparam = &nss_dp_get_pauseparam,
	.set_pauseparam = &nss_dp_set_pauseparam,
	.get_eee = &nss_dp_get_eee,
	.set_eee = &nss_dp_set_eee,
};

/*
 * nss_dp_set_ethtool_ops()
 *	Set ethtool operations
 */
void nss_dp_set_ethtool_ops(struct net_device *netdev)
{
	netdev->ethtool_ops = &nss_dp_ethtool_ops;
}
