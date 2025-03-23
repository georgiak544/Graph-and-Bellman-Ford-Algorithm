#ifndef _GRAPH_UI_
#define _GRAPH_UI_

using namespace std;

template <typename T>
int graphUI() {
  
  std::string option, line;
  int distance = 0;
  bool digraph = false;
  
  std::cin >> option;
  if(!option.compare("digraph"))
    digraph = true;

  Graph<T> g(digraph);
  
  while(true) {
    std::stringstream stream;
    std::cin >> option;
    
    if(!option.compare("av")) {
      getline(std::cin, line);
      stream << line;
      T vtx(stream);

      if(g.addVtx(vtx))
        std::cout << "av " << vtx << " OK\n";
      else
       std:: cout << "av " << vtx << " NOK\n";
    }
    else if(!option.compare("rv")) {
      getline(std::cin, line);
      stream << line;
      T vtx(stream);

      if(g.rmvVtx(vtx))
        std::cout << "rv " << vtx << " OK\n";
      else
        std::cout << "rv " << vtx << " NOK\n";
    }
    else if(!option.compare("ae")) {
      getline(std::cin, line);
      stream << line;
      T from(stream);
      stream << line;
      T to(stream);
      stream << line;
      stream >> distance;

      if(g.addEdg(from,to,distance))
        std::cout << "ae " << from << " " << to << " OK\n";
      else
        std::cout << "ae " << from << " " << to << " NOK\n";
    }
    else if(!option.compare("re")) {
      getline(std::cin, line);
      stream << line;
      T from(stream);
      stream << line;
      T to(stream);
      stream << line;
      stream >> distance;

      if(g.rmvEdg(from, to))
        std::cout << "ae " << from << " " << to << " OK\n";
      else
        std::cout << "ae " << from << " " << to << " NOK\n";
    }
    else if(!option.compare("dot")) {
      const char *filename = "o.dot";
      bool ret = g.print2DotFile(filename);
      if(!ret)
        std::cout << "Error: Could not create dot file." << std::endl;
    }
    else if(!option.compare("bfs")) {
      getline(std::cin, line);
      stream << line;
      T info(stream);

      std::list<T> result = g.bfs(info);

      std::cout << "\n----- BFS Traversal -----\n";
      for (auto it = result.begin(); it != std::prev(result.end()); ++it) {
        std::cout << *it << " -> ";
      }
      if (!result.empty()) 
        std::cout << result.back();
      
      std::cout << "\n-------------------------\n";
    }
    else if(!option.compare("dfs")) {
      getline(std::cin, line);
      stream << line;
      T info(stream);

      std::cout << "\n----- DFS Traversal -----\n";

      std::list<T> result = g.dfs(info);
      for (auto it = result.begin(); it != std::prev(result.end()); ++it) {
        std::cout << *it << " -> ";
      }
      if (!result.empty()) 
        std::cout << result.back();
      
      std::cout << "\n-------------------------\n";
    }
    else if(!option.compare("dijkstra")) {
      getline(std::cin, line);
      stream << line;
      T from(stream);
      T to(stream);

      std::list<T> ret = g.dijkstra(from,to);

      std::cout << "Dijkstra (" << from << " - " << to <<"): ";

      for(auto it = ret.begin(); it != std::prev(ret.end()); ++it) 
        std::cout << *it << ", ";
      
      if(!ret.empty())
        std::cout << ret.back() << std::endl;
      else
        std::cout << std::endl;
    }
    else if(!option.compare("bellman-ford")) {
      getline(std::cin, line);
      stream << line;
      T from(stream);
      T to(stream);

      std::cout << "Bellman-Ford (" << from << " - " << to <<"): ";
      
      std::list<T> rtr = g.bellmanFord(from,to);
      
      for(auto it = rtr.begin(); it != std::prev(rtr.end()); ++it) 
        std::cout << *it << ", ";
      
      if(!rtr.empty()) 
        std::cout << rtr.back() << std::endl;
      
      else
        std::cout << std::endl;
    }
    else if(!option.compare("mst")) {
      std::list<Edge<T>> result = g.mst();
      int dis = 0;

      std::cout << "\n--- Min Spanning Tree ---\n";
     
       for (auto it = result.begin(); it != result.end(); ++it) {
        std::cout << (*it).from << " -- " << (*it).to  << " (" << (*it).dist << ")"<< std::endl;
        dis += (*it).dist;
      }
      std::cout << "MST Cost: " << dis << std::endl;
    }
    else if(!option.compare("q")) {
      std::cerr << "bye bye...\n";
      return 0;
    }
    else if(!option.compare("#")) {
      std::string line;
      getline(std::cin,line);
      std::cerr << "Skipping line: " << line << std::endl;
    }
    else {
      std::cout << "INPUT ERROR\n";
      return -1;
    }
  }
  return -1;  
}

#endif

