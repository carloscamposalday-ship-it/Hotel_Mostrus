#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE 50
#define ARCHIVO_HAB "habitaciones.txt"
#define ARCHIVO_HUESP "huespedes.txt"
#define ARCHIVO_EST "estancias.txt"

typedef struct {
    int id;
    int numero;
    float tarifa;
    int tiene_ventanas;     /* 1 = Si, 0 = No */
    int paredes_ectoplasma; /* 1 = Si, 0 = No */
    int muebles_reforzados; /* 1 = Si, 0 = No */
    int ocupada;            /* 1 = Si, 0 = No */
} Habitacion;

typedef struct {
    int id;
    char nombre[MAX_NOMBRE];
    int tipo;               /* 1 = Vampiro, 2 = Fantasma, 3 = Hombre Lobo, 4 = Otro */
    int luna_llena;         /* 1 = Si, 0 = No */
    float danos_totales;
} Huesped;

typedef struct {
    int id;
    int id_huesped;
    int id_habitacion;
    int dias;
    float cargos_danos;
    float total_pagar;
    int activa;             /* 1 = Hospedado, 0 = Ya salio */
} Estancia;

typedef struct {
    Habitacion *habitaciones;
    int cant_hab;
    int cap_hab;

    Huesped *huespedes;
    int cant_huesp;
    int cap_huesp;

    Estancia *estancias;
    int cant_est;
    int cap_est;
} Hotel;

/* Reserva memoria inicial para las 3 listas */
Hotel *hotel_crear(int capacidad_inicial) {
    Hotel *h = malloc(sizeof(Hotel));
    if (h == NULL) {
        return NULL;
    }

    h->habitaciones = malloc(capacidad_inicial * sizeof(Habitacion));
    if (h->habitaciones == NULL) {
        free(h);
        return NULL;
    }

    h->huespedes = malloc(capacidad_inicial * sizeof(Huesped));
    if (h->huespedes == NULL) {
        free(h->habitaciones);
        free(h);
        return NULL;
    }

    h->estancias = malloc(capacidad_inicial * sizeof(Estancia));
    if (h->estancias == NULL) {
        free(h->habitaciones);
        free(h->huespedes);
        free(h);
        return NULL;
    }

    h->cant_hab = 0;
    h->cap_hab = capacidad_inicial;

    h->cant_huesp = 0;
    h->cap_huesp = capacidad_inicial;

    h->cant_est = 0;
    h->cap_est = capacidad_inicial;

    return h;
}

/* Agregar con realloc si se llena */
int agregar_habitacion(Hotel *h, Habitacion nueva) {
    if (h->cant_hab == h->cap_hab) {
        int nueva_cap = h->cap_hab * 2;
        Habitacion *tmp = realloc(h->habitaciones, nueva_cap * sizeof(Habitacion));
        if (tmp == NULL) {
            return 0;
        }
        h->habitaciones = tmp;
        h->cap_hab = nueva_cap;
    }
    h->habitaciones[h->cant_hab] = nueva;
    h->cant_hab++;
    return 1;
}

int agregar_huesped(Hotel *h, Huesped nuevo) {
    if (h->cant_huesp == h->cap_huesp) {
        int nueva_cap = h->cap_huesp * 2;
        Huesped *tmp = realloc(h->huespedes, nueva_cap * sizeof(Huesped));
        if (tmp == NULL) {
            return 0;
        }
        h->huespedes = tmp;
        h->cap_huesp = nueva_cap;
    }
    h->huespedes[h->cant_huesp] = nuevo;
    h->cant_huesp++;
    return 1;
}

int agregar_estancia(Hotel *h, Estancia nueva) {
    if (h->cant_est == h->cap_est) {
        int nueva_cap = h->cap_est * 2;
        Estancia *tmp = realloc(h->estancias, nueva_cap * sizeof(Estancia));
        if (tmp == NULL) {
            return 0;
        }
        h->estancias = tmp;
        h->cap_est = nueva_cap;
    }
    h->estancias[h->cant_est] = nueva;
    h->cant_est++;
    return 1;
}

