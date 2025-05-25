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

#ifndef THROTTR_PROTOCOL_REQUESTS_PURGE_HPP
#define THROTTR_PROTOCOL_REQUESTS_PURGE_HPP

namespace throttr {
    /**
     * Request purge header
     */
    struct request_purge_header {
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
     * Request purge header size
     */
    constexpr std::size_t request_purge_header_size = sizeof(request_purge_header);


    /**
     * Request purge
     */
    struct request_purge {
        /**
         * Header
         */
        const request_purge_header *header_ = nullptr;

        /**
         * Key
         */
        std::string_view key_;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_purge
         */
        static request_purge from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_purge_header *>(buffer.data()); // NOSONAR
            const auto _key = buffer.subspan(request_purge_header_size, _header->key_size_);

            return request_purge{
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
            _buffer.resize(request_purge_header_size + key_.size());

            std::memcpy(_buffer.data(), header_, request_purge_header_size);
            std::memcpy(_buffer.data() + request_purge_header_size, key_.data(), key_.size());

            return _buffer;
        }
    };

    /**
     * Request purge builder
     *
     * @param key
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_purge_builder(
        const std::string_view key = ""
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_purge_header_size + key.size());

        auto *_header = reinterpret_cast<request_purge_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::purge;
        _header->key_size_ = static_cast<uint8_t>(key.size());

        std::memcpy(_buffer.data() + request_purge_header_size, key.data(), key.size());

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_PURGE_HPP
