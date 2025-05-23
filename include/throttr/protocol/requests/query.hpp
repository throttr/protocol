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

#ifndef THROTTR_PROTOCOL_REQUESTS_QUERY_HPP
#define THROTTR_PROTOCOL_REQUESTS_QUERY_HPP

namespace throttr {
#pragma pack(push, 1)
    /**
     * Request query header
     */
    struct request_query_header {
        /**
         * Request type
         */
        request_types request_type_;

        /**
         * Key size
         */
        uint8_t key_size_;
    };
#pragma pack(pop)

    /**
     * Request query header size
     */
    constexpr std::size_t request_query_header_size = sizeof(request_query_header);

    /**
     * Request query
     */
    struct request_query {
        /**
         * Header
         */
        const request_query_header *header_ = nullptr;

        /**
         * Key
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_query
         */
        static request_query from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_query_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_query_header_size, _header->key_size_);

            return request_query{
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
            _buffer.resize(request_query_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_query_header_size);
            std::memcpy(_buffer.data() + request_query_header_size, key_.data(), key_.size());

            return _buffer;
        }
    };


    /**
     * Request query builder
     *
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_query_builder(
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_query_header_size + key.size());

        auto *_header = reinterpret_cast<request_query_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::query;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_query_header_size, key.data(), key.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_INSERT_HPP
