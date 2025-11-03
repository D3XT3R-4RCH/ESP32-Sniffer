# 📡 ESP32 WiFi Sniffer

## 🎯 Project Overview

An **interactive WiFi network sniffer** for ESP32 that captures, analyzes, and displays WiFi beacon frames in real-time. Perfect for learning about:

- WiFi protocols (802.11)
- Packet capture and analysis
- ESP32 microcontroller programming
- Network security fundamentals

---


## ✨ Features

### 🔍 Core Functionality
- **Promiscuous mode** - Capture all WiFi traffic in range
- **Beacon frame analysis** - Decode and extract network information
- **MAC-based deduplication** - Show each network only once
- **Real-time RSSI tracking** - Monitor signal strength changes
- **Auto-cleanup** - Remove networks after 15-second timeout
- **Live updating table** - Refresh display every 2 seconds

### 🎮 Interactive Controls
- **Channel selection** (1-13) - Scan specific WiFi channels
- **Start/Stop scanning** - Pause and resume on demand
- **Reset list** - Clear all detected networks
- **User-friendly interface** - Simple command system

### 📊 Information Displayed
- Network SSID (name)
- MAC address (BSSID)
- Signal strength (RSSI in dBm)
- Signal quality bars (████ to ▁)
- Time since last seen
- Current WiFi channel
- Scanning status (🟢/🔴)

---

## 🛠️ Hardware Requirements

- **ESP32** board (any variant):
  - ESP32-WROOM-32
  - ESP32-DevKit
  - ESP32-DevKitC
  - ESP32-WROVER
  - Or any compatible ESP32 module

- **USB cable** (data capable, not just power)

- **Computer** with USB port

---

## 📥 Software Requirements

### Required Software
- **Arduino IDE** 1.8.x or 2.x
- **ESP32 Board Support** for Arduino

### Installation Steps

1. **Install Arduino IDE**
   - Download from: https://www.arduino.cc/en/software

2. **Add ESP32 Board Manager URL**
   - Open Arduino IDE
   - Go to `File → Preferences`
   - Add this URL to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```

3. **Install ESP32 Board Support**
   - Go to `Tools → Board → Board Manager`
   - Search for "esp32"
   - Install **"ESP32 by Espressif Systems"**

4. **Select Your Board**
   - Go to `Tools → Board → ESP32 Arduino`
   - Select your ESP32 model (e.g., "ESP32 Dev Module")
   - Go to `Tools → Port`
   - Select the correct COM/USB port

---

## 🚀 Quick Start

### 1. Upload the Code

```bash
# Clone the repository
git clone https://github.com/yourusername/ESP32-Pentesting.git

# Open the project
cd ESP32-Pentesting/ScanNetworks
# Open sniffer_v2/sniffer_v2.ino in Arduino IDE
```

### 2. Configure Arduino IDE

- **Board:** ESP32 Dev Module (or your specific model)
- **Upload Speed:** 115200
- **Flash Frequency:** 80MHz
- **Flash Mode:** QIO
- **Flash Size:** 4MB (or your board's size)
- **Partition Scheme:** Default
- **Core Debug Level:** None

### 3. Upload and Run

1. Click **Upload** button (→)
2. Wait for "Done uploading"
3. Open **Serial Monitor** (`Tools → Serial Monitor`)
4. Set baud rate to **115200**
5. Follow on-screen instructions

---

## 📖 Usage Guide

### Starting the Sniffer

```
╔════════════════════════════════════════════════════════════╗
║          📡 ESP32 WiFi SNIFFER - INTERACTIVE              ║
╚════════════════════════════════════════════════════════════╝

➤ Enter channel (1-13): 6
```

Type a channel number (1-13) and press Enter.

### Interactive Commands

| Command | Description | Example |
|---------|-------------|---------|
| `1-13` | Select WiFi channel and start scanning | `6` |
| `S` or `stop` | Stop scanning (keep current list) | `s` |
| `R` or `reset` | Clear all detected networks | `r` |

### Example Output

```
╔══════════════════════════════════════════════════════════════════════╗
║                    📡 WiFi NETWORKS DETECTED                         ║
╠════╦═══════════════════════╦═══════════════════════╦═════════╦═══════╣
║ #  ║ SSID                  ║ MAC Address           ║ Signal  ║ Time  ║
╠════╬═══════════════════════╬═══════════════════════╬═════════╬═══════╣
║ 1  ║ Home_WiFi_5G          ║ aa:bb:cc:dd:ee:ff     ║ -42 ████ ║  2s   ║
║ 2  ║ Neighbor_Network      ║ 11:22:33:44:55:66     ║ -68 ██   ║  5s   ║
║ 3  ║ (Hidden)              ║ 99:88:77:66:55:44     ║ -75 █    ║  8s   ║
╚════╩═══════════════════════╩═══════════════════════╩═════════╩═══════╝

