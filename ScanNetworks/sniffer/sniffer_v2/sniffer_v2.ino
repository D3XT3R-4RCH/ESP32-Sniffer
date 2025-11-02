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
  // IMPORTANTE: Esta función solo se ejecuta cuando el modo promiscuo está activado
  // Al llamar esp_wifi_set_promiscuous(false), esta función DEJA DE SER LLAMADA
  
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
  // Verificar si hay datos disponibles en el Serial Monitor
  // Serial.available() devuelve cuántos bytes hay para leer
  if (Serial.available() > 0) {
    // Leer el comando como String (hasta encontrar Enter)
    // Similar a input().strip() en Python
    String comando = Serial.readStringUntil('\n');
    comando.trim();  // Eliminar espacios en blanco (como .strip() en Python)
    comando.toLowerCase();  // Convertir a minúsculas (como .lower() en Python)
    
    // ==========================================
    // COMANDO: DETENER (stop o s)
    // ==========================================
    if (comando == "s" || comando == "stop") {
      if (escaneando) {
        // DESACTIVAR el modo promiscuo (detiene la captura)
        // Esto hace que capturar_paquete() deje de ser llamada
        esp_wifi_set_promiscuous(false);
        
        Serial.println("\n⏸️  ESCANEO DETENIDO");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.printf("📊 Estadísticas:\n");
        Serial.printf("   • Canal escaneado: %d\n", canal);
        Serial.printf("   • Beacons capturados: %d\n", paquetes_capturados);
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        Serial.println("\n💡 Escribe 'r' para reiniciar o un nuevo canal\n");
        
        escaneando = false;  // Cambiar el estado a "no escaneando"
      } else {
        Serial.println("⚠️  El escaneo ya está detenido\n");
      }
    }
    
    // ==========================================
    // COMANDO: REINICIAR (reset o r)
    // ==========================================
    else if (comando == "r" || comando == "reset") {
      // Desactivar el modo promiscuo si estaba activo
      if (escaneando) {
        esp_wifi_set_promiscuous(false);
      }
      
      Serial.println("\n🔄 Reiniciando...\n");
      
      // Resetear variables (como en Python: variable = 0)
      escaneando = false;
      paquetes_capturados = 0;
      canal = 0;
      
      Serial.println("✅ Listo para escanear");
      Serial.println("💡 Escribe un canal (1-13) para comenzar\n");
    }
    
    // ==========================================
    // COMANDO: NÚMERO DE CANAL (1-13)
    // ==========================================
    else {
      // Intentar convertir el comando a número
      int nuevo_canal = comando.toInt();  // Similar a int(comando) en Python
      
      if (nuevo_canal >= 1 && nuevo_canal <= 13) {
        // Si ya estábamos escaneando, detener primero
        if (escaneando) {
          esp_wifi_set_promiscuous(false);
          Serial.printf("\n⏹️  Deteniendo canal %d...\n", canal);
          delay(100);
        }
        
        // Guardar el nuevo canal
        canal = nuevo_canal;
        paquetes_capturados = 0;  // Resetear contador
        
        Serial.printf("\n✅ Canal %d seleccionado\n", canal);
        Serial.println("🔧 Configurando ESP32...\n");
        
        // Inicializar WiFi en modo estación (STA)
        WiFi.mode(WIFI_STA);
        delay(100);
        
        // Desconectar de cualquier red
        WiFi.disconnect();
        delay(100);
        
        // ACTIVAR MODO PROMISCUO (permite capturar todos los paquetes)
        // Esto permite que capturar_paquete() sea llamada automáticamente
        esp_wifi_set_promiscuous(true);
        delay(100);
        
        // Registrar la función callback
        // Es como decir: "Cada vez que llegue un paquete, ejecuta capturar_paquete()"
        esp_wifi_set_promiscuous_rx_cb(&capturar_paquete);
        
        // Establecer el canal WiFi a escanear
        esp_wifi_set_channel(canal, WIFI_SECOND_CHAN_NONE);
        
        Serial.println("✅ Sniffer activado");
        Serial.printf("📡 Escuchando en canal %d...\n", canal);
        Serial.println("💡 Escribe 's' para detener");
        Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        
        escaneando = true;  // Marcar que estamos escaneando
      } else {
        Serial.println("❌ Comando inválido");
        Serial.println("💡 Usa: número (1-13), 's' (stop), o 'r' (reset)\n");
      }
    }
  }
  
  // Pequeña pausa para no saturar el CPU
  // En Python sería como time.sleep(0.01)
  delay(10);
}
