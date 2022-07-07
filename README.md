# **PIPEX** (Original perteneciente a Víctor, ligeramente modificado para mis necesidades)

## Parte obligatoria

### Consideraciones generales

En este proyecto introducimos una nueva variable en el main, se trata del char **env
int main(int argc, char **argv, char **env)

La variable de entorno es un array de punteros a las variables de entorno.
Normalmente, esos valores hacen referencia a archivos, directorios y funciones comunes del sistema cuya ruta concreta puede variar, pero que otros programas necesitan poder conocer.
En Linux, las variables de entorno son marcadores de posición para la información almacenada dentro del sistema que pasa datos a los programas iniciados en shells (intérpretes de comando) o sub-shells.

#### Proyecto PIPEX

##### int main(int argc, char **argv, char **env)

La parte obligatoria empezamos redireccionando las salidas del STDIN y el STOUT a dos FD de forma que el programa lea y escriba en ellos en vez de en las entradas/salidas estándar del sistema
Esto lo hacemos mediante la función DUP2 permitida en el proyecto

En la parte obligatoria sólo se permiten entradas de 5 argumentos:
- argv[0] : Nombre del programa
- argv[1] : Archivo de lectura (el archivo de donde va a leer para ejecutar los comandos)
- argv[2] : Comando 1
- argv[3] : Comando 2 (después del pipe, lo que sale del Archivo 1 comando 1 es la entrada del comando 2)
- argv[4] : Archivo de escritura donde se imprime el output, si no existe se crea

**Función DUP2**

*Utilizamos la función dup2 para duplicar un descriptor de archivo en C*
*Los archivos son normalmente manipulados después de haber sido abiertos usando la llamada al sistema open. En caso de éxito, open devuelve un nuevo descriptor de archivo asociado con el archivo recién abierto. En los sistemas basados en Unix, el sistema operativo mantiene una lista de archivos abiertos para cada programa en ejecución, llamada tabla de archivos. Cada entrada se representa con un entero de tipo int. Estos enteros se llaman descriptores de archivo en estos sistemas, y muchas llamadas al sistema toman los valores de los descriptores de archivo como parámetros.*
*Cada programa en ejecución tiene tres descriptores de archivo abiertos por defecto cuando se crea el proceso, a menos que se decida cerrarlos explícitamente. La función dup2 crea una copia del descriptor de fichero dado y le asigna un nuevo entero. dup2 toma como primer parámetro un descriptor de fichero antiguo a clonar y el segundo parámetro es el entero de un nuevo descriptor de fichero. Como resultado, ambos descriptores de archivo apuntan al mismo archivo y pueden usarse indistintamente. Tenga en cuenta que si el usuario especifica un entero utilizado actualmente por el archivo abierto como segundo parámetro, éste se cerrará y se reutilizará como el descriptor de archivo clonado.*

Empezamos primero abriendo o creando el archivo de escritura porque en el equivalente al pipex del programa, siempre se crea este archivo aunque falle luego el de lectura
Luego creamos el pipe y ejecutamos el primer comando con el infile, y sale por el extremo de escritura del pipe
Una vez ejecutado el primer comando ejecutamos el segundo comando y lo escribimos en el fichero de escritura creado.

##### int	ft_openfile(char *filename, int filetype)
La función personalizada    *int	ft_openfile(char *filename, int filetype)*  recibirá el archivo de lectura o escritura (argv[1] o argv[4]) y un flag indicando si es lectura/escritura
En el caso de que sea de lectura, comprobamos que existe y es accesible (función access(filename, F_OK) permitida en el subject) y en caso de que así sea se devuelve el fichero en solo lectura y se asigna al FD de lectura open (filename, O_RDONLY), si no existe o no es accesible, escribimos en el STDERR y salimos con la funicón exit(1)
En el caso de que el flag sea de escritura, el fichero se usa y se sobre escribe (TRUNC) y si no existe se crea *open (filename, O_CREAT | O_WRONLY | O_TRUNC, 0644))*