📊 Total: 3 | 📡 Channel: 6 | 🟢 SCANNING
💡 Commands: 1-13 (channel) | S (stop) | R (reset)
```

### Signal Strength Guide

| Bars | RSSI Range | Quality | Typical Distance |
|------|------------|---------|------------------|
| ████ | -30 to -50 dBm | Excellent | 0-10 meters |
| ███  | -50 to -60 dBm | Good | 10-30 meters |
| ██   | -60 to -70 dBm | Fair | 30-50 meters |
| █    | -70 to -80 dBm | Weak | 50-100 meters |
| ▁    | < -80 dBm | Very weak | > 100 meters |

---

## 📚 Learning Tutorial

### New to ESP32 or C/C++?

Check out the **progressive tutorial** in the `tutorial/` or `tutorial_en/` folder:

- **Level 1:** Hello WiFi World (15 min)
- **Level 2:** Capture Beacons (30 min)
- **Level 3:** No Duplicates (30 min)
- **Level 4:** Live Updating Table (45 min)
- **Level 5:** Complete Interactive Sniffer (1 hour)

Each level builds upon the previous one with extensive comments and Python comparisons for beginners.

📖 **See:** [Tutorial README](tutorial_en/README.md)

---

## 🧪 Troubleshooting

### No Networks Detected

**Possible causes:**
- No WiFi routers nearby
- Wrong channel selected
- Promiscuous mode not activated
- WiFi antenna not connected (if external)

**Solutions:**
- Try channels 1, 6, and 11 (most common)
- Ensure routers are powered on
- Reset ESP32 and re-upload code
- Check WiFi antenna connections

### Serial Monitor Shows Garbage

**Cause:** Wrong baud rate

**Solution:**
- Set Serial Monitor to **115200 baud**
- Reset ESP32 after changing baud rate

### Upload Failed

**Causes:**
- Serial Monitor is open
- Wrong COM port selected
- BOOT button needed

**Solutions:**
- Close Serial Monitor before uploading
- Select correct port in `Tools → Port`
- Hold BOOT button during upload (some ESP32 models)

### ESP32 Keeps Restarting

**Causes:**
- Power supply insufficient
- Code crash/memory issue
- Hardware fault

**Solutions:**
- Use quality USB cable and power source
- Check array bounds (MAX_NETWORKS)
- Try different ESP32 board

---

## 🔬 Technical Details

### WiFi Channels Scanned

- **2.4 GHz band:** Channels 1-13
- **Frequency range:** 2412 MHz - 2472 MHz
- **Note:** Channel 14 (2484 MHz) available in some regions but not used here

### Beacon Frame Structure

```
[0-1]   Frame Control (type, subtype, flags)
[2-3]   Duration
[4-9]   Destination Address (broadcast: ff:ff:ff:ff:ff:ff)
[10-15] Source Address (router MAC / BSSID)
[16-21] BSSID
[22-23] Sequence Control
[24-35] Timestamp, Beacon Interval, Capability Info
[36]    Tag Number (0 = SSID)
[37]    SSID Length
[38+]   SSID Characters
```

### Key Functions

#### `capturePacket()`
- **Purpose:** Callback for WiFi packet capture
- **Trigger:** Every received frame in promiscuous mode
- **Processing:** Filters beacons (type 0, subtype 8)

#### `addOrUpdateNetwork()`
- **Purpose:** Deduplication and tracking
- **Logic:** Search by MAC → Update if exists, add if new
- **Timestamp:** Updates `last_seen` for timeout tracking

#### `cleanupOldNetworks()`
- **Purpose:** Remove stale networks
- **Timeout:** 15 seconds (NETWORK_TIMEOUT)
- **Method:** Array shift and counter decrement

#### `displayTable()`
- **Purpose:** Formatted output to Serial
- **Refresh:** Every 2 seconds (REFRESH_INTERVAL)
- **Format:** ASCII table with Unicode box drawing

---

## 🎓 Educational Use

This project is ideal for:

- **University courses** in networking and security
- **Cybersecurity workshops** and bootcamps
- **IoT development** training
- **Self-learning** WiFi protocols
- **Security awareness** demonstrations

### Learning Outcomes

After completing this project, you will understand:

- ✅ WiFi 802.11 frame structure
- ✅ Promiscuous mode and packet capture
- ✅ ESP32 WiFi capabilities
- ✅ Callback functions in embedded C
- ✅ Data structures (structs, arrays)
- ✅ Timestamp-based state tracking
- ✅ Serial communication and user input
- ✅ Real-time data processing

---

## 🔐 Security and Ethics

### ⚠️ Legal Notice

**This tool is for educational and authorized testing only.**

**Legal uses:**
- ✅ Scanning your own networks
- ✅ Authorized penetration testing
- ✅ Academic research with permission
- ✅ Controlled laboratory environments

**Illegal uses:**
- ❌ Intercepting private communications
- ❌ Unauthorized network attacks
- ❌ Data theft or exploitation
- ❌ Any malicious activities

### 🛡️ What This Sniffer Does

**Captures:** ONLY beacon frames (public broadcast)
- Network names (SSID)
- MAC addresses (public identifiers)
- Signal strength (RSSI)
- WiFi channels

**Does NOT capture:**
- ❌ Passwords
- ❌ Encrypted traffic content
- ❌ User data
- ❌ Authentication details

**Think of it as:** A radio scanner that shows station names and frequencies - not recording the actual broadcasts.

### 📜 Responsible Disclosure

If you discover security vulnerabilities in WiFi networks using this tool:

1. **Do NOT exploit** the vulnerability
2. **Inform** the network owner privately
3. **Allow time** for fixes before public disclosure
4. **Follow** responsible disclosure guidelines

---

## 🚀 Advanced Modifications

### Add Channel Hopping

Auto-cycle through all channels:

```cpp
void loop() {
  static int channel = 1;
  static unsigned long last_hop = 0;
  
  if (millis() - last_hop > 3000) {  // Every 3 seconds
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    channel = (channel % 13) + 1;  // Cycle 1-13
    last_hop = millis();
  }
  
  // ... rest of loop
}
```

### Save to SD Card

Add SD card support:

```cpp
#include <SD.h>
#include <SPI.h>

