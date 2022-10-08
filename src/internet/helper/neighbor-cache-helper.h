/*
 * Copyright (c) 2022 ZHIHENG DONG
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Zhiheng Dong <dzh2077@gmail.com>
 */

#ifndef NEIGHBOR_CACHE_HELPER_H
#define NEIGHBOR_CACHE_HELPER_H

#include "ns3/arp-cache.h"
#include "ns3/arp-header.h"
#include "ns3/arp-l3-protocol.h"
#include "ns3/channel.h"
#include "ns3/icmpv6-l4-protocol.h"
#include "ns3/ipv4-address.h"
#include "ns3/ipv4-interface-container.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv6-interface-container.h"
#include "ns3/ipv6-interface.h"
#include "ns3/ipv6-l3-protocol.h"
#include "ns3/net-device-container.h"
#include "ns3/node-list.h"

namespace ns3
{

/**
 * \ingroup NeighborCacheHelper
 *
 * @brief A helper class to populate neighbor cache.
 *
 * This class is used to populate neighbor cache. Permanent entries will be added
 * on the scope of a channel, a NetDeviceContainer, an InterfaceContainer or globally.
 */
class NeighborCacheHelper
{
  public:
    /**
     * @brief Construct a helper class to make life easier while creating neighbor cache
     */
    NeighborCacheHelper();
    ~NeighborCacheHelper();

    /**
     * \brief Populate neighbor ARP and NDISC caches for all devices.
     * This method walks the global ChannelList.
     */
    void PopulateNeighborCache();

    /**
     * \brief Populate neighbor ARP and NDISC caches for all devices in the given
     * Channel.
     * \param channel the Channel to process
     */
    void PopulateNeighborCache(Ptr<Channel> channel) const;

    /**
     * \brief Populate neighbor ARP and NDISC caches for all devices in the given
     * NetDeviceContainer.
     * \param c the NetDevice container to process
     */
    void PopulateNeighborCache(const NetDeviceContainer& c) const;

    /**
     * \brief Populate neighbor ARP caches for all IPv4 interfaces in the given
     * Ipv4InterfaceContainer.
     * \param c the Ipv4InterfaceContainer to process
     */
    void PopulateNeighborCache(const Ipv4InterfaceContainer& c) const;

    /**
     * \brief Populate neighbor NDISC caches for all IPv6 interfaces in the given
     * Ipv6InterfaceContainer.
     * \param c the Ipv6InterfaceContainer to process
     */
    void PopulateNeighborCache(const Ipv6InterfaceContainer& c) const;

    /**
     * \brief Remove entries generated from NeighborCacheHelper from ARP cache and NDISC cache
     */
    void FlushAutoGenerated() const;

    /**
     * \brief Enable/diable dynamic neighbor cache, auto-generated neighbor cache will update
     * by IP addresses changed when dynamic neighbor cache enabled. User should ensure that
     * the NeighborCacheHelper object persists for the duration of the simulation when dynamic
     * neighbor cache is enabled.
     * \param enable enable state
     */
    void SetDynamicNeighborCache(bool enable);

  private:
    /**
     * \brief Populate neighbor ARP entries for given IPv4 interface.
     * \param ipv4Interface the Ipv4Interface to process
     * \param neighborDeviceInterface the potential neighbor Ipv4Interface
     */
    void PopulateNeighborEntriesIpv4(Ptr<Ipv4Interface> ipv4Interface,
                                     Ptr<Ipv4Interface> neighborDeviceInterface) const;

    /**
     * \brief Populate neighbor NDISC entries for given IPv6 interface.
     * \param ipv6Interface the Ipv6Interface to process
     * \param neighborDeviceInterface the potential neighbor Ipv6Interface
     */
    void PopulateNeighborEntriesIpv6(Ptr<Ipv6Interface> ipv6Interface,
                                     Ptr<Ipv6Interface> neighborDeviceInterface) const;

    /**
     * \brief Add an auto_generated entry to the ARP cache of an interface.
     * \param netDeviceInterface the Ipv4Interface that ARP cache belongs to
     * \param ipv4Address the IPv4 address will be added to the cache.
     * \param macAddress the MAC address will be added to the cache.
     */
    void AddEntry(Ptr<Ipv4Interface> netDeviceInterface,
                  Ipv4Address ipv4Address,
                  Address macAddress) const;

    /**
     * \brief Add an auto_generated entry to the NDISC cache of an interface.
     * \param netDeviceInterface the Ipv6Interface that NDISC cache belongs to
     * \param ipv6Address the IPv6 address will be added to the cache.
     * \param macAddress the MAC address will be added to the cache.
     */
    void AddEntry(Ptr<Ipv6Interface> netDeviceInterface,
                  Ipv6Address ipv6Address,
                  Address macAddress) const;

    /**
     * \brief Update neighbor caches when an address is removed from a Ipv4Interface with auto
     * generated neighbor cache. \param interface the Ipv4Interface that address removed from.
     * \param ifAddr the removed IPv4 interface address .
     */
    void UpdateCacheByIpv4AddressRemoved(const Ptr<Ipv4Interface> interface,
                                         const Ipv4InterfaceAddress ifAddr) const;

    /**
     * \brief Update neighbor caches when an address is added to a Ipv4Interface with auto generated
     * neighbor cache. \param interface the Ipv4Interface that address added to. \param ifAddr the
     * added IPv4 interface address.
     */
    void UpdateCacheByIpv4AddressAdded(const Ptr<Ipv4Interface> interface,
                                       const Ipv4InterfaceAddress ifAddr) const;

    /**
     * \brief Update neighbor caches when an address is removed from a Ipv6Interface with auto
     * generated neighbor cache. \param interface the Ipv6Interface that address removed from.
     * \param ifAddr the removed IPv6 interface address.
     */
    void UpdateCacheByIpv6AddressRemoved(const Ptr<Ipv6Interface> interface,
                                         const Ipv6InterfaceAddress ifAddr) const;

    /**
     * \brief Update neighbor cache when an address is added to a Ipv6Interface with auto generated
     * neighbor cache. \param interface the Ipv6Interface that address added to. \param ifAddr the
     * added IPv6 interface address.
     */
    void UpdateCacheByIpv6AddressAdded(const Ptr<Ipv6Interface> interface,
                                       const Ipv6InterfaceAddress ifAddr) const;

    bool m_globalNeighborCache{
        false}; //!< flag will set true if neighbor caches were generated for all devices

    bool m_dynamicNeighborCache{
        false}; //!< flag will set true if dynamic neighbor cache is enabled.
};

} // namespace ns3

#endif /* NEIGHBOR_CACHE_HELPER_H */
