# 📚 WiFi and ESP32 Fundamental Concepts

This guide explains the key technical concepts used in the ESP32 WiFi Sniffer project. It's designed for beginners who want to understand **how WiFi works at a low level**.

---

## 📡 WiFi Fundamentals

### What is WiFi?

**WiFi** (Wireless Fidelity) is a technology that allows devices to communicate wirelessly using radio waves.

**Key characteristics:**
- Based on the **IEEE 802.11** standard
- Operates in the **2.4 GHz** and **5 GHz** frequency bands
- Uses **CSMA/CA** (Carrier Sense Multiple Access with Collision Avoidance)
- Organizes data in **frames** (similar to packets)

**Analogy:**
Think of WiFi like a radio station:
- **Router** = radio transmitter
- **Device** = radio receiver
- **Channel** = specific frequency
- **Frame** = message being broadcast

---

## 📻 WiFi Channels

### What is a WiFi Channel?

A **channel** is a specific frequency within the WiFi band where communication occurs.

### 2.4 GHz Band (Used in This Project)

| Channel | Frequency (MHz) | Bandwidth | Common Use |
|---------|-----------------|-----------|------------|
| 1 | 2412 | 20 MHz | Very common, recommended |
| 2 | 2417 | 20 MHz | Rarely used |
| 3 | 2422 | 20 MHz | Rarely used |
| 4 | 2427 | 20 MHz | Rarely used |
| 5 | 2432 | 20 MHz | Rarely used |
| **6** | **2437** | **20 MHz** | **Very common, recommended** |
| 7 | 2442 | 20 MHz | Rarely used |
| 8 | 2447 | 20 MHz | Rarely used |
| 9 | 2452 | 20 MHz | Rarely used |
| 10 | 2457 | 20 MHz | Rarely used |
| **11** | **2462** | **20 MHz** | **Very common, recommended** |
| 12 | 2467 | 20 MHz | Regional availability |
| 13 | 2472 | 20 MHz | Regional availability |
| 14 | 2484 | 20 MHz | Japan only (not used here) |

### Non-Overlapping Channels

Channels 1, 6, and 11 **don't overlap** with each other:

```
Channel 1:  [====]
Channel 6:        [====]
Channel 11:             [====]

Frequency: 2412  2437  2462 MHz
```

**Why is this important?**
- Overlapping channels cause interference
- Routers typically use channels 1, 6, or 11 to minimize interference
- When scanning, prioritize these channels for best results

---

## 📦 WiFi Frames

### Frame Structure

A WiFi **frame** is a unit of data transmitted over the air. Similar to a network packet but with specific WiFi structure.

**Basic frame structure:**

```
┌─────────────────┬──────────────┬──────────┬─────────────┬─────┐
│ Frame Control   │  Duration    │ Address  │   Payload   │ FCS │
│    (2 bytes)    │  (2 bytes)   │ (6-24 B) │ (0-2312 B)  │(4 B)│
└─────────────────┴──────────────┴──────────┴─────────────┴─────┘
```

### Frame Types

WiFi has **3 main frame types**:

| Type | Value | Purpose | Examples |
|------|-------|---------|----------|
| **Management** | 0 | Network administration | Beacons, Probe Requests, Authentication |
| **Control** | 1 | Medium access control | ACK, RTS, CTS |
| **Data** | 2 | User data transmission | TCP/IP packets, HTTP data |

### Frame Control Field

The **first 2 bytes** of every frame contain critical information:

```
Byte 0:                    Byte 1:
┌──┬──┬──┬──┬──┬──┬──┬──┐  ┌──┬──┬──┬──┬──┬──┬──┬──┐
│  Version  │Type │Subt.│  │Flags and control bits│
└──┴──┴──┴──┴──┴──┴──┴──┘  └──┴──┴──┴──┴──┴──┴──┴──┘
 0  1  2  3  4  5  6  7    8  9 10 11 12 13 14 15
```

**Extracting type and subtype:**

