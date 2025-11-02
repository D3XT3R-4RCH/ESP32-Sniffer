# 📡 ESP32 WiFi Sniffer - Tutorial Completo

Aprende a crear un sniffer WiFi profesional desde cero usando ESP32 y C/Arduino.

## 🎯 Descripción

Este repositorio contiene un tutorial **progresivo y educativo** para aprender a programar un sniffer WiFi con ESP32. Cada nivel construye sobre el anterior, explicando conceptos paso a paso.

**Ideal para**: Estudiantes de ciberseguridad, principiantes en C/Arduino, y cualquiera interesado en redes WiFi.

---

## 📚 Roadmap de Aprendizaje

### 🟢 Nivel 1: Hola Mundo WiFi
**Dificultad**: Fácil  
**Tiempo**: 15 minutos

**Aprenderás:**
- ✅ Inicializar comunicación Serial
- ✅ Configurar WiFi del ESP32
- ✅ Activar modo promiscuo (monitor mode)
- ✅ Conceptos básicos de canales WiFi

**Conceptos nuevos**: `Serial.begin()`, `WiFi.mode()`, `esp_wifi_set_promiscuous()`

[📂 Ver código: 01_hola_wifi](./01_hola_wifi/)

---

### 🟡 Nivel 2: Capturar Beacons
**Dificultad**: Media  
**Tiempo**: 30 minutos

**Aprenderás:**
- ✅ Funciones callback en C
- ✅ Punteros y estructuras
- ✅ Decodificar paquetes WiFi
- ✅ Extraer SSID y MAC address

**Conceptos nuevos**: Callback functions, `void *buf`, paquetes beacon, operaciones bit a bit

[📂 Ver código: 02_capturar_beacons](./02_capturar_beacons/)

---

### 🟠 Nivel 3: Lista Sin Duplicados
**Dificultad**: Media  
**Tiempo**: 30 minutos

**Aprenderás:**
- ✅ Crear estructuras de datos (`struct`)
- ✅ Arrays en C
- ✅ Algoritmos de búsqueda
- ✅ Evitar duplicados

**Conceptos nuevos**: `struct`, arrays estáticos, funciones de búsqueda

[📂 Ver código: 03_sin_duplicados](./03_sin_duplicados/)

---

### 🔴 Nivel 4: Tabla Actualizable
**Dificultad**: Media-Alta  
**Tiempo**: 45 minutos

**Aprenderás:**
- ✅ Timestamps con `millis()`
- ✅ Timeout y eliminación automática
- ✅ Formateo de tablas
- ✅ Actualización periódica

**Conceptos nuevos**: `millis()`, timeouts, formateo avanzado, limpieza de arrays

[📂 Ver código: 04_tabla_actualizable](./04_tabla_actualizable/)

---

### 🟣 Nivel 5: Sniffer Completo
**Dificultad**: Alta  
**Tiempo**: 1 hora

**Aprenderás:**
- ✅ Comandos interactivos por Serial
- ✅ Máquina de estados
- ✅ Control de flujo completo
- ✅ Sistema profesional funcional

**Conceptos nuevos**: Entrada de usuario, estados, parsing de comandos

[📂 Ver código: 05_sniffer_completo](./05_sniffer_completo/)

---

## 🛠️ Requisitos

### Hardware
- **ESP32** (cualquier modelo compatible)
  - ESP32 WROOM-32
  - ESP32 DevKit V1
  - ESP32-CAM (sin usar cámara)

### Software
- **Arduino IDE** 1.8.x o 2.x
- **Board Manager**: ESP32 by Espressif Systems
- **Puerto Serial**: Velocidad 115200 baudios

---

## ⚙️ Instalación

### 1. Instalar Arduino IDE
Descarga desde: https://www.arduino.cc/en/software

### 2. Agregar soporte para ESP32

1. Abre Arduino IDE
2. Ve a **Archivo** → **Preferencias**
3. En "URLs de gestor de placas adicionales", agrega:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Ve a **Herramientas** → **Placa** → **Gestor de placas**
5. Busca "ESP32" e instala "**ESP32 by Espressif Systems**"

### 3. Seleccionar tu placa

1. Conecta el ESP32 por USB
2. Ve a **Herramientas** → **Placa** → **ESP32 Arduino**
3. Selecciona tu modelo (ej: "ESP32 Dev Module")
4. Selecciona el puerto COM correcto en **Herramientas** → **Puerto**

### 4. Verificar instalación

Sube el código del **Nivel 1** y verifica que compile correctamente.

---

## 🚀 Cómo usar este tutorial

### Opción 1: Aprendizaje Progresivo (Recomendado)
1. Empieza por el **Nivel 1**
2. Lee TODO el código y los comentarios
3. Sube el código a tu ESP32
4. Prueba y experimenta
5. Continúa con el siguiente nivel

### Opción 2: Uso Directo
Si ya sabes C/Arduino, puedes ir directamente al **Nivel 5** para tener el sniffer completo.

---

## 📖 Conceptos Importantes

### ¿Qué es un Sniffer WiFi?
Un sniffer WiFi captura y analiza paquetes que viajan por el aire en redes WiFi. Similar a Wireshark, pero enfocado en WiFi.

