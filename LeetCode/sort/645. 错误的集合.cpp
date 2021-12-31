class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        vector<int> result(2);
        int length = nums.size();
        /*----------------题目未说数组有序---------------*/

        /*---------method 1：哈希表 + 数列和------------*/
        /*
            时间复杂度O(n)  空间复杂度O(n)
         */
        /*
        int32_t sum = 0;
        int32_t totalsum = (length)*(length+1)/2;   //数列和,用于找到丢失的数据
        unordered_map<int16_t, int16_t> mmap;   //看题目要求降低内存分配
        for(int i=0;i<length;i++){
            mmap[nums[i]]++;
            sum += nums[i];
            if (mmap[nums[i]] > 1)result[0] = nums[i];
        }
        result[1] = totalsum-sum+result[0];
        */ 

        /*---------method 2：排序------------*/
        /*
            时间复杂度O(nlogn) 空间复杂度O(1)
         */
        /*
        sort(nums.begin(), nums.end());
        int pre = 0;
        for(auto &it:nums){
            if (it > pre + 1)result[1]=it-1;
            else if (it == pre)result[0]=it;
            pre = it;
        }
        if (nums[length-1] != length)result[1]=length;
        */

        /*---------method 3：桶排序------------*/
        /*
            时间复杂度O(n)  空间复杂度O(1)
         */
        /*
        for(int i = 0;i < length;i++){
            while(nums[i] != i+1 && nums[nums[i]-1] != nums[i]){
                swap(nums[i], nums[nums[i]-1]);
            }
        }
        for(int i = 0;i < length;i++){
            if (nums[i] != i+1){
                result[0] = nums[i];
                result[1] = i==0?1:nums[i-1]+1;
                break;
            }
        }
        */
        return result;
    }
};