```c
uint8_t frame_control = payload[0];
uint8_t type    = (frame_control & 0b00001100) >> 2;  // Bits 2-3
uint8_t subtype = (frame_control & 0b11110000) >> 4;  // Bits 4-7
```

**Example: Beacon frame**
- Type = 0 (Management)
- Subtype = 8 (Beacon)
- Binary: `10000000` in byte 0

---

## 📢 Beacon Frames

### What is a Beacon?

A **beacon** is a frame that routers broadcast periodically to announce their presence.

**Characteristics:**
- **Frequency:** ~10 per second (every 100 milliseconds)
- **Type:** Management frame (type 0, subtype 8)
- **Destination:** Broadcast (ff:ff:ff:ff:ff:ff)
- **Purpose:** Network advertisement and discovery

### Beacon Frame Structure

```
Offset  Field               Size    Description
──────────────────────────────────────────────────────────
0-1     Frame Control       2 B     Type 0, Subtype 8
2-3     Duration            2 B     Usually 0
4-9     Destination         6 B     ff:ff:ff:ff:ff:ff (broadcast)
10-15   Source (BSSID)      6 B     Router's MAC address
16-21   BSSID               6 B     Same as source
22-23   Sequence Control    2 B     Frame numbering
24-31   Timestamp           8 B     Time since router boot
32-33   Beacon Interval     2 B     Time between beacons (usually 100ms)
34-35   Capability Info     2 B     Features (encryption, QoS, etc.)
36+     Tagged Parameters   Var.    SSID, channel, rates, etc.
```

### SSID Extraction

The **SSID** (network name) is in the tagged parameters:

```
Offset  Field       Value   Description
────────────────────────────────────────
36      Tag Number  0       SSID tag
37      Length      X       SSID length (0-32)
38+     SSID        Var.    Network name (ASCII)
```

**Code to extract SSID:**

```c
int ssid_length = payload[37];

if (ssid_length > 0 && ssid_length <= 32) {
  for (int i = 0; i < ssid_length; i++) {
    char c = payload[38 + i];
    Serial.print(c);
  }
} else {
  Serial.print("(Hidden Network)");
}
```

### Hidden Networks

**Hidden networks** don't broadcast their SSID:
- SSID length = 0
- Tag still present but empty
- Network is still detectable by MAC address
- Devices must know the SSID beforehand to connect

**Security note:** Hiding SSID provides minimal security - MAC address is still visible.

---

## 🆔 MAC Address

### What is a MAC Address?

**MAC** (Media Access Control) is a **unique 48-bit identifier** assigned to each network interface.

**Format:** `aa:bb:cc:dd:ee:ff` (6 bytes in hexadecimal)

**Example:** `ac:84:c6:3f:a1:2b`

### MAC Address Structure

```
┌─────────────────────┬─────────────────────┐
│   OUI (24 bits)     │   NIC (24 bits)     │
│  Manufacturer ID    │  Device ID          │
└─────────────────────┴─────────────────────┘
  ac:84:c6            :3f:a1:2b
  (TP-Link)           (unique device)
```

### OUI (Organizationally Unique Identifier)

The **first 3 bytes** identify the manufacturer:

| OUI | Manufacturer |
|-----|-------------|
| `00:1A:11` | Google |
| `3C:37:86` | Apple |
| `AC:84:C6` | TP-Link |
| `F4:F5:D8` | Google Nest |
| `B8:27:EB` | Raspberry Pi Foundation |

**Database:** https://maclookup.app/

### BSSID vs MAC

- **BSSID** (Basic Service Set Identifier) = Router's MAC address
- Used to uniquely identify a WiFi access point
- In beacon frames: BSSID is at bytes 10-15

**Code to extract MAC:**

```c
char mac[18];  // Buffer for "aa:bb:cc:dd:ee:ff\0"

sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
        payload[10], payload[11], payload[12],
        payload[13], payload[14], payload[15]);
```

---

## 📶 RSSI (Signal Strength)

### What is RSSI?

**RSSI** (Received Signal Strength Indicator) measures the **power level** of the received signal.

