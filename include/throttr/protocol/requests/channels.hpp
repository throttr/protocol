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

#ifndef THROTTR_PROTOCOL_REQUESTS_CHANNELS_HPP
#define THROTTR_PROTOCOL_REQUESTS_CHANNELS_HPP

namespace throttr {
    /**
     * Request channels header
     */
    struct request_channels_header {
        /**
         * Request type
         */
        request_types request_type_;
    };

    /**
     * Request channels header size
     */
    constexpr std::size_t request_channels_header_size = sizeof(request_channels_header);

    /**
     * Request channels
     */
    struct request_channels {
        /**
         * Header
         */
        const request_channels_header *header_ = nullptr;

        /**
         * From buffer
         *
         * @param buffer
         * @return request_channels
         */
        static request_channels from_buffer(const std::span<const std::byte> &buffer) {
            const auto *_header = reinterpret_cast<const request_channels_header *>(buffer.data()); // NOSONAR

            return request_channels{
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
            _buffer.resize(request_channels_header_size);

            std::memcpy(_buffer.data(), header_, request_channels_header_size);
            return _buffer;
        }
    };

    /**
     * Request channels builder
     *
     * @return std::vector<std::byte>
     */
    inline std::vector<std::byte> request_channels_builder() {
        std::vector<std::byte> _buffer;
        _buffer.resize(request_channels_header_size);

        auto *_header = reinterpret_cast<request_channels_header *>(_buffer.data()); // NOSONAR
        _header->request_type_ = request_types::channels;

        return _buffer;
    }
}

#endif // THROTTR_PROTOCOL_REQUESTS_CHANNELS_HPP
