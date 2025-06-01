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

#ifndef THROTTR_PROTOCOL_REQUESTS_WHOAMI_HPP
#define THROTTR_PROTOCOL_REQUESTS_WHOAMI_HPP

namespace throttr {
    /**
     * Request whoami header
     */
    struct request_whoami_header {
        /**
         * Request type
         */
        request_types request_type_;
    };

    /**
     * Request whoami header size
     */
    constexpr std::size_t request_whoami_header_size = sizeof(request_whoami_header);

    /**
     * Request whoami
     */
    struct request_whoami {
        /**
         * Header
         */
        const request_whoami_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_whoami
         */
        static request_whoami from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_whoami_header *>(buffer.data()); // NOSONAR

            return request_whoami{
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
            _buffer.resize(request_whoami_header_size);

            std::memcpy(_buffer.data(), header_, request_whoami_header_size);
            return _buffer;
        }
    };

    /**
     * Request whoami builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_whoami_builder() {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_whoami_header_size);

        request_whoami_header _header{};
        _header.request_type_ = request_types::whoami;

        std::memcpy(_buffer.data(), &_header, sizeof(_header));

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_WHOAMI_HPP
