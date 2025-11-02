/*
 * ============================================
 * LEVEL 4: LIVE UPDATING TABLE
 * ============================================
 * 
 * OBJECTIVE: Display networks in a live table
 * - Timestamp tracking
 * - Auto-remove old networks (timeout)
 * - Formatted table display
 * 
 * NEW CONCEPTS:
 * - millis() - Time since startup
 * - Timeouts (auto-cleanup)
 * - String formatting
 * 
 * PYTHON EQUIVALENT:
 * import time
 * last_seen = time.time()
 * if time.time() - last_seen > 15:
 *     remove_network()
 */

#include <WiFi.h>
#include "esp_wifi.h"

// ==========================================
// CONFIGURATION CONSTANTS
// ==========================================
#define MAX_NETWORKS 50
#define REFRESH_INTERVAL 2000   // Refresh table every 2 seconds
#define NETWORK_TIMEOUT 15000   // Remove networks after 15 seconds

// ==========================================
// STRUCTURE WITH TIMESTAMP
// ==========================================
/*
 * Now we add a timestamp to know when we last
 * saw each network
 * 
 * In Python:
 * class Network:
 *     mac: str
 *     ssid: str
 *     rssi: int
 *     last_seen: float  # timestamp
 */

struct Network {
  String mac;
  String ssid;
  int rssi;
  unsigned long last_seen;  // Timestamp in milliseconds
};

Network networks[MAX_NETWORKS];
int network_count = 0;

unsigned long last_refresh = 0;  // Last table update

// ==========================================
// SEARCH FUNCTION
// ==========================================
int searchNetworkByMAC(String mac) {
  for (int i = 0; i < network_count; i++) {
    if (networks[i].mac == mac) {
      return i;
    }
  }
  return -1;
}

// ==========================================
// ADD OR UPDATE WITH TIMESTAMP
// ==========================================
void addOrUpdateNetwork(String mac, String ssid, int rssi) {
  unsigned long now = millis();  // Current time
  
  int idx = searchNetworkByMAC(mac);
  
  if (idx != -1) {
    // Already exists → update RSSI and timestamp
    networks[idx].rssi = rssi;
    networks[idx].last_seen = now;  // Update "last seen"
  } else {
    // Doesn't exist → add new
    if (network_count >= MAX_NETWORKS) {
      return;
    }
    
    networks[network_count].mac = mac;
    networks[network_count].ssid = ssid;
    networks[network_count].rssi = rssi;
    networks[network_count].last_seen = now;  // Save timestamp
    network_count++;
  }
}

// ==========================================
// CLEANUP OLD NETWORKS (TIMEOUT)
// ==========================================
/*
 * Removes networks that haven't been seen in 15 seconds
 * 
 * In Python it would be:
 * def cleanup_old_networks():
 *     now = time.time()
 *     networks[:] = [n for n in networks 
 *                    if now - n.last_seen < 15]
 */

void cleanupOldNetworks() {
  unsigned long now = millis();
  
  /*
   * WARNING: When removing from an array in C, you must:
   * 1. Shift all elements after the removed one
   * 2. Decrement the counter
   * 3. Adjust the loop index
   */
  
  for (int i = 0; i < network_count; i++) {
    // Calculate time elapsed since last beacon
    unsigned long elapsed = now - networks[i].last_seen;
    
    if (elapsed > NETWORK_TIMEOUT) {
      // This network has timed out → remove it
      
      // Shift all networks after this one to the left
      for (int j = i; j < network_count - 1; j++) {
        networks[j] = networks[j + 1];
      }
      
      network_count--;  // Decrement counter
      i--;              // Adjust index (since we shifted)
    }
  }
}

// ==========================================
// DISPLAY FORMATTED TABLE
// ==========================================
void displayTable() {
  // Clear with blank lines (ANSI codes don't work in Arduino Serial Monitor)
  for (int i = 0; i < 30; i++) {
    Serial.println();
  }
  
  Serial.println("╔══════════════════════════════════════════════════════════════════════╗");
  Serial.println("║                    📡 WiFi NETWORKS DETECTED                         ║");
  Serial.println("╠════╦═══════════════════════╦═══════════════════════╦═════════╦═══════╣");
  Serial.println("║ #  ║ SSID                  ║ MAC Address           ║ Signal  ║ Time  ║");
  Serial.println("╠════╬═══════════════════════╬═══════════════════════╬═════════╬═══════╣");
  
  unsigned long now = millis();
  
  for (int i = 0; i < network_count; i++) {
    // Calculate time since last seen
    unsigned long elapsed = (now - networks[i].last_seen) / 1000;  // in seconds
    
    // Format RSSI with bar indicator
    String signal_bar = "";
    if (networks[i].rssi > -50) signal_bar = "████";       // Excellent
    else if (networks[i].rssi > -60) signal_bar = "███ ";  // Good
    else if (networks[i].rssi > -70) signal_bar = "██  ";  // Fair
    else if (networks[i].rssi > -80) signal_bar = "█   ";  // Weak
    else signal_bar = "▁   ";                              // Very weak
    
    // Print table row
    Serial.printf("║ %-2d ║ %-21s ║ %-21s ║ %-3d %s ║ %2lus   ║\n",
                  i + 1,
                  networks[i].ssid.substring(0, 21).c_str(),
                  networks[i].mac.c_str(),
                  networks[i].rssi,
                  signal_bar.c_str(),
                  elapsed);
  }
  
  Serial.println("╚════╩═══════════════════════╩═══════════════════════╩═════════╩═══════╝");
  Serial.printf("\n📊 Total networks: %d | ⏱️  Updated: every %d seconds\n", 
                network_count, REFRESH_INTERVAL / 1000);
}

