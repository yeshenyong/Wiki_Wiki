class Solution {
public:
    int nthSuperUglyNumber(int n, vector<int>& primes) {
        /*----method 1:优先队列 + 去重----*/
        priority_queue<int, vector<int>, greater<>> MyQueue;
        unordered_set<int> MySet;
        int length = primes.size();
        int UglyNumber=1;
        MyQueue.push(1);
        while(!MyQueue.empty() && n--) {
            long long front = MyQueue.top();
            MyQueue.pop();
            for (int i=0;i < length;i++) {
                long long result = front * primes[i];
                if (result > INT_MAX) continue;
                if (!MySet.count(result)){
                    MyQueue.push(result);
                    MySet.insert(result);
                }
            }
            UglyNumber = front;
        }
        return UglyNumber;
    }
};