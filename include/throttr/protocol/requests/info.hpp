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

#ifndef THROTTR_PROTOCOL_REQUESTS_INFO_HPP
#define THROTTR_PROTOCOL_REQUESTS_INFO_HPP

namespace throttr {
    /**
     * Request info header
     */
    struct request_info_header {
        /**
         * Request type
         */
        request_types request_type_;
    };

    /**
     * Request info header size
     */
    constexpr std::size_t request_info_header_size = sizeof(request_info_header);

    /**
     * Request info
     */
    struct request_info {
        /**
         * Header
         */
        const request_info_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_info
         */
        static request_info from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_info_header *>(buffer.data()); // NOSONAR

            return request_info{
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
            _buffer.resize(request_info_header_size);

            std::memcpy(_buffer.data(), header_, request_info_header_size);
            return _buffer;
        }
    };

    /**
     * Request info builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_info_builder() {
        return {
            static_cast<std::byte>(request_types::info)
        };
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_INFO_HPP
