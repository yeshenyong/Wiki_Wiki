class Solution {
public:
    vector<vector<int>> result;
    void dfs(const vector<int>& nums, int index, vector<int>& cnt) {
        result.push_back(cnt);
        if (nums.size() <= index) {
            return;
        }
        for (int i=index;i < nums.size();i++) {
            cnt.push_back(nums[i]);
            if ((i==0) || (nums[i] == nums[i-1] && i-1 < index) || (nums[i] != nums[i-1])) {
                dfs(nums, i+1, cnt);
            }
            cnt.pop_back();
        }
    }
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        /*----method 1:排序+回溯----*/
        sort(nums.begin(), nums.end());
        vector<int> cnt;
        dfs(nums, 0, cnt);
        return result;
    }
};