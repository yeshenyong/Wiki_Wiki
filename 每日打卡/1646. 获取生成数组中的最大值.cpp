class Solution {
public:
    int getMaximumGenerated(int n) {
        /*---method 1:模拟打卡---*/
        if (n == 0 || n == 1)return n;
        vector<int> nums(n+1);
        nums[0] = 0;
        nums[1] = 1;
        int maxn = INT_MIN;
        for (int i=2;i <= n;i++) {
            if (i%2 == 0) {
                nums[i] = nums[i/2];
            } else {
                nums[i] = nums[i/2] + nums[(i+1)/2];
            }
            maxn = maxn < nums[i]?nums[i]:maxn;
        }
        return maxn;
    }
};