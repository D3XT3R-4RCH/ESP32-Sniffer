/*
 * ============================================
 * NIVEL 4: TABLA ACTUALIZABLE
 * ============================================
 * 
 * OBJETIVO: Mostrar tabla que se refresca automáticamente
 * - Timestamp (marca de tiempo)
 * - Eliminar redes antiguas
 * - Actualización periódica
 * - Formato de tabla profesional
 * 
 * CONCEPTOS NUEVOS:
 * - millis() - Tiempo transcurrido
 * - Timeout (tiempo de espera)
 * - Formateo de strings avanzado
 * 
 * EQUIVALENTE EN PYTHON:
 * import time
 * ultimo_refresco = time.time()
 * if time.time() - ultimo_refresco > 2:
 *     actualizar_pantalla()
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

// ==========================================
// ESTRUCTURA DE DATOS
// ==========================================
struct Red {
  String mac;
  String ssid;
  int rssi;
  unsigned long ultima_vez;  // ✨ NUEVO: Timestamp
};

// ==========================================
// CONFIGURACIÓN
// ==========================================
#define MAX_REDES 30
#define INTERVALO_REFRESCO 2000   // Actualizar cada 2 segundos
#define TIMEOUT_RED 15000         // Eliminar red si no se ve en 15 segundos

Red redes[MAX_REDES];
int total_redes = 0;
int paquetes_totales = 0;

unsigned long ultimo_refresco = 0;

// ==========================================
// FUNCIÓN: BUSCAR RED
// ==========================================
int buscarRedPorMAC(String mac) {
  for (int i = 0; i < total_redes; i++) {
    if (redes[i].mac == mac) {
      return i;
    }
  }
  return -1;
}

// ==========================================
// FUNCIÓN: LIMPIAR REDES ANTIGUAS
// ==========================================
// Elimina redes que llevan mucho tiempo sin verse
//
// Equivalente Python:
// def limpiar_redes_antiguas():
//     ahora = time.time()
//     redes[:] = [r for r in redes 
//                 if ahora - r.ultima_vez <= TIMEOUT]
void limpiarRedesAntiguas() {
  unsigned long ahora = millis();
  
  // Recorrer de atrás hacia adelante
  // (para poder eliminar sin problemas)
  for (int i = total_redes - 1; i >= 0; i--) {
    // ¿Cuánto tiempo sin ver esta red?
    unsigned long tiempo_sin_ver = ahora - redes[i].ultima_vez;
    
    if (tiempo_sin_ver > TIMEOUT_RED) {
      // Esta red ya no está en rango, eliminarla
      
      // Desplazar todas las redes siguientes
      for (int j = i; j < total_redes - 1; j++) {
        redes[j] = redes[j + 1];
      }
      
      total_redes--;
    }
  }
}

// ==========================================
// FUNCIÓN: MOSTRAR TABLA
// ==========================================
void mostrarTabla() {
  // Líneas en blanco para separar
  for (int i = 0; i < 3; i++) Serial.println();
  
  Serial.println("╔══════════════════════════════════════════════════════════════════════╗");
  Serial.printf("║  SNIFFER WiFi - Canal 6 | Redes: %d | Beacons: %d", 
                total_redes, paquetes_totales);
  
  // Calcular espacios para alinear
  int len = 35 + String(total_redes).length() + String(paquetes_totales).length();
  for (int i = 0; i < 70 - len; i++) Serial.print(" ");
  Serial.println("║");
  
  Serial.println("╠══════════════════════════════════════════════════════════════════════╣");
  Serial.println("║  #  │ SSID (Red WiFi)          │ MAC Address       │ Señal (dBm)  ║");
  Serial.println("╠══════════════════════════════════════════════════════════════════════╣");
  
  if (total_redes == 0) {
    Serial.println("║                       ⏳ Esperando redes...                          ║");
  } else {
    for (int i = 0; i < total_redes; i++) {
      // Truncar SSID si es muy largo
      String ssid_display = redes[i].ssid;
      if (ssid_display.length() > 24) {
        ssid_display = ssid_display.substring(0, 21) + "...";
      }
      
      Serial.printf("║ %2d  │ %-24s │ %s │ %4d dBm     ║\n",
                    i + 1,
                    ssid_display.c_str(),
                    redes[i].mac.c_str(),
                    redes[i].rssi);
    }
  }
  
  Serial.println("╚══════════════════════════════════════════════════════════════════════╝");
  
  // Mostrar tiempo de actualización
  Serial.printf("⏱️  Última actualización: %lu segundos desde el inicio\n", 
                millis() / 1000);
  Serial.println();
}

// ==========================================
// FUNCIÓN: AGREGAR O ACTUALIZAR RED
// ==========================================
void agregarOActualizarRed(String mac, String ssid, int rssi) {
  int indice = buscarRedPorMAC(mac);
  unsigned long ahora = millis();
  
  if (indice >= 0) {
    // Red existente - actualizar
    redes[indice].rssi = rssi;
    redes[indice].ultima_vez = ahora;  // ✨ Actualizar timestamp
  } else {
    // Red nueva - agregar
    if (total_redes < MAX_REDES) {
      redes[total_redes].mac = mac;
      redes[total_redes].ssid = ssid;
      redes[total_redes].rssi = rssi;
      redes[total_redes].ultima_vez = ahora;  // ✨ Guardar timestamp
      total_redes++;
    }
  }
}

// ==========================================
// CALLBACK: CAPTURA DE PAQUETES
// ==========================================
void capturar_paquete(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;
  
  wifi_promiscuous_pkt_t *paquete = (wifi_promiscuous_pkt_t *)buf;
  uint8_t *payload = paquete->payload;
  
  uint8_t fc = payload[0];
  uint8_t tipo = (fc >> 2) & 0x03;
  uint8_t subtipo = (fc >> 4) & 0x0F;
  
  if (tipo == 0 && subtipo == 8) {
    char mac[18];
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    int len_ssid = payload[37];
    String ssid = "";
    
    if (len_ssid > 0 && len_ssid < 33) {
      for (int i = 0; i < len_ssid; i++) {
        ssid += (char)payload[38 + i];
      }
    } else {
      ssid = "<oculto>";
    }
    
    int rssi = paquete->rx_ctrl.rssi;
    
    agregarOActualizarRed(String(mac), ssid, rssi);
    paquetes_totales++;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   NIVEL 4: Tabla Actualizable     ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  
  esp_wifi_set_promiscuous(true);
  delay(100);
  esp_wifi_set_promiscuous_rx_cb(&capturar_paquete);
  
  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);
  
  Serial.println("✅ Sniffer activado");
  Serial.println("📡 La tabla se actualiza cada 2 segundos");
  Serial.println("🗑️  Las redes se eliminan si no se ven en 15 segundos\n");
  
  ultimo_refresco = millis();
}

void loop() {
  handleSerialCommands();
  if (!is_running) {
    delay(500);
    return;
  }
  unsigned long ahora = millis();
  
  // ==========================================
  // ✨ ACTUALIZACIÓN AUTOMÁTICA
  // ==========================================
  // ¿Ya pasaron 2 segundos desde la última actualización?
  if (ahora - ultimo_refresco >= INTERVALO_REFRESCO) {
    limpiarRedesAntiguas();  // Eliminar redes antiguas
    mostrarTabla();          // Mostrar tabla actualizada
    ultimo_refresco = ahora; // Actualizar timestamp
  }
  
  delay(10);
}

/*
 * ============================================
 * EXPLICACIÓN DETALLADA
 * ============================================
 * 
 * ¿QUÉ ES millis()?
 * - Retorna el tiempo en milisegundos desde que se encendió el ESP32
 * - Ejemplo: si han pasado 5 segundos, millis() = 5000
 * - NUNCA usar delay() largo en programas que hacen varias cosas
 * 
 * Equivalente Python:
 * import time
 * inicio = time.time()
 * ahora = time.time()
 * tiempo_transcurrido = ahora - inicio
 * 
 * ¿CÓMO FUNCIONA EL TIMEOUT?
 * 1. Cada vez que vemos una red, guardamos el tiempo: millis()
 * 2. Cada 2 segundos, revisamos todas las redes
 * 3. Si una red no se ha visto en 15 segundos, la eliminamos
 * 
 * EJEMPLO DE TIMEOUT:
 * - Red detectada en t=0 segundos (ultima_vez=0)
 * - Ahora son t=20 segundos (ahora=20000)
 * - tiempo_sin_ver = 20000 - 0 = 20000 ms = 20 segundos
 * - 20 segundos > 15 segundos → ELIMINAR
 * 
 * ¿POR QUÉ RECORRER DE ATRÁS HACIA ADELANTE?
 * Cuando eliminas un elemento del medio de un array,
 * los índices cambian. Recorrer de atrás evita problemas.
 * 
 * Ejemplo:
 * Array: [A, B, C, D]
 * Eliminar B (índice 1):
 * - Si recorres de adelante: eliminas B, ahora C está en índice 1
 * - Saltas C sin querer!
 * - Si recorres de atrás: eliminas sin problemas
 * 
 * FORMATEO DE STRINGS:
 * - %-24s = string alineado a la izquierda, 24 caracteres
 * - %4d = número con 4 dígitos (rellena con espacios)
 * - %02x = hexadecimal con 2 dígitos (rellena con ceros)
 * 
 * PRÓXIMO NIVEL:
 * Agregar comandos interactivos para controlar el sniffer
 * desde el Serial Monitor (start, stop, cambiar canal, etc.)
 */
