class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        /*
            难点在于去重
        */
        /*-------------method 1:双指针-------------*/
        /*
            时间复杂度O(n^3)    空间复杂度O(4*n) O(cn)
            确定两个值，以此进行双指针，与三数之和同理
        */
        vector<vector<int>> Vec;
        sort(nums.begin(), nums.end());
        int length = nums.size();
        for(int16_t i=0;i < length-3;i++){
            if (i > 0 && nums[i]==nums[i-1])continue;
            for(int16_t j=i+1;j < length-2;j++){
                if (j > i+1 && nums[j]==nums[j-1])continue;
                int left = j+1, right = nums.size()-1;
                while(left < right){
                    int32_t result = nums[i]+nums[j]+nums[left]+nums[right];
                    if (result > target){
                        right--;
                    }else if(result < target){
                        left++;
                    }else{
                        Vec.push_back(vector<int>({nums[i], nums[j],nums[left],nums[right]}));
                        while(left < right && nums[left]==nums[left+1])left++;
                        while(left < right && nums[right]==nums[right-1])right--;
                        left++,right--;
                    }
                }
            }
        }
        return Vec;
    }
};