**Units:** dBm (decibel-milliwatts)

**Important:** RSSI is **always negative** because it represents power relative to 1 milliwatt.

### RSSI Scale

| RSSI (dBm) | Quality | Description | Typical Distance |
|------------|---------|-------------|------------------|
| -30 to -40 | Excellent | Maximum signal, device very close | 0-5 meters |
| -40 to -50 | Excellent | Strong signal, excellent connection | 5-10 meters |
| -50 to -60 | Good | Reliable connection, good speed | 10-30 meters |
| -60 to -70 | Fair | Usable, may experience slowdowns | 30-50 meters |
| -70 to -80 | Weak | Slow, unreliable, frequent disconnects | 50-100 meters |
| -80 to -90 | Very weak | Barely usable, severe issues | >100 meters |
| < -90 | Unusable | Cannot maintain connection | Out of range |

### Understanding dBm

**dBm** is a **logarithmic scale**:

- **-30 dBm** = 0.001 mW (very strong)
- **-60 dBm** = 0.000001 mW (1000x weaker than -30)
- **-90 dBm** = 0.000000001 mW (1000000x weaker than -30)

**Key insight:** Every **-3 dBm** = **50% power reduction**

### Factors Affecting RSSI

1. **Distance** - Further = weaker signal
2. **Obstacles** - Walls, furniture, metal
3. **Interference** - Other WiFi networks, microwave ovens, Bluetooth
4. **Antenna** - Quality and orientation
5. **Transmit power** - Router configuration

### Getting RSSI in ESP32

```c
const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
int rssi = pkt->rx_ctrl.rssi;

Serial.printf("Signal strength: %d dBm\n", rssi);
```

---

## 🔓 Promiscuous Mode

### What is Promiscuous Mode?

**Promiscuous mode** (also called **monitor mode**) allows a WiFi adapter to capture **all frames** in the air, not just those addressed to it.

### Normal Mode vs Promiscuous Mode

**Normal Mode (Station):**
```
Router ─────→ [Frame for Device A] ─────→ Device A ✓
                                     └───→ Device B ✗ (ignores)
                                     └───→ Device C ✗ (ignores)
```

**Promiscuous Mode:**
```
Router ─────→ [Frame for Device A] ─────→ Device A ✓
                                     └───→ Device B ✓ (captures)
                                     └───→ Device C ✓ (captures)
```

### Enabling in ESP32

```c
// 1. Initialize WiFi in Station mode
WiFi.mode(WIFI_STA);
delay(100);

// 2. Disconnect from any network
WiFi.disconnect();
delay(100);

// 3. Register callback function
esp_wifi_set_promiscuous_rx_cb(&my_callback);

// 4. Enable promiscuous mode
esp_wifi_set_promiscuous(true);
delay(100);

// 5. Set channel to listen
esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
```

### What Can You Capture?

**In promiscuous mode, you can capture:**
- ✅ Beacons (network advertisements)
- ✅ Probe requests (device searching for networks)
- ✅ Authentication frames
- ✅ Association frames
- ✅ Data frames (encrypted content)
- ✅ Control frames (ACK, etc.)

**What you CANNOT capture:**
- ❌ Encrypted data content (you see encrypted bytes, not plaintext)
- ❌ Passwords (unless network uses WEP, which is obsolete)
- ❌ HTTPS content (encrypted at application layer)

### Legal and Ethical Considerations

**Legal:**
- Capturing metadata (SSID, MAC, signal) from publicly broadcast beacons
- Analyzing your own networks
- Authorized security testing

**Illegal:**
- Decrypting traffic without authorization
- Attacking networks you don't own
- Intercepting private communications

**Think of it as:** Listening to radio station names and frequencies - not recording the actual shows.

---

## ⏱️ Timing and Timeouts

### millis() Function

**millis()** returns the number of **milliseconds** since the ESP32 started running.

```c
unsigned long start_time = millis();  // e.g., 5000 (5 seconds)
delay(2000);                           // Wait 2 seconds
unsigned long end_time = millis();    // e.g., 7000 (7 seconds)
unsigned long elapsed = end_time - start_time;  // 2000 ms
```

