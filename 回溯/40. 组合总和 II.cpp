class Solution {
public:

    vector<vector<int>> vec;

    void dfs(const vector<int>& candidates, const int& target, int sum, vector<int>& cnt, int index) {
        if (target < sum)return;
        if (target == sum) {
            vec.push_back(cnt);
        }
        for (int i=index;i < candidates.size() && sum <= target;i++) {
            if (i > index && candidates[i] == candidates[i-1])continue;
            cnt.push_back(candidates[i]);
            dfs(candidates, target, sum+candidates[i], cnt, i+1);
            cnt.pop_back();
        }
    }

    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        /*----------method 1:dfs + 去重----------*/
        sort(candidates.begin(), candidates.end());
        vector<int> cnt;
        dfs(candidates, target, 0, cnt, 0);
        return vec;
    }
};