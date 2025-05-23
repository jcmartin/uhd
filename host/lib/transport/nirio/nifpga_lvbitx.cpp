//
// Copyright 2013-2014 Ettus Research LLC
// Copyright 2018 Ettus Research, a National Instruments Company
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include <uhdlib/transport/nirio/nifpga_lvbitx.h>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <streambuf>
#include <string>

namespace uhd { namespace niusrprio {

std::string nifpga_lvbitx::_get_bitstream_checksum(const std::string& file_path)
{
    const std::regex md5_regex(
        "<BitstreamMD5>([a-fA-F0-9]{32})<\\/BitstreamMD5>", std::regex::icase);

    std::ifstream lvbitx_stream(file_path.c_str());
    if (!lvbitx_stream.is_open()) {
        return std::string();
    }

    std::string checksum, line;
    while (std::getline(lvbitx_stream, line)) {
        try {
            // short-circuiting the regex search with a simple find is faster
            // for cases where the tag doesn't exist
            std::smatch md5_match;
            if (line.find("<BitstreamMD5>") != std::string::npos
                && std::regex_search(line, md5_match, md5_regex)) {
                checksum = std::string(md5_match[1].first, md5_match[1].second);
                break;
            }
        } catch (boost::exception&) {
        }
    }
    boost::to_upper(checksum);
    return checksum;
}

}} // namespace uhd::niusrprio
