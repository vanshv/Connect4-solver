#ifndef HLPER_HPP
#define HLPER_HPP

#include <bits/stdc++.h>
#include "position.hpp"
using namespace std;

vector<string> vec;
unordered_map<string, int> mp;

void getSequences(int n)
{
    if (n == 0)
    {
        vec.push_back("");
        return;
    }
    getSequences(n - 1);
    vector<string> newvec;
    for (int i = 0; i < vec.size(); i++)
    {
        string newstr = vec[i] + to_string(n - 1);
        for (int k = 0; k < newstr.size(); k++)
        {
            swap(newstr[k], newstr[newstr.size() - 1]);
            newvec.push_back(newstr);
            swap(newstr[k], newstr[newstr.size() - 1]);
        }
    }
    vec = newvec;
}

// [lo, hi)
void genStrings(int lo, int hi)
{
    vector<string> vec;
    while (vec.size() < 1000)
    {
        string l = "";
        int len = lo + (rand() % (hi - lo));
        for (int j = 0; j < len; j++)
        {
            l += rand() % 7 + '1';
        }
        Position P;
        if (P.playSeq(l) == l.size())
        {
            vec.push_back(l);
        }
    }
    sort(vec.begin(), vec.end(),
         [](string a, string b)
         {
             return a.size() > b.size();
         });
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << "\n";
    }
}

// void findOrdering()
// {
//     long long minops = LLONG_MAX;
//     int saved = -1;
//     getSequences(7);
//     sort(vec.begin(), vec.end());
//     cout << vec.size() << " ";
//     for (int s = 0; s < 5040; s++)
//     {
//         string seq = vec[s];
//         reverse(seq.begin(), seq.end());
//         if(mp.find(seq) != mp.end()){
//             continue;
//         }
//         long long curops = 0;
//         bool broke = false;
//         Solver solver;
//         string line;
//         for (int i = 1; i <= 100; i++)
//         {
//             cin >> line;
//             if (broke){
//                 continue;
//             }

//             Position P;
//             P.playSeq(line);
//             solver.reset();
//             int score = solver.solve(P, s);
//             curops += solver.nodeCount;
//             if(curops > minops){
//                 broke = true;
//             }
//         }
//         if (curops < minops){
//             minops = curops;
//             saved = s;
//         }
//         mp[vec[s]] = 1;
//         cout << s << " " << saved << " " << vec[saved] << "\n";
//     }
// }

#endif