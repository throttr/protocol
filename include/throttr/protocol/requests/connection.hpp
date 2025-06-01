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

#ifndef THROTTR_PROTOCOL_REQUESTS_CONNECTION_HPP
#define THROTTR_PROTOCOL_REQUESTS_CONNECTION_HPP

namespace throttr {
    /**
     * Request connection header
     */
    struct request_connection_header {
        /**
         * Request type
         */
        request_types request_type_ = request_types::connection;

        /**
         * ID
         */
        std::array<std::byte, 16> id_;
    };

    /**
     * Request connection header size
     */
    constexpr std::size_t request_connection_header_size = sizeof(request_connection_header);


    /**
     * Request connection
     */
    struct request_connection {
        /**
         * Header
         */
        const request_connection_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_connection
         */
        static request_connection from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_connection_header *>(buffer.data()); // NOSONAR

            return request_connection{
                _header,
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
            _buffer.resize(request_connection_header_size);
            std::memcpy(_buffer.data(), header_, request_connection_header_size);

            return _buffer;
        }
    };

    /**
     * Request connection builder
     *
     * @param id
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_connection_builder(
        const std::array<std::byte, 16> &id
    ) {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_connection_header_size);

        request_connection_header _header{};
        _header.request_type_ = request_types::connection;
        _header.id_ = id;

        std::memcpy(_buffer.data(), &_header, sizeof(_header));

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_CONNECTION_HPP
