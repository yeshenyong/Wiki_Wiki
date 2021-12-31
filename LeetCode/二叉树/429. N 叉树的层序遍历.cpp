class Solution {
public:
    vector<vector<int>> levelOrder(Node* root) {
        /*-----------method 1:bfs + 多儿子节点遍历--------*/
        vector<vector<int>> tree;
        if (!root)return tree;
        queue<Node*> MyQueue;
        MyQueue.push(root);
        while (!MyQueue.empty()) {
            int size = MyQueue.size();
            vector<int> tmp;
            Node* cur;
            for (int i=0;i < size;i++) {
                cur = MyQueue.front();
                MyQueue.pop();
                tmp.push_back(cur->val);
                for (auto &it:cur->children) {
                    MyQueue.push(it);
                }
            }
            tree.push_back(tmp);
            cur = nullptr;
        }
        return tree;
    }
};