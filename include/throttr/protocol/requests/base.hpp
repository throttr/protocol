// Copyright (C) 2025 Ian Torres
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef THROTTR_PROTOCOL_REQUESTS_BASE_HPP
#define THROTTR_PROTOCOL_REQUESTS_BASE_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>
#include <throttr/protocol/request_types.hpp>

namespace throttr {
    inline void push_attribute(const request_types type, std::vector<std::byte> & buffer, std::size_t &offset, const std::string_view data = "") {
        std::memcpy(buffer.data() + offset, &type, sizeof(request_types));
        offset += sizeof(request_types);
        const auto _key_size = static_cast<uint8_t>(data.size());
        std::memcpy(buffer.data() + offset, &_key_size, sizeof(uint8_t));
        offset += sizeof(uint8_t);
        std::memcpy(buffer.data() + offset, data.data(), data.size());
    }
}

#endif //THROTTR_PROTOCOL_REQUESTS_BASE_HPP
