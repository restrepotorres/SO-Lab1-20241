#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // verificar el numero de argumentos
    if (argc > 3) {
        printf("Uso: %s [archivo_entrada] [archivo_salida]\n", argv[0]);
        printf("  Sin argumentos: lee de stdin y escribe a stdout\n");
        printf("  1 argumento: lee del archivo y escribe a stdout\n");
        printf("  2 argumentos: lee del archivo y escribe al archivo de salida\n");
        return 1;
    }
    
    FILE *fp;
    char **lines = NULL;  // Array dinámico de punteros a strings
    int line_count = 0;
    int capacity = 10;    // Capacidad inicial
    
    // Determinar como ingresan los datos
    // si argumentos se le ingresa el texto por consola
    // 1 o mas  argumentos, lee el argumento 1 como entrada
    if (argc == 1) {
        fp = stdin;
        printf("Ingrese el texto (presione Ctrl+D para terminar):\n");
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            printf("Error: No se pudo abrir el archivo %s\n", argv[1]);
            return 1;
        }
    }
    
    // Asignar memoria inicial para el array de punteros
    lines = malloc(capacity * sizeof(char*));
    if (lines == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        fclose(fp);
        return 1;
    }
    
    char buffer[1024];  // buffer para leer líneas
    
    // Leer todas las líneas del archivo
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // si se necesita mas espacio, se duplica la capacidad del array
        if (line_count >= capacity) {
            capacity *= 2;  // Duplicar la capacidad
            char **temp = realloc(lines, capacity * sizeof(char*));
            lines = temp;
        }
        
        // Asignar memoria para la línea actual, y se le suma el espacio para un 2 char para poder ponerle 
        // \n o \0, es decir un salto de linea o un simbolo de terminacion de archivo
        int line_length = strlen(buffer);
        lines[line_count] = malloc((line_length + 2) * sizeof(char)); 

        strcpy(lines[line_count], buffer);
        
        // Si la línea no termina con \n, agregarlo
        if (line_length > 0 && buffer[line_length - 1] != '\n') {
            lines[line_count][line_length] = '\n';
            lines[line_count][line_length + 1] = '\0';
        }
        
        line_count++;
    }
    
    // Cerrar archivo de entrada solo si no es stdin
    if (fp != stdin) {
        fclose(fp);
    }
    
    // Si 3 argumentos, entonces se guarda el resultado en el archivo del tercer argumento
    // sino, se muestra en la salida estandar 
    FILE *output_fp;
    int write_to_file = (argc == 3);
    
    if (write_to_file) {
        output_fp = fopen(argv[2], "w");
        if (output_fp == NULL) {
            printf("Error: No se pudo crear el archivo de salida %s\n", argv[2]);
            for (int i = 0; i < line_count; i++) {
                free(lines[i]);
            }
            free(lines);
            return 1;
        }
    } else {
        output_fp = stdout;  
    }
    
    // Imprimir líneas al reves con un ciclo hacia atras    
    for (int i = line_count - 1; i >= 0; i--) {
        fprintf(output_fp, "%s", lines[i]);
    }
    
    // Cerrar archivo de salida si se creó uno
    if (write_to_file) {
        fclose(output_fp);
        printf("Archivo invertido guardado como: %s\n", argv[2]);
    }
    
    // Liberar toda la memoria asignada
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
    
    return 0;
}