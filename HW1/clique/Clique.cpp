#include <iostream>
#include <vector>

using namespace std;

int maxCliqueSize = 0;

bool isClique(vector<int>& clique, vector<vector<int> >& graph) {
    for (int i = 0; i < clique.size(); i++) {
        for (int j = i + 1; j < clique.size(); j++) {
            if (!graph[clique[i]][clique[j]]) {
                return false;
            }
        }
    }
    return true;
}

void findMaxClique(vector<vector<int> >& graph, vector<int>& clique, int currNode) {
    if (currNode == graph.size()) {
        if (isClique(clique, graph) && clique.size() > maxCliqueSize) {
            maxCliqueSize = clique.size();
        }
        return;
    }
    // include the current node in the clique
    clique.push_back(currNode);
    findMaxClique(graph, clique, currNode + 1);
    clique.pop_back();
    // exclude the current node from the clique
    findMaxClique(graph, clique, currNode + 1);
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int> > graph(n, vector<int>(n));
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u][v] = 1;
        graph[v][u] = 1;
    }
    vector<int> clique;
    findMaxClique(graph, clique, 0);
    cout << "Maximum clique size: " << maxCliqueSize << endl;
    return 0;
}
