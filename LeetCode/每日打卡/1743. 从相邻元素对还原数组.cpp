class Solution {
public:
    vector<int> restoreArray(vector<vector<int>>& adjacentPairs) {
        /*------------
                method 1:本人想法：hashmap记录次数
                由出现一的次数往单边/两边夹(与官方思路一致开心)
        -------*/
        int length = adjacentPairs.size();
        vector<int> Array(length+1);
        unordered_map<int, vector<int>> mymap;
        for (auto &it:adjacentPairs) {
            mymap[it[0]].push_back(it[1]);
            mymap[it[1]].push_back(it[0]);
        }
        int begin;
        for (auto &it:mymap) {
            if (it.second.size() == 1){
                begin = it.first;
                break;
            }
        }
        int pre = -1;
        for(int i=0;i < length+1;i++){
            Array[i] = begin;
            for (auto &it:mymap[begin]) {
                if (it != pre) {
                    pre = begin;
                    begin = it;
                    break;
                }
            }
        }
        return Array;
    }
};