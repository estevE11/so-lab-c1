# Usage
Instalar GCC

## Run
```
make
```

####Y en terminales separadas:
```
./ser1.exe <puerto>
```
Por ejemplo `./ser1.exe 8080` 

```
./cli1.exe <ip> <palabra> <puerto>
```
Por ejemplo `./ser1.exe 127.0.0.1 hola 8080`
La *ip* como estamos en local tiene q ser *localhost* y el puerto tiene q ser en el que tenemos abierto el servidor.


# Resumen de los enunciados

## Actividad 1 (TCP)
El servidor genera un numero entre 0-100 y el cliente lo tiene que adivinar. No hace falta input del usuario, el cliente primero escoje la mitad de 0-100 = 50, el servidor le dice si es mas pequeño o mas grande. Si es mas pequeño 0-50 -> 25, si es mas grande 50-100 -> 75 y asi hasta que encuentre el numero.

## Actividad 2 (TCP)
Cliente y servidor igual, pero al servidor le tienes que pasar un archivo por parametro que tiene que leer y generar un array con el numero de caracteres de cada linia.

Ejemplo:
```
hola que
tal
va
todo
```
Generaria el array: `[8, 3, 2, 4]` que son el numero de caracteres de cada linia.

Cuando se haga la conexion, el servidor tiene que escojer una linia aleatoria y el numero de caracteres de esa linia es el numero que tiene que adivinar el cliente. El resto del juego es igual que en la actividad 1, de hecho yo creo que el cliente se puede aprovechar sin tocar nada. Solo hay que cambiar el servidor.

## Actividad 3 (TCP y UDP)
Hay que crear 3 archivos: **file3.c**, **cli3.c**,**ser3.c**

### file3.c
- Servidor **UDP**
- 2 parametros:
  - FILE: Archivo a leer
  - PORT: Puerto (opcional, def: 9999)
- Cuando se inicia tiene que cargar el archivo y guardar el numero de caracteres de cada linia en un array igual que la actividad 2.
- Va ha haver 2 peticiones del cliente:
  1. El numero de linias que tiene el archivo
  2. El numero de caracteres de la linia que pida el cliente.
- Ejecucion:
  - El servidor (file3.c) recive una conexion el cliente
  - El servidor envia el numero de linias del archivo
  - El cliente genera un numero aleatorio para escoger una de las linias y le envia el numero al servidor.
  - El servidor devuelve el numero de caracteres de la linia que el cliente ha escogido.

**OJO: El cliente de file3.c es ser3.c (NO cli3.c)**

### ser3.c
- Servidor **TCP**
- 3 parametros:
  - PORT_UDP: Puerto para conectarse al servidor file3.c
  - IP: IP del servidor file3.c
  - PORT_TCP: Puerto para abrir el servidor TCP
- Cuando se inicia tiene que hacer una peticion al servidor file3.c para obtener el numero de linias del archivo.
- Cuando le llegue una conexion de un cliente, va a generar un numero aleatorio y hacer una petucion al servidor file3.c para que le de el numero de caracteres de esa linia. Ese es el numero que el cliente tiene que adivinar.


### cli3.c
- Cliente **TCP**
- Igual que la actividad 2.

### Ejemplo
- Imaginamos que tenemos un archivo `x.txt` que tiene 4 linias de `[10, 23, 45, 145]` caracteres.
- Ejecutamos `./file3.c x.txt 700` (700 es el puerto)
- Se guarda la longitud de cada linia en un array: `[10, 23, 45, 145]`
- Ejecutamos `./ser3.c 700 127.0.0.1 8080`. Esto envia un mensaje al puerto 700 y de respuesta recibe el numero de linias del archivo `x.txt`.
- Ejecutamos `./cli3.c 127.0.0.1 8080` que hara una peticion para conectarse a *ser3.c*.
- Cuando reciba la conexion *ser3.c* va a generar un numero aleatorio entre 0-3 y se lo va a enviar a *file3.c*. Este le respondera con el numero de caracteres de esa linia. Por ejemplo, el numero aleatorio es 2, *file3.c* va a devolver 45.
- Sigue el juego igual que en la Actividad 2.


# Memoria
De todas las actividades hay que:
- Explicar como ejecutar el codigo
- Mostrar diferentes ejecuciones con diferentes parametros
- Mostrar la ejecucion del server con mas de un cliente
- Poner capturas de pantalla
- Poner explicaciones del codigo