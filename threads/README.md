# Threads

![img](/docs/TP2Taller.drawio.png?raw=true "img")

## Explicacion del uso de colas en el trabajo practico:

### Game

El juego posee una cola game_channel, la cual es el canal de comunicación de los jugadores con el juego. Aquí, los jugadores podrán depositar sus órdenes, que en este caso son mensajes para atacar a los enemigos. Dentro de la arquitectura planteada, el juego no puede ser bloqueante. Para cumplir con lo solicitado en el enunciado, corresponde que en cada iteración del juego simplemente compruebe si existe algún mensaje nuevo dentro de la cola y, en ese caso, lo saque y realice las acciones correspondientes, de manera que no existan retrasos en la lógica del mismo.

Además, el juego debe comunicarse con los jugadores conectados, pero esta lógica es delegada a un monitor.

### Monitor

El monitor es el recurso compartido principal en la arquitectura. Posee una referencia a todas las colas de comunicación de los jugadores y se encarga de transmitir todos los eventos ocurridos en la lógica central del juego, ya sea que un enemigo reviva o muera.

Dado que el monitor actúa simplemente como mediador entre el juego y el hilo del emisor (sender), utilizando la cola para ello, la operación de difusión (broadcast) no será bloqueante.

### Sender 

El hilo sender es el encargado de enviar información a los clientes. Recibe los mensajes del estado del juego (muerte o resurrección de los enemigos) a través de la cola, de la cual el monitor tiene una referencia.

### Receiver 

El hilo receiver se encarga de recibir los ataques del cliente y pasarlos a la cola de ataques del juego.

#### General

Tanto el Sender como el Receiver usan lógica de cola bloqueante. En este caso, como las colas no tienen un límite (que podría ser agregado), no se ven los efectos bloqueantes de las mismas. Esto es una opción lógica, dado que si alguno de estos hilos se bloquea esperando que se realice una acción, como un nuevo mensaje o que la cola se desocupe (si tuviera límite), no correrían de forma "busy wait", gastando recursos del procesador de manera innecesaria. Además, no bloquearían la lógica del juego.