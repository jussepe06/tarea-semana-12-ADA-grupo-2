# Problema de la Máxima Ganancia sin Proyectos Adyacentes

## 1. Diseño del Algoritmo

### Contexto y Restricción Operativa
Dado un conjunto secuencial de $n$ proyectos donde cada proyecto $i \in \{0, 1, \dots, n-1\}$ aporta una ganancia estimada positiva $\text{proyectos}[i] > 0$, se busca determinar el subconjunto de proyectos que maximice la ganancia total acumulada bajo la restricción de **no adyacencia**: si se selecciona el proyecto $i$, no es posible seleccionar los proyectos inmediatos $i-1$ ni $i+1$ debido a que comparten recursos humanos y tecnológicos.

### Paradigma de Diseño: Programación Dinámica
El problema exhibe las dos propiedades fundamentales para aplicar Programación Dinámica (enfoque *Bottom-Up* / tabulación iterativa):

1. **Subestructura Óptima:** La solución óptima para los primeros $i$ proyectos se construye directamente a partir de las soluciones óptimas de subproblemas más pequeños (específicamente, los subproblemas correspondientes a los prefijos hasta $i-1$ e $i-2$).
2. **Superposición de Subproblemas:** Un enfoque recursivo ingenuo recalcularía múltiples veces la ganancia máxima para los mismos prefijos de proyectos (generando un árbol de recursión exponencial de orden $O(2^n)$). Al almacenar o propagar los resultados de estados previos de manera iterativa, cada subproblema se resuelve exactamente una vez en tiempo constante $O(1)$.

---

## 2. Formulación Matemática y Relación de Recurrencia

### Definición del Estado
Sea $dp[i]$ la máxima ganancia total que se puede obtener considerando únicamente el subconjunto de proyectos desde el índice $0$ hasta el índice $i$ (es decir, el prefijo $\text{proyectos}[0 \dots i]$).

### Análisis de Decisiones (Transición de Estado)
Para cada proyecto en la posición $i$ ($i \ge 2$), existen exactamente dos decisiones mutuamente excluyentes:

1. **No seleccionar el proyecto $i$:**  
   No se incorpora la ganancia $\text{proyectos}[i]$, pero tampoco se impone ninguna restricción sobre el proyecto $i-1$. Por lo tanto, la ganancia máxima óptima acumulada es idéntica a la mejor solución obtenida hasta el proyecto anterior:
   $$\text{Ganancia}_{\text{excluir}} = dp[i-1]$$

2. **Seleccionar el proyecto $i$:**  
   Se obtiene la ganancia $\text{proyectos}[i]$. Debido a la restricción de proyectos consecutivos, queda prohibido seleccionar el proyecto $i-1$. En consecuencia, a la ganancia actual se le suma la ganancia máxima óptima acumulada hasta el proyecto $i-2$:
   $$\text{Ganancia}_{\text{incluir}} = \text{proyectos}[i] + dp[i-2]$$

### Ecuación de Recurrencia (Ecuación de Bellman)
Para maximizar el beneficio en el paso $i$, se elige el máximo entre ambas alternativas:

$$dp[i] = \max\big(dp[i-1], \;\text{proyectos}[i] + dp[i-2]\big) \quad \forall \, i \in \{2, \dots, n-1\}$$

### Casos Base
Para detener la recurrencia y cimentar la progresión iterativa, se definen los casos frontera:
- **Arreglo vacío ($n = 0$):** La ganancia máxima es $0$.
- **Un solo proyecto ($i = 0$):** Al ser todas las ganancias positivas y no existir conflicto de adyacencia, la decisión óptima es elegir el único proyecto disponible:
  $$dp[0] = \text{proyectos}[0]$$
- **Dos proyectos ($i = 1$):** Como son adyacentes entre sí, solo es posible financiar uno de los dos. Se escoge aquel con mayor beneficio:
  $$dp[1] = \max(\text{proyectos}[0], \text{proyectos}[1])$$

---

## 3. ¿Cómo se optimizó este algoritmo para que no use un arreglo `dp`, sino solo dos variables auxiliares?

### Análisis de Dependencia de Estados
En la formulación clásica con arreglo dinámico (`vector<int> dp(n)`), se reserva un bloque continuo de memoria de tamaño proporcional a $n$ para guardar todos los estados desde $dp[0]$ hasta $dp[n-1]$. Sin embargo, al observar la ecuación de transición:

$$dp[i] = \max(dp[i-1], \;\text{proyectos}[i] + dp[i-2])$$

se evidencia que el cálculo del estado actual $dp[i]$ **depende única y exclusivamente de los dos estados inmediatamente anteriores**: $dp[i-1]$ y $dp[i-2]$. Una vez calculado $dp[i]$, todos los estados históricos $dp[0], dp[1], \dots, dp[i-3]$ jamás vuelven a ser consultados en las iteraciones futuras.

### El Concepto de "Ventana Móvil" (*Sliding Window*) sobre los Estados
Dado que en cualquier instante de la iteración solo necesitamos recordar los **dos últimos valores calculados**, podemos reemplazar la tabla completa $dp[0 \dots n-1]$ por una **ventana móvil de tamaño fijo 2**, representada mediante dos variables escalares auxiliares:

