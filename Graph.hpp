#ifndef _GRAPH_HPP_ 
#define _GRAPH_HPP_

using namespace std;

#include <list>
#include <ostream>
#include <vector>
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>
#include <fstream>
#include <stack>
#include <climits>
#include <numeric>
#include <functional>

template<typename T>
struct Edge {
  T from;
  T to;
  int dist;
  Edge(T f, T t, int d): from(f), to(t), dist(d) {
  }
  bool operator<(const Edge<T>& e) const;
  bool operator>(const Edge<T>& e) const;
  template<typename U>
  friend std::ostream& operator<<(std::ostream& out, const Edge<U>& e);
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Edge<T>& e) {
  out << e.from << " -- " << e.to << " (" << e.dist << ")";
  return out;
}

template<typename T>
bool Edge<T>::operator<(const Edge<T>& e) const{
  return dist < e.dist;
}
template<typename T>
bool Edge<T>::operator>(const Edge<T>& e) const{
  return dist > e.dist;
}

template <typename T>
class Graph {

private:
  std::vector<std::pair<T, std::vector<Edge<T>>>> adjList;
  bool isDirected;
  
public:
  Graph(bool isDirectedGraph = true, int capacity = 2) : isDirected(isDirectedGraph) {
    adjList.reserve(capacity); 
  }

  // Check if there is "info" in the graph
  bool contains(const T& info){
    for(const auto& vertex: adjList){
      if(vertex.first == info)
        return true;
    }
    return false;
  }
  // Add a vertex with "info" in the graph
  bool addVtx(const T& info){
    if(contains(info))
      return false;

    std::pair<T, std::vector<Edge<T>>> newVertex(info, std::vector<Edge<T>>());
    adjList.push_back(newVertex);

    return true;
  }

  // Remove a vertex with "info" from the graph
  bool rmvVtx(const T& info){
    int i;
    if(!contains(info))
      return false;

    for (i = 0; i < static_cast<int>(adjList.size()); ++i) {
      if(adjList[i].first == info){
        auto it = adjList[i].second.begin();
        auto it2 = adjList[i].second.end();
        adjList[i].second.erase(it,it2);
        break;
      }
    }
    auto it = adjList.begin() + i;
    adjList.erase(it);
    return true;
  }

  // Add an Edge with "distance" in the graph
  bool addEdg(const T& from, const T& to, int distance){
    int fromPos = -1, toPos = -1;
  
    if(!contains(from) || !contains(to))
      return false;

    for (int i = 0; i < static_cast<int>(adjList.size()); ++i) {
      if(adjList[i].first == from)
        fromPos = i;

      if(adjList[i].first == to)
        toPos = i;

      if(toPos != -1 && fromPos != -1)
        break;
    }
    for(auto it = adjList[fromPos].second.begin(); it != adjList[fromPos].second.end(); ++it){
      if((*it).to == to){
        return false;
      }
    }
    adjList[fromPos].second.push_back(Edge<T>(adjList[fromPos].first,adjList[toPos].first, distance));
    if(!isDirected)
      adjList[toPos].second.push_back(Edge<T>(adjList[toPos].first,adjList[fromPos].first, distance));
    
    return true;
  }

  // Remove an Edge from the graph
  bool rmvEdg(const T& from, const T& to){
    int fromPos = -1, toPos = -1;
    int check = 0;

    if(!contains(from) || !contains(to))
       return false;
    
    for (int i = 0; i < static_cast<int>(adjList.size()); ++i) {
      if(adjList[i].first == from)
        fromPos = i;

      if(adjList[i].first == to)
        toPos = i;

      if(toPos != -1 && fromPos != -1)
        break;
    }
    for(auto it = adjList[fromPos].second.begin(); it != adjList[fromPos].second.end(); ++it){
      if((*it).to == to){
        adjList[fromPos].second.erase(it);
        check++;
        break;
      }
    }
    if(check == 0)
      return false;

    if(!isDirected){
      for(auto it = adjList[toPos].second.begin(); it != adjList[toPos].second.end(); ++it){
        if((*it).from == from){
          adjList[fromPos].second.erase(it);
          break;
        }
      } 
    }
    return true;
  }

  // Make the bfs algorithm
  std::list<T> bfs(const T& info) const{
    std::list<T> result;
    std::list<T> elements;
    std::vector<bool> visited(adjList.size(), false);
  
    int pos = find(info);
    if (pos == -1) 
      return result;
    
    visited[pos] = true;
    elements.push_back(adjList[pos].first);

    while (!elements.empty()) {
      T currVertex = elements.front();
      elements.pop_front();
      result.push_back(currVertex);
      pos = find(currVertex);
      if (pos == -1) 
        return result;
  
      std::vector<int> neighbors;
      for (const auto& adjVertex : adjList[pos].second) {
        int neighborPos = find(adjVertex.to);
        if (neighborPos != -1 && !visited[neighborPos]) 
            neighbors.push_back(neighborPos);
      }
      std::sort(neighbors.begin(), neighbors.end());

      // Add sorted neighbors to the queue
      for (int neighborPos : neighbors) {
        visited[neighborPos] = true;
        elements.push_back(adjList[neighborPos].first);
      }
    }
    return result;
  }
  // Find the position of "info" in the adjList
  int find(const T& info) const{
    int i;
    for ( i = 0; i < static_cast<int>(adjList.size()); ++i) {
      if(adjList[i].first == info)
        return i;
    }
    return -1;
  }

