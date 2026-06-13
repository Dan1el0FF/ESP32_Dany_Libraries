# ESP32_Dany_Libraries

Colección de librerías en C++ para ESP32 sobre ESP-IDF, pensadas como componentes reutilizables entre distintos proyectos: sensores, actuadores, buses de comunicación, interfaces y algoritmos de control. Cada librería es independiente y se puede copiar a un proyecto nuevo tomando solo lo que se necesite.

Requisitos


Framework: ESP-IDF (no Arduino). El código usa directamente ledc, i2c_master, rmt, adc, uart, esp_now, esp_wifi, esp_timer y FreeRTOS.
Driver I2C: las librerías de I2C usan el driver nuevo driver/i2c_master.h, disponible a partir de ESP-IDF v5.2. Versiones anteriores no compilarán sin adaptar.
Lenguaje: C++ (los archivos son .cpp / .h).
Hardware: ESP32. Algunas librerías incluyen detalles específicos del chip (por ejemplo, UltraSonic incluye una cabecera de la familia C-series); revisa la nota de cada componente si usas una variante distinta.


Nota: sustituye los datos de placa/versión exacta por los tuyos si difieren. El requisito de ESP-IDF v5.2 está deducido del uso de i2c_master.h.

La pieza central es MultipleI2C. Es el bus del que dependen casi todos los sensores I2C (BMI160, BMP280, HMC5883L) y el display OLED. El patrón es siempre el mismo: creas una instancia de MultipleI2C, la configuras, y le pasas el puntero a cada sensor que cuelga del bus. Por eso varios sensores traen una carpeta Dependencies/ con su copia de MultipleI2C — para que la librería sea autocontenida si la copias suelta.

Regla rápida para ubicar cualquier cosa nueva: ¿mide algo del entorno? -> Sensors. ¿Acciona algo físico o lo muestra? -> Drivers. ¿Habla con el hardware a bajo nivel o por red? -> COMS. ¿Solo procesa números? -> Math o Control.

Estructura del repositorio

ESP32_Dany_Libraries/
├── COMS/        -> Comunicacion: buses de bajo nivel y red
├── Sensors/     -> Sensores: miden magnitudes del entorno
├── Drivers/     -> Actuadores y salida: mueven o muestran algo
├── Control/     -> Algoritmos de control y estimacion (solo software)
└── Math/        -> Matematicas para robotica (solo software)]

COMS — Comunicación

MultipleI2C: Bus I2C maestro que permite manejar varios dispositivos sobre el mismo bus (un solo bus, varios device_address). Es la dependencia principal de los sensores I2C y el OLED. API: setup(), read(), write() (devuelven bool de éxito).

SimpleI2C: Versión simplificada de I2C maestro para un uso más directo (sus read/write no devuelven estado). Útil cuando no necesitas el manejo multi-dispositivo.

SimpleUart: Envoltorio sencillo de UART: read(), write(), available(). Maneja puerto, baud rate y buffer por cola.

ESPNow: Comunicación inalámbrica ESP-NOW entre placas ESP32 (sin router). Registro de peers por MAC, envío y recepción de mensajes de hasta 250 bytes. API: setup(), addPeer(), send_data(), is_data_received(), get_received_data().

Wifi: Conexión WiFi en modo estación (STA) con máquina de estados (CONNECTING, CONNECTED, etc.). Basada en un tutorial de embeddedtutorials.com, adaptada a C++. API: SetCredentials(), Init(), Begin(), GetState().


Sensors — Sensores

BMI160: IMU de 6 ejes (acelerómetro + giroscopio). Lee aceleración en m/s² y velocidad angular en °/s, con rangos configurables, autocalibración de offsets y lectura de temperatura del chip.

BMP280: Sensor de presión y temperatura. Devuelve temperatura, presión y altitud (absoluta o relativa con autocalibración).

HMC5883L: Magnetómetro de 3 ejes. Devuelve campo magnético X/Y/Z, magnitud y heading (rumbo), con calibración de offsets y corrección por declinación magnética.

UltraSonic: Sensor de distancia ultrasónico (HC-SR04). Mide distancia por tiempo de vuelo del eco. API: getDistance().

