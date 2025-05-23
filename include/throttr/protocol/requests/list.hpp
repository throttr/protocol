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

#ifndef THROTTR_PROTOCOL_REQUESTS_LIST_HPP
#define THROTTR_PROTOCOL_REQUESTS_LIST_HPP

namespace throttr {
#pragma pack(push, 1)
    /**
     * Request list header
     */
    struct request_list_header {
        /**
         * Request type
         */
        request_types request_type_;
    };
#pragma pack(pop)

    /**
     * Request list header size
     */
    constexpr std::size_t request_list_header_size = sizeof(request_list_header);

    /**
     * Request list
     */
    struct request_list {
        /**
         * Header
         */
        const request_list_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_list
         */
        static request_list from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_list_header *>(buffer.data()); // NOSONAR

            return request_list{
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
            _buffer.resize(request_list_header_size);

            std::memcpy(_buffer.data(), header_, request_list_header_size);
            return _buffer;
        }
    };

    /**
     * Request list builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_list_builder() {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_list_header_size);

        auto *_header = reinterpret_cast<request_list_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::list;

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_LIST_HPP
