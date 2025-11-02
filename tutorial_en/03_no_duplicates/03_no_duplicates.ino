/*
 * ============================================
 * LEVEL 3: NO DUPLICATES
 * ============================================
 * 
 * OBJECTIVE: Show each network only once
 * - Create structures (structs)
 * - Save networks in an array
 * - Search by MAC to avoid duplicates
 * 
 * NEW CONCEPTS:
 * - struct (like classes in Python but simpler)
 * - Arrays (fixed lists in C)
 * - Search functions
 * 
 * PYTHON EQUIVALENT:
 * class Network:
 *     def __init__(self, mac, ssid, rssi):
 *         self.mac = mac
 *         self.ssid = ssid
 *         self.rssi = rssi
 * 
 * networks = []  # dynamic list
 */

#include <WiFi.h>
#include "esp_wifi.h"

// ==========================================
// DEFINE STRUCTURE FOR WiFi NETWORKS
// ==========================================
/*
 * A struct is like a simple class in Python
 * Groups related data together
 * 
 * In Python it would be:
 * class Network:
 *     mac: str
 *     ssid: str
 *     rssi: int
 */

struct Network {
  String mac;    // MAC address (unique identifier)
  String ssid;   // Network name
  int rssi;      // Signal strength (dBm)
};

// ==========================================
// ARRAY TO STORE NETWORKS
// ==========================================
/*
 * In C, arrays have FIXED size
 * In Python you would use: networks = []
 * 
 * Here we define:
 * - MAX_NETWORKS = 50 (maximum capacity)
 * - networks[] = array of 50 Network structs
 * - network_count = how many networks we've found
 */

#define MAX_NETWORKS 50
Network networks[MAX_NETWORKS];  // Array of 50 networks
int network_count = 0;            // Counter

// ==========================================
// SEARCH FUNCTION: FIND NETWORK BY MAC
// ==========================================
/*
 * Searches through the array for a network with a specific MAC
 * Returns the index if found, -1 if not found
 * 
 * In Python it would be:
 * def search_network_by_mac(mac):
 *     for i, network in enumerate(networks):
 *         if network.mac == mac:
 *             return i
 *     return -1
 */

int searchNetworkByMAC(String mac) {
  for (int i = 0; i < network_count; i++) {
    if (networks[i].mac == mac) {
      return i;  // Found! Return position
    }
  }
  return -1;  // Not found
}

// ==========================================
// FUNCTION: ADD OR UPDATE NETWORK
// ==========================================
/*
 * 1. Search if the network already exists (by MAC)
 * 2. If it exists → update RSSI
 * 3. If it doesn't exist → add new network
 * 
 * In Python it would be:
 * def add_or_update_network(mac, ssid, rssi):
 *     idx = search_network_by_mac(mac)
 *     if idx != -1:
 *         networks[idx].rssi = rssi  # update
 *     else:
 *         networks.append(Network(mac, ssid, rssi))  # add
 */

void addOrUpdateNetwork(String mac, String ssid, int rssi) {
  // Search if it already exists
  int idx = searchNetworkByMAC(mac);
  
  if (idx != -1) {
    // Already exists → update signal strength
    networks[idx].rssi = rssi;
    Serial.printf("🔄 Updated: %s (RSSI: %d dBm)\n", 
                  networks[idx].ssid.c_str(), rssi);
  } else {
    // Doesn't exist → add new network
    
    // Check if we have space
    if (network_count >= MAX_NETWORKS) {
      Serial.println("⚠️  Array full! Can't add more networks");
      return;
    }
    
    // Add to the end of the array
    networks[network_count].mac = mac;
    networks[network_count].ssid = ssid;
    networks[network_count].rssi = rssi;
    network_count++;
    
    Serial.printf("✅ New network #%d: %s (RSSI: %d dBm)\n", 
                  network_count, ssid.c_str(), rssi);
  }
}

// ==========================================
// CALLBACK TO CAPTURE PACKETS
// ==========================================
void capturePacket(void *buf, wifi_promiscuous_pkt_type_t type) {
  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  const uint8_t *payload = pkt->payload;
  
  // Decode Frame Control
  uint8_t frame_control = payload[0];
  uint8_t frame_type    = (frame_control & 0b00001100) >> 2;
  uint8_t frame_subtype = (frame_control & 0b11110000) >> 4;
  
  // Filter beacons (type 0, subtype 8)
  if (frame_type == 0 && frame_subtype == 8) {
    
    // ==========================================
    // EXTRACT MAC ADDRESS
    // ==========================================
    char mac[18];  // String to store MAC (17 chars + null terminator)
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    // ==========================================
    // EXTRACT SSID
    // ==========================================
    int ssid_length = payload[37];
    String ssid = "";
    
    if (ssid_length > 0 && ssid_length <= 32) {
      for (int i = 0; i < ssid_length; i++) {
        ssid += (char)payload[38 + i];
      }
    } else {
      ssid = "(Hidden)";
    }
    
    // ==========================================
    // GET SIGNAL STRENGTH (RSSI)
    // ==========================================
    /*
     * RSSI = Received Signal Strength Indicator
     * Measured in dBm (negative values)
     * -30 dBm = excellent signal
     * -90 dBm = very weak signal
     */
    int rssi = pkt->rx_ctrl.rssi;
    
    // ==========================================
    // ADD OR UPDATE NETWORK
    // ==========================================
    addOrUpdateNetwork(String(mac), ssid, rssi);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║    LEVEL 3: No Duplicates         ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // Initialize WiFi
  Serial.println("🔧 Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  
  // Register callback
  esp_wifi_set_promiscuous_rx_cb(&capturePacket);
  
  // Activate promiscuous mode
  esp_wifi_set_promiscuous(true);
  delay(100);
  
  // Set channel
  int channel = 6;
  Serial.printf("📡 Listening on channel %d...\n\n", channel);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  
  Serial.println("✅ Capturing networks (no duplicates):\n");
  Serial.println("═══════════════════════════════════════════════════");
}

void loop() {
  // The callback runs automatically
  delay(10);
}

/*
 * ============================================
 * BEGINNER'S EXPLANATION
 * ============================================
 * 
 * WHAT IS A STRUCT?
 * - It's a way to group related data
 * - Like a simple class in Python without methods
 * - In this case: Network has mac, ssid, rssi
 * 
 * WHAT ARE ARRAYS IN C?
 * - Fixed-size lists
 * - In Python: list = [] (dynamic)
 * - In C: int array[50] (fixed to 50 elements)
 * - You must control the size manually
 * 
 * WHY USE MAC ADDRESS?
 * - SSID can be duplicated (many routers called "HOME")
 * - MAC is unique for each device (like a serial number)
 * - It's the reliable way to identify a network
 * 
 * WHAT IS RSSI?
 * - Received Signal Strength Indicator
 * - Measured in dBm (always negative)
 * - Scale:
 *   * -30 dBm = excellent (very close)
 *   * -50 dBm = good
 *   * -70 dBm = weak
 *   * -90 dBm = very poor
 * 
 * HOW DOES THE SEARCH WORK?
 * 1. For each captured beacon:
 * 2. Extract MAC, SSID, RSSI
 * 3. Search if that MAC exists in the array
 * 4. If yes → update RSSI
 * 5. If no → add new network
 * 
 * WHY UPDATE RSSI?
 * - The same router sends beacons constantly
 * - Signal strength can change (you move, interference)
 * - We keep the most recent value
 * 
 * NEXT LEVEL:
 * You will learn to display networks in a formatted table
 * and auto-remove networks that disappear.
 */
