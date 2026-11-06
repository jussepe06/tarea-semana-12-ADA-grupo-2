# Problema de la Máxima Ganancia sin Proyectos Adyacentes

## 1. Diseño del Algoritmo

### Contexto del Problema
El problema nos da una lista de n proyectos, donde cada proyecto tiene una ganancia positiva (proyectos[i] > 0). Lo que nos piden es elegir qué proyectos realizar para conseguir la mayor ganancia total posible.

La condición que nos ponen es que no podemos elegir dos proyectos que estén uno al lado del otro (adyacentes), porque en la empresa compartirían al personal y los equipos técnicos.

### ¿Por qué lo resolvemos con Programación Dinámica?
Para resolver esto de forma eficiente conviene usar Programación Dinámica con un enfoque iterativo (de abajo hacia arriba):

1. Subestructura Óptima: Para saber la mejor ganancia posible hasta el proyecto i, nos apoyamos directamente en las mejores respuestas de los proyectos anteriores (hasta i - 1 y hasta i - 2).
2. Subproblemas Superpuestos: Si intentáramos probar todas las combinaciones con recursión simple, estaríamos recalculando lo mismo una y otra vez, tardando demasiado tiempo: O(2^n). Con programación dinámica vamos calculando en orden de izquierda a derecha y cada paso se resuelve de una sola vez en tiempo O(1).

---

## 2. Relación de Recurrencia

### Definición del Estado
Llamamos dp[i] a la máxima ganancia acumulada que podemos conseguir considerando los proyectos desde el inicio (índice 0) hasta el proyecto actual i.

### Decisiones en cada paso
Cuando estamos parados en el proyecto i (para i >= 2), solo tenemos dos opciones:

1. No elegir el proyecto i:
   No sumamos su ganancia, lo que nos permite quedarnos con la mejor ganancia que ya teníamos hasta el proyecto anterior:
   Ganancia si no se elige = dp[i - 1]

2. Elegir el proyecto i:
   Sumamos la ganancia de este proyecto (proyectos[i]). Como la regla nos prohíbe elegir el anterior (i - 1), le sumamos la mejor ganancia que traíamos hasta dos proyectos atrás (i - 2):
   Ganancia si se elige = proyectos[i] + dp[i - 2]

### Ecuación de transición
Como queremos sacar la mayor ganancia posible, en cada paso nos quedamos con el máximo entre las dos alternativas:

dp[i] = max(dp[i - 1], proyectos[i] + dp[i - 2])

### Casos Base
Para que el algoritmo pueda arrancar, definimos los casos iniciales:
- Si no hay proyectos (n = 0): la ganancia es 0.
- Si hay 1 solo proyecto (i = 0): como solo hay uno y las ganancias son positivas, lo tomamos directo:
  dp[0] = proyectos[0]
- Si hay 2 proyectos (i = 1): como son vecinos directos, solo podemos escoger uno. Nos quedamos con el que tenga mayor valor:
  dp[1] = max(proyectos[0], proyectos[1])

---

## 3. ¿Cómo se optimizó este algoritmo para que no use un arreglo dp, sino solo dos variables auxiliares?

### Dependencia de los estados
En la versión típica de este ejercicio se suele crear un arreglo o vector completo de tamaño n (vector<int> dp(n)). Pero si revisamos bien la fórmula:

dp[i] = max(dp[i - 1], proyectos[i] + dp[i - 2])

nos damos cuenta de que para calcular el valor actual de dp[i] únicamente necesitamos saber dos datos:
- El resultado inmediatamente anterior: dp[i - 1]
- El resultado de hace dos posiciones: dp[i - 2]

Los valores más viejos (como dp[i - 3], dp[i - 4], etc.) ya no se vuelven a consultar nunca más.

### El concepto de "Ventana Móvil"
Como solo nos importan los dos últimos valores calculados, no hace falta guardar todo el historial en un arreglo. En vez de eso, usamos una ventana móvil de tamaño 2 que se va moviendo hacia adelante usando solo dos variables:

- prev2: guarda el valor de dp[i - 2] (dos posiciones atrás).
- prev1: guarda el valor de dp[i - 1] (una posición atrás).

#### ¿Cómo se mueve la ventana en cada vuelta?
1. Inicio:
   - prev2 = proyectos[0] (equivale a dp[0])
   - prev1 = max(proyectos[0], proyectos[1]) (equivale a dp[1])

2. Iteración (desde i = 2 hasta n - 1):
   - Calculamos el valor actual: actual = max(prev1, proyectos[i] + prev2)
   - Movemos la ventana un paso a la derecha para la siguiente vuelta:
     - prev2 = prev1 (lo que estaba una posición atrás ahora pasa a ser el de hace dos posiciones)
     - prev1 = actual (el valor recién calculado pasa a ser el anterior)

### ¿Por qué la memoria pasa de O(n) a O(1)?
- Con arreglo dp: la memoria que gastamos va creciendo según cuántos proyectos tengamos. Si tuviéramos un millón de proyectos, tendríamos que apartar un arreglo con un millón de enteros en memoria (complejidad O(n)).
- Con las dos variables: no importa si tenemos 6 proyectos o un millón, el código solo usa las variables prev2, prev1 y actual en la memoria local. Como el espacio ya no depende de n, el consumo de memoria es constante: O(1).

---

## 4. Prueba paso a paso con el ejemplo

Hacemos la prueba con el ejemplo del ejercicio: proyectos = {5, 5, 10, 100, 10, 5} con n = 6:

| Paso (i) | proyectos[i] | prev2 (dp[i-2]) | prev1 (dp[i-1]) | Cálculo de actual: max(prev1, proyectos[i] + prev2) | Nuevo prev2 | Nuevo prev1 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Casos Base (0, 1) | 5, 5 | — | — | dp[0] = 5, dp[1] = max(5, 5) = 5 | 5 | 5 |
| i = 2 | 10 | 5 | 5 | max(5, 10 + 5) = 15 | 5 | 15 |
| i = 3 | 100 | 5 | 15 | max(15, 100 + 5) = 105 | 15 | 105 |
| i = 4 | 10 | 15 | 105 | max(105, 10 + 15) = 105 | 105 | 105 |
| i = 5 | 5 | 105 | 105 | max(105, 5 + 105) = 110 | 105 | 110 |

Al terminar el recorrido, la variable prev1 queda con el resultado final: 110 (que viene de elegir los proyectos en los índices 0, 3 y 5: 5 + 100 + 5 = 110).

---

## 5. Complejidad

- Tiempo: O(n), porque solo recorremos la lista una sola vez con un bucle desde 2 hasta n - 1. En cada iteración solo hacemos una suma y una comparación simple.
- Espacio: O(1), porque no creamos ningún arreglo adicional y solo usamos un par de variables en memoria.

---

## 6. Conclusiones

1. Cómo resolver problemas con restricciones adyacentes: Cuando la regla dice que no podemos elegir elementos seguidos, en cada paso la decisión se resume en dos caminos: o nos quedamos con el acumulado anterior, o tomamos el valor actual y le sumamos lo que teníamos dos pasos atrás.
2. Ahorro de tiempo con Programación Dinámica: Al ir resolviendo los subproblemas paso a paso de forma iterativa, pasamos de un tiempo exponencial O(2^n) a un tiempo lineal O(n), evitando calcular cosas repetidas.
3. Optimización de memoria con ventana móvil: No siempre hace falta guardar una tabla entera para programación dinámica. Si la fórmula solo necesita los dos últimos datos calculados, basta con dos variables para tener un algoritmo que use espacio constante O(1).
