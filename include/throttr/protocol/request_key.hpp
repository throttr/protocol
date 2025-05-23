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

#ifndef THROTTR_PROTOCOL_REQUEST_KEY_HPP
#define THROTTR_PROTOCOL_REQUEST_KEY_HPP

namespace throttr {

    /**
     * Request key
     */
    struct request_key {
        /**
         * Key
         */
        std::string_view key_;

        /**
         * Comparator
         *
         * @param other
         * @return bool
         */
        bool operator==(const request_key &other) const = default;
    };
}

#endif // THROTTR_PROTOCOL_REQUEST_KEY_HPP
