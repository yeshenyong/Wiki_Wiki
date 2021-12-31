class Solution {
    class Pri_Queue_Comp{
    public:
        bool operator() (const pair<int, int>& lhs, const pair<int, int>& rhs) {
            return lhs.second > rhs.second;
        }
    };
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        /*--------------method 1:哈希表 + 优先队列 + 仿函数/结构体重写------------*/
        unordered_map<int, int> mapp;
        priority_queue<pair<int, int>, vector<pair<int, int>>, Pri_Queue_Comp> que;
        vector<int> result;
        for(int &it:nums){
            mapp[it]++;
        }
        for(auto &it:mapp){
            que.push(it);
            if (que.size() > k)que.pop();
        }
        while(k--){
            result.push_back(que.top().first);
            que.pop();
        }
        return result;
    }
};