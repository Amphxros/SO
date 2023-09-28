
# Ejercicios

### Ejercicio 1: Compilacion
- ¿Qué ha ocurrido con la “llamada a min()” en hello2.i?
En cuanto a min() lo que ha ocurrido es que se ha sustituido la macro

hello.c
```
#define min(x,y) ( (x<y)?x:y )

...

for (i=0;i<N;i++) {
   printf("%s \t a= %d b= %d\n",cad,a,b);
   a++;
   a = min(a,b);
 }

```
hello.i

```
no define
...
  for (i=0;i<5;i++) {
   printf("%s \t a= %d b= %d\n",cad,a,b);
   a++;
   a = ( (a<b)?a:b );
 }

```

- ¿Qué efecto ha tenido la directiva #include <stdio.h>?

En el caso de stdio se ha incluido el contenido de stdio en el "hello.i"

### Ejercicio 1: Make

```
usuario_local@pto0707:~/Documentos/SO/ficheros_p1/ejercicio1/2-Make$ make
gcc -Wall -g -c -o aux.o aux.c
gcc -Wall -g -c -o init.o init.c
gcc -Wall -g -o prueba aux.o init.o -lm
```