Hay muchos flags para la función open pero nosotros usamos:
FLAGS OPEN:
O_CREAT If pathname does not exist, create it as a regular file.
        The argument flags must include one of the following access
        modes: O_RDONLY, O_WRONLY, or O_RDWR.  These request opening the
        file read-only, write-only, or read/write, respectively.
O_TRUNC If the file already exists and is a regular file and the
        access mode allows writing (i.e., is O_RDWR or O_WRONLY)
        it will be truncated to length 0.  If the file is a FIFO
        or terminal device file, the O_TRUNC flag is ignored.
        Otherwise, the effect of O_TRUNC is unspecified.


Después duplicamos (dup2) el STDIN en el FD de lectura y el STDOUT en el FD de escritura

##### void ft_pipex(char *command1, char **env)

A continuación lanzamos el pipex y enviamos el Comando1 (argv[2]) y las variables de entorno
Al crear un pipe (comunicación de procesos unidireccional, tiene un extremo de escritura y otro de lectura se comunican procesos relacionados (padre-hijo o hijo-hijo)
Un pipe lleva asociado dos FD, uno para lectura y otro para escritura, en el pipe la información se mantiene FIFO.
Un pipe tiene como argumento un array de dos enteros el fd[0] es el extremo de lectura y el fd[1] el de escritura
Se inicializa con la función pipe(fd) que está permitida en el subject.  

Posteriormente se procede a llamar a la función pid_t fork(void);
fork  crea  un proceso hijo que difiere de su proceso padre sólo en su PID y PPID, y en el hecho de que el uso de recursos esté asignado a 0.  Los candados de fichero (file locks) y las señales pendientes no se heredan.
En linux, fork está implementado usando páginas de copia-en-escritura (copy-on-write), así que la única penalización en que incurre fork es en el tiempo y  memoria  requeridos  para duplicar las tablas de páginas del padre, y para crear una única estructura de tarea (task structure) para el hijo.

Si el fork devuelve -1 es que ha dado error, y lo gestionamos, si da 0 estamos en el hijo y si da distinto de 0 estamos en el padre.
Tal y como lo hemos programado, el padre lee y el hijo escribe en el pipe (el hijo es el proceso que primero se ejecuta)
Lo que hacemos con el hijo es cerrar el pipe en el extremo de lectura, duplicamos el STDOUT en el fd[1] de forma que el output en vez de ir al STDOUT vaya a al fd[1] del extremo de escritura del pipe y miramos si podemos ejecutar el comando (miramos si el comando existe y es accesible)
El padre lo que hace es cerrar el pipe en el extremo de escritura y duplica el STDIN en el fd[0] para que el output del hijo sea lo que lee como STDIN para el siguiente comando

##### void ft_getenv(char *command1, char **env)
Para ver si el comando que hemos pasado existe y es accesible, usamos la función personalizada ft_getenv, si existe lo ejecutamos
Para poder guardar el comando con los flags (por ejemplo, "ls -la") lo que hacemos es un split que almacenamos en char	**command; y que terminará en NULL
Además también necesitamos una variable tipo char *path donde almacenamos la ruta donde buscar en las variables de entorno, esta ruta que se obtiene de char **env tiene el siguiente formato *PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin* habrá que comprobar en cada una de las rutas que vienen delimitadas por :

Primero hacemos un split por espacio, en la primera posición es donde se almacenará el comando
Puede darse el caso de que el usuario especifique la ruta del comando, por lo que si en el primer valor, encontramos una '/' no buscaremos el PATH y ejecutaremos el comando directamente sin esta comprobación
En caso de que no encontremos la '/' buscamos la ruta del comando y si lo encontramos, ejecutamos

Para ejecutar usamos la función del subject exceve

**Función EXCEVE**

*Execve es una llamada al sistema del sistema operativo UNIX, estandarizada en el estándar POSIX y otros.*

*Su prototipo es el siguiente (en lenguaje de programación C): int execve (const char *filename, const char *argv [], const char *envp[]);*
*execve ejecuta el programa indicado por filename. filename debe ser un binario ejecutable, o bien un guion shell (shell script) comenzando con una línea de la forma "#! intérprete [arg]" (conocida como shebang). En el segundo caso, el intérprete debe ser un nombre de camino válido para un ejecutable que no sea él mismo un guion y que será ejecutado con los argumentos [arg] (opcionales) más el archivo filename a interpretar.*

*argv es un array de cadenas de argumentos pasados al nuevo programa. envp es un array de cadenas, convencionalmente de la forma clave=valor, que se pasan como entorno al nuevo programa. Tanto argv como envp deben terminar en un puntero nulo. El vector de argumentos y el entorno pueden ser accedidos por la función "main" del programa invocado cuando se define como int main(int argc, char *argv[], char *envp[]).*

*execve no regresa en caso de éxito, y el segmento de código, segmento de datos, BSS y la pila del proceso invocador se sobreescriben con los correspondientes del programa cargado. El programa invocado hereda el PID del proceso invocador y cualquier descriptor de archivo abierto que no se hubiera configurado para "cerrar en ejecución" (close on exec). Las señales pendientes del proceso invocador se limpian. Cualquier señal capturada por el proceso invocador es devuelta a su comportamiento por defecto.*

Si la función tiene éxito lanza la función y en vez de sacarlo por el STDOUT lo mete en el extremo de escritura del pipe y no sigue ejecutando el resto de instrucciones si no tiene éxito es que ha dado error y lo gestionamos diciendo que no se ha encontrado el comando por la STDERR

##### char	*getPath (char *cmd, char **env)
Para poder coger la ruta de la lista de variables de entorno, primero buscamos la línea que empiece por PATH= ya que en las variables de entorno están ubicadas en la siguiente línea PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin

Ahora creamos un bin para cada ":" y miramos en el bin si está el comando buscado si está y es accesible, devolvemos la ruta junto con el comando por ejemplo /usr/local/bin/ls que es lo que se ejectará en el exceve

y luego lanzamos el ejecutable sobre el Comando 2 (argv[3])

### Gestión de errores

Para la gestión de errores usaremos la macro errno y las funciones autorizadas en el subject strerror y perror
Crearemos una función en la libft ft_error(int *error) donde recibimos el número del error y lo imprimimos por el sterror y salimos con un exit y el número de error, esto sólo sirve para los errores predefinidos en la variable global errno

## Corrección parte obligatoria

Para las pruebas realizamos los siguientes test:

1.1 Comprobamos que ocurre si lanzamos un pipex con un infile que no existe

		<tests/infile ls -la | wc -l > tests/outfile
		./pipex tests/infile "ls -la" "wc -l" tests/outfile

El error debería ser *No such file or directory*

1.2 Comprobamos que ocurre si lanzamos un pipex con un infile que no tiene permisos de lectura

		<tests/infileNOK ls -la | wc -l > tests/outfile
		./pipex tests/infileNOK "ls -la" "wc -l" tests/outfile

El error debería ser *permission denied*  

1.3 Comprobamos que ocurre si lanzamos un pipex con un outfile que no tiene permisos de escritura

		<tests/infileOK ls -la | wc -l > tests/outfileNOK  
		./pipex tests/infileOK "ls -la" "wc -l" tests/outfileNOK

El error debería ser *permission denied*  

1.4 Comprobamos que ocurre si lanzamos un pipex sin comandos o con menos y más comandos que los permitidos en el subject

		./pipex tests/infileOK tests/outfileOK
		./pipex tests/infileOK "ls -la" tests/outfileOK
		./pipex tests/infileOK "ls -la" "wc -l" "ls -l" tests/outfileOK

Un error de *Invalid number of arguments*  

1.5 Comprobamos que ocurre si lanzamos un pipex con un comando que no exista

		<tests/infileOK c -la | wc -l > tests/outfileOK  
		./pipex tests/infileOK "c -la" "wc -l" tests/outfileOK

1.6 Comprobamos el funcionamiento de un pipex que busque el texto name en el infile y nos diga cuantas veces sale y lo escriba en el outfile (lo probamos con comilla simple y sin comilla simple)

		<tests/infileOK grep 'name' | wc -l > tests/outfileOK  
		./pipex tests/infileOK "grep name" "wc -l" tests/outfileOK  
		./pipex tests/infileOK "grep 'name'" "wc -l" tests/outfileOK
		
1.7 Probamos doble check para un echo con comillas

		<tests/infileOK echo '42 test' | sed 's/42/ft/g' > tests/outfileOK  
		./pipex tests/infileOK "echo 42 test" "sed s/42/ft/g" tests/outfileOK  
		./pipex tests/infileOK "echo '42 test'" "sed 's/42/ft/g'" tests/outfileOK

1.8 Probamos el primer comando no necesita input y el segundo comando no existe

		< tests/infileOK ls | nopeee > tests/outfileOK  
		./pipex tests/infileOK "ls" "nopeee" tests/outfileOK  

1.9 Probamos el primer comando existe pero con un flag que no existe

		< tests/infileOK cat -42_Madrid | grep '42_test' > tests/outfileOK  
		./pipex tests/infileOK "cat -42_Madrid" "grep '42_test'" tests/outfileOK

1.10 Probamos el segundo comando existe pero con un flag que no existe

		< tests/infileOK cat -t | cat -42_Madrid '42 test' > tests/outfileOK  
		./pipex tests/infileOK "cat -t" "cat -42_Madrid '42 test'" tests/outfileOK  

## Parte bonus

La parte bonus tiene dos aspectos cláramento diferenciados, por una parte, la posibilidad de gestionar múltiples pipes que es literalmente el mismo concepto que el bonus del GNL (se añade una condición para que se ejecute si el número de argumentos es superior a 5) y luego se lee el infile y se van comprobando y ejecutando los diferentes argumentos hasta llegar al outfile.

En la parte bonus de multiples pipes como mínimo la entrada debe de ser de 5 argumentos:
- argv[0] : Nombre del programa
- argv[1] : Archivo de lectura (el archivo de donde va a leer para ejecutar los comandos)
- argv[2] : Comando 1
- argv[3] : Comando 2 (después del pipe, lo que sale del Archivo 1 comando 1 es la entrada del comando 2)
- argv[4] : Comando 3 (después del pipe, lo que sale del comando 2 es la entrada del comando 3)
- argv[n-1] : Comando n - 3 (después del pipe, lo que sale del comando n - 4 es la entrada del comando n - 3)
- argv[n] : Archivo de escritura donde se imprime el output, si no existe se crea

La segunda parte tiene más contenido y consiste en replicar el here_doc

En la parte bonus de multiples pipes como mínimo la entrada debe de ser de 5 argumentos:
- argv[0] : Nombre del programa
- argv[1] : here_doc
- argv[2] : Limitador
- argv[3] : Comando 1 (La entrada por terminal pasa por el pipe y es la entrada del comando 1)
- argv[4] : Comando 2 (después del pipe, lo que sale del comando 1 es la entrada del comando 2)
- argv[n-1] : Comando n - 4 (después del pipe, lo que sale del comando n - 5 es la entrada del comando n - 4)
- argv[n] : Archivo de escritura donde se imprime el output, si no existe se crea

El ejemplo más sencillo es proporcionar entradas al comando cat:

	cat << TERMINAR | wc -l > tests/outfile  
	Y nos irá poniendo "pipe heredoc> " hasta que escribamos TERMINAR  
	pipe heredoc> Hola  
	pipe heredoc> Esto es un  
	pipe heredoc> here document  
	pipe heredoc> adiós  
	pipe heredoc> TERMINAR 


Para hacer la redirección con > (sobreescribe en el fichero) o con >> (que es lo que nos piden y hace un APPEND, lo pone al final del fichero)

Los here documents pueden parecer una prestación un tanto curiosa, pero son muy potentes porque le permiten invocar un programa interactivo como un editor y proporcionarle alguna entrada predefinida. Sin embargo, se suelen usar para enviar grandes cantidades de texto desde dentro del script, tal como vimos previamente, y para evitar tener que usar sentencias echo en cada línea. Puede usar símbolos de exclamación (¡!) a cada lado del identificador para que no haya confusión.

Si queremos procesar varias líneas de un archivo siguiendo un modo predeterminado, puede usar el editor de línea ed y proporcionarle comandos de un here document en un script shell.

El primer Bonus se consigue ejecutando el pipex en bucle hasta llegar al último comando
while(x < (argc -2))
	ft_pipex(argv[x++], env);
Donde x toma el valor de 2 (si no estamos en el caso del here_doc el argv[2] sigue siendo el comando1)

Si el primer argumento (argv[1] es "here_doc") el stdin ya no será un fichero, y además el fichero de escritura se abre (o se crea) en formato APPEND  
En vez de llamar a la función pipex, llamamos a la función ft_heredoc y no le pasamos las variables de entorno, sólo el argc (número de argumentos) y el argv (argumentos) *void ft_heredoc(int argc, char **argv)*  
El heredoc como replica el terminal, irá poniendo por pantalla (por eso no hacemos un dup2 del stdout en este momento) piex heredoc> y guardando cada entrada por terminal hasta que le metamos la palabra que hayamos definido como LIMITADOR.  
Primero hacemos el pipe y el fork con dos procesos uno de escritura y uno de lectura.  
Cada vez que introducimos un texto por el STDIN lo escribimos en el extremo de escritura del pipe hasta que escribimos el limitador. Cuando encuentra el limitador, salimos de este proceso con exit(0) que significa que salimos con EXITO y entra en acción el proceso de lectura del pipe (el padre) que recibe todo lo que hayamos metido por pantalla.  

En el caso del here_doc el número de argumentos es 6 o más, si es menor gestionamos el error.  
El programa una vez recibido el input por terminal lo mete en el pipe y va a ir haciendo pipes por cada comando hasta que tengamos la salida

Para un heredoc por bash, si el output no tiene permiso de escritura, lo que te permite es meter datos por terminal pero cuando los pasa por el pipe y llega al fichero de escritura da el mensaje de error, es por ello, que primero llamamos a la función de heredoc y posteriormente hacemos el openfile sobre el writefile

## Corrección parte bonus - multiples pipes

Para las pruebas realizamos los siguientes test:

1.1 Comprobamos que ocurre si lanzamos un pipex con un infile que no existe

		<tests/infile ls -la | wc -l > tests/outfileOK  
		./pipex tests/infile "ls -la" "wc -l" tests/outfileOK  

El error debería ser *No such file or directory*  

1.2 Comprobamos que ocurre si lanzamos un pipex con un infile que no tiene permisos de lectura

		<tests/infileNOK ls -la | wc -l > tests/outfileOK  
		./pipex tests/infileNOK "ls -la" "wc -l" tests/outfileOK 

El error debería ser *permission denied*  

1.3 Comprobamos que ocurre si lanzamos un pipex con un outfile que no tiene permisos de escritura

		<tests/infileOK ls -la | wc -l > tests/outfileNOK  
		./pipex tests/infileOK "ls -la" "wc -l" tests/outfileNOK  

El error debería ser *permission denied*  

1.4 Comprobamos que ocurre si lanzamos un pipex sin comandos o con menos que los permitidos en el subject

		./pipex tests/infileOK  tests/outfileOK  
		./pipex tests/infileOK "ls -la" tests/outfileOK  

Un error de *Invalid number of arguments*  

1.5 Comprobamos que ocurre si lanzamos un pipex con un comando que no exista

		<tests/infileOK c -la | wc -l > tests/outfileOK  
		./pipex tests/infileOK "c -la" "wc -l" tests/outfileOK  

1.6 Comprobamos el funcionamiento de un pipex que cambie la palabra 'name' por 'fame', luego busque el texto 'fame' en el pipe y nos diga cuantas veces sale y lo escriba en el outfile (lo probamos con comilla simple y sin comilla simple)

		<tests/infileOK sed 's/name/fame/g' | grep 'fame' | wc -l > tests/outfileOK
		./pipex tests/infileOK "sed s/name/fame/g" "grep 'fame'" "wc -l" tests/outfileOK
		./pipex tests/infileOK "sed s/name/fame/g" "grep fame" "wc -l" tests/outfileOK

1.7 Probamos doble check vamos cambiando la palabra name varias veces y la imprimimos al final

		<tests/infileOK sed 's/name/fame/g' | grep 'fame' | sed s/fame/rame/g | sed s/rame/mame/g> tests/outfileOK  
		./pipex tests/infileOK "sed s/name/fame/g" "grep fame" "sed s/fame/rame/g" "sed s/rame/mame/g" tests/outfileOK  
		./pipex tests/infileOK "sed 's/name/fame/g'" "grep 'fame'" "sed 's/fame/rame/g'" "sed 's/rame/mame/g'" tests/outfileOK  

1.8 Probamos el primer y segundo comando no necesitan input y el tercer comando no existe

		< tests/infileOK ls | pwd | nopeee > tests/outfileOK  
		./pipex tests/infileOK "ls" "pwd" "nopeee" tests/outfileOK  

1.9 Probamos el primer comando existe pero con un flag que no existe

		< tests/infileOK cat -42_Madrid | grep '42_test' > tests/outfileOK  
		./pipex tests/infileOK "cat -42_Madrid" "grep '42_test'" tests/outfileOK  

1.10 Probamos el segundo comando existe pero con un flag que no existe

		< tests/infileOK cat -t | cat -42_Madrid '42_test' > tests/outfileOK  
		./pipex tests/infileOK "cat -t" "cat -42_Madrid '42_test'" tests/outfileOK  

## Corrección parte bonus - here_doc
1.1 Lo primero que probamos es qué hace un pipex por el bash, para ello escribimos en el terminal

	cat << EOF | wc -l > tests/outfilecmd
	Y nos irá poniendo "pipe heredoc> " hasta que escribamos TERMINAR  
	pipe heredoc> Hola
	pipe heredoc> Esto es un
	pipe heredoc> here document
	pipe heredoc> adiós
	pipe heredoc> EOF

	Luego probamos lo mismo con el pipex: ./pipex here_doc EOF "cat" "wc -l" tests/outfilepipex y tiene que hacer lo mismo  
	pipex heredoc> Hola
	pipex heredoc> esto es un
	pipex heredoc> heredoc
	pipex here_doc> adiós
	pipex here_doc> EOF

El output que hayamos definido, deberá escribirse en el fichero de salida determinado en el pipex

1.2 Ahora hacemos una de las pruebas de multiples pipes, pero en vez de coger del infile, cogerá lo que le metamos por pantalla, como siempre, primero probamos lo que hace por terminal

		sed 's/name/fame/g' << EOF | grep 'fame' | sed 's/fame/rame/g' | sed 's/rame/Victor/g' > tests/outfile
		./pipex here_doc EOF "sed s/name/fame/g" "grep fame" "sed s/fame/rame/g" "sed s/rame/Victor/g" tests/outfileOK

1.3 Comprobamos que ocurre si lanzamos un pipex con un outfile que no tiene permisos de escritura

		sed 's/name/fame/g' << EOF | grep 'fame' | wc -l > tests/outfileNOK 
		./pipex here_doc EOF "sed s/name/fame/g" "grep fame" "wc -l" tests/outfileNOK  

El error debería ser *permission denied* pero primero te tiene que dejar meter datos por terminal

1.4 Comprobamos que ocurre si lanzamos un pipex sin comandos o con menos que los permitidos en el subject

		./pipex here_doc EOF "sed s/name/fame/g" tests/outfileOK

Un error de *Invalid number of arguments* 

1.5 Comprobamos que ocurre si lanzamos un pipex con un comando que no exista

		c -la << EOF | wc -l > tests/outfileOK
		./pipex here_doc EOF "c -la" "wc -l" tests/outfileOK

El error debería ser *Command not found: c* pero primero te tiene que dejar meter datos por terminal


## Comprobamos leaks
Con el comando leaks pipex en terminal 

		c3r13s6% leaks pipex
		You have access to multiple processes named pipex:
			a) 85238 ./pipex
    		b) 85239 ./pipex

Te dice que proceso quieres revisar, previamente le hemos puesto un while(1) antes del exceve en el hijo y miramos los dos procesos a ver si dan leaks

