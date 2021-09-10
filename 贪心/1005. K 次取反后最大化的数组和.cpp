class Solution {
public:
    int largestSumAfterKNegations(vector<int>& nums, int k) {
        /*---method 1:排序sort + 贪心---*/
        sort(nums.begin(), nums.end(), [](int a, int b) {
            return abs(a) > abs(b);
        });
        int result = 0;
        int n = nums.size();
        for (int i=0;i < n;i++) {
            if (k > 0 && nums[i] < 0) {
                k--;
                nums[i] = -nums[i];
            }
        }
        if (k%2 == 1) nums[n-1] = -nums[n-1];
        for (int a:nums) result += a;
        return result;
    }
};