**Important:**
- Resets to 0 on power-on or reset
- Overflows after ~49.7 days (goes back to 0)
- Resolution: 1 millisecond
- Does NOT stop during `delay()`

### Implementing Timeouts

**Goal:** Remove networks that haven't been seen in 15 seconds.

```c
#define NETWORK_TIMEOUT 15000  // 15 seconds in milliseconds

struct Network {
  String mac;
  String ssid;
  int rssi;
  unsigned long last_seen;  // Timestamp
};

void addOrUpdateNetwork(String mac, String ssid, int rssi) {
  unsigned long now = millis();  // Current time
  
  // ... search for network ...
  
  networks[idx].last_seen = now;  // Update timestamp
}

void cleanupOldNetworks() {
  unsigned long now = millis();
  
  for (int i = 0; i < network_count; i++) {
    unsigned long elapsed = now - networks[i].last_seen;
    
    if (elapsed > NETWORK_TIMEOUT) {
      // Network is stale, remove it
    }
  }
}
```

### Refresh Intervals

**Goal:** Update display every 2 seconds.

```c
#define REFRESH_INTERVAL 2000  // 2 seconds

unsigned long last_refresh = 0;

void loop() {
  unsigned long now = millis();
  
  if (now - last_refresh >= REFRESH_INTERVAL) {
    displayTable();        // Update display
    last_refresh = now;    // Reset timer
  }
}
```

### Why These Values?

| Value | Reason |
|-------|--------|
| **15 seconds timeout** | Routers send beacons every 100ms. If we haven't seen one in 15s, the network is gone or out of range. |
| **2 seconds refresh** | Fast enough to feel "live", slow enough to be readable. |

---

## 🗂️ Data Structures in C

### Structs

A **struct** groups related data together (like a simple class in Python).

**Python equivalent:**

```python
class Network:
    def __init__(self, mac, ssid, rssi, last_seen):
        self.mac = mac
        self.ssid = ssid
        self.rssi = rssi
        self.last_seen = last_seen
```

**C equivalent:**

```c
struct Network {
  String mac;
  String ssid;
  int rssi;
  unsigned long last_seen;
};
```

**Creating instances:**

```c
// Single network
Network net1;
net1.mac = "aa:bb:cc:dd:ee:ff";
net1.ssid = "MyWiFi";
net1.rssi = -45;
net1.last_seen = millis();

// Array of networks
Network networks[50];
networks[0].mac = "aa:bb:cc:dd:ee:ff";
```

### Arrays in C vs Python

**Python (dynamic list):**

```python
networks = []                    # Empty list
networks.append(net1)            # Add element
networks.remove(net1)            # Remove element
print(len(networks))             # Get size
```

**C (fixed array):**

```c
#define MAX_NETWORKS 50
Network networks[MAX_NETWORKS];  // Fixed size: 50
int network_count = 0;           // Manual counter

// Add element (manual)
networks[network_count] = net1;
network_count++;

// Remove element (manual shift)
for (int j = i; j < network_count - 1; j++) {
  networks[j] = networks[j + 1];
}
network_count--;
```

**Key differences:**

| Feature | Python | C |
|---------|--------|---|
| Size | Dynamic | Fixed |
| Add element | `list.append()` | Manual index + counter |
| Remove element | `list.remove()` | Manual shift + counter |
| Resize | Automatic | Impossible (would need malloc) |
| Bounds checking | Automatic | **Manual (critical!)** |

### String Comparison

**Python:**

```python
if network.mac == "aa:bb:cc:dd:ee:ff":
    print("Found!")
```

**C (Arduino String):**

```c
if (networks[i].mac == "aa:bb:cc:dd:ee:ff") {
  Serial.println("Found!");
}
```

**C (char array):**

```c
if (strcmp(networks[i].mac, "aa:bb:cc:dd:ee:ff") == 0) {
  Serial.println("Found!");
}
```

---

