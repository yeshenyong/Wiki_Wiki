class Solution {
public
    int minDepth(TreeNode root) {
        ----------method 1层序遍历-----------
        if (!root)return 0;
        queueTreeNode MyQueue;
        MyQueue.push(root);
        int level = 0;
        while(!MyQueue.empty()) {
            int size = MyQueue.size();
            level++;
            TreeNode cur;
            for (int i=0;i  size;i++) {
                cur = MyQueue.front();
                MyQueue.pop();
                if (cur-left)MyQueue.push(cur-left);
                if (cur-right)MyQueue.push(cur-right);
                if (!cur-left && !cur-right)return level;
            }
            
        }
        return level;
    }
};