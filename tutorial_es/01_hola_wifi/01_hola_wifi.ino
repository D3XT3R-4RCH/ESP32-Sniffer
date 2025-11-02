/*
 * ============================================
 * NIVEL 1: HOLA MUNDO WiFi
 * ============================================
 * 
 * OBJETIVO: Entender los conceptos básicos
 * - Inicializar comunicación serial
 * - Inicializar WiFi del ESP32
 * - Mostrar información del chip
 * 
 * CONCEPTOS NUEVOS:
 * - Serial.begin() - Iniciar comunicación
 * - WiFi.mode() - Configurar modo WiFi
 * - esp_wifi_set_promiscuous() - Modo monitor
 * 
 * EQUIVALENTE EN PYTHON:
 * import serial
 * import wifi
 * wifi.set_monitor_mode(True)
 */

#include <WiFi.h>       // Librería WiFi del ESP32
#include "esp_wifi.h"   // Funciones avanzadas de WiFi

// Control remoto por Serial: 's' = stop, 'r' = reset
bool is_running = true;
void handleSerialCommands() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's' || c == 'S') {
      is_running = false;
      Serial.println("⏸️  STOP: desactivando captura...");
      esp_wifi_set_promiscuous(false);
      Serial.println("✅ Captura detenida");
    } else if (c == 'r' || c == 'R') {
      Serial.println("🔁 Reiniciando (ESP.restart())...");
      delay(100);
      ESP.restart();
    }
  }
}

void setup() {
  // ==========================================
  // 1. INICIALIZAR SERIAL (como print en Python)
  // ==========================================
  Serial.begin(115200);  // Velocidad: 115200 baudios
  delay(1000);           // Esperar 1 segundo
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║    NIVEL 1: Hola Mundo WiFi       ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // ==========================================
  // 2. MOSTRAR INFORMACIÓN DEL CHIP
  // ==========================================
  Serial.println("📌 Información del ESP32:");
  Serial.printf("   • Chip: %s\n", ESP.getChipModel());
  Serial.printf("   • Frecuencia CPU: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("   • Memoria libre: %d bytes\n", ESP.getFreeHeap());
  
  // Obtener la MAC del ESP32 (dirección física única)
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);
  Serial.printf("   • MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  // ==========================================
  // 3. INICIALIZAR WiFi EN MODO ESTACIÓN
  // ==========================================
  Serial.println("🔧 Inicializando WiFi...");
  
  // Configurar WiFi en modo STA (Station)
  // Es necesario inicializar WiFi antes del modo promiscuo
  WiFi.mode(WIFI_STA);
  delay(100);
  
  // Desconectar de cualquier red
  WiFi.disconnect();
  delay(100);
  
  Serial.println("✅ WiFi inicializado en modo STA\n");
  
  // ==========================================
  // 4. ACTIVAR MODO PROMISCUO (Monitor Mode)
  // ==========================================
  Serial.println("🔓 Activando modo promiscuo...");
  
  // Modo promiscuo = capturar TODOS los paquetes WiFi
  // No solo los dirigidos a nosotros
  esp_wifi_set_promiscuous(true);
  delay(100);
  
  Serial.println("✅ Modo promiscuo activado");
  Serial.println("   (Ahora podemos capturar paquetes WiFi)\n");
  
  // ==========================================
  // 5. CONFIGURAR CANAL WiFi
  // ==========================================
  int canal = 6;  // Canal 6 (uno de los más usados)
  
  Serial.printf("📡 Configurando canal %d...\n", canal);
  
  // Establecer el canal WiFi a escuchar
  esp_wifi_set_channel(canal, WIFI_SECOND_CHAN_NONE);
  
  Serial.printf("✅ Escuchando en canal %d\n\n", canal);
  
  // ==========================================
  // RESUMEN
  // ==========================================
  Serial.println("╔════════════════════════════════════╗");
  Serial.println("║        ✨ TODO LISTO ✨           ║");
  Serial.println("╠════════════════════════════════════╣");
  Serial.println("║ WiFi:    Inicializado ✓           ║");
  Serial.println("║ Modo:    Promiscuo ✓               ║");
  Serial.println("║ Canal:   6 ✓                       ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  Serial.println("💡 Siguiente paso: Nivel 2 - Capturar beacons");
}

void loop() {
  handleSerialCommands();
  if (!is_running) {
    delay(500);
    return;
  }

  // Por ahora, no hacemos nada en el loop
  // Solo dejamos el WiFi en modo promiscuo
  delay(1000);
  Serial.println("⏱️  WiFi escuchando... (nada se captura aún)");
}

/*
 * ============================================
 * EXPLICACIÓN PARA PRINCIPIANTES
 * ============================================
 * 
 * ¿Qué es el MODO PROMISCUO?
 * - Normalmente, tu WiFi solo "escucha" paquetes dirigidos a ti
 * - En modo promiscuo, escuchas TODOS los paquetes en el aire
 * - Es como poner tu oído WiFi en "modo espía"
 * 
 * ¿Qué son los CANALES WiFi?
 * - WiFi 2.4 GHz tiene 13 canales (1-13)
 * - Cada router transmite en UN canal específico
 * - Debes sintonizar el canal correcto para capturar paquetes
 * - Canales populares: 1, 6, 11 (menos interferencia)
 * 
 * COMANDOS IMPORTANTES:
 * - Serial.begin(115200)           → Iniciar serial
 * - WiFi.mode(WIFI_STA)            → Modo estación
 * - esp_wifi_set_promiscuous(true) → Activar modo monitor
 * - esp_wifi_set_channel(6, ...)   → Sintonizar canal 6
 * 
 * PRÓXIMO NIVEL:
 * Aprenderás a capturar y decodificar los paquetes beacon
 * que los routers transmiten constantemente.
 */
