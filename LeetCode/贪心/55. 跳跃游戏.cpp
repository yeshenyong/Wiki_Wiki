class Solution {
public:
    bool canJump(vector<int>& nums) {
        /*---method 1:动态规划---*/
        int n = nums.size();
        if (nums.size() == 1) return true;
        vector<bool> accessful(n+1, false);
        accessful[0]=true;
        for (int i=0;i < n;i++) {
            if (accessful[i]) {
                for (int j=1;j <= nums[i] && j < n;j++) {
                    accessful[j+i] = true;
                    if (accessful[n-1]) return true;
                }
            }
        }
        return accessful[n-1];
        /*---method 2:贪心---*/
        /*
        int cover = 0;
        if (nums.size() == 1) return true;
        for (int i=0;i <= cover;i++) {
            cover = max(i + nums[i], cover);
            if (cover >= nums.size()-1) return true;
        }
        return false;
        */
    }
};