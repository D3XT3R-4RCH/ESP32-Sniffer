#include <WiFi.h>
#include "esp_wifi.h"

#define MAX_NETWORKS 50   // cantidad máxima de redes a guardar
#define CHANNEL 6         // canal que quieres monitorear

// estructura para guardar información de cada red
struct NetworkInfo {
  String mac;
  String ssid;
  int rssi;
};

NetworkInfo networks[MAX_NETWORKS];
int networkCount = 0;

// función para buscar si ya tenemos una red guardada
bool isAlreadyListed(String mac) {
  for (int i = 0; i < networkCount; i++) {
    if (networks[i].mac == mac) return true;
  }
  return false;
}

// callback del modo promiscuo
void wifi_sniffer_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return; // solo queremos management frames

  wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
  int len = ppkt->rx_ctrl.sig_len;
  int rssi = ppkt->rx_ctrl.rssi;
  uint8_t *payload = ppkt->payload;

  // verificar longitud mínima
  if (len < 36) return;

  uint8_t fc0 = payload[0];
  uint8_t subtype = (fc0 >> 4) & 0x0F;
  uint8_t frame_type = (fc0 >> 2) & 0x03;

  // solo capturamos tramas beacon (type=0, subtype=8)
  if (frame_type == 0 && subtype == 8) {
    // extraer MAC del AP (bytes 10 a 15 del header)
    char macStr[18];
    sprintf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);

    String mac = String(macStr);
    if (isAlreadyListed(mac)) return; // evitar duplicados

    // extraer SSID (en los Beacons, empieza en byte 38)
    int ssid_len = payload[37];
    String ssid = "";
    if (ssid_len > 0 && ssid_len < 33) {
      for (int ch = 1; ch <= 13; ch++) {
         esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
         delay(2000);
      }
    } else {
      ssid = "<oculto>";
    }

    // guardar red en lista
    if (networkCount < MAX_NETWORKS) {
      networks[networkCount].mac = mac;
      networks[networkCount].ssid = ssid;
      networks[networkCount].rssi = rssi;
      networkCount++;
    }

    // imprimir por serial
    Serial.printf("📶 Red detectada: SSID: %s | MAC: %s | RSSI: %d dBm\n",
                  ssid.c_str(), mac.c_str(), rssi);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  WiFi.mode(WIFI_MODE_NULL);
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_cb);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);

  Serial.println("🔍 Escaneando redes Wi-Fi (solo canal 6)...");
}

void loop() {
  delay(2000);
}
for (int ch = 1; ch <= 13; ch++) {
  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
  delay(2000);
}