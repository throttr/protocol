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

#ifndef THROTTR_PROTOCOL_REQUESTS_CHANNELS_HPP
#define THROTTR_PROTOCOL_REQUESTS_CHANNELS_HPP

namespace throttr {
    /**
     * Request channels header
     */
    struct request_channels_header {
        /**
         * Request type
         */
        request_types request_type_;
    };

    /**
     * Request channels header size
     */
    constexpr std::size_t request_channels_header_size = sizeof(request_types);

    /**
     * Request channels builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_channels_builder() {
        return {
            static_cast<std::byte>(request_types::channels)
        };
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_CHANNELS_HPP
