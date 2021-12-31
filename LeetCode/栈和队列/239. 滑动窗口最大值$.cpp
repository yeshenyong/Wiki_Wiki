class Solution {
public:
    class MyQueue{
        private:
            deque<int> myqueue;
        public:
            void pop(int x){
                if(!myqueue.empty() && myqueue.front() == x)
                    myqueue.pop_front();
            }
            void push(int x){
                while(!myqueue.empty() && myqueue.back() < x) {
                    myqueue.pop_back();
                }
                myqueue.push_back(x);
            }
            int front(){
                return myqueue.front();
            }
    };
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        /*------------method 1:单调队列---------*/
        int length = nums.size();
        vector<int> Maxn;
        unique_ptr<MyQueue> q(new MyQueue());
        for(int i=0;i < length && i < k;i++) {
            q->push(nums[i]);
        }
        Maxn.push_back(q->front());
        for(int i=k;i < length;i++) {
            q->pop(nums[i-k]);
            q->push(nums[i]);
            Maxn.push_back(q->front());
        }
        return Maxn;
    }
};