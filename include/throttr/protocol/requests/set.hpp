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
#pragma pack(push, 1)
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
#pragma pack(pop)

    /**
     * Request set header size
     */
    constexpr std::size_t request_set_header_size = sizeof(request_set_header);

    /**
     * Request set
     */
    struct request_set {
        /**
         * Header
         */
        const request_set_header *header_ = nullptr;

        /**
         * Key ID
         */
        std::string_view key_;

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
            const auto *_header = reinterpret_cast<const request_set_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_set_header_size, _header->key_size_);
            const auto _value = buffer.subspan(request_set_header_size + _header->key_size_, _header->value_size_);

            return request_set{
                _header,
                std::string_view(reinterpret_cast<const char *>(_key.data()), _key.size()), // NOSONAR
                _value,
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
            _buffer.resize(request_set_header_size + key_.size() + value_.size());

            std::memcpy(_buffer.data(), header_, request_set_header_size);
            std::memcpy(_buffer.data() + request_set_header_size, key_.data(), key_.size());
            std::memcpy(_buffer.data() + request_set_header_size + key_.size(), value_.data(), value_.size());

            return _buffer;
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
        _buffer.resize(request_set_header_size + key.size() + buffer.size());

        auto *_header = reinterpret_cast<request_set_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::set;
        _header->ttl_type_ = ttl_type;
        _header->ttl_ = ttl;
        _header->key_size_ = static_cast<uint8_t>(key.size());
        _header->value_size_ = static_cast<value_type>(buffer.size());

        std::memcpy(_buffer.data() + request_set_header_size, key.data(), key.size());
        std::memcpy(
            _buffer.data() + request_set_header_size + key.size(),
            buffer.data(),
            buffer.size()
        );

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_SET_HPP
