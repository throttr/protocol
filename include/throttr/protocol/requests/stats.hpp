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

#ifndef THROTTR_PROTOCOL_REQUESTS_STATS_HPP
#define THROTTR_PROTOCOL_REQUESTS_STATS_HPP

namespace throttr {
    /**
     * Request stats header
     */
    struct request_stats_header {
        /**
         * Request type
         */
        request_types request_type_;
    };

    /**
     * Request stats header size
     */
    constexpr std::size_t request_stats_header_size = sizeof(request_stats_header);

    /**
     * Request stats builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_stats_builder() {
        return {
            static_cast<std::byte>(request_types::stats)
        };
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_STATS_HPP
