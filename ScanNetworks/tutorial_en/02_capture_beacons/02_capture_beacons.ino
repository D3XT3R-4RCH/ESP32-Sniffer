/*
 * ============================================
 * LEVEL 2: CAPTURE BEACONS
 * ============================================
 * 
 * OBJECTIVE: Capture and decode WiFi packets
 * - Create a callback function
 * - Decode beacon frames
 * - Extract SSID and MAC
 * 
 * NEW CONCEPTS:
 * - Callback functions (like event handlers)
 * - Pointers (void *buf)
 * - Frame Control (frame type and subtype)
 * 
 * PYTHON EQUIVALENT:
 * def on_packet_received(packet):
 *     if packet.type == "beacon":
 *         print(packet.ssid)
 */

#include <WiFi.h>
#include "esp_wifi.h"

// ==========================================
// CALLBACK FUNCTION - "Event Handler"
// ==========================================
/*
 * In C, a callback is a function that is called
 * when an event occurs.
 * 
 * In Python it would be:
 * def my_callback(data):
 *     print("Event received!")
 * 
 * esp_wifi_set_promiscuous_rx_cb(my_callback)
 * is like saying: "call my_callback every time a packet arrives"
 */

void my_callback(void *buf, wifi_promiscuous_pkt_type_t type) {
  // ==========================================
  // 1. UNDERSTAND THE PARAMETERS
  // ==========================================
  /*
   * buf  = pointer to packet data (like a reference in Python)
   * type = packet type (DATA, MGMT, CTRL)
   * 
   * In Python it would be:
   * def my_callback(packet_data, packet_type):
   */
  
  // Convert pointer to usable structure
  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  
  // Get pointer to packet payload (actual data)
  const uint8_t *payload = pkt->payload;
  
  // ==========================================
  // 2. DECODE FRAME CONTROL (first 2 bytes)
  // ==========================================
  /*
   * Frame Control tells us what TYPE of packet it is:
   * - Type 0 = Management frame (beacons, probe requests)
   * - Type 1 = Control frame
   * - Type 2 = Data frame
   * 
   * Byte structure:
   * payload[0] = first byte (contains subtype and part of type)
   * payload[1] = second byte
   */
  
  uint8_t frame_control = payload[0];
  
  // Extract type and subtype using bit masks
  uint8_t frame_type    = (frame_control & 0b00001100) >> 2;  // Bits 2-3
  uint8_t frame_subtype = (frame_control & 0b11110000) >> 4;  // Bits 4-7
  
  /*
   * BITMASK EXPLANATION:
   * 
   * If frame_control = 10000000 (in binary)
   * 
   * frame_type:
   * 10000000 & 00001100 = 00000000 → shift >> 2 → 00 = type 0
   * 
   * frame_subtype:
   * 10000000 & 11110000 = 10000000 → shift >> 4 → 1000 = subtype 8
   * 
   * In Python it would be:
   * frame_type = (byte & 0b00001100) >> 2
   * frame_subtype = (byte & 0b11110000) >> 4
   */
  
  // ==========================================
  // 3. FILTER BEACONS
  // ==========================================
  /*
   * Beacons are management frames (type 0) with subtype 8
   * 
   * In Python it would be:
   * if frame_type == 0 and frame_subtype == 8:
   *     print("Beacon detected!")
   */
  
  if (frame_type == 0 && frame_subtype == 8) {
    // ==========================================
    // 4. EXTRACT MAC ADDRESS (source)
    // ==========================================
    /*
     * 802.11 frame structure:
     * [0-1]   Frame Control
     * [2-3]   Duration
     * [4-9]   Destination MAC
     * [10-15] Source MAC (the router that sent the beacon)
     * [16-21] BSSID
     * 
     * In Python it would be:
     * source_mac = packet[10:16]
     */
    
    Serial.printf("📡 Beacon detected! MAC: %02x:%02x:%02x:%02x:%02x:%02x",
                  payload[10], payload[11], payload[12],
                  payload[13], payload[14], payload[15]);
    
    // ==========================================
    // 5. EXTRACT SSID (network name)
    // ==========================================
    /*
     * SSID is inside the beacon payload:
     * [36] = SSID length (number of characters)
     * [37...] = SSID characters
     * 
     * In Python it would be:
     * ssid_length = packet[36]
     * ssid = packet[37:37+ssid_length].decode('utf-8')
     */
    
    int ssid_length = payload[37];  // SSID length
    
    if (ssid_length > 0 && ssid_length <= 32) {
      Serial.print(" | SSID: ");
      
      // Print each SSID character
      for (int i = 0; i < ssid_length; i++) {
        Serial.print((char)payload[38 + i]);
      }
      
      Serial.println();
    } else {
      Serial.println(" | Hidden network");
    }
  }
}

void setup() {
  // ==========================================
  // INITIALIZE SERIAL
  // ==========================================
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   LEVEL 2: Capture Beacons        ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // ==========================================
  // INITIALIZE WiFi
  // ==========================================
  Serial.println("🔧 Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  
  // ==========================================
  // REGISTER CALLBACK FUNCTION
  // ==========================================
  Serial.println("📞 Registering callback...");
  
  /*
   * esp_wifi_set_promiscuous_rx_cb() tells the ESP32:
   * "When you receive a packet, call this function"
   * 
   * In Python it would be:
   * wifi.on_packet_received = my_callback
   */
  
  esp_wifi_set_promiscuous_rx_cb(&my_callback);
  
  // ==========================================
  // ACTIVATE PROMISCUOUS MODE
  // ==========================================
  Serial.println("🔓 Activating promiscuous mode...");
  esp_wifi_set_promiscuous(true);
  delay(100);
  
  // ==========================================
  // CONFIGURE CHANNEL
  // ==========================================
  int channel = 6;
  Serial.printf("📡 Listening on channel %d...\n\n", channel);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  
  Serial.println("✅ Capturing beacons! Watch below:\n");
  Serial.println("═══════════════════════════════════════════════════");
}

void loop() {
  // The callback runs automatically when packets are received
  // No code needed here!
  
  delay(10);  // Small delay to avoid blocking
}

/*
 * ============================================
 * BEGINNER'S EXPLANATION
 * ============================================
 * 
 * WHAT IS A BEACON?
 * - It's a packet that routers send every ~100ms
 * - Contains information about the network:
 *   * SSID (network name)
 *   * MAC address (unique router identifier)
 *   * Channel
 *   * Security (WPA2, etc.)
 * 
 * WHAT IS A CALLBACK?
 * - It's a function that runs when an event occurs
 * - In this case: "when a packet arrives, call my_callback"
 * - The ESP32 handles this automatically, you just provide the function
 * 
 * WHAT ARE POINTERS?
 * - In C, a pointer is a variable that contains a memory address
 * - void *buf = pointer to packet data
 * - In Python you don't use pointers explicitly
 * 
 * WHAT IS FRAME CONTROL?
 * - First 2 bytes of a WiFi packet
 * - Tells you the packet type:
 *   * Type 0, Subtype 8 = Beacon
 *   * Type 0, Subtype 4 = Probe Request
 *   * Type 2 = Data
 * 
 * WHY ARE SOME NETWORKS "Hidden"?
 * - Some routers don't broadcast their SSID
 * - The beacon has ssid_length = 0
 * - You need to know the name beforehand to connect
 * 
 * NEXT LEVEL:
 * You will learn to avoid duplicate networks
 * using data structures (structs and arrays).
 */
