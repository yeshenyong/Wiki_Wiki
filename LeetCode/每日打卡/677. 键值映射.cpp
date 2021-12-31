class MapSum {
public:
    /*---method 1:字典树---*/
    typedef struct TreeNode{
        int val;
        TreeNode* next[26];
        TreeNode() {
            this->val = 0;
            for (int i=0;i < 26;i++) this->next[i] = nullptr;
        }
    };
    MapSum() {
        root = new TreeNode();
    }
    void insert(string key, int val) {
        int delta = val;
        if (cnt.count(key)) {
            delta -= cnt[key];
        }
        cnt[key] = val;
        TreeNode *node = root;
        for (auto it:key) {
            if (node->next[it-'a'] == nullptr) {
                node->next[it-'a'] = new TreeNode();
            }
            node = node->next[it-'a'];
            node->val += delta;
        }
    }
    
    int sum(string prefix) {
        TreeNode* node = root;
        for (auto it:prefix) {
            if (node->next[it-'a'] == nullptr) return 0;
            node = node->next[it-'a'];
        }
        return node->val;
    }
private:
    TreeNode *root = nullptr;
    unordered_map<string, int> cnt;
};