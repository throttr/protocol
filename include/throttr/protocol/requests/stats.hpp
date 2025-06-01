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
     * Request stats
     */
    struct request_stats {
        /**
         * Header
         */
        const request_stats_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_stats
         */
        static request_stats from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_stats_header *>(buffer.data()); // NOSONAR

            return request_stats{
                _header
            };
        }

        /**
         * To buffer
         *
         * @return std::vector<std::byte>
         */
        [[nodiscard]]
        std::vector<std::byte> to_buffer() const {
            std::vector<std::byte> _buffer;
            _buffer.resize(request_stats_header_size);

            std::memcpy(_buffer.data(), header_, request_stats_header_size);
            return _buffer;
        }
    };

    /**
     * Request stats builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_stats_builder() {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_stats_header_size);

        request_stats_header _header{};
        _header.request_type_ = request_types::stats;

        std::memcpy(_buffer.data(), &_header, sizeof(_header));

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_STATS_HPP
