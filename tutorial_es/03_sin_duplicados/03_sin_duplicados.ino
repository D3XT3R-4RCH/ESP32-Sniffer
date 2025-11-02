/*
 * ============================================
 * NIVEL 3: LISTA SIN DUPLICADOS
 * ============================================
 * 
 * OBJETIVO: Mostrar cada red solo UNA VEZ
 * - Crear una estructura de datos
 * - Almacenar redes en un array
 * - Buscar duplicados antes de agregar
 * 
 * CONCEPTOS NUEVOS:
 * - struct (estructuras de datos)
 * - Arrays en C
 * - Funciones personalizadas
 * - Búsqueda en listas
 * 
 * EQUIVALENTE EN PYTHON:
 * class Red:
 *     def __init__(self, mac, ssid, rssi):
 *         self.mac = mac
 *         self.ssid = ssid
 *         self.rssi = rssi
 * 
 * redes = []  # Lista de redes
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
// Una estructura es como una "clase simple" en Python
// Agrupa varios datos relacionados
struct Red {
  String mac;    // MAC address (identificador único)
  String ssid;   // Nombre de la red
  int rssi;      // Señal
};

// ==========================================
// VARIABLES GLOBALES
// ==========================================
#define MAX_REDES 30    // Máximo de redes a almacenar

// Array de estructuras (como lista de objetos en Python)
Red redes[MAX_REDES];   // redes = [Red(), Red(), Red(), ...]

int total_redes = 0;    // Contador de redes almacenadas

// ==========================================
// FUNCIÓN: BUSCAR RED POR MAC
// ==========================================
// Busca si una MAC ya existe en la lista
// Retorna: índice si encuentra, -1 si no encuentra
//
// Equivalente Python:
// def buscar_red(mac):
//     for i, red in enumerate(redes):
//         if red.mac == mac:
//             return i
//     return -1
int buscarRedPorMAC(String mac) {
  // Recorrer todas las redes almacenadas
  for (int i = 0; i < total_redes; i++) {
    // ¿Esta red tiene la misma MAC?
    if (redes[i].mac == mac) {
      return i;  // Encontrada! Retornar el índice
    }
  }
  return -1;  // No encontrada
}

// ==========================================
// FUNCIÓN: AGREGAR O ACTUALIZAR RED
// ==========================================
// Si la red es nueva, la agrega
// Si ya existe, actualiza la señal
//
// Equivalente Python:
// def agregar_red(mac, ssid, rssi):
//     indice = buscar_red(mac)
//     if indice >= 0:
//         redes[indice].rssi = rssi
//     else:
//         redes.append(Red(mac, ssid, rssi))
void agregarOActualizarRed(String mac, String ssid, int rssi) {
  // Buscar si ya existe
  int indice = buscarRedPorMAC(mac);
  
  if (indice >= 0) {
    // ==========================================
    // RED EXISTENTE - SOLO ACTUALIZAR SEÑAL
    // ==========================================
    redes[indice].rssi = rssi;
    
    // Mostrar que se actualizó
    Serial.printf("🔄 Actualizada: %-20s (Señal: %d dBm)\n", 
                  ssid.c_str(), rssi);
  } else {
    // ==========================================
    // RED NUEVA - AGREGAR A LA LISTA
    // ==========================================
    // ¿Hay espacio en el array?
    if (total_redes < MAX_REDES) {
      // Agregar al final del array
      redes[total_redes].mac = mac;
      redes[total_redes].ssid = ssid;
      redes[total_redes].rssi = rssi;
      
      total_redes++;  // Incrementar contador
      
      // Mostrar que se agregó
      Serial.printf("✨ Nueva red [%d]: %-20s | MAC: %s | Señal: %d dBm\n",
                    total_redes,
                    ssid.c_str(),
                    mac.c_str(),
                    rssi);
    } else {
      // Array lleno!
      Serial.println("⚠️  Lista llena! No se pueden agregar más redes");
    }
  }
}

// ==========================================
// FUNCIÓN: MOSTRAR RESUMEN
// ==========================================
void mostrarResumen() {
  Serial.println("\n╔════════════════════════════════════════════════════════╗");
  Serial.printf("║  RESUMEN: %d redes únicas encontradas", total_redes);
  
  // Calcular espacios para alinear
  int espacios = 54 - 34 - String(total_redes).length();
  for (int i = 0; i < espacios; i++) Serial.print(" ");
  Serial.println("║");
  
  Serial.println("╠════════════════════════════════════════════════════════╣");
  
  if (total_redes == 0) {
    Serial.println("║  No se han encontrado redes aún                       ║");
  } else {
    for (int i = 0; i < total_redes; i++) {
      Serial.printf("║ %2d. %-25s | %4d dBm ║\n",
                    i + 1,
                    redes[i].ssid.c_str(),
                    redes[i].rssi);
    }
  }
  
  Serial.println("╚════════════════════════════════════════════════════════╝\n");
}

// ==========================================
// CALLBACK: CAPTURA DE PAQUETES
// ==========================================
void capturar_paquete(void *buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;
  
  wifi_promiscuous_pkt_t *paquete = (wifi_promiscuous_pkt_t *)buf;
  uint8_t *payload = paquete->payload;
  
  uint8_t frame_ctrl = payload[0];
  uint8_t tipo = (frame_ctrl >> 2) & 0x03;
  uint8_t subtipo = (frame_ctrl >> 4) & 0x0F;
  
  // ¿Es un beacon?
  if (tipo == 0 && subtipo == 8) {
    // Extraer MAC
    char mac[18];
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    // Extraer SSID
    int len_ssid = payload[37];
    String ssid = "";
    
    if (len_ssid > 0 && len_ssid < 33) {
      for (int i = 0; i < len_ssid; i++) {
        ssid += (char)payload[38 + i];
      }
    } else {
      ssid = "<oculto>";
    }
    
    // Señal
    int rssi = paquete->rx_ctrl.rssi;
    
    // ==========================================
    // ✨ NOVEDAD: AGREGAR O ACTUALIZAR
    // ==========================================
    agregarOActualizarRed(String(mac), ssid, rssi);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║  NIVEL 3: Lista sin duplicados    ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // Inicializar WiFi
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  
  esp_wifi_set_promiscuous(true);
  delay(100);
  esp_wifi_set_promiscuous_rx_cb(&capturar_paquete);
  
  int canal = 6;
  esp_wifi_set_channel(canal, WIFI_SECOND_CHAN_NONE);
  
  Serial.println("✅ Sniffer activado");
  Serial.printf("📡 Escuchando canal %d...\n", canal);
  Serial.println("📋 Solo se muestran redes NUEVAS o ACTUALIZADAS\n");
}

unsigned long ultimo_resumen = 0;

void loop() {
  handleSerialCommands();
  if (!is_running) {
    delay(500);
    return;
  }

  // Mostrar resumen cada 10 segundos
  unsigned long ahora = millis();

  if (ahora - ultimo_resumen >= 10000) {
    mostrarResumen();
    ultimo_resumen = ahora;
  }

  delay(100);
}

/*
 * ============================================
 * EXPLICACIÓN DETALLADA
 * ============================================
 * 
 * ¿QUÉ ES UNA ESTRUCTURA (struct)?
 * - Agrupa variables relacionadas
 * - Similar a una clase en Python (pero más simple)
 * - Ejemplo:
 *   struct Persona {
 *     String nombre;
 *     int edad;
 *   };
 *   Persona juan;
 *   juan.nombre = "Juan";
 *   juan.edad = 25;
 * 
 * ¿QUÉ ES UN ARRAY?
 * - Lista de elementos del mismo tipo
 * - Tamaño FIJO en C (no crece como en Python)
 * - Se accede por índice: redes[0], redes[1], ...
 * 
 * DIFERENCIAS CON PYTHON:
 * 
 * Python (lista dinámica):
 * redes = []
 * redes.append(nueva_red)  # Crece automáticamente
 * 
 * C (array fijo):
 * Red redes[30];  # Máximo 30 elementos
 * redes[0] = nueva_red;  # Asignar manualmente
 * 
 * ¿POR QUÉ USAR LA MAC COMO IDENTIFICADOR?
 * - La MAC es ÚNICA para cada dispositivo
 * - Dos redes pueden tener el mismo SSID
 * - Pero nunca tendrán la misma MAC
 * 
 * ALGORITMO DE BÚSQUEDA:
 * 1. Recibir beacon con MAC "aa:bb:cc:dd:ee:ff"
 * 2. Buscar en la lista si existe esa MAC
 * 3. Si existe → actualizar señal
 * 4. Si no existe → agregar a la lista
 * 
 * PRÓXIMO NIVEL:
 * Aprenderás a crear una tabla actualizable
 * que se refresca automáticamente en pantalla.
 */
