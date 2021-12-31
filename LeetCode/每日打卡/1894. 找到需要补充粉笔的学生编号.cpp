class Solution {
public:
    int chalkReplacer(vector<int>& chalk, int k) {
        /*---method 1:前綴和 + 二分查找---*/
        long long sum = chalk[0];
        if (chalk[0] > k) {
            return 0;
        }
        int n = chalk.size();
        for (int i=1;i < n;i++) {
            sum+=chalk[i];
            chalk[i] += chalk[i-1];
            if (chalk[i] > k) return i;
        }
        int cul = k % sum;
        int left = 0, right = n-1;
        while(left <= right) {
            int mid = left + (right-left)/2;
            if (chalk[mid] > cul) {
                right = mid - 1;
            } else if (chalk[mid] < cul) {
                left = mid + 1;
            } else {
                return mid + 1;
            }
        }
        return left;
    }
};