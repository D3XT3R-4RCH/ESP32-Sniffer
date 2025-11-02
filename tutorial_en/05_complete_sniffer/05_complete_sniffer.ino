/*
 * ============================================
 * LEVEL 5: COMPLETE INTERACTIVE SNIFFER
 * ============================================
 * 
 * OBJECTIVE: Full-featured WiFi scanner
 * - Start/stop scanning control
 * - Interactive channel selection
 * - Reset command
 * - State management
 * 
 * NEW CONCEPTS:
 * - String parsing (readStringUntil, trim)
 * - State machine (bool scanning)
 * - User input handling
 * 
 * PYTHON EQUIVALENT:
 * user_input = input("Enter command: ")
 * if user_input == "stop":
 *     scanning = False
 */

#include <WiFi.h>
#include "esp_wifi.h"

// ==========================================
// CONFIGURATION
// ==========================================
#define MAX_NETWORKS 50
#define REFRESH_INTERVAL 2000
#define NETWORK_TIMEOUT 15000

// ==========================================
// STRUCTURE
// ==========================================
struct Network {
  String mac;
  String ssid;
  int rssi;
  unsigned long last_seen;
};

Network networks[MAX_NETWORKS];
int network_count = 0;
unsigned long last_refresh = 0;

// ==========================================
// SCANNING STATE (State Machine)
// ==========================================
/*
 * A state machine uses a boolean to control behavior
 * 
 * In Python:
 * scanning = True
 * while scanning:
 *     scan_networks()
 */

bool scanning = false;  // Start in stopped state
int current_channel = 6;

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
// ADD OR UPDATE
// ==========================================
void addOrUpdateNetwork(String mac, String ssid, int rssi) {
  unsigned long now = millis();
  int idx = searchNetworkByMAC(mac);
  
  if (idx != -1) {
    networks[idx].rssi = rssi;
    networks[idx].last_seen = now;
  } else {
    if (network_count >= MAX_NETWORKS) return;
    
    networks[network_count].mac = mac;
    networks[network_count].ssid = ssid;
    networks[network_count].rssi = rssi;
    networks[network_count].last_seen = now;
    network_count++;
  }
}

// ==========================================
// CLEANUP OLD NETWORKS
// ==========================================
void cleanupOldNetworks() {
  unsigned long now = millis();
  
  for (int i = 0; i < network_count; i++) {
    unsigned long elapsed = now - networks[i].last_seen;
    
    if (elapsed > NETWORK_TIMEOUT) {
      for (int j = i; j < network_count - 1; j++) {
        networks[j] = networks[j + 1];
      }
      network_count--;
      i--;
    }
  }
}

// ==========================================
// DISPLAY TABLE
// ==========================================
void displayTable() {
  for (int i = 0; i < 30; i++) Serial.println();
  
  Serial.println("╔══════════════════════════════════════════════════════════════════════╗");
  Serial.println("║                    📡 WiFi NETWORKS DETECTED                         ║");
  Serial.println("╠════╦═══════════════════════╦═══════════════════════╦═════════╦═══════╣");
  Serial.println("║ #  ║ SSID                  ║ MAC Address           ║ Signal  ║ Time  ║");
  Serial.println("╠════╬═══════════════════════╬═══════════════════════╬═════════╬═══════╣");
  
  unsigned long now = millis();
  
  for (int i = 0; i < network_count; i++) {
    unsigned long elapsed = (now - networks[i].last_seen) / 1000;
    
    String signal_bar = "";
    if (networks[i].rssi > -50) signal_bar = "████";
    else if (networks[i].rssi > -60) signal_bar = "███ ";
    else if (networks[i].rssi > -70) signal_bar = "██  ";
    else if (networks[i].rssi > -80) signal_bar = "█   ";
    else signal_bar = "▁   ";
    
    Serial.printf("║ %-2d ║ %-21s ║ %-21s ║ %-3d %s ║ %2lus   ║\n",
                  i + 1,
                  networks[i].ssid.substring(0, 21).c_str(),
                  networks[i].mac.c_str(),
                  networks[i].rssi,
                  signal_bar.c_str(),
                  elapsed);
  }
  
  Serial.println("╚════╩═══════════════════════╩═══════════════════════╩═════════╩═══════╝");
  Serial.printf("\n📊 Total: %d | 📡 Channel: %d | ", network_count, current_channel);
  
  if (scanning) {
    Serial.println("🟢 SCANNING");
  } else {
    Serial.println("🔴 STOPPED");
  }
  
  Serial.println("\n💡 Commands: 1-13 (channel) | S (stop) | R (reset)");
}

