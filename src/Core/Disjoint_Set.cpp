#include"Disjoint_Set.h"

Disjoint_Set::Disjoint_Set() {
    parent.push_back(0);
    rank.push_back(0);
}

unsigned int Disjoint_Set::find_set(unsigned int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v]);
}

void Disjoint_Set::make_set(unsigned int v) {
    //This will only work if v starts with one and is increased by one each call. Which it is in our case
    parent.push_back(v);
    rank.push_back(0);
}

void Disjoint_Set::union_sets(unsigned int a, unsigned int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank[a] < rank[b])
            swap(a, b);
        parent[b] = a;
        if (rank[a] <= rank[b])
            rank[a]++;
    }
}