/* Libera toda la memoria al final */
void hotel_destruir(Hotel *h) {
    if (h != NULL) {
        free(h->habitaciones);
        free(h->huespedes);
        free(h->estancias);
        free(h);
    }
}

/* Guardar datos en los 3 archivos */
void hotel_guardar(const Hotel *h) {
    FILE *f1 = fopen(ARCHIVO_HAB, "w");
    if (f1 != NULL) {
        for (int i = 0; i < h->cant_hab; i++) {
            fprintf(f1, "%d;%d;%.2f;%d;%d;%d;%d\n",
                    h->habitaciones[i].id, h->habitaciones[i].numero,
                    h->habitaciones[i].tarifa, h->habitaciones[i].tiene_ventanas,
                    h->habitaciones[i].paredes_ectoplasma, h->habitaciones[i].muebles_reforzados,
                    h->habitaciones[i].ocupada);
        }
        fclose(f1);
    }

    FILE *f2 = fopen(ARCHIVO_HUESP, "w");
    if (f2 != NULL) {
        for (int i = 0; i < h->cant_huesp; i++) {
            fprintf(f2, "%d;%d;%d;%.2f;%s\n",
                    h->huespedes[i].id, h->huespedes[i].tipo,
                    h->huespedes[i].luna_llena, h->huespedes[i].danos_totales,
                    h->huespedes[i].nombre);
        }
        fclose(f2);
    }

    FILE *f3 = fopen(ARCHIVO_EST, "w");
    if (f3 != NULL) {
        for (int i = 0; i < h->cant_est; i++) {
            fprintf(f3, "%d;%d;%d;%d;%.2f;%.2f;%d\n",
                    h->estancias[i].id, h->estancias[i].id_huesped,
                    h->estancias[i].id_habitacion, h->estancias[i].dias,
                    h->estancias[i].cargos_danos, h->estancias[i].total_pagar,
                    h->estancias[i].activa);
        }
        fclose(f3);
    }
    printf("Datos guardados correctamente.\n");
}

/* Cargar datos desde los archivos si existen */
void hotel_cargar(Hotel *h) {
    FILE *f1 = fopen(ARCHIVO_HAB, "r");
    if (f1 != NULL) {
        Habitacion hab;
        while (fscanf(f1, "%d;%d;%f;%d;%d;%d;%d\n",
                      &hab.id, &hab.numero, &hab.tarifa, &hab.tiene_ventanas,
                      &hab.paredes_ectoplasma, &hab.muebles_reforzados, &hab.ocupada) == 7) {
            agregar_habitacion(h, hab);
        }
        fclose(f1);
    }

    FILE *f2 = fopen(ARCHIVO_HUESP, "r");
    if (f2 != NULL) {
        Huesped p;
        while (fscanf(f2, "%d;%d;%d;%f;%49[^\n]\n",
                      &p.id, &p.tipo, &p.luna_llena, &p.danos_totales, p.nombre) == 5) {
            agregar_huesped(h, p);
        }
        fclose(f2);
    }

    FILE *f3 = fopen(ARCHIVO_EST, "r");
    if (f3 != NULL) {
        Estancia e;
        while (fscanf(f3, "%d;%d;%d;%d;%f;%f;%d\n",
                      &e.id, &e.id_huesped, &e.id_habitacion, &e.dias,
                      &e.cargos_danos, &e.total_pagar, &e.activa) == 7) {
            agregar_estancia(h, e);
        }
        fclose(f3);
    }
}

/* Calcula que tan mala es una habitacion para un monstruo */
int calcular_conflicto(Huesped *p, Habitacion *hab) {
    int puntos = 0;

    if (p->tipo == 1 && hab->tiene_ventanas == 1) {
        puntos = puntos + 100; /* Vampiros odian ventanas */
    }
    if (p->tipo == 2 && hab->paredes_ectoplasma == 0) {
        puntos = puntos + 60;  /* Fantasmas atraviesan paredes */
    }
    if (p->tipo == 3 && hab->muebles_reforzados == 0) {
        if (p->luna_llena == 1) {
            puntos = puntos + 90; /* Lobo en luna llena rompe todo */
        } else {
            puntos = puntos + 30;
        }
    }

    return puntos;
}

