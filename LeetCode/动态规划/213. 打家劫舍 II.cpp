class Solution {
public:
    int rob(vector<int>& nums) {
        /*----method 1:动态规划 + 环形多走一步----*/
        if (nums.size() == 1) return nums[0];
        return max(getMaxn(nums, 0, nums.size()-1), getMaxn(nums, 1, nums.size()));
    }
    /* dp[i] = max (dp[i-2], dp[i-3]) + nums[i] */
    int getMaxn(const vector<int>& nums, int begin, int end) {
        int maxn = 0;
        vector<int> dp(nums.size()+3, 0);
        for (int i=begin+3;i < end+3;i++) {
            dp[i] = max(dp[i-2], dp[i-3]) + nums[i-3]; 
            maxn = dp[i] > maxn?dp[i]:maxn;
        }
        return maxn;
    }

};