- `prev2`: Almacena el estado $dp[i-2]$ (la mejor ganancia acumulada dos posiciones atrás).
- `prev1`: Almacena el estado $dp[i-1]$ (la mejor ganancia acumulada una posición atrás).

#### Dinámica de Desplazamiento de la Ventana
1. **Inicialización (Posiciones $0$ y $1$):**
   - `prev2 = proyectos[0]` (equivale a $dp[0]$)
   - `prev1 = max(proyectos[0], proyectos[1])` (equivale a $dp[1]$)

2. **Transición y Avance (Para cada $i$ desde $2$ hasta $n-1$):**
   - Se calcula el nuevo estado óptimo: `actual = max(prev1, proyectos[i] + prev2)` (equivale a $dp[i]$).
   - La ventana se desliza un paso hacia la derecha para preparar la iteración $i+1$:
     - El antiguo $dp[i-1]$ pasa a ser el nuevo $dp[i-2]$: `prev2 = prev1`
     - El recién calculado $dp[i]$ pasa a ser el nuevo $dp[i-1]$: `prev1 = actual`

### Reducción de Memoria de $O(n)$ a $O(1)$
- **Con arreglo `dp` ($O(n)$):** La memoria auxiliar crece linealmente con la cantidad de proyectos $n$. Si $n = 10^7$, el arreglo requiere aproximadamente $40\text{ MB}$ de memoria RAM adicional.
- **Con ventana móvil de dos variables ($O(1)$):** Sin importar si $n = 6$ o $n = 10^9$, el algoritmo utiliza únicamente un número constante de variables enteras en la pila (*stack*): `prev2`, `prev1` y `actual`. El consumo de memoria auxiliar permanece estrictamente constante, logrando complejidad espacial **$O(1)$** mientras conserva intacta la complejidad temporal **$O(n)$**.

---

## 4. Traza de Ejecución Paso a Paso

Para la entrada `proyectos = {5, 5, 10, 100, 10, 5}` ($n = 6$):

| Iteración ($i$) | `proyectos[i]` | `prev2` ($dp[i-2]$) | `prev1` ($dp[i-1]$) | Cálculo de `actual` ($\max(\text{prev1}, \text{proyectos}[i] + \text{prev2})$) | Nuevo `prev2` | Nuevo `prev1` |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Casos Base** ($0, 1$) | $5, 5$ | — | — | $dp[0] = 5$, $dp[1] = \max(5, 5) = 5$ | $5$ | $5$ |
| $i = 2$ | $10$ | $5$ | $5$ | $\max(5, 10 + 5) = \mathbf{15}$ | $5$ | $15$ |
| $i = 3$ | $100$ | $5$ | $15$ | $\max(15, 100 + 5) = \mathbf{105}$ | $15$ | $105$ |
| $i = 4$ | $10$ | $15$ | $105$ | $\max(105, 10 + 15) = \mathbf{105}$ | $105$ | $105$ |
| $i = 5$ | $5$ | $105$ | $105$ | $\max(105, 5 + 105) = \mathbf{110}$ | $105$ | $110$ |

Al finalizar el bucle, `prev1` contiene el valor **`110`**, que corresponde a seleccionar los proyectos en los índices $0, 3 \text{ y } 5$ ($5 + 100 + 5 = 110$).

---

## 5. Complejidad Algorítmica

- **Complejidad Temporal:** $O(n)$, dado que se recorre la secuencia de proyectos una sola vez desde $i = 2$ hasta $n - 1$, realizando operaciones aritméticas y de comparación en tiempo $O(1)$ por cada elemento.
- **Complejidad Espacial:** $O(1)$ de espacio auxiliar, al prescindir de estructuras de datos dinámicas y emplear únicamente variables escalares para mantener la ventana móvil de estados.

---

## 6. Conclusiones de la Sesión Didáctica

1. **Identificación de Subestructura Óptima en Problemas con Restricciones Locales:** Los problemas de optimización combinatoria donde una elección invalida elementos adyacentes pueden modelarse eficazmente mediante Programación Dinámica reduciendo cada etapa a una decisión binaria (incluir el elemento actual sumado al estado $i-2$, o excluirlo conservando el estado $i-1$).
2. **Eliminación de Trabajo Redundante:** A diferencia de la fuerza bruta recursiva que explora todas las combinaciones válidas en tiempo exponencial $O(2^n)$, la formulación iterativa *Bottom-Up* resuelve cada subproblema una única vez, alcanzando una eficiencia temporal óptima de $O(n)$.
3. **Optimización Espacial mediante Ventana Móvil de Estados:** Cuando una relación de recurrencia de orden $k$ depende únicamente de los últimos $k$ estados previos ($k=2$ en este caso), no es necesario almacenar toda la tabla de programación dinámica en memoria; basta con mantener una ventana deslizante de $k$ variables auxiliares para reducir la complejidad espacial de $O(n)$ a $O(1)$.

