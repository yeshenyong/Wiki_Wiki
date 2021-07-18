class Solution {
public:
    Node* connect(Node* root) {
        /*--------method 1:bfs + 指多下一个------*/
        if (!root)return root;
        queue<Node*> MyQueue;
        MyQueue.push(root);
        while(!MyQueue.empty()) {
            int size = MyQueue.size();
            Node *tmp;
            for (int i=0;i < size;i++) {
                tmp = MyQueue.front();
                MyQueue.pop();
                if (i != size-1) {
                    tmp->next = MyQueue.front();
                } else {
                    tmp->next = NULL;
                }
                if (tmp->left) MyQueue.push(tmp->left);
                if (tmp->right) MyQueue.push(tmp->right);
            }
            tmp = nullptr;
        }
        return root;
    }
};