#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int maximaGananciaNoAdyacente(const vector<int>& proyectos) {
    int n = static_cast<int>(proyectos.size());
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return proyectos[0];
    }

    int prev2 = proyectos[0];
    int prev1 = max(proyectos[0], proyectos[1]);

    for (int i = 2; i < n; ++i) {
        int actual = max(prev1, proyectos[i] + prev2);
        prev2 = prev1;
        prev1 = actual;
    }

    return prev1;
}

int main() {
    int n;
    if (cin >> n) {
        vector<int> proyectos(n);
        for (int i = 0; i < n; ++i) {
            cin >> proyectos[i];
        }
        cout << maximaGananciaNoAdyacente(proyectos) << endl;
    }
    return 0;
}
