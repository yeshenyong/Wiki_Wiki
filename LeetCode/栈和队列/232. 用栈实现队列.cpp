class MyQueue {
private:
    stack<int> conta_stack;
    stack<int> order_stack;
public:
    /** Initialize your data structure here. */
    MyQueue() {

    }
    
    /** Push element x to the back of queue. */
    void push(int x) {
        conta_stack.push(x);
    }
    
    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        if (order_stack.empty()){
            while(!conta_stack.empty()){
                order_stack.push(conta_stack.top());
                conta_stack.pop();
            }
        }
        int front = order_stack.top();
        order_stack.pop();
        return front;
    }
    
    /** Get the front element. */
    int peek() {
        /* 复用代码 */
        int front = this->pop();
        order_stack.push(front);
        return front;
    }
    
    /** Returns whether the queue is empty. */
    bool empty() {
        return conta_stack.empty() && order_stack.empty();
    }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */