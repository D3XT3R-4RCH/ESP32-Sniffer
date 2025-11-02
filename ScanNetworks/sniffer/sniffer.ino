// ========================================
// LIBRERÍAS (como import en Python)
// ========================================
#include <WiFi.h>       // Para usar funciones WiFi del ESP32
#include "esp_wifi.h"   // Para funciones avanzadas de WiFi (modo promiscuo)

// ========================================
// VARIABLES GLOBALES
// ========================================
int canal = 0;           // Canal WiFi a escanear (1-13)
bool escaneando = false; // ¿Estamos escaneando? (como un bool en Python)
int paquetes_capturados = 0;  // Contador de paquetes (para estadísticas)

// ========================================
// FUNCIÓN CALLBACK (se ejecuta cada vez que se captura un paquete WiFi)
// Similar a una función que pasas como parámetro en Python
// ========================================
void capturar_paquete(void *buf, wifi_promiscuous_pkt_type_t tipo_paquete) {
  // IMPORTANTE: Esta función solo se ejecuta si escaneando == true
  // Cuando desactivamos el modo promiscuo, esta función deja de ser llamada
  
  // Solo nos interesan los paquetes de GESTIÓN (MGMT)
  // Los beacons (anuncios de redes WiFi) son paquetes de gestión
  if (tipo_paquete != WIFI_PKT_MGMT) return;
  
  // Convertir el buffer a una estructura de paquete WiFi
  wifi_promiscuous_pkt_t *paquete = (wifi_promiscuous_pkt_t *)buf;
  
  // Obtener la señal (RSSI = Received Signal Strength Indicator)
  // Similar a paquete.rx_ctrl.rssi en Python
  int rssi = paquete->rx_ctrl.rssi;
  
  // payload es el contenido del paquete (los bytes de datos)
  uint8_t *payload = paquete->payload;
  
  // Extraer el tipo y subtipo del frame WiFi
  // fc0 es el primer byte del frame control
  uint8_t fc0 = payload[0];
  uint8_t tipo = (fc0 >> 2) & 0x03;      // Bits 2-3
  uint8_t subtipo = (fc0 >> 4) & 0x0F;   // Bits 4-7
  
  // Verificar si es un BEACON (tipo=0, subtipo=8)
  // Los beacons son los paquetes que anuncian las redes WiFi
  if (tipo == 0 && subtipo == 8) {
    // Extraer la MAC del router (dirección física del dispositivo)
    // Los bytes 10-15 del payload contienen la MAC
    char mac[18];  // Array de caracteres (string en C)
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            payload[10], payload[11], payload[12],
            payload[13], payload[14], payload[15]);
    
    // Extraer el SSID (nombre de la red WiFi)
    // El byte 37 indica cuántos caracteres tiene el SSID
    int longitud_ssid = payload[37];
    String ssid = "";  // String vacío (como "" en Python)
    
    if (longitud_ssid > 0 && longitud_ssid < 33) {
      // Recorrer cada carácter del SSID
      for (int i = 0; i < longitud_ssid; i++) {
        ssid += (char)payload[38 + i];  // += es como append en Python
      }
    } else {
      ssid = "<oculto>";  // Red sin SSID visible
    }
    
    // Mostrar en el Serial Monitor (como print() en Python)
    Serial.printf("📶 Red: %s | MAC: %s | Señal: %d dBm\n", 
                  ssid.c_str(), mac, rssi);
    
    paquetes_capturados++;  // Incrementar contador (como += 1 en Python)
  }
}

// ========================================
// SETUP (se ejecuta UNA VEZ al inicio)
// Similar a if __name__ == "__main__": en Python
// ========================================
void setup() {
  // Iniciar comunicación serial a 115200 baudios
  Serial.begin(115200);
  delay(1000);  // Esperar 1 segundo (1000 milisegundos)
  
  Serial.println("\n╔══════════════════════════════════════╗");
  Serial.println("║   SNIFFER WiFi ESP32 - Modo Básico   ║");
  Serial.println("╚══════════════════════════════════════╝\n");
  
  // Mostrar instrucciones
  Serial.println("📋 Canales WiFi disponibles:");
  Serial.println("   • Canales 1-13: WiFi 2.4 GHz");
  Serial.println("   • Canales populares: 1, 6, 11");
  Serial.println("\n💡 Comandos disponibles:");
  Serial.println("   • Escribe un número (1-13) para escanear ese canal");
  Serial.println("   • Escribe 's' o 'stop' para DETENER el escaneo");
  Serial.println("   • Escribe 'r' o 'reset' para reiniciar");
  Serial.println();
}

// ========================================
// LOOP (se ejecuta EN BUCLE constantemente)
// Similar a while True: en Python
// ========================================
void loop() {
  // Si NO estamos escaneando, esperar a que el usuario ingrese un canal
  if (!escaneando) {
    // Serial.available() devuelve cuántos bytes hay disponibles para leer
    // Es como verificar si hay input() disponible en Python
    if (Serial.available() > 0) {
      // Leer el número ingresado por el usuario
      // Similar a int(input()) en Python
      canal = Serial.parseInt();
      
      // Validar que el canal esté entre 1 y 13
      if (canal >= 1 && canal <= 13) {
        Serial.printf("\n✅ Canal %d seleccionado\n", canal);
        Serial.println("🔧 Configurando ESP32...\n");
        
        // Inicializar WiFi en modo estación (STA)
        WiFi.mode(WIFI_STA);
        delay(100);
        
        // Desconectar de cualquier red
        WiFi.disconnect();
        delay(100);
        
        // ACTIVAR MODO PROMISCUO (permite capturar todos los paquetes)
        // Es como poner la tarjeta de red en "monitor mode" en Linux
        esp_wifi_set_promiscuous(true);
        delay(100);
        
        // Registrar la función callback que se ejecutará por cada paquete
        // Similar a pasar una función como parámetro en Python
        esp_wifi_set_promiscuous_rx_cb(&capturar_paquete);
        
        // Establecer el canal WiFi a escanear
        esp_wifi_set_channel(canal, WIFI_SECOND_CHAN_NONE);
        
        Serial.println("✅ Sniffer activado");
        Serial.printf("� Escuchando en canal %d...\n", canal);
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        
        escaneando = true;  // Marcar que ya estamos escaneando
      } else {
        Serial.println("❌ Canal inválido. Debe ser entre 1 y 13\n");
      }
    }
  }
  
  // Si estamos escaneando, solo esperar
  // La función capturar_paquete() se ejecuta automáticamente
  // cuando el ESP32 detecta un paquete WiFi
  delay(10);
}
