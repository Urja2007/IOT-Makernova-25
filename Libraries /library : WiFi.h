/*
 WiFi.h - esp32 WiFi support.
 
 This is the main WiFi library header file for ESP32 when using Arduino framework.
 It provides classes and functions to control WiFi features such as:
 - Station (connecting ESP32 to WiFi networks like a phone/laptop does)
 - Access Point (turning ESP32 into a WiFi hotspot)
 - Scanning nearby WiFi networks
 - Handling TCP/UDP communication (clients/servers)
 
 Based on Arduino WiFi shield library, but extended for ESP32.

 History:
   - Original Arduino WiFi library copyright (2011-2014).
   - Modified for ESP32 by Ivan Grokhotkov, December 2014.
 
 License:
   This library is under GNU Lesser General Public License (LGPL).
   Which means: free to use, modify, and redistribute, 
   but comes WITHOUT ANY WARRANTY.
*/

#pragma once  
// Ensures this header file is included only once during compilation.
// Equivalent to "#ifndef WIFI_H ... #define WIFI_H ... #endif"

/* ------------------- Required Headers ------------------- */

#include "soc/soc_caps.h"   // Provides information about ESP32 SoC capabilities
#include "sdkconfig.h"      // Configuration settings from ESP-IDF (SDK config)
#if SOC_WIFI_SUPPORTED || CONFIG_ESP_WIFI_REMOTE_ENABLED
// Compile WiFi features ONLY if the ESP32 chip supports WiFi
// or if remote WiFi configuration is enabled in sdkconfig

#include <stdint.h>         // Standard C header for fixed-width integer types

#include "Print.h"          // Arduino base class for print functionality (Serial, etc.)
#include "IPAddress.h"      // For handling IP addresses in Arduino style

// Core WiFi classes for ESP32
#include "WiFiType.h"       // Defines WiFi-related types, enums, constants
#include "WiFiSTA.h"        // Station mode (ESP32 connects to a router)
#include "WiFiAP.h"         // Access Point mode (ESP32 creates its own WiFi network)
#include "WiFiScan.h"       // Scanning for available networks
#include "WiFiGeneric.h"    // Generic WiFi functions (MAC, channel, event handling, etc.)

// Networking classes
#include "WiFiClient.h"     // TCP client (ESP32 connects to a server)
#include "WiFiServer.h"     // TCP server (ESP32 becomes a server)
#include "WiFiUdp.h"        // UDP communication

/* ------------------- WiFiClass Definition ------------------- */

// WiFiClass is the main user-facing class you interact with as "WiFi"
// Example: WiFi.begin("SSID", "password");

class WiFiClass : 
    public WiFiGenericClass,   // Inherits generic WiFi methods (init, events, channel control)
    public WiFiSTAClass,       // Inherits station mode methods (connect, disconnect, SSID, RSSI)
    public WiFiScanClass,      // Inherits scanning methods (scan for networks, get results)
    public WiFiAPClass         // Inherits access point methods (start AP, stop AP, AP settings)
{
private:
  bool prov_enable;  // Flag to check if WiFi provisioning (configuration via external method) is enabled

public:
  // Constructor
  WiFiClass() {
    prov_enable = false;  // By default, provisioning is disabled
  }

  // "using" brings specific methods from parent classes into WiFiClass scope
  // This allows the user to call WiFi.SSID() instead of WiFiSTAClass::SSID()

  using WiFiGenericClass::channel;  // Channel functions from generic class

  // From Station class
  using WiFiSTAClass::BSSID;   // Get MAC address of connected AP
  using WiFiSTAClass::BSSIDstr;// Get MAC address as string
  using WiFiSTAClass::RSSI;    // Get Received Signal Strength Indicator
  using WiFiSTAClass::SSID;    // Get SSID of connected AP

  // From Scan class
  using WiFiScanClass::BSSID;         // Get BSSID of scanned network
  using WiFiScanClass::BSSIDstr;      // Get BSSID as string
  using WiFiScanClass::channel;       // Get channel of scanned network
  using WiFiScanClass::encryptionType;// Get encryption type (WPA2, Open, etc.)
  using WiFiScanClass::RSSI;          // Get RSSI of scanned network
  using WiFiScanClass::SSID;          // Get SSID of scanned network

public:
  // Prints diagnostic info about current WiFi status (to Serial or another Print stream)
  void printDiag(Print &dest);

  // Declare friendship with networking classes
  // This allows them to access private/protected members of WiFiClass
  friend class NetworkClient;
  friend class NetworkServer;
  friend class NetworkUDP;

  // Enable or disable provisioning (external WiFi config)
  void enableProv(bool status);

  // Check if provisioning is enabled
  bool isProvEnabled();
};

// Declare global WiFi object so users can directly use "WiFi" in their sketch
extern WiFiClass WiFi;

#endif /* SOC_WIFI_SUPPORTED */
// End of conditional inclusion (only compile this if WiFi is supported on the chip)
+++
