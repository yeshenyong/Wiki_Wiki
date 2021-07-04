class Solution {
public:
    typedef unordered_map<int, int> mp;
    const mp getdpMap(const int& length, mp& dp, const vector<int>& nums){
        mp record;
        for(int i=0;i<length;i++){
            for(auto& it:dp){
                int result = it.second;
                record[it.first + nums[i]] += result;
            } 
        }
        return record;
    }
    int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
        int length = nums1.size();
        /*------method 1:分步动态规划--------*/
        /*
            反思：做法有点傻和重复
            时间复杂度O(n^2) 空间复杂度O(n^2) 但是时间和空间上都比方法二慢
         */
        /*
        mp dp;
        for(int i=0;i<length;i++)dp[nums1[i]]+=1;
        mp dp1 = getdpMap(length, dp, nums2);
        mp dp2 = getdpMap(length, dp1, nums3);
        mp dp3 = getdpMap(length, dp2, nums4);     
        return dp3[0];
        */

        /*-------method 2:四层循环变两层-------*/
        /*
            时间复杂度O(n^2)    空间复杂度O(n^2)
         */
        /*
        mp record;
        for(auto& i:nums1){
            for(auto& j:nums2){
                record[i+j]++;
            }
        }
        int sum = 0;
        for(auto& i:nums3){
            for(auto& j:nums4){
                if (record.find(0-(i+j)) != record.end())sum+=record[(0-(i+j))];
            }
        }
        return sum;
        */
    }
};