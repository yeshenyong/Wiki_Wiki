class Solution {
public:
    int waysToPartition(vector<int>& nums, int k) {
        /*---method 1:哈希表 + 枚举 + 前缀和后缀和---*/
        int length = nums.size();
        int count = 0;
        unordered_map<long long, vector<int>> mmap;
        /*---前缀和、后缀和皆包含自身---*/
        vector<long long> preSum(length, 0), orderSum(length, 0), dis(length, 0);
        for (int i=0;i < length;i++) {
            if (i == 0) preSum[i] = nums[i];
            else preSum[i] = preSum[i-1] + nums[i];
        }
        for (int i=length-1;i >= 0;i--) {
            if (i == length-1) orderSum[i] = nums[i];
            else orderSum[i] = orderSum[i+1] + nums[i];
        }
        for (int i=0;i < length;i++) {
            dis[i] = k - nums[i];
        }

        for (int i=1;i < length;i++) {
            /* 左右都需要考虑 */
            long long left = preSum[i] - nums[i], right = orderSum[i];
            mmap[left - right].emplace_back(i);
            if (left == right) count++;
        }

        for (int i=0;i < length;i++) {
            int tmp = 0;
            tmp += binarySearch(mmap[dis[i]], i+1, 1);
            tmp += binarySearch(mmap[-dis[i]], i+1, -1);
            // printf("i = %d, dis[%d] = %d, tmp = %d\n", i, i, dis[i], tmp);
            count = tmp > count ? tmp : count;
        }
        return count;
    }
    int binarySearch(const vector<int>& vec, int length, int flag) {
        int count = 0;
        if (flag == 1) {
            for (int i=0;i < vec.size() && vec[i] < length;i++) {
                count++;
            }
        } else {
            for (int i=vec.size()-1;i >= 0 && vec[i] >= length;i--) {
                count++;
            }
        }
        return count;
    }
};





