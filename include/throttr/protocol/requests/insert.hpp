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

#ifndef THROTTR_PROTOCOL_REQUESTS_INSERT_HPP
#define THROTTR_PROTOCOL_REQUESTS_INSERT_HPP

namespace throttr {
    /**
     * Request insert header
     */
    struct request_insert_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Quota
         */
        value_type quota_;

        /**
         * TTL type
         */
        ttl_types ttl_type_;

        /**
         * TTL
         */
        value_type ttl_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };

    /**
     * Request insert header size
     */
    constexpr std::size_t request_insert_header_size = sizeof(request_insert_header);

    /**
     * Request insert
     */
    struct request_insert {
        /**
         * Quota
         */
        std::span<const std::byte> quota_;

        /**
         * TTL Type
         */
        ttl_types ttl_type_;

        /**
         * TTL
         */
        std::span<const std::byte> ttl_;

        /**
         * Key ID
         */
        std::span<const std::byte> key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_insert
         */
        static request_insert from_buffer(const std::span<const std::byte> &buffer) {
            std::size_t _offset = 1;

            const auto _quota = buffer.subspan(_offset, sizeof(value_type));
            _offset += sizeof(value_type);

            const auto _ttl_type = static_cast<ttl_types>(std::to_integer<uint8_t>(buffer[_offset]));
            _offset++;

            const auto _ttl = buffer.subspan(_offset, sizeof(value_type));
            _offset += sizeof(value_type);

            const auto _key_size = std::to_integer<uint8_t>(buffer[_offset]);
            _offset++;

            const auto _key = buffer.subspan(_offset, _key_size);

            return request_insert{
                _quota,
                _ttl_type,
                _ttl,
                _key
            };
        }
    };


    /**
     * Request insert builder
     *
     * @param quota
     * @param ttl_type
     * @param ttl
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_insert_builder(
        const value_type quota = 0,
        const ttl_types ttl_type = ttl_types::milliseconds,
        const value_type ttl = 0,
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_insert_header_size + key.size());


        request_insert_header _header{};
        _header.request_type_ = request_types::insert;
        _header.quota_ = quota;
        _header.ttl_type_ = ttl_type;
        _header.ttl_ = ttl;
        _header.key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data(), &_header, sizeof(_header));
        std::memcpy(_buffer.data() + request_insert_header_size, key.data(), key.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_INSERT_HPP
