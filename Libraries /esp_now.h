/*
 * SPDX-FileCopyrightText: 2019-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * License: Apache 2.0 → Open-source, allows usage, modification, distribution,
 * but requires attribution to original authors.
 */

#ifndef __ESP_NOW_H__       // Start of include guard (prevents multiple inclusion)
#define __ESP_NOW_H__

#include <stdbool.h>        // For "bool" type (true/false)
#include "esp_err.h"        // Common ESP-IDF error codes (ESP_OK, ESP_FAIL, etc.)
#include "esp_wifi_types.h" // WiFi types (MAC address length, tx info, etc.)

#ifdef __cplusplus
extern "C" {
#endif
// Allows this header to be used in C++ projects without name mangling.

/* ----------------------------- Documentation Groups -----------------------------
   These are Doxygen tags used to auto-generate API documentation for ESP-IDF.
   WiFi_APIs -> General WiFi APIs
   ESPNOW_APIs -> ESP-NOW specific APIs
---------------------------------------------------------------------------------- */

/* ----------------------------- Error Codes ----------------------------- */
#define ESP_ERR_ESPNOW_BASE         (ESP_ERR_WIFI_BASE + 100) // Base error code for ESP-NOW
#define ESP_ERR_ESPNOW_NOT_INIT     (ESP_ERR_ESPNOW_BASE + 1) // ESP-NOW not initialized
#define ESP_ERR_ESPNOW_ARG          (ESP_ERR_ESPNOW_BASE + 2) // Invalid argument passed
#define ESP_ERR_ESPNOW_NO_MEM       (ESP_ERR_ESPNOW_BASE + 3) // Out of memory
#define ESP_ERR_ESPNOW_FULL         (ESP_ERR_ESPNOW_BASE + 4) // Peer list full
#define ESP_ERR_ESPNOW_NOT_FOUND    (ESP_ERR_ESPNOW_BASE + 5) // Peer not found
#define ESP_ERR_ESPNOW_INTERNAL     (ESP_ERR_ESPNOW_BASE + 6) // Internal error
#define ESP_ERR_ESPNOW_EXIST        (ESP_ERR_ESPNOW_BASE + 7) // Peer already exists
#define ESP_ERR_ESPNOW_IF           (ESP_ERR_ESPNOW_BASE + 8) // Interface error
#define ESP_ERR_ESPNOW_CHAN         (ESP_ERR_ESPNOW_BASE + 9) // Channel mismatch

/* ----------------------------- Constants ----------------------------- */
#define ESP_NOW_ETH_ALEN             6     // MAC address length = 6 bytes
#define ESP_NOW_KEY_LEN              16    // Local Master Key length for encryption = 16 bytes
#define ESP_NOW_MAX_TOTAL_PEER_NUM   20    // Max total peers in list
#define ESP_NOW_MAX_ENCRYPT_PEER_NUM 6     // Max encrypted peers allowed
#define ESP_NOW_MAX_IE_DATA_LEN      250   // Max payload size for v1.0
#define ESP_NOW_MAX_DATA_LEN  ESP_NOW_MAX_IE_DATA_LEN // Alias for max data size v1.0
#define ESP_NOW_MAX_DATA_LEN_V2      1470  // Max payload size for v2.0 (like near-MTU size)

/* ----------------------------- Enums ----------------------------- */
// Sending status
typedef enum {
    ESP_NOW_SEND_SUCCESS = WIFI_SEND_SUCCESS, // Data sent successfully
    ESP_NOW_SEND_FAIL = WIFI_SEND_FAIL,       // Sending failed
} esp_now_send_status_t;

/* ----------------------------- Structs ----------------------------- */

// Peer information
typedef struct esp_now_peer_info {
    uint8_t peer_addr[ESP_NOW_ETH_ALEN]; // Peer MAC address (6 bytes)
    uint8_t lmk[ESP_NOW_KEY_LEN];        // Local master key (16 bytes, optional encryption)
    uint8_t channel;                     // WiFi channel for communication (0 = use current channel)
    wifi_interface_t ifidx;              // WiFi interface (STA or AP)
    bool encrypt;                        // Whether communication with peer is encrypted
    void *priv;                          // Private data pointer (optional user data)
} esp_now_peer_info_t;

// Number of peers info
typedef struct esp_now_peer_num {
    int total_num;   // Total peers added (<= 20)
    int encrypt_num; // Encrypted peers (<= 6)
} esp_now_peer_num_t;

// Info about received packet
typedef struct esp_now_recv_info {
    uint8_t * src_addr;                 // Source MAC address
    uint8_t * des_addr;                 // Destination MAC address
    wifi_pkt_rx_ctrl_t * rx_ctrl;       // RX metadata (RSSI, rate, etc.)
} esp_now_recv_info_t;

// Aliases for WiFi transmit info
typedef wifi_tx_info_t esp_now_send_info_t;      
typedef wifi_tx_rate_config_t esp_now_rate_config_t;

/* ----------------------------- Callback Typedefs ----------------------------- */

// Callback for received data
typedef void (*esp_now_recv_cb_t)(const esp_now_recv_info_t * esp_now_info,
                                  const uint8_t *data, int data_len);

// Callback for send status
typedef void (*esp_now_send_cb_t)(const esp_now_send_info_t *tx_info,
                                  esp_now_send_status_t status);

/* ----------------------------- Core ESP-NOW APIs ----------------------------- */

// Initialize ESPNOW
esp_err_t esp_now_init(void);

// De-initialize (cleanup resources)
esp_err_t esp_now_deinit(void);

// Get ESPNOW protocol version (v1.0 or v2.0)
esp_err_t esp_now_get_version(uint32_t *version);

// Register / Unregister callbacks
esp_err_t esp_now_register_recv_cb(esp_now_recv_cb_t cb);
esp_err_t esp_now_unregister_recv_cb(void);
esp_err_t esp_now_register_send_cb(esp_now_send_cb_t cb);
esp_err_t esp_now_unregister_send_cb(void);

// Send data (unicast or broadcast)
// peer_addr = NULL → send to all peers
esp_err_t esp_now_send(const uint8_t *peer_addr, const uint8_t *data, size_t len);

// Manage peers (Add / Delete / Modify / Get)
esp_err_t esp_now_add_peer(const esp_now_peer_info_t *peer);
esp_err_t esp_now_del_peer(const uint8_t *peer_addr);
esp_err_t esp_now_mod_peer(const esp_now_peer_info_t *peer);
esp_err_t esp_now_get_peer(const uint8_t *peer_addr, esp_now_peer_info_t *peer);
esp_err_t esp_now_fetch_peer(bool from_head, esp_now_peer_info_t *peer);
bool esp_now_is_peer_exist(const uint8_t *peer_addr);

// Get peer count
esp_err_t esp_now_get_peer_num(esp_now_peer_num_t *num);

// Set Primary Master Key (PMK) → used to encrypt Local Master Keys
esp_err_t esp_now_set_pmk(const uint8_t *pmk);

// Power saving: configure wake window
esp_err_t esp_now_set_wake_window(uint16_t window);

// Set / Get custom OUI (Organization Unique Identifier)
// Default = 0x18FE34 (Espressif’s OUI)
esp_err_t esp_now_set_user_oui(uint8_t *oui);
esp_err_t esp_now_get_user_oui(uint8_t *oui);

/* ----------------------------- Deprecated API ----------------------------- */
// Old API to configure data rate (replaced by esp_now_set_peer_rate_config)
esp_err_t esp_wifi_config_espnow_rate(wifi_interface_t ifx, wifi_phy_rate_t rate)
__attribute__((deprecated("Use esp_now_set_peer_rate_config for full rate support.")));

// New API: set peer-specific data rate
esp_err_t esp_now_set_peer_rate_config(const uint8_t *peer_addr,
                                       esp_now_rate_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* __ESP_NOW_H__ */
