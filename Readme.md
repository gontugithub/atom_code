# ATOM: CÓDIGO

Proyecto de robótica modular para la asignatura de taller I, que integra control remoto mediante mando de PlayStation, comunicación Bluetooth con modulo HC-05 y identificación del robot rival para sumo y sigue lineas con modulo array de diodos pololu.

---

## 📁 Estructura del Repositorio

```
atom_code/
├── control_remote/
|   ├── conexion_bluetooth.py  
|
├── ino/
|   ├── line_follower.ino              
|   ├── sumo_controll.ino          
|            
├── requirements.txt              
├── Readme.md 
```

### Descripción de Archivos

| Archivo | Descripción |
|---------|-------------|
| `control_remote/conexion_bluetooth.py` | Script en Python para procesar entrada del mando PS4 y transmitir vía Bluetooth |
| `ino/line_follower.ino` | Código para seguidor de líneas utilizando pololu array 8 diodos |
| `ino/sumo_controll.ino` | Codigo para sumo con control manual |
| `requirements.txt` | Librerías de Python necesarias para instalar en el entorno virtual `.venv` |

---

## Configuración del Software (PC con Linux)

### 1. Preparar el Entorno de Python

La carpeta `.venv` no se incluye en el repositorio. Para recrearla:

```bash
# Crear el entorno virtual
python3 -m venv .venv

# Activarlo
source .venv/bin/activate

# Instalar dependencias
pip install -r requirements.txt
```

### 2. Configurar el Enlace Bluetooth (HC-05)

Para que Linux reconozca el módulo como un puerto serie:

**Vincular el dispositivo:**
```bash
bluetoothctl trust 00:00:13:01:92:98
bluetoothctl pair 00:00:13:01:92:98
# Usar PIN 1234 si se solicita en la terminal
```

**Mapear puerto serie:**
```bash
sudo rfcomm bind 0 00:00:13:01:92:98
sudo chmod 666 /dev/rfcomm0
```

---

## Conexiones de Hardware (HAY QUE PONERLAS BIEN)

### Motores & Bluetooth

| Componente | Pin Arduino | Función |
|-----------|-------------|---------|
| HC-05 TXD | 2 | RX Software Serial |
| HC-05 RXD | 13 | TX Software Serial |
| L298N ENA | D3 | Velocidad Motor A (PWM) |
| L298N IN1/IN2 | D4 / D5 | Dirección Motor A |
| L298N ENB | D9 | Velocidad Motor B (PWM) |
| L298N IN3/IN4 | D8 / D7 | Dirección Motor B |

### Sensor Ultrasónico (HC-SR04)

| Sensor | Pin Arduino | Función |
|--------|-------------|---------|
| VCC | 5V | Alimentación |
| Trig | D11 | Disparo |
| Echo | D12 | Eco |
| GND | GND | Tierra |

---

## 🚀 Ejecución del Proyecto

### Paso 1: Cargar el código en Arduino
Sube `ino/sumo_controll.ino` a tu placa Arduino mediante el IDE de Arduino.

### Paso 2: Conectar el mando
Enchufa tu mando de PlayStation (PS4/PS5) vía USB a la PC.

### Paso 4: Enciende el robot
Enciende el robot, y comprueba que el modulo `HC-05` , esta encendido (para ello tiene que tener una luz roja parpadeante)

### Paso 3: Iniciar el control remoto
```bash
source .venv/bin/activate
python3 control_remote/conexion_bluetooth.py
```

---

## 📋 Requisitos

- **Hardware:**
  - Arduino Nano/Uno
  - Módulo Bluetooth HC-05
  - Driver Motor L298N
  - Sensor Ultrasónico HC-SR04
  - Sensores QTR pololu 8 sensores (para line_follower)
  - Mando de PlayStation (PS4/PS5)

- **Software:**
  - Python 3.7+
  - Arduino IDE
  - Librerías de Python (ver `requirements.txt`)

---

## 🎮 Características Principales

- ✅ Control remoto con mando de PlayStation
- ✅ Comunicación Bluetooth HC-05
- ✅ Seguidor de líneas automático
- ✅ Modo combate sumo robótico
- ✅ Gestión modular del código

---

## 📝 Notas Importantes

- El entorno virtual (`.venv`) debe crearse localmente en cada PC
- Para usar Bluetooth en Linux, asegúrate de tener permisos de usuario sobre los puertos
- El PIN del módulo HC-05 por defecto es **1234**
- La dirección MAC puede variar según tu dispositivo Bluetooth
