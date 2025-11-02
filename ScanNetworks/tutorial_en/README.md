# 📡 ESP32 WiFi Sniffer - Complete Tutorial

## 🎯 What is this tutorial?

This is a **progressive educational tutorial** to learn how to create a WiFi sniffer with ESP32 from scratch. It's designed for beginners who have **Python experience** but are new to **C/C++ and microcontrollers**.

Each level builds upon the previous one, explaining new concepts with comparisons to Python.

---

## 📚 Tutorial Structure

The tutorial is divided into **5 progressive levels**:

| Level | Name | Difficulty | Time | What you'll learn |
|-------|------|------------|------|-------------------|
| **01** | Hello WiFi World | ⭐ Easy | 15 min | Serial communication, WiFi initialization, promiscuous mode |
| **02** | Capture Beacons | ⭐⭐ Medium | 30 min | Callbacks, pointers, beacon frame decoding |
| **03** | No Duplicates | ⭐⭐ Medium | 30 min | Structs, arrays, MAC address search |
| **04** | Live Updating Table | ⭐⭐⭐ Medium-High | 45 min | Timestamps, timeouts, formatted output |
| **05** | Complete Interactive Sniffer | ⭐⭐⭐⭐ Advanced | 1 hour | User input, state machines, interactive commands |

**Total estimated time:** 3-4 hours (with reading and experimentation)

---

## 🛠️ Requirements

### Hardware
- **ESP32** (any model: ESP32-WROOM-32, ESP32-DevKit, etc.)
- **USB cable** (data, not just power)
- Computer with USB port

### Software
- **Arduino IDE** 1.8.x or 2.x
- **ESP32 board support** installed
- USB serial driver (usually automatic)

### Knowledge
- Basic **Python** (comparisons are made throughout)
- Basic programming concepts (variables, functions, loops)
- **No prior C/C++ or microcontroller experience needed!**

---

## 📥 Installation

### 1. Install Arduino IDE

Download from: https://www.arduino.cc/en/software

### 2. Install ESP32 Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Board Manager**
5. Search for **"esp32"**
6. Install **"ESP32 by Espressif Systems"**

### 3. Select Your Board

1. Go to **Tools → Board → ESP32 Arduino**
2. Select your model (e.g., **"ESP32 Dev Module"**)
3. Go to **Tools → Port**
4. Select the COM/USB port where your ESP32 is connected

### 4. Test Connection

1. Open **File → Examples → 01.Basics → Blink**
2. Click **Upload** (→ button)
3. The onboard LED should blink

If it works → you're ready to start! 🚀

---

## 🚀 How to Use This Tutorial

### Recommended Learning Path

1. **Start with Level 01** - Even if you have experience, it establishes the basics
2. **Read ALL comments** - They explain every line with Python comparisons
3. **Upload each level to your ESP32** - Don't just read, test!
4. **Open Serial Monitor** (Tools → Serial Monitor, 115200 baud)
5. **Experiment** - Change values, break things, learn!
6. **Move to the next level** when you understand the current one

### How Each Level is Organized

Each `.ino` file contains:

```cpp
/* 
 * ============================================
 * LEVEL X: Name
 * ============================================
 * OBJECTIVE: What you'll learn
 * NEW CONCEPTS: Concepts introduced
 * PYTHON EQUIVALENT: How it would be in Python
 */

// Code with extensive comments

/*
 * ============================================
 * BEGINNER'S EXPLANATION
 * ============================================
 * Detailed explanations of concepts
 */
```

---

## 📖 Level Descriptions

### 📌 Level 01: Hello WiFi World

**Objective:** Understand the basics of ESP32 and WiFi.

**What you'll learn:**
- Initialize serial communication (`Serial.begin`)
- Configure WiFi in Station mode (`WiFi.mode`)
- Activate promiscuous mode (monitor mode)
- Set WiFi channel

**Key concepts:**
- What is promiscuous mode?
- What are WiFi channels?
- Basic ESP32 information

**Expected output:**
```
╔════════════════════════════════════╗
║    LEVEL 1: Hello WiFi World     ║
╚════════════════════════════════════╝

📌 ESP32 Information:
   • Chip: ESP32-D0WDQ6
   • CPU Frequency: 240 MHz
   ...
✅ WiFi initialized in STA mode
✅ Promiscuous mode activated
```

