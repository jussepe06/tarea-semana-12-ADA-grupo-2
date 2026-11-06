#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

int maximaGananciaNoAdyacente(const std::vector<int>& proyectos) {
    int n = static_cast<int>(proyectos.size());
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return proyectos[0];
    }

    int prev2 = proyectos[0];
    int prev1 = std::max(proyectos[0], proyectos[1]);

    for (int i = 2; i < n; ++i) {
        int actual = std::max(prev1, proyectos[i] + prev2);
        prev2 = prev1;
        prev1 = actual;
    }

    return prev1;
}

int main() {
    std::vector<int> proyectos = {5, 5, 10, 100, 10, 5};
    int gananciaMaxima = maximaGananciaNoAdyacente(proyectos);
    assert(gananciaMaxima == 110);
    std::cout << gananciaMaxima << std::endl;
    return 0;
}

