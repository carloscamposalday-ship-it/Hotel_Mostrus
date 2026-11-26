Hotel Mostrus

Sistema de gestión hotelera en consola desarrollado en **C**, diseñado específicamente para administrar habitaciones, huéspedes monstruosos (vampiros, fantasmas, hombres lobo, entre otros) y estancias, optimizando la asignación de cuartos según el nivel de compatibilidad y riesgo de daños.


Características Principales

* **Gestión Dinámica de Memoria:** Uso de arreglos dinámicos (`malloc`, `realloc`, `free`) que escalan automáticamente según la cantidad de registros.
* **Asignación Inteligente (Check-in):** Evalúa las características de cada habitación frente al tipo de monstruo para minimizar conflictos:
  * **Vampiros:** Penalización alta (`+100 pts`) si la habitación tiene ventanas.
  * **Fantasmas:** Penalización media (`+60 pts`) si no cuenta con paredes de ectoplasma.
  * **Hombres Lobo:** Penalización por falta de muebles reforzados (`+30 pts` normal, `+90 pts` en luna llena/crisis).
* **Control de Estancias y Daños (Check-out):** Cálculo automático de tarifas por noche, registro de cargos adicionales por destrozos e historial acumulado de daños por huésped.
* **Persistencia de Datos:** Guarda y carga automáticamente toda la información en archivos de texto plano delimitados por punto y coma (`;`).
* **Reportes en Tiempo Real:** Consulta de porcentaje de ocupación, ingresos totales recaudados y listado de huéspedes problemáticos.

---

Requisitos y Compilación

Para compilar el proyecto necesitas un compilador de C estándar como `gcc` o `clang`.

1. **Clonar el repositorio:**
   ```bash
   git clone [https://github.com/carloscamposalday-ship-it/Hotel_Mostrus.git](https://github.com/carloscamposalday-ship-it/Hotel_Mostrus.git)
   cd Hotel_Mostrus
