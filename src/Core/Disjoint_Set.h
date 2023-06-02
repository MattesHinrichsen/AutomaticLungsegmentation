/**
 * @file Disjoint_Set.h
 * @author https://cp-algorithms.com/data_structures/disjoint_set_union.html and Group 3 (mattes.hinrichsen@rwth-aachen.de)
 * @brief Implementation of a union find data structure.
 * 
 */

#pragma once

#include<vector>
using namespace std;

// Via: https://cp-algorithms.com/data_structures/disjoint_set_union.html

class Disjoint_Set {
    vector<unsigned int> parent;  /** List of the parent of the current index*/
    vector<short> rank; /** List of the rank of the index. Used to minimize height of the tree*/
public:
    /**
     * @brief Construct a new Disjoint_Set object
     * 
     */
    Disjoint_Set() ;

    /**
     * @brief Finds the representative of the specified index
     * 
     * @param v Index
     * @return unsigned int Value of the representative
     */
    unsigned int find_set(unsigned int v) ;

    /**
     * @brief Creates a new entry in the tree
     * 
     * @param v Value of the entry
     */
    void make_set(unsigned int v) ;

    /**
     * @brief Joins to entries into one equivalence class
     * 
     * @param a Index one
     * @param b Index two
     */
    void union_sets(unsigned int a, unsigned int b) ;
};
