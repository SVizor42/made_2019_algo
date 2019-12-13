/*

11_1. Цикл минимальной длины
Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.
Ввод: v: кол-во вершин (макс. 50000), n: кол-во ребер (макс. 200000), n пар реберных вершин.
Вывод: одно целое число равное длине минимального цикла. Если цикла нет, то вывести -1.

*/

#include <iostream>
#include <queue>
#include <vector>
#include <cmath>

int get_min_cycle(const std::vector< std::vector<size_t> >& graph) {
    size_t min_cycle = 0;
    for (size_t i = 0; i < graph.size(); i++) {
        
        std::vector<char> color(graph.size(), 0);
        std::vector<size_t> dist(graph.size(), 0);
        std::vector<size_t> parents(graph.size(), 0);        
        size_t cycle = 0;
        
        std::queue<size_t> vertex_queue;
        vertex_queue.push(i);
        color[i] = 1;
        
        bool flag = 0;
        while (!vertex_queue.empty()) {
            size_t parent = vertex_queue.front();
            vertex_queue.pop();
            for (auto child : graph[parent]) {
                if (color[child] && parents[parent] == child)
                    continue;
                
                if (color[child]) {
                    cycle = dist[parent] + dist[child] + 1;
                    flag = 1;
                    break;
                }
                
                vertex_queue.push(child);
                color[child] = 1;
                dist[child] = dist[parent] + 1;
                parents[child] = parent;
            }
            if (flag) 
                break;
        }

        if (min_cycle) {
            if (cycle)
                min_cycle = std::min(min_cycle, cycle);
        }
        else {
            min_cycle = cycle;
        }
    }
            
    return (min_cycle ? min_cycle : -1);
}

int main() {
    // number of vertices & edges
    size_t v = 0, n = 0;
    std::cin >> v >> n;
    std::vector< std::vector<size_t> > graph(v);
    size_t from = 0, to = 0;
    for (size_t i = 0; i < n; i++) {
        std::cin >> from >> to;
        graph[from].push_back(to);
        graph[to].push_back(from);
    }    
    
    std::cout << get_min_cycle(graph) << std::endl;
        
    return 0;
}