void saveToSD() {
  File file = SD.open("/networks.csv", FILE_WRITE);
  file.println("SSID,MAC,RSSI,Timestamp");
  
  for (int i = 0; i < network_count; i++) {
    file.printf("%s,%s,%d,%lu\n",
                networks[i].ssid.c_str(),
                networks[i].mac.c_str(),
                networks[i].rssi,
                networks[i].last_seen);
  }
  
  file.close();
}
```

### Web Interface

Serve results via HTTP:

```cpp
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  String html = "<html><body><h1>WiFi Networks</h1><table>";
  
  for (int i = 0; i < network_count; i++) {
    html += "<tr><td>" + networks[i].ssid + "</td>";
    html += "<td>" + networks[i].mac + "</td>";
    html += "<td>" + String(networks[i].rssi) + " dBm</td></tr>";
  }
  
  html += "</table></body></html>";
  server.send(200, "text/html", html);
}
```

---

## 🤝 Contributing

Contributions are welcome! Here's how:

### Ways to Contribute

1. **Bug Reports**
   - Open an issue with detailed description
   - Include ESP32 model and Arduino IDE version
   - Provide Serial Monitor output if applicable

2. **Feature Requests**
   - Describe the feature and use case
   - Explain why it would be beneficial

3. **Code Contributions**
   - Fork the repository
   - Create a feature branch
   - Submit a pull request with clear description

4. **Documentation**
   - Improve comments and explanations
   - Add translations
   - Create video tutorials

### Code Style

- Use clear variable names
- Add comments for complex logic
- Follow existing formatting
- Test on real hardware before submitting

---

## 📝 License

This project is licensed under the **MIT License**.

```
MIT License

Copyright (c) 2024

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

[Full license text in LICENSE file]
```

---

## 📧 Contact

- **Issues:** GitHub Issues tab
- **Discussions:** GitHub Discussions
- **Pull Requests:** Always welcome!

---

## 🙏 Acknowledgments

- **Espressif Systems** - ESP32 platform and documentation
- **Arduino Community** - ESP32 Arduino core
- **WiFi Security Researchers** - Educational resources
- **Open Source Contributors** - Making this possible

---

## 📚 Further Reading

### ESP32 Resources
- [ESP32 Official Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

### WiFi Protocol Resources
- [IEEE 802.11 Standard](https://standards.ieee.org/standard/802_11-2020.html)
- [WiFi Frame Types](https://mrncciew.com/2014/10/08/cwap-802-11-mgmt-frame-types/)
- [Wireshark WiFi Capture](https://wiki.wireshark.org/CaptureSetup/WLAN)

### Security Learning
- [OWASP IoT Security](https://owasp.org/www-project-internet-of-things/)
- [WiFi Security Best Practices](https://www.wi-fi.org/discover-wi-fi/security)

---

**⭐ If you found this project helpful, please star it on GitHub! ⭐**

---

**Made with ❤️ for learning and education**

*Remember: Use your skills to build, protect, and educate - never to harm.*
