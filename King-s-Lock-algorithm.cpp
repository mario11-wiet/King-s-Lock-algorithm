//
// Created by Mariusz on 08.02.2021.
//

#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>

struct Graph {
    const int n;
    std::vector<std::vector<int>> adj;
    explicit Graph(int _n) : n(_n), adj(_n) {}
    void add_edge(const int src, const int dst) { adj[src].push_back(dst); }
};

struct Data {
    explicit Data(int _s) : size(_s), item(_s) {}
    std::shared_ptr<Data> prev = nullptr, nxt = nullptr;
    int size, size_new = 0;
    std::vector<int> item;
};

std::vector<int> LexBfs(const Graph &g) {

    std::vector<int> order(g.n, -1);
    std::vector<char> visited(g.n, false);
    std::vector<std::pair<std::shared_ptr<Data>, int>> ptr(g.n);
    std::shared_ptr<Data> data = std::make_shared<Data>(g.n);
    for (int i = 0;  i < g.n; ++i) {
        ptr[i] = std::make_pair(data, i);
        data->item[i] = i;
    }

    std::shared_ptr<Data> head(data);
    int size_order = 0;
    while (size_order < g.n) {
        const int pivot = order[size_order++] = head->item[0];
        visited[pivot] = true;
        std::swap(head->item[0], head->item[head->size - 1]);
        std::swap(ptr[head->item[0]].second, ptr[head->item[head->size - 1]].second);
        --(head->size);
        if (head->size == 0) head = head->nxt;

        for (const auto u : g.adj[pivot]) {
            if (visited[u]) continue;

            std::shared_ptr<Data> cur(ptr[u].first);
            if (cur->size == 1) continue;

            const int idx = ptr[u].second;
            std::swap(cur->item[idx], cur->item[cur->size - cur->size_new - 1]);
            std::swap(ptr[u].second, ptr[cur->item[idx]].second);
            ++(cur->size_new);
        }

        for (const auto u : g.adj[pivot]) {
            if (visited[u]) continue;

            std::shared_ptr<Data> cur(ptr[u].first);
            if (cur->size_new == 0 || cur->size == cur->size_new) {
                cur->size_new = 0;
                continue;
            }

            const int size_new = cur->size_new;
            std::shared_ptr<Data> new_data = std::make_shared<Data>(size_new);
            for (int i = 0; i < size_new; ++i) {
                new_data->item[i] = cur->item[cur->size - size_new + i];
                ptr[new_data->item[i]].first = new_data;
                ptr[new_data->item[i]].second = i;
            }

            if (head == cur) head = new_data;

            cur->size -= cur->size_new;
            cur->size_new = 0;
            new_data->prev = cur->prev;
            new_data->nxt = cur;
            if (new_data->prev != nullptr) new_data->prev->nxt = new_data;
            cur->prev = new_data;
        }
    }

    return order;
}

int main() {
    int numberOfSets;

    std::scanf("%d",&numberOfSets);
    while(numberOfSets > 0) {
        int result=2;

        numberOfSets -= 1;
        int n, m, v[2];
        std::scanf("%d %d",&n,&m);
        Graph g(n);
        for (int i = 0; i < m; i++) {
            std::scanf("%d %d",&v[0],&v[1]);
            g.add_edge(v[0]-1, v[1]-1);
            g.add_edge(v[1]-1, v[0]-1);
        }

        auto ord = LexBfs(g);
        for (int i = 3; i < ord.size(); i++) {
            //printf("\n%d\n",ord[i]);
            //printf("%d ",ord[i]);
            int current=0;
            for(int j=0;j<i;j++)
            {
                for(auto &e:g.adj[ord[i]])
                {
                    //printf("%d-%d-%d ",ord[i],e,j);
                    if(e==ord[j])
                    {
                        current++;
                    }
                }
                //printf("\n");
            }
            //printf("%d   ",current);
            if(current>result)
            {
                result=current;
            }
        }
        printf("%d\n",result);
    }
    return 0;
}