---

### 📌 Level 02: Capture Beacons

**Objective:** Capture and decode WiFi beacon frames.

**What you'll learn:**
- Callback functions (event handlers)
- Pointers in C (`void *buf`)
- Decode Frame Control bytes
- Extract SSID and MAC address

**Key concepts:**
- What is a beacon frame?
- How to decode WiFi packets?
- What are bitmasks?

**Expected output:**
```
📡 Beacon detected! MAC: aa:bb:cc:dd:ee:ff | SSID: MyWiFi
📡 Beacon detected! MAC: 11:22:33:44:55:66 | SSID: Neighbor_WiFi
```

---

### 📌 Level 03: No Duplicates

**Objective:** Show each network only once using data structures.

**What you'll learn:**
- Structures (`struct`) in C
- Fixed-size arrays
- Search functions by MAC address
- Add and update networks

**Key concepts:**
- Difference between arrays in C and lists in Python
- Why use MAC address as unique ID?
- What is RSSI (signal strength)?

**Expected output:**
```
✅ New network #1: MyWiFi (RSSI: -45 dBm)
✅ New network #2: Neighbor_WiFi (RSSI: -67 dBm)
🔄 Updated: MyWiFi (RSSI: -43 dBm)
```

---

### 📌 Level 04: Live Updating Table

**Objective:** Display networks in a formatted table with auto-cleanup.

**What you'll learn:**
- Timestamps with `millis()`
- Implement timeouts (15 seconds)
- Format tables with alignment
- Auto-refresh every 2 seconds

**Key concepts:**
- How to track time in microcontrollers?
- How to remove elements from arrays in C?
- What are good timeout and refresh values?

**Expected output:**
```
╔══════════════════════════════════════════════════════════════════════╗
║                    📡 WiFi NETWORKS DETECTED                         ║
╠════╦═══════════════════════╦═══════════════════════╦═════════╦═══════╣
║ #  ║ SSID                  ║ MAC Address           ║ Signal  ║ Time  ║
╠════╬═══════════════════════╬═══════════════════════╬═════════╬═══════╣
║ 1  ║ MyWiFi                ║ aa:bb:cc:dd:ee:ff     ║ -45 ████ ║  2s   ║
║ 2  ║ Neighbor_WiFi         ║ 11:22:33:44:55:66     ║ -67 ██   ║  5s   ║
╚════╩═══════════════════════╩═══════════════════════╩═════════╩═══════╝
```

---

### 📌 Level 05: Complete Interactive Sniffer

**Objective:** Add user control with interactive commands.

**What you'll learn:**
- Read user input from Serial
- Parse strings (`trim`, `toLowerCase`, `toInt`)
- State machines (`bool scanning`)
- Interactive command system

**Key concepts:**
- How to handle user input in Arduino?
- What is a state machine?
- How to change WiFi channels dynamically?

**Available commands:**
- `1-13` → Select channel and start scanning
- `S` or `stop` → Stop scanning
- `R` or `reset` → Clear network list

**Expected output:**
```
╔════════════════════════════════════════════════════════════╗
║          📡 ESP32 WiFi SNIFFER - INTERACTIVE              ║
╚════════════════════════════════════════════════════════════╝

➤ Enter channel (1-13): 6

📡 Channel changed to: 6
🟢 Scanning STARTED

[Live updating table...]

💡 Commands: 1-13 (channel) | S (stop) | R (reset)
```

---

## 🧪 Troubleshooting

### Problem: Nothing appears in Serial Monitor

**Solutions:**
1. Check baud rate is **115200**
2. Verify correct COM/USB port is selected
3. Close and reopen Serial Monitor
4. Press ESP32 RESET button

### Problem: "Compilation error"

**Solutions:**
1. Verify ESP32 board support is installed
2. Check correct board is selected
3. Make sure all code is copied correctly
4. Try **Sketch → Verify/Compile** first

### Problem: "Upload failed"

**Solutions:**
1. Hold BOOT button while uploading (some ESP32 models)
2. Close Serial Monitor before uploading
3. Try different USB cable
4. Check USB drivers are installed

