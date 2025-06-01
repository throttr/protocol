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

#ifndef THROTTR_PROTOCOL_REQUESTS_SET_HPP
#define THROTTR_PROTOCOL_REQUESTS_SET_HPP

namespace throttr {
    /**
     * Request set header
     */
    struct request_set_header {
        /**
         * Request type
         */
        request_types request_type_;

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

        /**
         * Value size
         */
        value_type value_size_;
    };

    /**
     * Request set header size
     */
    constexpr std::size_t request_set_header_size = sizeof(request_types) + sizeof(ttl_types) + sizeof(uint8_t) + sizeof(value_type) * 2;

    /**
     * Request set
     */
    struct request_set {
        /**
         * TTL
         */
        std::span<const std::byte> ttl_;

        /**
         * TTL Type
         */
        ttl_types ttl_type_;

        /**
         * Key ID
         */
        std::span<const std::byte> key_;

        /**
         * Value
         */
        std::span<const std::byte> value_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_set
         */
        static request_set from_buffer(const std::span<const std::byte> &buffer) {
            std::size_t _offset = 1;

            const auto _ttl_type = static_cast<ttl_types>(std::to_integer<uint8_t>(buffer[_offset]));
            _offset++;

            const auto _ttl = buffer.subspan(_offset, sizeof(value_type));
            _offset += sizeof(value_type);

            const auto _key_size = std::to_integer<uint8_t>(buffer[_offset]);
            _offset++;

            value_type _value_size = 0;
            std::memcpy(&_value_size, buffer.data() + _offset, sizeof(value_type));
            _offset += sizeof(value_type);

            const auto _key = buffer.subspan(_offset, _key_size);
            _offset += _key_size;

            const auto _value = buffer.subspan(_offset, _value_size);

            return request_set{
                _ttl,
                _ttl_type,
                _key,
                _value
            };
        }
    };

    /**
    * Request set builder
    *
    * @param buffer
    * @param ttl_type
    * @param ttl
    * @param key
    * @return std::vector<std::byte>
    */
    inline std::vector<std::byte> request_set_builder(
        const std::vector<std::byte> &buffer,
        const ttl_types ttl_type = ttl_types::milliseconds,
        const value_type ttl = 0,
        const std::string_view key = ""
        ) {
        std::vector<std::byte> _buffer;
        const std::size_t _total_size = request_set_header_size + key.size() + buffer.size();
        _buffer.resize(_total_size);

        std::size_t _offset = 0;

        constexpr auto _request_type = request_types::set;
        std::memcpy(_buffer.data() + _offset, &_request_type, sizeof(request_types));
        _offset += sizeof(request_types);

        std::memcpy(_buffer.data() + _offset, &ttl_type, sizeof(ttl_types));
        _offset += sizeof(ttl_types);

        std::memcpy(_buffer.data() + _offset, &ttl, sizeof(value_type));
        _offset += sizeof(value_type);

        const auto _key_size = static_cast<uint8_t>(key.size());
        std::memcpy(_buffer.data() + _offset, &_key_size, sizeof(uint8_t));
        _offset += sizeof(uint8_t);

        const auto _value_size = static_cast<value_type>(key.size());
        std::memcpy(_buffer.data() + _offset, &_value_size, sizeof(value_type));
        _offset += sizeof(value_type);

        std::memcpy(_buffer.data() + _offset, key.data(), key.size());
        _offset += key.size();

        std::memcpy(_buffer.data() + _offset, buffer.data(), buffer.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_SET_HPP
