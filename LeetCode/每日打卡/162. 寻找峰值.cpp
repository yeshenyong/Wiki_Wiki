class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        /*---method 1:二分 + 4种情况考虑---*/
        /*
            递增、递减、最大局部峰值、最小局部峰值
         */
        int n = nums.size();
        int left = 0, right = n-1;
        if (n == 1) return 0;
        if (nums[0] > nums[1]) return 0;
        if (nums[right] > nums[right-1]) return right;
        while(left <= right) {
            int mid = left + (right-left)/2;
            if (mid == 0) {
                left = mid + 1;
                continue;
            } else if (mid == n-1) {
                right = mid - 1;
                continue;
            }
            if (nums[mid] < nums[mid-1] && nums[mid] > nums[mid+1]) {
                right = mid - 1;
            } else if (nums[mid] > nums[mid-1] && nums[mid] < nums[mid+1]) {
                left = mid + 1;
            } else if (nums[mid] < nums[mid-1] && nums[mid] < nums[mid+1]) {
                if (left >= n-right) left = mid + 1;
                else right = mid - 1;
            } else {
                return mid;
            }
        }
        return -1;
    }
};