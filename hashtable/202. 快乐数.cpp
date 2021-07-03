class Solution {
public:
    int getSum(int n){
        int sum = 0;
        while(n){
            int cal = n%10;
            sum += cal*cal;
            n /= 10;
        }
        return sum;
    }
    bool isHappy(int n) {
        /* 
            看起来是道数学题，但是一套分析下来
            唔，是自己想多了
            在此提一下，看到这些题，第一时间最好是找规律
            把1~2个数手写出结果，就知道，happynum肯定是不循环的
         */
        unordered_set<int> myset;
        while(true){
            int sum = getSum(n);
            if (sum == 1)return true;
            else{
                if(myset.count(sum))return false;
                myset.insert(sum);
            }
            n = sum;
        }
        return false;
    }
};