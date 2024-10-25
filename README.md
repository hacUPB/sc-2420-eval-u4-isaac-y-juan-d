[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XglgMq0o)
# Documentación del Proyecto
---
Estudiante:  
Id:  
---
# Documentacion de ejercicios


## Ejercicio 5

En este ejercicio se tiene un codigo el cual el ejecutarse no pasa nada
![image](https://github.com/user-attachments/assets/db569554-458c-4d45-893c-0cc8cba7eba7)

y pues al analizar el codigo nos damos cuenta que es porque el hilo principal se cierra antes de que los otros hilos puedan hacer algo, lo que se puede hacer es utilizar una funcion join para que el programa espere que se ejecuten ambos hilos antes de cerrarse. 

## Ejercicio 6 

en este caso se presenta la solucion al problema anterior y al ejecutarlo se ve así

![image](https://github.com/user-attachments/assets/eb27f4b8-fb32-4d21-9c8a-de495375c473)

se nota que primero ejecuta todas las a y luego todas las b, por lo que efectivamente para esperar a que un hilo termine se debe de usar la funcion join 
en el fragmento de codigo que sigue 

```c
  pthread_create(&threadID1,NULL,&imprime, &threadParam1);
  pthread_join(threadID1,NULL);
  pthread_create(&threadID2,NULL,&imprime, &threadParam2);
  pthread_join(threadID2,NULL);
```

para este caso primero se crea un hilo y se epera que este termine para crear el otro y esperar que el otro termine es decir se ejecuta uno despues del otro, mientras que en el otro fragmento se ve asi:

```c
  pthread_create(&threadID1,NULL,&imprime, &threadParam1);
  pthread_create(&threadID2,NULL,&imprime, &threadParam2);
  pthread_join(threadID1,NULL);
  pthread_join(threadID2,NULL);
```

en este caso primero se crean ambos hilos y luego se ejecutan ambos casi de forma simultanea, se espera a que ambos terminen antes de terminar la ejecucion del programa.