### ¿Qué es el Modo Promiscuo?
Normalmente, tu tarjeta WiFi solo escucha paquetes dirigidos a ti. En modo promiscuo, escucha **TODOS** los paquetes en el canal actual.

### ¿Qué son los Beacons?
Paquetes que los routers transmiten constantemente (~10/seg) anunciando su presencia. Contienen:
- SSID (nombre de la red)
- MAC Address
- Canal
- Tipo de seguridad
- Otras capacidades

### Canales WiFi 2.4 GHz
WiFi 2.4 GHz tiene 13 canales (1-13). Los más usados:
- **Canal 1**: 2.412 GHz
- **Canal 6**: 2.437 GHz
- **Canal 11**: 2.462 GHz

---

## 🎓 Comparación con Python

Si vienes de Python, aquí hay equivalencias:

| **Concepto C** | **Equivalente Python** |
|---|---|
| `Serial.begin(115200)` | `import serial` |
| `WiFi.mode(WIFI_STA)` | `wifi.set_mode("station")` |
| `void funcion()` | `def funcion():` |
| `String texto = "hola"` | `texto = "hola"` |
| `int numero = 5` | `numero = 5` |
| `for (int i=0; i<10; i++)` | `for i in range(10):` |
| `if (condicion) {...}` | `if condicion:` |
| `delay(1000)` | `time.sleep(1)` |
| `millis()` | `time.time() * 1000` |

---

## 📊 Ejemplo de Salida

```
╔══════════════════════════════════════════════════════════════════════╗
║  SNIFFER WiFi - Canal 6 | Redes: 5 | Beacons: 142                   ║
╠══════════════════════════════════════════════════════════════════════╣
║  #  │ SSID (Red WiFi)          │ MAC Address       │ Señal (dBm)  ║
╠══════════════════════════════════════════════════════════════════════╣
║  1  │ MiWiFi_Casa              │ a4:91:b1:2c:3d:4e │  -45 dBm     ║
║  2  │ Vecino_5G                │ c8:3a:35:11:22:33 │  -67 dBm     ║
║  3  │ <oculto>                 │ 00:11:22:33:44:55 │  -72 dBm     ║
║  4  │ CLARO_WiFi               │ 9c:c8:fc:49:4b:b9 │  -81 dBm     ║
║  5  │ Movistar_2.4G            │ 30:f9:47:cd:7c:92 │  -88 dBm     ║
╚══════════════════════════════════════════════════════════════════════╝
💡 Comandos: 's'=detener | 'r'=reiniciar | 1-13=cambiar canal
```

---

## 🔧 Solución de Problemas

### El ESP32 no se reconoce
- Instala drivers: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
- Prueba otro cable USB (algunos solo cargan, no transmiten datos)
- Presiona el botón **BOOT** mientras subes el código

### No se capturan paquetes
- Verifica que el canal tenga redes activas
- Prueba con canal 6 u 11 (los más usados)
- Asegúrate de que la velocidad serial sea 115200

### Errores de compilación
- Verifica que instalaste **ESP32 by Espressif Systems**
- Actualiza a la última versión del board manager
- Selecciona la placa correcta en Herramientas → Placa

---

## ⚖️ Consideraciones Legales

⚠️ **IMPORTANTE**: 
- Este proyecto es **SOLO para fines educativos**
- Capturar tráfico de redes ajenas puede ser ilegal en tu país
- Usa solo en redes de tu propiedad o con permiso explícito
- El autor no se responsabiliza por el mal uso de este código

---

## 📝 Recursos Adicionales

### Documentación Oficial
- [ESP32 Arduino Core](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [ESP32 WiFi API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)

### Tutoriales Recomendados
- [C Programming for Beginners](https://www.learn-c.org/)
- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [WiFi 802.11 Frame Types](https://en.wikipedia.org/wiki/802.11_Frame_Types)

---

## 🤝 Contribuciones

¡Las contribuciones son bienvenidas! Si encuentras un error o quieres agregar un nivel adicional:

1. Fork el repositorio
2. Crea una rama: `git checkout -b mejora-nivel-x`
3. Haz commit: `git commit -m 'Agrega explicación de X'`
4. Push: `git push origin mejora-nivel-x`
5. Abre un Pull Request

---

## 📜 Licencia

Este proyecto está bajo la licencia MIT. Ver archivo `LICENSE` para más detalles.

---

## ✨ Créditos

Creado con ❤️ para la comunidad de aprendizaje

**Autor**: D3XT3R-4RCH  
**Año**: 2025  
**Propósito**: Educación en seguridad de redes WiFi

---

## 🎯 Próximos Pasos

Después de completar todos los niveles, puedes:

1. **Agregar detección de seguridad** (WPA, WPA2, WEP)
2. **Escaneo automático** de todos los canales
3. **Guardar resultados** en tarjeta SD
4. **Enviar datos** a un servidor web
5. **Crear interfaz web** para control remoto
6. **Detectar dispositivos** conectados a cada red
7. **Analizar tráfico** de datos (no solo beacons)

---

¡Happy hacking! 🚀📡