// ==========================================
// CALLBACK TO CAPTURE PACKETS
// ==========================================
void capturePacket(void *buf, wifi_promiscuous_pkt_type_t type) {
  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  const uint8_t *payload = pkt->payload;
  
  uint8_t frame_control = payload[0];
  uint8_t frame_type    = (frame_control & 0b00001100) >> 2;
  uint8_t frame_subtype = (frame_control & 0b11110000) >> 4;
  
  if (frame_type == 0 && frame_subtype == 8) {
    // Extract MAC
    char mac[18];
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    // Extract SSID
    int ssid_length = payload[37];
    String ssid = "";
    
    if (ssid_length > 0 && ssid_length <= 32) {
      for (int i = 0; i < ssid_length; i++) {
        ssid += (char)payload[38 + i];
      }
    } else {
      ssid = "(Hidden)";
    }
    
    // Get RSSI
    int rssi = pkt->rx_ctrl.rssi;
    
    // Add or update
    addOrUpdateNetwork(String(mac), ssid, rssi);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   LEVEL 4: Live Updating Table    ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  
  // Register callback
  esp_wifi_set_promiscuous_rx_cb(&capturePacket);
  esp_wifi_set_promiscuous(true);
  delay(100);
  
  // Set channel
  int channel = 6;
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  
  Serial.println("✅ Scanning WiFi networks...");
  Serial.println("📌 Table updates every 2 seconds");
  Serial.println("⏱️  Networks timeout after 15 seconds\n");
  
  delay(2000);  // Wait before first table display
}

void loop() {
  unsigned long now = millis();
  
  // ==========================================
  // UPDATE TABLE EVERY 2 SECONDS
  // ==========================================
  /*
   * Check if 2 seconds have elapsed since last update
   * 
   * In Python it would be:
   * if time.time() - last_refresh > 2:
   *     display_table()
   *     last_refresh = time.time()
   */
  
  if (now - last_refresh >= REFRESH_INTERVAL) {
    cleanupOldNetworks();  // Remove old networks first
    displayTable();         // Then display table
    last_refresh = now;     // Update timestamp
  }
  
  delay(10);
}

/*
 * ============================================
 * BEGINNER'S EXPLANATION
 * ============================================
 * 
 * WHAT IS millis()?
 * - Returns time in milliseconds since ESP32 startup
 * - Example: after 5 seconds running, millis() = 5000
 * - In Python: time.time() (but in seconds, not milliseconds)
 * 
 * HOW DO TIMEOUTS WORK?
 * 1. Save current time when network is detected: last_seen = millis()
 * 2. Calculate elapsed time: elapsed = millis() - last_seen
 * 3. If elapsed > 15000 ms (15 seconds) → remove network
 * 
 * WHY 15 SECONDS?
 * - Routers send beacons every ~100ms
 * - If we haven't seen a beacon in 15 seconds = network is gone
 * - Either the router was turned off or we're out of range
 * 
 * WHY REFRESH EVERY 2 SECONDS?
 * - Too fast (100ms) → table flickers, hard to read
 * - Too slow (10s) → doesn't feel "live"
 * - 2 seconds = good balance
 * 
 * HOW TO REMOVE FROM ARRAY IN C?
 * - In Python: list.remove(item) or list.pop(index)
 * - In C: you must manually shift all elements left
 * - Example: [A, B, C, D] remove B → shift C,D left → [A, C, D, _]
 * 
 * WHAT ARE THE SIGNAL BARS?
 * - Visual representation of RSSI
 * - ████ = -50 dBm (excellent)
 * - ███  = -60 dBm (good)
 * - ██   = -70 dBm (fair)
 * - █    = -80 dBm (weak)
 * - ▁    = below -80 dBm (very weak)
 * 
 * NEXT LEVEL:
 * You will learn to add user commands to control
 * scanning (start/stop/reset) and change channels interactively.
 */
