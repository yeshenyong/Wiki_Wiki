class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        /*------------method 1:栈解决问题-----------*/
        stack<int> st;
        int result = 0;
        for(int i=0;i < tokens.size();i++){
            if (tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/"){
                int right = st.top();
                st.pop();
                int left = st.top();
                st.pop();
                int sum = 0;
                if (tokens[i] == "+")sum=(left+right);
                else if (tokens[i] == "-")sum=(left-right);
                else if (tokens[i] == "*")sum=(left*right);
                else sum=(left/right);
                st.push(sum);
            }
            else st.push(stoi(tokens[i]));
        }
        return st.top();
    }
};