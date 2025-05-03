# 📜 Throttr Protocol

<p align="center">
<a href="https://github.com/throttr/protocol/actions/workflows/build.yml"><img src="https://github.com/throttr/protocol/actions/workflows/build.yml/badge.svg" alt="Build"></a>
<a href="https://codecov.io/gh/throttr/protocol"><img src="https://codecov.io/gh/throttr/protocol/graph/badge.svg?token=R3CDT8AR2F" alt="Coverage"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=alert_status" alt="Quality Gate"></a>
</p>

<p align="center">
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=bugs" alt="Bugs"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=vulnerabilities" alt="Vulnerabilities"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=code_smells" alt="Code Smells"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=duplicated_lines_density" alt="Duplicated Lines"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=sqale_index" alt="Technical Debt"></a>
</p>

<p align="center">
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=reliability_rating" alt="Reliability"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=security_rating" alt="Security"></a>
<a href="https://sonarcloud.io/project/overview?id=throttr_protocol"><img src="https://sonarcloud.io/api/project_badges/measure?project=throttr_protocol&metric=sqale_rating" alt="Maintainability"></a>
</p>

## 🪐 About

**Throttr** defines a minimal, efficient binary protocol based on three main request types:

- Insert Request
- Query Request
- Purge Request
- Update Request

# 📚 Concepts to Understand the Throttr Protocol

- **Consumer**: An entity (user, client, device, session, or any identifier like UUID, hash, or IP) that is subject to traffic or quota control.
- **Resource**: A target associated with a consumer. It can represent anything: an API endpoint, a file, a user ID, a service, or any abstract entity.
- **Quota**: The maximum number of allowed operations (requests, actions, accesses) a consumer can perform on a resource during a valid TTL period.
- **Usage**: The amount deducted from the available quota with each request. Usually `1`, but can represent batched or weighted operations.
- **TTL (Time To Live)**: The lifetime duration (in nanoseconds, milliseconds, or seconds) during which a quota remains valid before it expires and resets.
- **TTL Type**: Defines the unit used for the TTL: nanoseconds (`ns`), milliseconds (`ms`), or seconds (`s`).
- **Expires At**: The absolute expiration timestamp, calculated from the TTL and the time of insertion or update.
- **Request Insert**: Operation that creates or resets a quota and TTL for a specific consumer-resource pair.
- **Request Query**: Operation that retrieves the current quota and TTL without modifying any data.
- **Request Update**: Operation that patches, increases, or decreases the quota or TTL dynamically for a consumer-resource pair.
- **Request Purge**: Operation that permanently deletes a consumer-resource pair from the state.

### 📥 Insert Request Format

| Field              | Type       | Size    | Description                        |
|:-------------------|:-----------|:--------|:-----------------------------------|
| `request_type`     | `uint8_t`  | 1 byte  | Always 0x01 for insert.            |
| `request_id`       | `uint32_t` | 4 bytes | Request ID.                        |
| `quota`            | `uint64_t` | 8 bytes | Maximum number of allowed actions. |
| `ttl_type`         | `uint8_t`  | 1 byte  | 0 = ns, 1 = ms, 2 = s.             |
| `ttl`              | `uint64_t` | 8 bytes | Time to live value.                |
| `consumer_id_size` | `uint8_t`  | 1 byte  | Size of Consumer ID.               |
| `resource_id_size` | `uint8_t`  | 1 byte  | Size of Resource ID.               |
| `consumer_id`      | `char[N]`  | N bytes | Consumer identifier.               |
| `resource_id`      | `char[M]`  | M bytes | Resource identifier.               |

### 🔍 Query and 🧹 Purge Request Format

| Field              | Type       | Size    | Description                       |
|:-------------------|:-----------|:--------|:----------------------------------|
| `request_type`     | `uint8_t`  | 1 byte  | 0x02 for query and 0x04 on purge. |
| `request_id`       | `uint32_t` | 4 bytes | Request ID.                       |
| `consumer_id_size` | `uint8_t`  | 1 byte  | Size of Consumer ID.              |
| `resource_id_size` | `uint8_t`  | 1 byte  | Size of Resource ID.              |
| `consumer_id`      | `char[N]`  | N bytes | Consumer identifier.              |
| `resource_id`      | `char[M]`  | M bytes | Resource identifier.              |

### ♻️ Update Request Format

| Field              | Type       | Size    | Description                             |
|:-------------------|:-----------|:--------|:----------------------------------------|
| `request_type`     | `uint8_t`  | 1 byte  | Always 0x03 for update.                 |
| `request_id`       | `uint32_t` | 4 bytes | Request ID.                             |
| `attribute`        | `uint8_t`  | 1 byte  | 0 = quota, 1 = ttl.                     |
| `change`           | `uint8_t`  | 1 byte  | 0 = patch, 1 = increase, 2 = decrease.  |
| `value`            | `uint64_t` | 8 bytes | Value to apply according to the change. |
| `consumer_id_size` | `uint8_t`  | 1 byte  | Size of Consumer ID.                    |
| `resource_id_size` | `uint8_t`  | 1 byte  | Size of Resource ID.                    |
| `consumer_id`      | `char[N]`  | N bytes | Consumer identifier.                    |
| `resource_id`      | `char[M]`  | M bytes | Resource identifier.                    |

### 📦 Response Format

Server on almost cases should respond with 5 bytes `Request ID` +  `0x00 or 0x01` (failure or success).

If the hash generated based on `consumer + resource` exists then **Query** will respond with extra fields:

| Field      | Type       | Size    | Description            |
|:-----------|:-----------|:--------|:-----------------------|
| `quota`    | `uint64_t` | 8 bytes | Available quota.       |
| `ttl_type` | `uint8_t`  | 1 byte  | 0 = ns, 1 = ms, 2 = s. |
| `ttl`      | `uint64_t` | 8 bytes | Time to expire.        |


### ⚖️ License

This software has been built by **Ian Torres** and released using [GNU Affero General Public License](./LICENSE).

We can talk, send me something to [iantorres@outlook.com](mailto:iantorres@outlook.com).
