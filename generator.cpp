#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> generate_matrix(int n, int max) {
    // n - size of matrix
    // max - max value of element
    srand(time(NULL));
    vector<vector<int>> tab;
    for (int i = 0; i < n; i++) {
        vector<int> row;
        for (int j = 0; j < n; j++) {
            row.push_back(rand() % (max + 1));
        }
        tab.push_back(row);
    }
    return tab;
}

vector<vector<int>> generate_graph(int n, int max) {
    // n - number of vertices
    // max - max value of edge
    vector<vector<int>> tab = generate_matrix(n, max);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            tab[i][j] = tab[j][i];
        }
    }
    if (max == 1) {
        // if it is not a multi-graph then we have to set diagonal to 0
        for (int i = 0; i < n; i++) {
            tab[i][i] = 0;
        }
    }
    return tab;
}

void print_matrix(vector<vector<int>> tab) {
    cout << tab.size() << endl;
    for (int i = 0; i < tab.size(); i++) {
        for (int j = 0; j < tab[i].size(); j++) {
            cout << tab[i][j] << " ";
        }
        cout << endl;
    }
}
int main() {
    int n;
    int max = 1;
    cin >> n;
    cin >> max;
    vector<vector<int>> tab = generate_graph(n, max);

    print_matrix(tab);
    return 0;
}