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
    constexpr std::size_t request_update_header_size = sizeof(request_update_header);


    /**
     * Request update
     */
    struct request_update {
        /**
         * Header
         */
        const request_update_header *header_ = nullptr;

        /**
         * Key
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_update
         */
        static request_update from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_update_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_update_header_size, _header->key_size_);

            return request_update{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
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
            _buffer.resize(request_update_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_update_header_size);
            std::memcpy(_buffer.data() + request_update_header_size, key_.data(), key_.size());

            return _buffer;
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
        _buffer.resize(request_update_header_size + key.size());

        auto *_header = reinterpret_cast<request_update_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::update;
        _header->attribute_ = attribute;
        _header->change_ = change;
        _header->value_ = value;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_update_header_size, key.data(), key.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_UPDATE_HPP
