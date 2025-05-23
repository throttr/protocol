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

#ifndef THROTTR_PROTOCOL_CHANGE_TYPES_HPP
#define THROTTR_PROTOCOL_CHANGE_TYPES_HPP

#include <cstdint>

namespace throttr {
    /**
     * Change types
     */
    enum class change_types : uint8_t {
        /**
         * Patch
         */
        patch = 0x00,

        /**
         * Increase
         */
        increase = 0x01,

        /**
         * Decrease
         */
        decrease = 0x02,
    };
}

#endif // THROTTR_PROTOCOL_CHANGE_TYPES_HPP
