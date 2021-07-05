class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        /*-------------method 1:hashtable--------------*/
        /*
            时间复杂度O(n^2)    空间复杂度O(n)
         */
        /*
        vector<vector<int>> vec;
        sort(nums.begin(), nums.end());
        int lenth = nums.size();
        for(int i=0;i<lenth-2;i++){
            if (nums[i] > 0)break;
            if (i > 0 && nums[i] == nums[i-1])continue;
            unordered_set<int> myset;
            for(int j=i+1;j<lenth;j++){
                if (j > i + 2
                        && nums[j] == nums[j-1]
                        && nums[j-1] == nums[j-2]) { // 三元组元素b去重
                    continue;
                }
                int other = 0-(nums[i]+nums[j]);
                if (myset.find(other) != myset.end()){
                    vec.push_back(vector<int>({nums[i], nums[j], other}));
                    myset.erase(other);
                }else{
                    myset.insert(nums[j]);
                }
            }
        }
        return vec;
        */
        /*------------method 2:双指针----------------*/
        /*
            时间复杂度O(n)  空间复杂度O(3*n)返回值
        */
        /*
        sort(nums.begin(), nums.end());
        vector<vector<int>> vec;
        int length = nums.size();
        for (int i=0;i<length;i++) {
            if (nums[i] > 0)break;
            if (i > 0 && nums[i] == nums[i-1])continue;
            int left = i+1, right = nums.size()-1;
            while(left < right){
                if (nums[left] + nums[right] + nums[i] == 0){
                    vec.push_back(vector<int>{nums[i], nums[left], nums[right]});
                    while(left < right && nums[left] == nums[left+1])left++;
                    while(left < right && nums[right] == nums[right-1])right--;
                    left++,right--;
                }
                else if (nums[left] + nums[right] + nums[i] > 0)right--;
                else left++;
            }
        }
        return vec;  
        */
    }
};