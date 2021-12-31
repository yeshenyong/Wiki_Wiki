class Solution {
public:

    vector<vector<int>> vec;
    vector<int> cnt;
    void dfs(vector<int>& candidates, int target, int sum, int index) {
        if (sum == target) {
            vec.push_back(cnt);
            return;
        }

        for (int i=index;i < candidates.size();i++) {
            if (sum + candidates[i] > target)continue;
            cnt.push_back(candidates[i]);
            dfs(candidates, target, sum+candidates[i], i);
            cnt.pop_back();
        }

    }

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        /*---------method 1:dfs + 开头----------*/
        dfs(candidates, target, 0, 0);
        return vec;
    }
};