// ==========================================
// CALLBACK TO CAPTURE PACKETS
// ==========================================
void capturePacket(void *buf, wifi_promiscuous_pkt_type_t type) {
  // Only capture if scanning is active
  if (!scanning) return;
  
  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  const uint8_t *payload = pkt->payload;
  
  uint8_t frame_control = payload[0];
  uint8_t frame_type    = (frame_control & 0b00001100) >> 2;
  uint8_t frame_subtype = (frame_control & 0b11110000) >> 4;
  
  if (frame_type == 0 && frame_subtype == 8) {
    char mac[18];
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    int ssid_length = payload[37];
    String ssid = "";
    
    if (ssid_length > 0 && ssid_length <= 32) {
      for (int i = 0; i < ssid_length; i++) {
        ssid += (char)payload[38 + i];
      }
    } else {
      ssid = "(Hidden)";
    }
    
    int rssi = pkt->rx_ctrl.rssi;
    addOrUpdateNetwork(String(mac), ssid, rssi);
  }
}

// ==========================================
// PROCESS USER COMMANDS
// ==========================================
/*
 * Reads commands from Serial and executes actions
 * 
 * In Python:
 * def process_command(cmd):
 *     if cmd == "stop":
 *         scanning = False
 *     elif cmd == "reset":
 *         networks.clear()
 *     elif cmd.isdigit():
 *         channel = int(cmd)
 */

