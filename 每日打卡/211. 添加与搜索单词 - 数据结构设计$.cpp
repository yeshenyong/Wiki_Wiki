class WordDictionary {
public:
    /* method 1:字典树　+ dfs */
    WordDictionary():root_(std::make_shared<TrieNode>()) {

    }
    
    void addWord(string word) {
        auto cnt = root_;
        for (auto &it : word) {
            if (cnt->children_[it - 'a'] == nullptr) {
                cnt->children_[it - 'a'] = std::make_shared<TrieNode>();
            }
            cnt = cnt->children_[it - 'a'];
        }
        cnt->isWord_ = true;
    }
    bool search(string word) {
        return find(root_, word, 0);
    }
private:
    typedef struct TrieNode{
        TrieNode():isWord_(false), children_(26, nullptr) {}
        bool isWord_;
        std::vector<std::shared_ptr<TrieNode>> children_;
    }TrieNode;
    bool find(std::shared_ptr<TrieNode> res, string& pre, int idx) {
        if (res != nullptr) {
            if (pre.size() == idx) return res->isWord_;
            if (pre[idx] == '.') {
                for (auto chid : res->children_) {
                    if (find(chid, pre, idx+1)) return true;
                }
            } else {
                return find(res->children_[pre[idx] - 'a'], pre, idx+1);
            }
        }
        return false;
    }
    std::shared_ptr<TrieNode> root_;
};