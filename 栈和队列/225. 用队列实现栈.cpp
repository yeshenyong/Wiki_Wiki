/*---------method 1:两个队列实现栈--------------*/
/*
class MyStack {
private:
    queue<int> queIn;
    queue<int> queTmp;
public:
    // Initialize your data structure here. 
    MyStack() {

    }
    // Push element x onto stack. 
    void push(int x) {
        queIn.push(x);
    }
    // Removes the element on top of the stack and returns that element. 
    int pop() {
        int length = queIn.size();
        while(length-- > 1){
            queTmp.push(queIn.front());
            queIn.pop();
        }
        int result = queIn.front();
        queIn.pop();
        while(!queTmp.empty()){
            queIn.push(queTmp.front());
            queTmp.pop();
        }
        return result;
    }
    // Get the top element
    int top() {
        return queIn.back();
    }
    // Returns whether the stack is empty
    bool empty() {
        return queIn.empty();
    }
};
*/
/*--------method 2:一个队列实现栈---------------*/
class MyStack {
private:
    queue<int> queIn;
public:
    /** Initialize your data structure here. */
    MyStack() {

    }
    /** Push element x onto stack. */
    void push(int x) {
        queIn.push(x);
    }
    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        int length = queIn.size();
        while(length-- > 1){
            queIn.push(queIn.front());
            queIn.pop();
        }
        int result = queIn.front();
        queIn.pop();
        return result;
    }
    /** Get the top element. */
    int top() {
        return queIn.back();
    }
    /** Returns whether the stack is empty. */
    bool empty() {
        return queIn.empty();
    }
};