void processCommand(String cmd) {
  cmd.trim();       // Remove whitespace
  cmd.toLowerCase(); // Convert to lowercase
  
  // ==========================================
  // COMMAND: STOP (or S)
  // ==========================================
  if (cmd == "stop" || cmd == "s") {
    scanning = false;
    Serial.println("\n⏸️  Scanning STOPPED");
    Serial.println("💡 Enter a channel (1-13) to start scanning again\n");
  }
  
  // ==========================================
  // COMMAND: RESET (or R)
  // ==========================================
  else if (cmd == "reset" || cmd == "r") {
    network_count = 0;  // Clear network list
    Serial.println("\n🔄 List RESET");
    Serial.println("💡 All networks cleared\n");
  }
  
  // ==========================================
  // COMMAND: CHANNEL (1-13)
  // ==========================================
  else {
    int channel = cmd.toInt();  // Convert string to integer
    
    if (channel >= 1 && channel <= 13) {
      current_channel = channel;
      esp_wifi_set_channel(current_channel, WIFI_SECOND_CHAN_NONE);
      scanning = true;
      
      Serial.printf("\n📡 Channel changed to: %d\n", current_channel);
      Serial.println("🟢 Scanning STARTED\n");
    } else {
      Serial.println("\n❌ Invalid command");
      Serial.println("💡 Valid: 1-13 (channel), S (stop), R (reset)\n");
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // ==========================================
  // WELCOME SCREEN
  // ==========================================
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║                                                            ║");
  Serial.println("║          📡 ESP32 WiFi SNIFFER - INTERACTIVE              ║");
  Serial.println("║                                                            ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");
  
  Serial.println("📚 TUTORIAL LEVEL 5: Complete Interactive Sniffer\n");
  
  // ==========================================
  // INITIALIZE WiFi
  // ==========================================
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  
  esp_wifi_set_promiscuous_rx_cb(&capturePacket);
  esp_wifi_set_promiscuous(true);
  delay(100);
  
  Serial.println("✅ WiFi initialized in promiscuous mode");
  Serial.println("🔴 Scanning STOPPED (waiting for commands)\n");
  
  // ==========================================
  // SHOW INSTRUCTIONS
  // ==========================================
  Serial.println("╔════════════════════════════════════════════════════════════╗");
  Serial.println("║                    📖 INSTRUCTIONS                        ║");
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║                                                            ║");
  Serial.println("║  Enter a WiFi channel (1-13) to start scanning:           ║");
  Serial.println("║                                                            ║");
  Serial.println("║  • 1-13  → Select channel and start scanning               ║");
  Serial.println("║  • S     → Stop scanning                                   ║");
  Serial.println("║  • R     → Reset network list                              ║");
  Serial.println("║                                                            ║");
  Serial.println("║  Popular channels: 1, 6, 11 (less interference)            ║");
  Serial.println("║                                                            ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");
  
  Serial.print("➤ Enter channel (1-13): ");
}

void loop() {
  unsigned long now = millis();
  
  // ==========================================
  // CHECK FOR USER INPUT
  // ==========================================
  /*
   * Serial.available() checks if there's data in the serial buffer
   * 
   * In Python:
   * if input_available():
   *     cmd = input()
   *     process_command(cmd)
   */
  
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');  // Read until Enter key
    Serial.println(cmd);  // Echo command
    processCommand(cmd);   // Execute command
    
    if (!scanning) {
      Serial.print("\n➤ Enter command: ");
    }
  }
  
  // ==========================================
  // UPDATE TABLE IF SCANNING
  // ==========================================
  if (scanning && (now - last_refresh >= REFRESH_INTERVAL)) {
    cleanupOldNetworks();
    displayTable();
    last_refresh = now;
  }
  
  delay(10);
}

/*
 * ============================================
 * BEGINNER'S EXPLANATION
 * ============================================
 * 
 * WHAT IS A STATE MACHINE?
 * - A pattern that controls program behavior with states
 * - In this case: scanning (true/false)
 * - When scanning=true → capture packets and update table
 * - When scanning=false → ignore packets, wait for commands
 * 
 * HOW DOES STRING PARSING WORK?
 * - Serial.readStringUntil('\n') → read until Enter key
 * - trim() → remove spaces/tabs/newlines
 * - toLowerCase() → convert "STOP" to "stop"
 * - toInt() → convert "6" to 6
 * 
 * In Python it would be:
 * cmd = input().strip().lower()
 * channel = int(cmd)
 * 
 * WHY ECHO THE COMMAND?
 * - Serial.println(cmd) shows what you typed
 * - In some terminals you don't see what you type
 * - Echoing gives visual feedback
 * 
 * HOW TO CHANGE CHANNELS?
 * - Each WiFi channel is a different frequency
 * - Channel 1 = 2412 MHz, Channel 6 = 2437 MHz, etc.
 * - esp_wifi_set_channel() tunes the radio
 * - You can only listen to ONE channel at a time
 * 
 * WHY STOP SCANNING?
 * - To pause and analyze current results
 * - To change channels without clearing the list
 * - To save battery (in battery-powered projects)
 * 
 * WHY RESET?
 * - Clear the list when changing locations
 * - Start fresh when testing
 * - Remove all old/stale networks
 * 
 * PRACTICAL USAGE:
 * 1. Start with channel 6 (most common)
 * 2. Let it scan for 30 seconds
 * 3. Stop with "S" to analyze
 * 4. Try channel 1 or 11 to find more networks
 * 5. Reset with "R" if you move to a new location
 * 
 * NEXT STEPS:
 * - Try adding channel hopping (auto-scan all channels)
 * - Save networks to SD card
 * - Add web interface
 * - Implement deauth detection
 * - Capture probe requests
 * 
 * CONGRATULATIONS!
 * You've completed all 5 tutorial levels!
 * You now understand:
 * ✅ WiFi initialization and promiscuous mode
 * ✅ Callback functions and packet capture
 * ✅ Structs and arrays in C
 * ✅ Timestamp tracking and timeouts
 * ✅ String parsing and user input
 * ✅ State machines and control flow
 * 
 * Keep experimenting and learning! 🚀
 */