## 🔄 Callback Functions

### What is a Callback?

A **callback** is a function that is called when an event occurs. In this project, it's called when a WiFi frame is received.

**Python equivalent:**

```python
def my_callback(packet):
    print(f"Received: {packet.ssid}")

wifi.on_packet_received = my_callback  # Register callback
```

**C equivalent:**

```c
void my_callback(void *buf, wifi_promiscuous_pkt_type_t type) {
  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  Serial.println("Packet received!");
}

void setup() {
  esp_wifi_set_promiscuous_rx_cb(&my_callback);  // Register
  esp_wifi_set_promiscuous(true);                // Enable
}
```

### How It Works

```
1. ESP32 receives WiFi frame
         ↓
2. ESP32 calls registered callback
         ↓
3. Your function processes the frame
         ↓
4. Function returns
         ↓
5. ESP32 continues listening
```

**Important:**
- Callbacks run in **interrupt context**
- Keep them **fast** (no long delays)
- Don't do heavy processing inside
- Don't use `Serial.println()` excessively (can cause crashes)

### Callback Parameters

```c
void callback(void *buf, wifi_promiscuous_pkt_type_t type)
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `buf` | `void *` | Pointer to packet data |
| `type` | `wifi_promiscuous_pkt_type_t` | Packet type (DATA, MGMT, CTRL) |

**Accessing packet data:**

```c
const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
const uint8_t *payload = pkt->payload;  // Frame bytes
int rssi = pkt->rx_ctrl.rssi;           // Signal strength
int channel = pkt->rx_ctrl.channel;     // WiFi channel
```

---

## 📊 Comparison: Python vs C (Arduino)

### Serial Output

| Python | C (Arduino) |
|--------|-------------|
| `print("Hello")` | `Serial.println("Hello");` |
| `print(f"Value: {x}")` | `Serial.printf("Value: %d\n", x);` |
| Input: `x = input()` | `String x = Serial.readStringUntil('\n');` |

### Variables

| Python | C (Arduino) |
|--------|-------------|
| `x = 10` | `int x = 10;` |
| `name = "WiFi"` | `String name = "WiFi";` |
| `pi = 3.14` | `float pi = 3.14;` |
| `active = True` | `bool active = true;` |

### Loops

| Python | C (Arduino) |
|--------|-------------|
| `for i in range(10):` | `for (int i = 0; i < 10; i++)` |
| `while active:` | `while (active) {` |
| `for item in list:` | *(No direct equivalent, use index)* |

### Functions

| Python | C (Arduino) |
|--------|-------------|
| `def add(a, b):` | `int add(int a, int b) {` |
| `    return a + b` | `    return a + b;` |
| | `}` |

### Strings

| Python | C (Arduino) |
|--------|-------------|
| `s = "Hello"` | `String s = "Hello";` |
| `s.lower()` | `s.toLowerCase();` |
| `s.strip()` | `s.trim();` |
| `len(s)` | `s.length();` |
| `s[0:5]` | `s.substring(0, 5);` |

---

## 🎓 Further Learning

### Recommended Topics

1. **WiFi Protocol Deep Dive**
   - 802.11 frame types and subtypes
   - Encryption (WPA2, WPA3)
   - Handshake process

2. **ESP32 Advanced Features**
   - Deep sleep modes
   - BLE (Bluetooth Low Energy)
   - Web server hosting

3. **Network Security**
   - Penetration testing
   - Vulnerability assessment
   - Responsible disclosure

4. **Tools to Explore**
   - **Wireshark** - Packet analyzer
   - **Aircrack-ng** - WiFi security suite
   - **Scapy (Python)** - Packet manipulation

### Resources

- **ESP32 Documentation:** https://docs.espressif.com/
- **IEEE 802.11 Standard:** https://standards.ieee.org/
- **Wireshark User Guide:** https://www.wireshark.org/docs/
- **WiFi Alliance:** https://www.wi-fi.org/

---

**End of Concepts Guide**

*Return to the [Tutorial README](README.md) to continue learning!*
