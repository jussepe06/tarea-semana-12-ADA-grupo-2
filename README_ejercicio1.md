# Problema de la Máxima Ganancia sin Proyectos Adyacentes

## 1. Diseño del Algoritmo

### Contexto y Restricción Operativa
Tenemos una lista de n proyectos en orden, donde cada proyecto i (desde 0 hasta n - 1) nos da una ganancia positiva (proyectos[i] > 0). La meta es elegir proyectos de manera que obtengamos la mayor ganancia total posible. La única condición es que no podemos elegir dos proyectos que estén juntos (adyacentes), porque en la empresa comparten los mismos recursos de personal y tecnología.

### Enfoque: Programación Dinámica
Para resolver esto de forma eficiente usamos Programación Dinámica con un enfoque iterativo (de abajo hacia arriba):

1. **Subestructura Óptima:** Para saber la mejor ganancia posible hasta el proyecto i, nos apoyamos directamente en las mejores decisiones que ya calculamos para los proyectos anteriores (hasta i - 1 y hasta i - 2).
2. **Subproblemas Superpuestos:** Si intentáramos probar todas las combinaciones posibles de manera recursiva, repetiríamos los mismos cálculos una y otra vez (llegando a una complejidad exponencial de O(2^n)). Con programación dinámica guardamos o reutilizamos las respuestas previas, resolviendo cada paso una sola vez en tiempo O(1).

---

## 2. Relación de Recurrencia

### Definición del Estado
Llamamos dp[i] a la máxima ganancia acumulada que podemos conseguir considerando únicamente los proyectos desde el inicio (índice 0) hasta el proyecto actual (índice i).

### Opciones en cada paso
Cuando estamos parados en el proyecto i (para i >= 2), solo tenemos dos caminos posibles:

1. **No elegir el proyecto i:**  
   Si decidimos no tomarlo, no sumamos su ganancia, pero tenemos la libertad de haber tomado el proyecto anterior. Así que la ganancia acumulada es simplemente la misma que ya teníamos hasta el proyecto anterior:  
   Ganancia = dp[i - 1]

2. **Elegir el proyecto i:**  
   Si lo tomamos, sumamos su ganancia (proyectos[i]). Pero como no podemos tomar dos consecutivos, no podemos usar el proyecto i - 1; entonces le sumamos la mejor ganancia que traíamos hasta dos proyectos atrás (i - 2):  
   Ganancia = proyectos[i] + dp[i - 2]

### Ecuación de Transición
Como buscamos maximizar la ganancia, en cada paso tomamos el valor más alto entre esas dos opciones:

dp[i] = max(dp[i - 1], proyectos[i] + dp[i - 2])

### Casos Base
Para que el algoritmo pueda arrancar, definimos los casos iniciales:
- **Sin proyectos (n = 0):** La ganancia es 0.
- **Un solo proyecto (i = 0):** Como solo hay uno y las ganancias son positivas, lo elegimos directamente:  
  dp[0] = proyectos[0]
- **Dos proyectos (i = 1):** Como están juntos, solo podemos quedarnos con uno de ellos. Elegimos el que dé más ganancia:  
  dp[1] = max(proyectos[0], proyectos[1])

---

## 3. ¿Cómo se optimizó este algoritmo para que no use un arreglo dp, sino solo dos variables auxiliares?

### Dependencia de Estados
Normalmente, al implementar esto se crea un vector o arreglo completo de tamaño n (`vector<int> dp(n)`). Sin embargo, si miramos con atención la fórmula:

dp[i] = max(dp[i - 1], proyectos[i] + dp[i - 2])

nos damos cuenta de que para calcular el estado actual dp[i] únicamente necesitamos dos valores: el que está justo antes (dp[i - 1]) y el que está dos lugares antes (dp[i - 2]). Los resultados más antiguos (como dp[i - 3], dp[i - 4], etc.) ya no se vuelven a consultar nunca más.

