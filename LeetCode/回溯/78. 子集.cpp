class Solution {
public:

    vector<vector<int>> vec;

    void dfs(const vector<int>& nums, const int length, int index, vector<int>& cnt) {
        vec.push_back(cnt);
        if (index == length)return;
        for (int i=index;i < length;i++) {
            cnt.push_back(nums[i]);
            dfs(nums, length, i+1, cnt);
            cnt.pop_back();
        }
    }


    vector<vector<int>> subsets(vector<int>& nums) {
        /*------method 1:普通回溯-------*/
        vector<int> cnt;
        dfs(nums, nums.size(), 0, cnt);
        return vec;
    }
};