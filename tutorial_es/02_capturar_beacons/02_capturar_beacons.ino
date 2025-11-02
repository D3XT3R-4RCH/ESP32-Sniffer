/*
 * ============================================
 * NIVEL 2: CAPTURAR BEACONS WiFi
 * ============================================
 * 
 * OBJETIVO: Capturar y decodificar paquetes beacon
 * - Registrar una función callback
 * - Decodificar paquetes WiFi
 * - Extraer SSID y MAC de los beacons
 * 
 * CONCEPTOS NUEVOS:
 * - Función callback (función que se ejecuta automáticamente)
 * - Punteros en C (void *buf)
 * - Estructuras (wifi_promiscuous_pkt_t)
 * - Bytes y hexadecimal
 * 
 * EQUIVALENTE EN PYTHON:
 * def on_packet(packet):
 *     print(packet.ssid, packet.mac)
 * wifi.on_packet_received(on_packet)
 */

#include <WiFi.h>
#include "esp_wifi.h"

// Control remoto por Serial: 's' = stop, 'r' = reset
bool is_running = true;
void handleSerialCommands() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's' || c == 'S') {
      is_running = false;
      Serial.println("⏸️  STOP: desactivando captura...");
      esp_wifi_set_promiscuous_rx_cb(NULL);
      esp_wifi_set_promiscuous(false);
      Serial.println("✅ Captura detenida");
    } else if (c == 'r' || c == 'R') {
      Serial.println("🔁 Reiniciando (ESP.restart())...");
      delay(100);
      ESP.restart();
    }
  }
}

int paquetes_capturados = 0;  // Contador global

