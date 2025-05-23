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

#ifndef THROTTR_PROTOCOL_REQUEST_TYPES_HPP
#define THROTTR_PROTOCOL_REQUEST_TYPES_HPP

#include <cstdint>

namespace throttr {
    /**
     * Request types
     */
    enum class request_types : uint8_t {
        /**
         * Insert
         */
        insert = 0x01,

        /**
         * Query
         */
        query = 0x02,

        /**
         * Update
         */
        update = 0x03,

        /**
         * Purge
         */
        purge = 0x04,

        /**
         * Set
         */
        set = 0x05,

        /**
         * Get
         */
        get = 0x06,

        /**
         * List
         */
        list = 0x07,

        /**
         * Info
         */
        info = 0x08,

        /**
         * Stat
         */
        stat = 0x09,

        /**
         * Stats
         */
        stats = 0x10,

        /**
         * Subscribe
         */
        subscribe = 0x11,

        /**
         * Unsubscribe
         */
        unsubscribe = 0x12,

        /**
         * Publish
         */
        publish = 0x13,

        /**
         * Connections
         */
        connections = 0x14,

        /**
         * Connection
         */
        connection = 0x15,

        /**
         * Channels
         */
        channels = 0x16,

        /**
         * Channel
         */
        channel = 0x17,

        /**
         * whoami
         */
        whoami = 0x18,
    };
}

#endif // THROTTR_PROTOCOL_REQUEST_TYPES_HPP
