// Copyright (c) 2018, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "parse.h"

#include "net/tor_address.h"
#include "net/i2p_address.h"
#include "string_tools.h"

namespace net
{
    void get_network_address_host_and_port(const std::string& address, std::string& host, std::string& port)
    {
        // require ipv6 address format "[addr:addr:addr:...:addr]:port"
        if (address.find(']') != std::string::npos)
        {
            host = address.substr(1, address.rfind(']') - 1);
            if ((host.size() + 2) < address.size())
            {
                port = address.substr(address.rfind(':') + 1);
            }
        }
        else
        {
            host = address.substr(0, address.rfind(':'));
            if (host.size() < address.size())
            {
                port = address.substr(host.size() + 1);
            }
        }
    }

    expect<epee::net_utils::network_address>
    get_network_address(const boost::string_ref address, const std::uint16_t default_port)
    {
        std::string host_str = "";
        std::string port_str = "";

        bool ipv6 = false;

        get_network_address_host_and_port(std::string(address), host_str, port_str);

        boost::string_ref host_str_ref(host_str);
        boost::string_ref port_str_ref(host_str);

        if (host_str.empty())
            return make_error_code(net::error::invalid_host);
        if (host_str_ref.ends_with(".onion"))
            return tor_address::make(address, default_port);
        if (host_str_ref.ends_with(".i2p"))
            return i2p_address::make(address, default_port);

        ipv6 = (host_str.find(':') != std::string::npos);

        std::uint16_t port = default_port;
        if (port_str.size())
        {
            if (!epee::string_tools::get_xtype_from_string(port, port_str))
                return make_error_code(net::error::invalid_port);
        }

        if (ipv6)
        {
            return {epee::net_utils::ipv6_network_address{host_str, port}};
        }
        else
        {
            std::uint32_t ip = 0;
            if (epee::string_tools::get_ip_int32_from_string(ip, host_str))
                return {epee::net_utils::ipv4_network_address{ip, port}};
        }

        return make_error_code(net::error::unsupported_address);
    }
}