### Problem: No networks detected

**Solutions:**
1. Make sure there are WiFi routers nearby
2. Try different channels (1, 6, 11)
3. Check promiscuous mode is activated
4. Verify WiFi antennas are connected (if external)

---

## 🔬 Technical Concepts

### What is Promiscuous Mode?

- **Normal mode:** WiFi only captures packets directed to your device
- **Promiscuous mode:** Captures ALL packets in the air
- Also called **"monitor mode"** or **"sniffer mode"**
- Essential for network analysis and security research

### WiFi Channels (2.4 GHz)

| Channel | Frequency | Common Use |
|---------|-----------|------------|
| 1 | 2412 MHz | Very common, less interference with 6 and 11 |
| 6 | 2437 MHz | Most used worldwide |
| 11 | 2462 MHz | Common, less interference with 1 and 6 |
| 2-5, 7-10, 12-13 | Various | Less common, more interference |

**Tip:** Channels 1, 6, and 11 don't overlap, so they're the most used by routers.

### Beacon Frames

- **Type:** 0 (Management)
- **Subtype:** 8 (Beacon)
- **Frequency:** ~10 per second (every 100ms)
- **Content:** SSID, MAC, channel, security, capabilities

### RSSI (Signal Strength)

| RSSI (dBm) | Quality | Distance (approx) |
|------------|---------|-------------------|
| -30 to -50 | Excellent | 0-10 meters |
| -50 to -60 | Good | 10-30 meters |
| -60 to -70 | Fair | 30-50 meters |
| -70 to -80 | Weak | 50-100 meters |
| < -80 | Very weak | > 100 meters |

---

## 🎓 Next Steps After Completing the Tutorial

Once you've mastered all 5 levels, you can:

### 🚀 Improvements and Extensions

1. **Channel Hopping**
   - Auto-scan all channels (1-13) in rotation
   - Complete scan every few seconds

2. **Data Persistence**
   - Save networks to SD card
   - Generate CSV or JSON files
   - Network history logs

3. **Web Interface**
   - Create access point with ESP32
   - Serve results via HTTP
   - Real-time web dashboard

4. **Advanced Analysis**
   - Detect probe requests
   - Identify device manufacturers (OUI lookup)
   - Deauthentication attack detection
   - Hidden network discovery

5. **Optimization**
   - Reduce memory consumption
   - Optimize callback performance
   - Battery saving modes

### 📚 Recommended Learning Resources

- **ESP32 Official Documentation:** https://docs.espressif.com/
- **WiFi 802.11 Protocol:** Study frame structures
- **Wireshark:** Learn to analyze WiFi traffic
- **Scapy (Python):** Practice packet manipulation

---

## ⚖️ Legal and Ethical Notice

### ⚠️ IMPORTANT: Responsible Use

This tutorial is for **educational purposes only**. When using WiFi sniffers:

**✅ Legal:**
- Analyze YOUR OWN networks
- Study in controlled laboratory environments
- Educational and research projects
- Capture only metadata (SSID, MAC, signal)

**❌ Illegal/Unethical:**
- Intercept private communications
- Decrypt passwords or sensitive data
- Perform attacks on third-party networks
- Use without permission on corporate networks

**Remember:**
- Laws vary by country
- Ignorance is not a defense
- Always obtain proper authorization
- Use your knowledge ethically

---

## 🤝 Contributing

Found an error? Have an improvement? Want to add a new level?

**Ways to contribute:**
1. Open an Issue reporting errors or suggestions
2. Submit a Pull Request with improvements
3. Share your modifications and extensions
4. Help translate to other languages

---

## 📝 License

This project is under the **MIT License** - see the LICENSE file for details.

Free to use, modify, and distribute with attribution.

---

## 👨‍💻 Author

Created with ❤️ for the ESP32 and cybersecurity learning community.

**If this tutorial helped you, give it a ⭐ on GitHub!**

---

## 🙏 Acknowledgments

- ESP32 community for excellent documentation
- Python programmers transitioning to microcontrollers
- Everyone who contributes to open-source education

---

**Happy hacking! 🚀🔒**

*Remember: With great power comes great responsibility. Use your knowledge to learn, teach, and protect - never to harm.*
