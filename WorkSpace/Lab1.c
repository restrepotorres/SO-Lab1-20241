#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Verificar argumentos
    if (argc != 2 && argc != 3) {
        printf("Uso: %s <archivo_entrada> [archivo_salida]\n", argv[0]);
        printf("  Si no se especifica archivo_salida, se imprime en consola\n");
        return 1;
    }
    
    FILE *fp;
    char **lines = NULL;  // Array dinámico de punteros a strings
    int line_count = 0;
    int capacity = 10;    // Capacidad inicial
    
    // Abrir archivo
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", argv[1]);
        return 1;
    }
    
    // Asignar memoria inicial para el array de punteros
    lines = malloc(capacity * sizeof(char*));
    if (lines == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        fclose(fp);
        return 1;
    }
    
    char buffer[1024];  // Buffer temporal para leer líneas
    
    // Leer todas las líneas del archivo
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Si necesitamos más espacio, redimensionar el array
        if (line_count >= capacity) {
            capacity *= 2;  // Duplicar la capacidad
            char **temp = realloc(lines, capacity * sizeof(char*));
            if (temp == NULL) {
                printf("Error: No se pudo redimensionar memoria\n");
                // Liberar memoria ya asignada
                for (int i = 0; i < line_count; i++) {
                    free(lines[i]);
                }
                free(lines);
                fclose(fp);
                return 1;
            }
            lines = temp;
        }
        
        // Asignar memoria para la línea actual (con espacio extra por si necesitamos \n)
        int line_length = strlen(buffer);
        lines[line_count] = malloc((line_length + 2) * sizeof(char)); // +2 para \n y \0
        if (lines[line_count] == NULL) {
            printf("Error: No se pudo asignar memoria para la línea\n");
            // Liberar memoria ya asignada
            for (int i = 0; i < line_count; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(fp);
            return 1;
        }
        
        strcpy(lines[line_count], buffer);
        
        // Si la línea no termina con \n, agregarlo
        if (line_length > 0 && buffer[line_length - 1] != '\n') {
            lines[line_count][line_length] = '\n';
            lines[line_count][line_length + 1] = '\0';
        }
        
        line_count++;
    }
    
    fclose(fp);
    
    // Determinar dónde escribir la salida
    FILE *output_fp;
    int write_to_file = (argc == 3);
    
    if (write_to_file) {
        output_fp = fopen(argv[2], "w");
        if (output_fp == NULL) {
            printf("Error: No se pudo crear el archivo de salida %s\n", argv[2]);
            // Liberar memoria ya asignada
            for (int i = 0; i < line_count; i++) {
                free(lines[i]);
            }
            free(lines);
            return 1;
        }
    } else {
        output_fp = stdout;  // Usar la salida estándar (consola)
    }
    
    // Imprimir líneas en orden inverso
    if (!write_to_file) {
        fprintf(output_fp, "Contenido del archivo en orden inverso:\n");
        fprintf(output_fp, "=====================================\n");
    }
    
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