// ==========================================
// FUNCIÓN CALLBACK - SE EJECUTA POR CADA PAQUETE
// ==========================================
// Esta función se llama AUTOMÁTICAMENTE cada vez que
// el ESP32 captura un paquete WiFi en el aire
//
// Parámetros:
// - buf: Buffer con los datos del paquete (puntero a memoria)
// - type: Tipo de paquete (Management, Control, Data)
void mi_callback(void *buf, wifi_promiscuous_pkt_type_t type) {
  
  // ==========================================
  // 1. FILTRAR SOLO PAQUETES DE GESTIÓN
  // ==========================================
  // Los beacons son paquetes de tipo "Management" (MGMT)
  if (type != WIFI_PKT_MGMT) {
    return;  // Si no es MGMT, salir de la función
  }
  
  // ==========================================
  // 2. CONVERTIR EL BUFFER A ESTRUCTURA
  // ==========================================
  // Convertir el puntero genérico (void*) a una estructura específica
  // Es como hacer "casting" en Python: paquete = (WiFiPacket)buf
  wifi_promiscuous_pkt_t *paquete = (wifi_promiscuous_pkt_t *)buf;
  
  // ==========================================
  // 3. OBTENER EL PAYLOAD (CONTENIDO)
  // ==========================================
  // payload = los bytes de datos del paquete
  // Es un array de bytes (uint8_t = unsigned integer de 8 bits = 0-255)
  uint8_t *payload = paquete->payload;
  
  // ==========================================
  // 4. DECODIFICAR EL FRAME CONTROL
  // ==========================================
  // El primer byte (payload[0]) contiene info del tipo de frame
  uint8_t frame_control = payload[0];
  
  // Extraer bits específicos:
  // tipo = bits 2-3 (desplazar 2 y aplicar máscara 0x03)
  // subtipo = bits 4-7 (desplazar 4 y aplicar máscara 0x0F)
  uint8_t tipo = (frame_control >> 2) & 0x03;
  uint8_t subtipo = (frame_control >> 4) & 0x0F;
  
  // ==========================================
  // 5. VERIFICAR SI ES UN BEACON
  // ==========================================
  // Beacon = tipo 0 (Management) + subtipo 8
  if (tipo == 0 && subtipo == 8) {
    
    // ==========================================
    // 6. EXTRAER LA MAC ADDRESS
    // ==========================================
    // La MAC del router está en los bytes 10-15 del payload
    // Formato: aa:bb:cc:dd:ee:ff (6 bytes)
    char mac_address[18];  // Array para almacenar la MAC como string
    
    // sprintf = "string printf" - formatear texto
    // %02x = hexadecimal de 2 dígitos con ceros a la izquierda
    sprintf(mac_address, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    // ==========================================
    // 7. EXTRAER EL SSID (NOMBRE DE LA RED)
    // ==========================================
    // El byte 37 indica cuántos caracteres tiene el SSID
    int longitud_ssid = payload[37];
    
    // String = tipo de dato para texto (como str en Python)
    String ssid = "";
    
    // ¿El SSID es válido? (entre 1 y 32 caracteres)
    if (longitud_ssid > 0 && longitud_ssid < 33) {
      // Recorrer cada carácter del SSID
      for (int i = 0; i < longitud_ssid; i++) {
        // Los caracteres del SSID empiezan en el byte 38
        ssid += (char)payload[38 + i];
      }
    } else {
      // SSID oculto o inválido
      ssid = "<oculto>";
    }
    
    // ==========================================
    // 8. OBTENER LA SEÑAL (RSSI)
    // ==========================================
    // RSSI = Received Signal Strength Indicator
    // Valores típicos: -30 (excelente) a -90 (muy débil)
    int señal = paquete->rx_ctrl.rssi;
    
    // ==========================================
    // 9. MOSTRAR LA INFORMACIÓN
    // ==========================================
    paquetes_capturados++;
    
    Serial.printf("📶 [%d] SSID: %-20s | MAC: %s | Señal: %d dBm\n",
                  paquetes_capturados,
                  ssid.c_str(),
                  mac_address,
                  señal);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   NIVEL 2: Capturar Beacons       ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // Inicializar WiFi (igual que en Nivel 1)
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  
  // Activar modo promiscuo
  esp_wifi_set_promiscuous(true);
  delay(100);
  
  // ==========================================
  // ✨ NOVEDAD: REGISTRAR LA FUNCIÓN CALLBACK
  // ==========================================
  // Le decimos al ESP32: "Cada vez que captures un paquete,
  // ejecuta la función mi_callback()"
  //
  // & = operador de dirección (pasar la dirección de memoria de la función)
  esp_wifi_set_promiscuous_rx_cb(&mi_callback);
  
  // Configurar canal
  int canal = 6;
  esp_wifi_set_channel(canal, WIFI_SECOND_CHAN_NONE);
  
  Serial.println("✅ Sniffer activado");
  Serial.printf("📡 Escuchando beacons en canal %d...\n\n", canal);
}

void loop() {
  handleSerialCommands();
  if (!is_running) {
    delay(500);
    return;
  }

  // ¡No necesitamos hacer nada aquí!
  // La función mi_callback() se ejecuta AUTOMÁTICAMENTE
  // cada vez que llega un paquete
  delay(100);
}

/*
 * ============================================
 * EXPLICACIÓN DETALLADA
 * ============================================
 * 
 * ¿QUÉ ES UN BEACON?
 * - Paquete que los routers transmiten constantemente
 * - Anuncian: "¡Hola! Soy la red X, estoy aquí"
 * - Se transmiten ~10 veces por segundo
 * - Contienen: SSID, MAC, canal, seguridad, etc.
 * 
 * ¿QUÉ ES UNA FUNCIÓN CALLBACK?
 * - Función que TÚ defines, pero OTRO código la llama
 * - En Python sería como:
 *   def mi_funcion():
 *       print("Hola")
 *   
 *   wifi.cuando_recibas_paquete(mi_funcion)
 * 
 * ¿QUÉ SON LOS PUNTEROS?
 * - void *buf = puntero a memoria (dirección)
 * - Es como una "flecha" que apunta a datos
 * - En Python no existen (Python los oculta)
 * 
 * ESTRUCTURA DE UN BEACON:
 * 
 * Byte:   0    1    2  ...  10-15   ...  37   38-...
 *         │    │    │        │             │     │
 *      Frame  ...  ...    MAC Addr      Len  SSID
 *      Control                          SSID
 * 
 * OPERACIONES BIT A BIT:
 * - >> = desplazamiento a la derecha
 * - & = AND bit a bit (máscara)
 * 
 * Ejemplo: extraer bits 2-3
 * 11010110 >> 2 = 00110101
 * 00110101 & 0x03 = 00000001
 * 
 * PRÓXIMO NIVEL:
 * Aprenderás a evitar duplicados y crear una lista
 * de redes únicas.
 */