Encoder: Encoder incremental por interrupciones: cuenta pulsos A/B, calcula RPM y dirección de giro. Pensado para motores con PPR conocido.

QuadratureEncoder: Decodificación en cuadratura por tabla de estados (más robusta que contar flancos sueltos). Devuelve ángulo, velocidad y dirección.

GhostEncoder: Encoder "virtual": estima posición y velocidad de un motor a pasos a partir de los pulsos que tú generas (step/dir) con un timer, sin encoder físico. Útil para CNC/steppers en lazo abierto.

Joysick: Joystick analógico por ADC: lee ejes VRX/VRY (12 bits) y el botón SW, con zona muerta, centro e inversión de eje configurables. Nota: aunque está en Sensors por leer posición vía ADC, conceptualmente es una interfaz de entrada de usuario.

Drivers — Actuadores y salida

PwmPin: Generador de PWM genérico sobre LEDC, base de varios actuadores. Hay tres versiones de complejidad creciente: PwmPin (frecuencia configurable), PwmPinV2 (añade resolución en bits y threshold), PwmPinV3. Útil para LEDs, motores DC, o cualquier salida modulada.

ServoMotor (ESP32Servo): Control de servo estándar: mapea 0–180° a pulsos de 1–2 ms a 50 Hz (resolución de 10 bits). API: setAngle(), getAngle().

ESC_Driver: Control de ESC (variador para motores brushless) por PWM, mapeando 0–100 % a pulsos de 1–2 ms. Dos variantes: ESC_50Hz (PWM clásico) y ESC_400Hz (refresco rápido, 12 bits, mejor resolución de throttle — verifica que tu ESC soporte 400 Hz).

ESC_ONESHOT: Control de ESC por protocolo OneShot125 (pulsos de 125–250 µs, mucho más rápido que PWM). Dos implementaciones: RMT (precisa, usa el periférico RMT) y OneShot/LEDC (más simple, algo menos precisa).

OLED_128x64_SSD1315: Display OLED I2C de 128×64. Arquitectura en dos capas: SSD1315 maneja el hardware y el framebuffer (drawPixel, update, clear), y MiniGFX dibuja encima (texto, líneas, rectángulos, bitmaps, íconos). Depende de MultipleI2C.

Control — Control y estimación

PID: Controlador PID con anti-windup (límite de integral), reset de integral y soporte para variables circulares (ángulos). API: setup(gains, dt, windup, circular), calculate(ref, measurement), update_gains(), update_dt().

KalmanFilter: Filtro de Kalman 1D para fusión de sensores (típicamente IMU + magnetómetro) y estimación de ángulos (roll/pitch/yaw). Incluye compensación de inclinación (tilt compensation) y manejo de declinación magnética. API: setup(), calculate(), tilt_compensation().

Math — Matemáticas para robótica

HM (Matriz Homogénea): Construcción de matrices de transformación homogénea a partir de parámetros DH, multiplicación de matrices y extracción de ángulos de Euler. Base para la cinemática.

Kinematics: Cinemática de robots: directa (FK), inversa (IK con dos soluciones), Jacobiano y selección de la mejor solución de movimiento. Recibe los parámetros DH y el número de articulaciones.

Uso general

Cada librería trae su propio archivo de ejemplo (*_example.cpp) que muestra el uso mínimo. El patrón típico para un sensor I2C es:

#include "MultipleI2C.h"
#include "BMI160.h"

extern "C" void app_main(void) {
    MultipleI2C bus;            // 1. Crea el bus una sola vez
    BMI160 imu(&bus);           // 2. Pasalo a cada dispositivo del bus
    imu.init_BMI160();          // 3. Inicializa

    float acc[3], gyro[3];
    while (true) {
        imu.readAcc(acc);
        imu.readGyro(gyro);
        // ... usa los datos ...
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

Para integrar una librería en un proyecto ESP-IDF, copia su carpeta dentro de components/ (o de tu carpeta de fuentes) y asegúrate de incluir sus dependencias — sobre todo MultipleI2C para los componentes I2C.

Autor

Librerías desarrolladas por Dany para ESP32.
