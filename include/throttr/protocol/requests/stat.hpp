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

#ifndef THROTTR_PROTOCOL_REQUESTS_STAT_HPP
#define THROTTR_PROTOCOL_REQUESTS_STAT_HPP

namespace throttr {
    /**
     * Request stat header
     */
    struct request_stat_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };

    /**
     * Request stat header size
     */
    constexpr std::size_t request_stat_header_size = sizeof(request_stat_header);


    /**
     * Request stat
     */
    struct request_stat {
        /**
         * Key
         */
        std::span<const std::byte> key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_stat
         */
        static request_stat from_buffer(const std::span<const std::byte> &buffer) {
            const auto _key_size = std::to_integer<uint8_t>(buffer[1]);
            const auto _key = buffer.subspan(request_stat_header_size, _key_size);
            return request_stat{
                _key
            };
        }
    };

    /**
     * Request stat builder
     *
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_stat_builder(
        const std::string_view key = ""
        ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_stat_header_size + key.size());

        request_stat_header _header{};
        _header.request_type_ = request_types::stat;
        _header.key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data(), &_header, sizeof(_header));
        std::memcpy(_buffer.data() + request_stat_header_size, key.data(), key.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_STAT_HPP
