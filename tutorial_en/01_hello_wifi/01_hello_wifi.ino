/*
 * ============================================
 * LEVEL 1: HELLO WIFI WORLD
 * ============================================
 * 
 * OBJECTIVE: Understand basic concepts
 * - Initialize serial communication
 * - Initialize ESP32 WiFi
 * - Show chip information
 * 
 * NEW CONCEPTS:
 * - Serial.begin() - Start communication
 * - WiFi.mode() - Configure WiFi mode
 * - esp_wifi_set_promiscuous() - Monitor mode
 * 
 * PYTHON EQUIVALENT:
 * import serial
 * import wifi
 * wifi.set_monitor_mode(True)
 */

#include <WiFi.h>       // ESP32 WiFi library
#include "esp_wifi.h"   // Advanced WiFi functions

void setup() {
  // ==========================================
  // 1. INITIALIZE SERIAL (like print in Python)
  // ==========================================
  Serial.begin(115200);  // Speed: 115200 baud
  delay(1000);           // Wait 1 second
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║    LEVEL 1: Hello WiFi World     ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // ==========================================
  // 2. SHOW ESP32 CHIP INFORMATION
  // ==========================================
  Serial.println("📌 ESP32 Information:");
  Serial.printf("   • Chip: %s\n", ESP.getChipModel());
  Serial.printf("   • CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("   • Free Memory: %d bytes\n", ESP.getFreeHeap());
  
  // Get ESP32 MAC address (unique physical address)
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);
  Serial.printf("   • MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  // ==========================================
  // 3. INITIALIZE WiFi IN STATION MODE
  // ==========================================
  Serial.println("🔧 Initializing WiFi...");
  
  // Configure WiFi in STA (Station) mode
  // WiFi must be initialized before promiscuous mode
  WiFi.mode(WIFI_STA);
  delay(100);
  
  // Disconnect from any network
  WiFi.disconnect();
  delay(100);
  
  Serial.println("✅ WiFi initialized in STA mode\n");
  
  // ==========================================
  // 4. ACTIVATE PROMISCUOUS MODE (Monitor Mode)
  // ==========================================
  Serial.println("🔓 Activating promiscuous mode...");
  
  // Promiscuous mode = capture ALL WiFi packets
  // Not just the ones directed to us
  esp_wifi_set_promiscuous(true);
  delay(100);
  
  Serial.println("✅ Promiscuous mode activated");
  Serial.println("   (Now we can capture WiFi packets)\n");
  
  // ==========================================
  // 5. CONFIGURE WiFi CHANNEL
  // ==========================================
  int channel = 6;  // Channel 6 (one of the most used)
  
  Serial.printf("📡 Configuring channel %d...\n", channel);
  
  // Set the WiFi channel to listen
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  
  Serial.printf("✅ Listening on channel %d\n\n", channel);
  
  // ==========================================
  // SUMMARY
  // ==========================================
  Serial.println("╔════════════════════════════════════╗");
  Serial.println("║        ✨ ALL READY ✨            ║");
  Serial.println("╠════════════════════════════════════╣");
  Serial.println("║ WiFi:    Initialized ✓            ║");
  Serial.println("║ Mode:    Promiscuous ✓            ║");
  Serial.println("║ Channel: 6 ✓                      ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  Serial.println("💡 Next step: Level 2 - Capture beacons");
}

void loop() {
  // For now, we do nothing in the loop
  // Just keep WiFi in promiscuous mode
  
  delay(1000);
  Serial.println("⏱️  WiFi listening... (nothing captured yet)");
}

/*
 * ============================================
 * BEGINNER'S EXPLANATION
 * ============================================
 * 
 * WHAT IS PROMISCUOUS MODE?
 * - Normally, your WiFi only "listens" to packets directed to you
 * - In promiscuous mode, you listen to ALL packets in the air
 * - It's like putting your WiFi ear in "spy mode"
 * 
 * WHAT ARE WiFi CHANNELS?
 * - WiFi 2.4 GHz has 13 channels (1-13)
 * - Each router broadcasts on ONE specific channel
 * - You must tune to the correct channel to capture packets
 * - Popular channels: 1, 6, 11 (less interference)
 * 
 * IMPORTANT COMMANDS:
 * - Serial.begin(115200)           → Start serial
 * - WiFi.mode(WIFI_STA)            → Station mode
 * - esp_wifi_set_promiscuous(true) → Activate monitor mode
 * - esp_wifi_set_channel(6, ...)   → Tune to channel 6
 * 
 * NEXT LEVEL:
 * You will learn to capture and decode beacon packets
 * that routers constantly broadcast.
 */