void accion_agregar_habitacion(Hotel *h) {
    Habitacion hab;

    if (h->cant_hab == 0) {
        hab.id = 1;
    } else {
        hab.id = h->habitaciones[h->cant_hab - 1].id + 1;
    }

    printf("Numero de habitacion: ");
    if (scanf("%d", &hab.numero) != 1 || hab.numero <= 0) {
        printf("Numero invalido.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Tarifa por noche: ");
    if (scanf("%f", &hab.tarifa) != 1 || hab.tarifa < 0) {
        printf("Tarifa invalida.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Tiene ventanas? (1=Si, 0=No): ");
    scanf("%d", &hab.tiene_ventanas);
    if (hab.tiene_ventanas != 0 && hab.tiene_ventanas != 1) {
        printf("Opcion invalida.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Paredes anti-fantasmas? (1=Si, 0=No): ");
    scanf("%d", &hab.paredes_ectoplasma);
    if (hab.paredes_ectoplasma != 0 && hab.paredes_ectoplasma != 1) {
        printf("Opcion invalida.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Muebles reforzados? (1=Si, 0=No): ");
    scanf("%d", &hab.muebles_reforzados);
    if (hab.muebles_reforzados != 0 && hab.muebles_reforzados != 1) {
        printf("Opcion invalida.\n");
        while (getchar() != '\n');
        return;
    }

    hab.ocupada = 0;

    if (agregar_habitacion(h, hab)) {
        printf("Habitacion agregada con exito (ID: %d).\n", hab.id);
    } else {
        printf("Error de memoria.\n");
    }
}

void accion_agregar_huesped(Hotel *h) {
    Huesped p;

    if (h->cant_huesp == 0) {
        p.id = 1;
    } else {
        p.id = h->huespedes[h->cant_huesp - 1].id + 1;
    }
    p.danos_totales = 0;

    printf("Nombre del monstruo: ");
    getchar();
    fgets(p.nombre, MAX_NOMBRE, stdin);
    p.nombre[strcspn(p.nombre, "\n")] = '\0';

    printf("Tipo (1=Vampiro, 2=Fantasma, 3=Hombre Lobo, 4=Otro): ");
    if (scanf("%d", &p.tipo) != 1 || p.tipo < 1 || p.tipo > 4) {
        printf("Tipo invalido.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Hay luna llena o crisis? (1=Si, 0=No): ");
    if (scanf("%d", &p.luna_llena) != 1 || (p.luna_llena != 0 && p.luna_llena != 1)) {
        printf("Opcion invalida.\n");
        while (getchar() != '\n');
        return;
    }

    if (agregar_huesped(h, p)) {
        printf("Huesped agregado con exito (ID: %d).\n", p.id);
    } else {
        printf("Error de memoria.\n");
    }
}

void accion_listar(const Hotel *h) {
    printf("\n--- HABITACIONES ---\n");
    if (h->cant_hab == 0) {
        printf("No hay habitaciones.\n");
    }
    for (int i = 0; i < h->cant_hab; i++) {
        printf("ID:%d | Num:%d | Tarifa:%.2f | Ventanas:%d | Ectoplasma:%d | Reforzados:%d | Ocupada:%d\n",
               h->habitaciones[i].id, h->habitaciones[i].numero, h->habitaciones[i].tarifa,
               h->habitaciones[i].tiene_ventanas, h->habitaciones[i].paredes_ectoplasma,
               h->habitaciones[i].muebles_reforzados, h->habitaciones[i].ocupada);
    }

    printf("\n--- HUESPEDES ---\n");
    if (h->cant_huesp == 0) {
        printf("No hay huespedes.\n");
    }
    for (int i = 0; i < h->cant_huesp; i++) {
        printf("ID:%d | %s | Tipo:%d | Luna llena:%d | Danos:$%.2f\n",
               h->huespedes[i].id, h->huespedes[i].nombre, h->huespedes[i].tipo,
               h->huespedes[i].luna_llena, h->huespedes[i].danos_totales);
    }

    printf("\n--- ESTANCIAS ---\n");
    if (h->cant_est == 0) {
        printf("No hay estancias.\n");
    }
    for (int i = 0; i < h->cant_est; i++) {
        printf("ID:%d | ID Huesped:%d | ID Hab:%d | Dias:%d | Total:$%.2f | Activa:%d\n",
               h->estancias[i].id, h->estancias[i].id_huesped, h->estancias[i].id_habitacion,
               h->estancias[i].dias, h->estancias[i].total_pagar, h->estancias[i].activa);
    }
}

/* Entrada (Check-in) buscando la mejor habitacion */
void accion_entrada(Hotel *h) {
    if (h->cant_hab == 0 || h->cant_huesp == 0) {
        printf("Faltan habitaciones o huespedes registrados.\n");
        return;
    }

    int id_huesp;
    printf("ID del huesped que entra: ");
    if (scanf("%d", &id_huesp) != 1) {
        printf("Entrada invalida.\n");
        while (getchar() != '\n');
        return;
    }

    /* Buscar el huesped con apuntador */
    Huesped *p = NULL;
    for (int i = 0; i < h->cant_huesp; i++) {
        if (h->huespedes[i].id == id_huesp) {
            p = &h->huespedes[i];
            break;
        }
    }

    if (p == NULL) {
        printf("No existe ese huesped.\n");
        return;
    }

    /* Revisar que no este ya hospedado */
    for (int i = 0; i < h->cant_est; i++) {
        if (h->estancias[i].id_huesped == id_huesp && h->estancias[i].activa == 1) {
            printf("Ese huesped ya esta hospedado.\n");
            return;
        }
    }

    /* Buscar la habitacion libre con menor conflicto */
    Habitacion *mejor_hab = NULL;
    int menor_conflicto = 9999;

    for (int i = 0; i < h->cant_hab; i++) {
        if (h->habitaciones[i].ocupada == 0) {
            int c = calcular_conflicto(p, &h->habitaciones[i]);
            if (c < menor_conflicto) {
                menor_conflicto = c;
                mejor_hab = &h->habitaciones[i];
            }
        }
    }

    if (mejor_hab == NULL) {
        printf("No hay habitaciones libres.\n");
        return;
    }

    int dias;
    printf("Cuantos dias se va a quedar?: ");
    if (scanf("%d", &dias) != 1 || dias <= 0) {
        printf("Dias invalidos.\n");
        while (getchar() != '\n');
        return;
    }

    Estancia e;
    if (h->cant_est == 0) {
        e.id = 1;
    } else {
        e.id = h->estancias[h->cant_est - 1].id + 1;
    }
    e.id_huesped = p->id;
    e.id_habitacion = mejor_hab->id;
    e.dias = dias;
    e.cargos_danos = 0;
    e.total_pagar = mejor_hab->tarifa * dias;
    e.activa = 1;

    if (agregar_estancia(h, e)) {
        mejor_hab->ocupada = 1;
        printf("Asignada habitacion %d a %s (Conflicto: %d puntos).\n",
               mejor_hab->numero, p->nombre, menor_conflicto);
        printf("Total estimado: $%.2f\n", e.total_pagar);
    }
}

/* Salida (Check-out) y cobro de danos */
void accion_salida(Hotel *h) {
    int id_est;
    printf("ID de la estancia que termina: ");
    if (scanf("%d", &id_est) != 1) {
        printf("Entrada invalida.\n");
        while (getchar() != '\n');
        return;
    }

    Estancia *est = NULL;
    for (int i = 0; i < h->cant_est; i++) {
        if (h->estancias[i].id == id_est && h->estancias[i].activa == 1) {
            est = &h->estancias[i];
            break;
        }
    }

    if (est == NULL) {
        printf("Estancia activa no encontrada.\n");
        return;
    }

    float danos;
    printf("Costo por danos causados ($0 si no rompio nada): ");
    if (scanf("%f", &danos) != 1 || danos < 0) {
        printf("Monto invalido.\n");
        while (getchar() != '\n');
        return;
    }

    est->cargos_danos = danos;
    est->total_pagar = est->total_pagar + danos;
    est->activa = 0;

    /* Liberar habitacion */
    for (int i = 0; i < h->cant_hab; i++) {
        if (h->habitaciones[i].id == est->id_habitacion) {
            h->habitaciones[i].ocupada = 0;
            break;
        }
    }

    /* Sumar danos al historial del huesped */
    for (int i = 0; i < h->cant_huesp; i++) {
        if (h->huespedes[i].id == est->id_huesped) {
            h->huespedes[i].danos_totales = h->huespedes[i].danos_totales + danos;
            break;
        }
    }

    printf("Salida registrada. Total cobrado: $%.2f\n", est->total_pagar);
}

/* Eliminar huesped sin corromper datos */
void accion_eliminar_huesped(Hotel *h) {
    int id;
    printf("ID del huesped a eliminar: ");
    if (scanf("%d", &id) != 1) {
        printf("Entrada invalida.\n");
        while (getchar() != '\n');
        return;
    }

    int pos = -1;
    for (int i = 0; i < h->cant_huesp; i++) {
        if (h->huespedes[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Huesped no encontrado.\n");
        return;
    }

    /* No dejar eliminar si esta hospedado ahorita */
    for (int i = 0; i < h->cant_est; i++) {
        if (h->estancias[i].id_huesped == id && h->estancias[i].activa == 1) {
            printf("No se puede eliminar: el huesped sigue hospedado.\n");
            return;
        }
    }

    for (int i = pos; i < h->cant_huesp - 1; i++) {
        h->huespedes[i] = h->huespedes[i + 1];
    }
    h->cant_huesp--;
    printf("Huesped eliminado correctamente.\n");
}

/* Reportes pedidos */
void accion_reportes(const Hotel *h) {
    int ocupadas = 0;
    for (int i = 0; i < h->cant_hab; i++) {
        if (h->habitaciones[i].ocupada == 1) {
            ocupadas++;
        }
    }

    float ingresos = 0;
    for (int i = 0; i < h->cant_est; i++) {
        if (h->estancias[i].activa == 0) {
            ingresos = ingresos + h->estancias[i].total_pagar;
        }
    }

    printf("\n--- REPORTES ---\n");
    printf("Ocupacion: %d de %d habitaciones ocupadas.\n", ocupadas, h->cant_hab);
    printf("Ingresos totales: $%.2f\n", ingresos);
    printf("Huespedes problematicos:\n");

    int hay = 0;
    for (int i = 0; i < h->cant_huesp; i++) {
        if (h->huespedes[i].danos_totales > 0) {
            printf("- %s (Danos acumulados: $%.2f)\n",
                   h->huespedes[i].nombre, h->huespedes[i].danos_totales);
            hay = 1;
        }
    }
    if (hay == 0) {
        printf("Ninguno por ahora.\n");
    }
}

int main(void) {
    Hotel *hotel = hotel_crear(2);
    if (hotel == NULL) {
        printf("Error al reservar memoria inicial.\n");
        return 1;
    }

    hotel_cargar(hotel);

    int opcion = -1;
    while (opcion != 0) {
        printf("\n1) Agregar Hab  2) Agregar Huesped  3) Entrada  4) Salida\n");
        printf("5) Listar       6) Reportes         7) Eliminar 0) Guardar y salir\n> ");
        
        if (scanf("%d", &opcion) != 1) {
            printf("Opcion no valida.\n");
            while (getchar() != '\n');
            opcion = -1;
            continue;
        }

        switch (opcion) {
            case 1: accion_agregar_habitacion(hotel); break;
            case 2: accion_agregar_huesped(hotel); break;
            case 3: accion_entrada(hotel); break;
            case 4: accion_salida(hotel); break;
            case 5: accion_listar(hotel); break;
            case 6: accion_reportes(hotel); break;
            case 7: accion_eliminar_huesped(hotel); break;
            case 0: hotel_guardar(hotel); break;
            default: printf("Opcion no valida.\n");
        }
    }

    hotel_destruir(hotel);
    return 0;
}