  // Make the dfs algorithm
  std::list<T> dfs(const T& info) const{
    std::list<T> result;
    int n = adjList.size();
    std::vector<bool> visited(n, false);
    std::stack<T> to_visit;
    std::vector<T> elements;
    
    int startIndex = find(info);
    if (startIndex == -1) 
      return result; 

    to_visit.push(info);

    while (!to_visit.empty()) {
      T node = to_visit.top();
      to_visit.pop();

      int pos = find(node);
      if (pos == -1) 
        continue;
            
      if (!visited[pos]) {
        visited[pos] = true;
        result.push_back(adjList[pos].first);

        std::vector<T> neighbors;
        for (const auto& edge : adjList[pos].second) {
          int neighbIntex = find(edge.to);
          if (neighbIntex != -1 && !visited[neighbIntex]) 
            neighbors.push_back(edge.to);
        }
        std::sort(neighbors.begin(), neighbors.end(), [&](const T& a, const T& b) {
          return find(a) < find(b);
        });

        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) 
          to_visit.push(*it);
      }
      if (to_visit.empty()) {
        for (int i = pos; i < n; ++i) {
          if (!visited[i]) {
            to_visit.push(adjList[i].first);
            cout << adjList[i].first << endl;
            break;
          }
        }
      }
    }
    return result;
  }

  // Make bellman-Ford algorithm
  std::list<T> bellmanFord(const T& from, const T& to) {
    int n = adjList.size();
    std::vector<int> distances(n, INT_MAX);
    std::vector<int> parents(n, -1);
    std::vector<Edge<T>> edges;

    int startIndex = find(from);
    int endIndex = find(to);
    
    if (startIndex == -1 || endIndex == -1) {
      std:: cout <<  "Negative Graph Cycle!";
      return {}; 
    }
    for (const auto& node : adjList) {
      for (const auto& edge : node.second) 
        edges.push_back(edge);
    }
    distances[startIndex] = 0.0;

    for (int i = 0; i < n - 1; ++i) {
       for (const auto& edge : edges) {
        int u = find(edge.from);
        int v = find(edge.to);
        if (u != -1 && v != -1 && distances[u] != INT_MAX && distances[u] + edge.dist < distances[v]) {
          distances[v] = distances[u] + edge.dist;
          parents[v] = u;
        }
      }
    }
    for (const auto& edge : edges) {
      int u = find(edge.from);
      int v = find(edge.to);
      if (u != -1 && v != -1 && distances[u] != INT_MAX && distances[u] + edge.dist < distances[v]) {
        std::cout << "Negative Graph Cycle!";
        return {};
      }
    }
    std::list<T> path;
    for (int at = endIndex; at != -1; at = parents[at]) 
      path.push_front(adjList[at].first);
        
    if (path.front() != from) {
      std::cout << "Negative Graph Cycle!";
      return {}; 
    }
    return path;
  }

  // Make mst algorithm
  std::list<Edge<T>> mst() {
    std::list<Edge<T>> result;
    int n = adjList.size();
    std::vector<int> parent(n);

    std::iota(parent.begin(), parent.end(), 0);

    std::function<int(int)> findSet = [&](int i) {
      if (i != parent[i])
        parent[i] = findSet(parent[i]);
      return parent[i];
    };

    auto unionSets = [&](int u, int v) {
      parent[findSet(u)] = findSet(v);
    };

    std::vector<Edge<T>> edges;
    for (const auto& node : adjList) {
        for (const auto& edge : node.second) 
          edges.push_back(edge);  
    }

    std::sort(edges.begin(), edges.end());

    for (const auto& edge : edges) {
      int u = find(edge.from);
      int v = find(edge.to);

      if (u == -1 || v == -1) continue;

      int setU = findSet(u);
      int setV = findSet(v);

      if (setU != setV && u < v) {
        result.push_back(edge);
        unionSets(u, v);
      }
    }
    return result;
 }

  // Make dijkstra algorithm
  std::list<T> dijkstra(const T& from, const T& to){
    int n = adjList.size();
    std::vector<int> distances(n, INT_MAX);
    std::vector<int> parents(n, -1);
    auto cmp = [&](int left, int right) { return distances[left] > distances[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    int startIndex = find(from);
    int endIndex = find(to);
    if(startIndex == -1 || endIndex == -1) {
      return {}; 
    }
    distances[startIndex] = 0.0;
    pq.push(startIndex);

    while(!pq.empty()) {
      int current = pq.top();
      pq.pop();

      if(current == endIndex) break;

      for(const auto& edge : adjList[current].second) {
        int adjIndex = find(edge.to);
        if(adjIndex == -1) continue;

        double newDist = distances[current] + edge.dist;
        if(newDist < distances[adjIndex]) {
          distances[adjIndex] = newDist;
          parents[adjIndex] = current;
          pq.push(adjIndex);
        }
      }
    }
    std::list<T> path;
    if(distances[endIndex] == INT_MAX) {
      return path;
    }
    for(int at = endIndex; at != startIndex; at = parents[at]) {
      path.push_front(adjList[at].first);
    }
    path.push_front(from);

    return path;
  }
  bool print2DotFile(const char *filename) const{

  std::ofstream dotFile(filename, std::ios::out);

  if(!dotFile.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
  }
  dotFile << "digraph G {" << std::endl;

  for (size_t i = 0; i < adjList.size(); ++i) {
    dotFile << "\"" << adjList[i].first << "\"" << ";" << std::endl;
  }
  for (size_t i = 0; i < adjList.size(); ++i) {
    for (auto edge : adjList[i].second) {
      dotFile << "\"" << edge.from << "\"" << "->" << "\"" << edge.to << "\"" << " ";
      if (edge.dist != 0) { 
        dotFile << "[label=\"" << edge.dist<< "\"]";
      }
      if(!isDirected)
        dotFile << "[dir=none];" << std::endl;
      else
        dotFile << ";" << std::endl;
    }
  }

  dotFile << "}" << std::endl;
  dotFile.close();

  std::cout << "2dot file generated: " << filename << std::endl;
  return true;
}
  
};
#endif
