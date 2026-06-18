/*
(https://algo-logic.info/trie-tree/)
<Trie木>
    - 一つの頂点が一つの文字を表す
    - 文字列の先頭部分(prefix)が共通する場合は，なるべく無駄がないように共有する
    - 各頂点に，いろいろな情報(子/その頂点で終わる文字列が存在するか/その頂点を通る文字列の個数...)を持たせておく

    (主な機能)
    - 挿入
    - 検索(文字列がトライ木に保存されているか)

*/

template <int char_size, int base>
struct Trie {
    struct Node {            
        vector<int> next;    // 子の頂点番号を格納。
        vector<int> accept;  // 末端がこの頂点になる単語の word_id を保存
        int c;               // base からの間隔をint型で表現したもの
        int common;          // いくつの単語がこの頂点を共有しているか
        Node(int c_) : c(c_), common(0) {
            next.assign(char_size, -1);
        }
    };
    vector<Node> nodes;  
    int root;
    Trie() : root(0) {
        nodes.push_back(Node(root));
    }
    // 単語の挿入
    void insert(const string &word, int word_id) {
        int node_id = 0;
        for (int i = 0; i < (int)word.size(); i++) {
            int c = (int)(word[i] - base);
            int &next_id = nodes[node_id].next[c];
            if (next_id == -1) {  // 次の頂点が存在しなければ追加
                next_id = (int)nodes.size();
                nodes.push_back(Node(c));
            }
            ++nodes[node_id].common;
            node_id = next_id;
        }
        ++nodes[node_id].common;
        nodes[node_id].accept.push_back(word_id);
    }
    void insert(const string &word) {
        insert(word, nodes[0].common);
    }
    // 単語とprefixの検索
    int search(const string &word, bool prefix = false) {
        int node_id = 0;
        for (int i = 0; i < (int)word.size(); i++) {
            int c = (int)(word[i] - base);
            int &next_id = nodes[node_id].next[c];
            if (next_id == -1) {  
                return 0;
            }
            node_id = next_id;
        }
        if(prefix) return nodes[node_id].common;
        else return nodes[node_id].accept.size();
    }
    // prefix を持つ単語が何個存在するかの検索
    int start_with(const string &prefix) {
        return search(prefix, true);
    }
    // wordに一致する文字列が何個存在するか
    int word_count(const string &word){
        return search(word, false);
    }
    // 挿入した単語の数
    int count() const {
        return (nodes[0].common);
    }
    // Trie木のノード数
    int size() const {
        return ((int)nodes.size());
    }
};

// int main() {
//     Trie<26, 'A'> trie;
//     trie.insert("FIRE");
//     cout << trie.search("FIRE") << endl;  // 1 を返す
//     cout << trie.search("FI") << endl;    // 0 を返す
//     trie.insert("FIREMAN");
//     trie.insert("FIREARM");
//     trie.insert("FIREWORK");
//     trie.insert("ALGO");
//     return 0;
// }