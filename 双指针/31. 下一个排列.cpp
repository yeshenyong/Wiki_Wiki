class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        /*---method 1:双指针 + 排序 + 找区间---*/
        int fixLidx = -1, fixRidx = -1;
        int n = nums.size();
        for (int i=1;i < n;i++) {
            if (nums[i] > nums[i-1]) {
                fixLidx = i-1;
            }
        }
        if (fixLidx == -1 && fixRidx == -1) {
            sort(nums.begin(), nums.end());
        } else {
            int left = fixLidx, right = n - 1;
            for (int i=n-1;i > fixLidx;i--) {
                if (nums[fixLidx] < nums[i]) {
                    fixRidx = i;
                    break;
                }
            }
            swap(nums[fixLidx], nums[fixRidx]);
            sort(nums.begin() + fixLidx + 1, nums.end());
        }
        // cout << "fixLidx = " << fixLidx << ", fixRidx = " << fixRidx << endl;
        return;
    }
};