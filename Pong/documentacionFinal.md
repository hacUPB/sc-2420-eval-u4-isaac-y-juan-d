# Documentacion Final 


lo primero fue tomar un juego en SDL2 estable que se pudiese usar para implementar los elementos de esta unidad, se tuvo en cuenta que se debian insertar 5 sonidos y un power up, este primero fue hacer que la bola se acelerara al tocar la paleta, y se reiniciara al cumplirse las condiciones del game over:

![alt text](image.png)

luego de implementar esto se comenzo con la implementacion del audio, se cargarom y pusieron a sonar por separado cada audio para verificar que si los leyera todos, lo que afortunadamente pasó, luego se comenzaron a implementar, como se ve en el codigo anterior que ya estan siendo llamados, se modifico la funcion play audio y load audio para que los cargara todos al tiempo y se creo un hilo con un semaforo que los va dejando en cola para que los valla reproduciendo, pero esto origino un proble y es que al ejecutar el codigo sale un error que dice que no es posible cargar lo audios. 

![alt text](image-1.png)

como la imagen muestra aparece este mensaje de error el cual no se porque esta sucediendo, me estoy ayudando con ChatGPT a ver si soy capaz de identificar el porque pasa esto, se propuso la siguiente solucion: 

![alt text](image-2.png)

la cual "funciono" porque ahora si se esta reproduciendo el audio, pero se reproduce el mismo para todos los casos y sigue saliendo el mensaje de error, esto me hace sospechar que el error puede estar en como se llama los sonidos, esto se esta haciendo a travez de una variable llamada file que almacena el nombre del audio y se escribe en cada caso, pero creo que no va a funcionar por lo que parece que voy a tener que crear una funcion para cada audio.

se hizo un cambio en el codigo para que los sonido se inicialicen primero antes de depronto intentar cargar uno por uno cuando se van a reproducir, el problema es que ahora solo se esta reproduciendo el de la condicion de chocar con las paredes por lo que parece que si se va a necesitar el cambio. 

![alt text](image-3.png)

con este cambio en la funcion play audio se logra hacer que los otros audios se reproduzcan almenos una vez inicial pero no se repiten, el unico que lo hace es el de las paredes, ese si se reproduce todas las veces que pase y esta vez no se que es lo que puede estar causando eso, sigo creyendo que es la forma en que se llaman pero no estoy seguro.

ajuste los audios para que fuesen mas cortos y mejoro la forma en que sonaban y sospechaba que era que como no terminaba un audio no comenzaba el otro pero no es eso, por lo que no se que hacer, porque no hay diferencia en como se llaman los audios y las soluciones que le pido a ChatGPT no hacen nada o incluso dañan el codigo 


