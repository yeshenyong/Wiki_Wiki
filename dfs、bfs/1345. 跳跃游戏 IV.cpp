class Solution {
public:
    int minJumps(vector<int>& arr) {
        /*---method 1:bfs + 哈希表 + 记录 + 去重 板题---*/
        queue<int> myqueue;
        unordered_map<int, vector<int>> mymap;
        unordered_map<int, bool> valUmp;
        vector<bool> visited(arr.size(), false);
        int minStep = 0;
        for (int i=0;i < arr.size();i++) {
            mymap[arr[i]].emplace_back(i);
        }
        myqueue.emplace(0);
        visited[0] = true;
        while (!myqueue.empty()) {
            int size = myqueue.size();
            // cout << "size = " << size << endl;
            for (int i=0;i < size;i++) {
                auto entryque = myqueue.front();
                myqueue.pop();
                if (entryque == arr.size() - 1) return minStep;
                if (entryque - 1 >= 0 && entryque - 1 < arr.size() && !visited[entryque - 1]) {
                    visited[entryque - 1] = true;
                    myqueue.emplace(entryque - 1);
                }
                if (entryque + 1 < arr.size() && entryque + 1 < arr.size() && !visited[entryque + 1]) {
                    visited[entryque + 1] = true;
                    myqueue.emplace(entryque + 1);
                }
                if (mymap.count(arr[entryque]) == 1 && valUmp.count(arr[entryque]) == 0) {
                    for (int j=mymap[arr[entryque]].size()-1;j >= 0;j--) {
                        if (visited[mymap[arr[entryque]][j]] == false) {
                            visited[mymap[arr[entryque]][j]] = true;
                            myqueue.emplace(mymap[arr[entryque]][j]);
                        }
                    }
                    valUmp[arr[entryque]] = 1;
                }
            }
            minStep++;
        }
        return minStep;
    }
};