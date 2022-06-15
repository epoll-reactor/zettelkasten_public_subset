#include<bits/stdc++.h>

# define INF 0x3f3f3f3f

void AddEdge(std::vector<std::pair<int, int>> Graph[], int U, int V, int Weight) {
  Graph[U].emplace_back(V, Weight);
  Graph[V].emplace_back(U, Weight);
}

void Prim(std::vector<std::pair<int, int>> Graph[], int V) {
  std::priority_queue<
      std::pair<int, int>,
      std::vector<std::pair<int, int>>,
      std::greater<std::pair<int, int>>
  > Queue;

  int Src = 0;

  std::vector<int> Key(V, INF);

  std::vector<int> Parent(V, -1);
  std::vector<bool> Visited(V, false);

  Queue.emplace(0, Src);
  Key[Src] = 0;

  while (!Queue.empty()) {
    int U = Queue.top().second;
    Queue.pop();

    if(Visited[U])
      continue;

    Visited[U] = true;

    for (auto [Vertex, Weight] : Graph[U])
      if (!Visited[Vertex] && Key[Vertex] > Weight) {
        Key[Vertex] = Weight;
        Queue.emplace(Key[Vertex], Vertex);
        Parent[Vertex] = U;
      }
  }

  for (int i = 1; i < V; ++i)
    printf("%d - %d\n", Parent[i], i);
}

int main() {
  int V = 9;
  std::vector<std::pair<int, int>> Graph[V];

  AddEdge(Graph, 0, 1, 4);
  AddEdge(Graph, 0, 7, 8);
  AddEdge(Graph, 1, 2, 8);
  AddEdge(Graph, 1, 7, 11);
  AddEdge(Graph, 2, 3, 7);
  AddEdge(Graph, 2, 8, 2);
  AddEdge(Graph, 2, 5, 4);
  AddEdge(Graph, 3, 4, 9);
  AddEdge(Graph, 3, 5, 14);
  AddEdge(Graph, 4, 5, 10);
  AddEdge(Graph, 5, 6, 2);
  AddEdge(Graph, 6, 7, 1);
  AddEdge(Graph, 6, 8, 6);
  AddEdge(Graph, 7, 8, 7);

  Prim(Graph, V);

  return 0;
}
