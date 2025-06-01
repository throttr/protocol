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

#ifndef THROTTR_PROTOCOL_REQUESTS_UPDATE_HPP
#define THROTTR_PROTOCOL_REQUESTS_UPDATE_HPP

namespace throttr {
    /**
     * Request update header
     */
    struct request_update_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Attribute
         */
        attribute_types attribute_;

        /**
         * Change
         */
        change_types change_;

        /**
         * Value
         */
        value_type value_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };

    /**
     * Request update header size
     */
    constexpr std::size_t request_update_header_size = sizeof(request_types) + sizeof(attribute_types) + sizeof(change_types) + sizeof(value_type) + sizeof(uint8_t);


    /**
     * Request update
     */
    struct request_update {
        /**
         * Attribute
         */
        attribute_types attribute_;

        /**
         * Change
         */
        change_types change_;

        /**
         * Value
         */
        value_type value_;

        /**
         * Key
         */
        std::span<const std::byte> key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_update
         */
        static request_update from_buffer(const std::span<const std::byte> &buffer) {
            std::size_t _offset = 1;

            const auto _attribute = static_cast<attribute_types>(
                std::to_integer<uint8_t>(buffer[_offset])
            );
            _offset++;

            const auto _change = static_cast<change_types>(
                std::to_integer<uint8_t>(buffer[_offset])
            );
            _offset++;

            value_type _value = 0;
            std::memcpy(&_value, buffer.data() + _offset, sizeof(value_type));
            _offset += sizeof(value_type);

            const auto _key_size = std::to_integer<uint8_t>(buffer[_offset]);
            _offset++;

            const auto _key = buffer.subspan(_offset, _key_size);

            return request_update{
                _attribute,
                _change,
                _value,
                _key
            };
        }
    };


    /**
     * Request update builder
     *
     * @param attribute
     * @param change
     * @param value
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_update_builder(
        const attribute_types attribute = attribute_types::quota,
        const change_types change = change_types::patch,
        const value_type value = 0,
        const std::string_view key = ""
        ) {
        std::vector<std::byte> _buffer;
        const std::size_t _total_size = sizeof(request_types) + sizeof(attribute_types) + sizeof(change_types) + sizeof(value_type) + sizeof(uint8_t) + key.size();
        _buffer.resize(_total_size);

        std::size_t _offset = 0;

        constexpr auto _request_type = request_types::update;
        std::memcpy(_buffer.data() + _offset, &_request_type, sizeof(request_types)); // NOSONAR
        _offset += sizeof(request_types);

        std::memcpy(_buffer.data() + _offset, &attribute, sizeof(attribute_types)); // NOSONAR
        _offset += sizeof(attribute_types);

        std::memcpy(_buffer.data() + _offset, &change, sizeof(change_types)); // NOSONAR
        _offset += sizeof(change_types);

        std::memcpy(_buffer.data() + _offset, &value, sizeof(value_type));
        _offset += sizeof(value_type);

        const auto _key_size = static_cast<uint8_t>(key.size());
        std::memcpy(_buffer.data() + _offset, &_key_size, sizeof(uint8_t)); // NOSONAR
        _offset += sizeof(uint8_t);

        std::memcpy(_buffer.data() + _offset, key.data(), key.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_UPDATE_HPP
