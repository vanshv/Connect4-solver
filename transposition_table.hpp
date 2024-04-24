#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include <bits/stdc++.h>

using namespace std;

class TranspositionTable
{
public:
    struct KeyVal
    {
        uint64_t key : 56;
        uint8_t val;
    };

    vector<KeyVal> Table;
    int n;

    TranspositionTable(int n)
    {
        Table.resize(n);
        this->n = n;
    }

    void reset()
    {
        Table.clear();
    }

    void put(uint64_t key, uint8_t val)
    {
        int index = key % n;
        Table[index].key = key;
        Table[index].val = val;
    }

    uint8_t get(uint64_t key)
    {
        int index = key % n;
        if (Table[index].key == key)
        {
            return Table[index].val;
        }
        return 0;
    }
};

#endif