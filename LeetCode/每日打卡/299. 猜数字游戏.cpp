class Solution {
public:
    string getHint(string secret, string guess) {
        /*---method 1:标记+hashmap---*/
        int Buills = 0, Cows = 0;
        string cnt = "";
        int n = secret.size();
        vector<bool> visited(n, false);
        for (int i=0;i < n;i++) {
            if (secret[i] == guess[i]) {
                Buills++;
                visited[i] = true;
            }
        }

        unordered_map<char, int> mmap;

        for (int i=0;i < n;i++) {
            if (!visited[i]) mmap[secret[i]]++;
        }

        for (int i=0;i < n;i++) {
            if (!visited[i] && mmap[guess[i]] > 0) {
                Cows++;
                mmap[guess[i]]--;
            }
        }
        cnt += to_string(Buills);
        cnt.push_back('A');
        cnt += to_string(Cows);
        cnt.push_back('B');
        return cnt;
    }
};