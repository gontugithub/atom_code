import pygame
import serial
import time

puerto_bt = '/dev/rfcomm0' 

try:
    arduino = serial.Serial(puerto_bt, 9600, timeout=1)
    print("¡Conectado al Bluetooth del Arduino!")
except:
    print(f"Error: No se pudo abrir {puerto_bt}. ¿Hiciste el sudo rfcomm bind?")
    exit()

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No se detecta el mando. Conéctalo por USB o Bluetooth al PC.")
    exit()

mando = pygame.joystick.Joystick(0)
mando.init()
print(f"Leyendo mando: {mando.get_name()}")

try:
    while True:
        pygame.event.pump()
        
        # --- LECTURA DE BOTONES PARA ATAQUES ESPECIALES ---
        # Los índices pueden variar según tu mando. Prueba con 4, 5, 6 y 7.
        btn_l1 = mando.get_button(4) 
        btn_r1 = mando.get_button(5) 
        btn_l2 = mando.get_button(6) 
        btn_r2 = mando.get_button(7) 

        macro_enviada = False

        if btn_r2:
            arduino.write("M1\n".encode()) # Comando M1 = Torero
            print("\n¡ATAQUE: Torero activado! (R2)")
            macro_enviada = True
            time.sleep(0.5) # Pausa para no enviar el comando 20 veces por el rebote del botón
            
        elif btn_l2:
            arduino.write("M2\n".encode()) # Comando M2 = Tornado
            print("\n¡ATAQUE: Tornado activado! (L2)")
            macro_enviada = True
            time.sleep(0.5)
            
        elif btn_r1:
            arduino.write("M3\n".encode()) # Comando M3 = Embestida
            print("\n¡ATAQUE: Embestida activada! (R1)")
            macro_enviada = True
            time.sleep(0.5)
            
        elif btn_l1:
            arduino.write("M4\n".encode()) # Comando M4 = Freno de ancla
            print("\n¡ATAQUE: Freno de Ancla! (L1)")
            macro_enviada = True
            time.sleep(0.5)

        # --- MOVIMIENTO NORMAL SI NO HAY ATAQUE ---
        if not macro_enviada:
            velocidad = int(mando.get_axis(1) * -100) 
            giro = int(mando.get_axis(3) * 100) 
            
            datos = f"X{giro},Y{velocidad}\n"
            arduino.write(datos.encode())
            print(f"Enviando -> {datos}   ", end='\r') # El \r hace que se sobreescriba la línea, queda más limpio
            
        time.sleep(0.05) 

except KeyboardInterrupt:
    print("\nDeteniendo...")
    arduino.close()