### El concepto de "Ventana Móvil"
En lugar de guardar en memoria todo el historial con un arreglo de tamaño n, podemos usar una ventana móvil de tamaño 2 que solo recuerde los dos últimos valores calculados a medida que avanzamos. Para esto nos bastan dos variables simples:

- `prev2`: guarda el valor de dp[i - 2] (dos posiciones atrás).
- `prev1`: guarda el valor de dp[i - 1] (una posición atrás).

#### ¿Cómo se mueve la ventana en cada paso?
1. **Inicio:**
   - `prev2 = proyectos[0]` (representa dp[0])
   - `prev1 = max(proyectos[0], proyectos[1])` (representa dp[1])

2. **Iteración (desde i = 2 hasta n - 1):**
   - Calculamos el valor actual: `actual = max(prev1, proyectos[i] + prev2)`
   - Desplazamos la ventana un lugar a la derecha:
     - `prev2 = prev1` (lo que estaba una posición atrás pasa a estar dos atrás)
     - `prev1 = actual` (el valor que acabamos de calcular pasa a ser el anterior para la siguiente vuelta)

### Por qué la memoria pasa de O(n) a O(1)
- **Con arreglo dp:** la memoria que gastamos crece con n. Si tuviéramos un millón de proyectos, el arreglo ocuparía varios megabytes en memoria.
- **Con las dos variables auxiliares:** sin importar si tenemos 6 proyectos o un millón, el algoritmo solo usa un par de variables locales en memoria (`prev2`, `prev1` y `actual`). Por eso el uso de memoria adicional es constante: pasa de O(n) a O(1).

---

## 4. Prueba paso a paso con el ejemplo

Para el ejemplo del ejercicio: `proyectos = {5, 5, 10, 100, 10, 5}` con n = 6

| Paso (i) | proyectos[i] | prev2 (dp[i-2]) | prev1 (dp[i-1]) | Cálculo de actual: max(prev1, proyectos[i] + prev2) | Nuevo prev2 | Nuevo prev1 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Bases** (0, 1) | 5, 5 | — | — | dp[0] = 5, dp[1] = max(5, 5) = 5 | 5 | 5 |
| i = 2 | 10 | 5 | 5 | max(5, 10 + 5) = 15 | 5 | 15 |
| i = 3 | 100 | 5 | 15 | max(15, 100 + 5) = 105 | 15 | 105 |
| i = 4 | 10 | 15 | 105 | max(105, 10 + 15) = 105 | 105 | 105 |
| i = 5 | 5 | 105 | 105 | max(105, 5 + 105) = 110 | 105 | 110 |

Al terminar el recorrido, `prev1` queda con el resultado final: **110** (que corresponde a sumar los proyectos en los índices 0, 3 y 5: 5 + 100 + 5 = 110).

---

## 5. Complejidad

- **Tiempo:** O(n), porque solo hacemos un bucle que recorre la lista una sola vez desde i = 2 hasta n - 1. En cada iteración solo hacemos operaciones elementales (una suma y una comparación max).
- **Espacio:** O(1), porque no creamos ningún arreglo adicional y solo usamos un número fijo de variables auxiliares en memoria.

---

## 6. Conclusiones

1. **Modelado con Programación Dinámica:** En problemas donde no podemos elegir elementos consecutivos, la decisión en cada paso se reduce a dos opciones claras: tomar el elemento actual (sumándolo al resultado de hace dos posiciones) o no tomarlo (manteniendo el acumulado de la posición anterior).
2. **Eficiencia frente a la fuerza bruta:** La formulación iterativa resuelve cada subproblema en orden y una sola vez, logrando un tiempo lineal O(n) y evitando la explosión combinatoria O(2^n) de la recursión simple.
3. **Optimización con ventana móvil:** Como cada estado solo depende de los dos resultados inmediatamente anteriores, no hace falta guardar todo el arreglo en memoria. Con solo dos variables podemos llevar la cuenta, reduciendo el espacio de O(n) a O(1).
