/*
  <両端priority-queue>
    - 最小値，最大値両方を効率的に取り出す 

    [実装/関数]
        - DoubleEndedPriorityQueue

    [計算時間]
        - 
    
    [備考]
        - 
        - 
    
    [参照]
        - chatgpt

    [verified at]
        - 
    
    [使用例]
        
        
*/


template<typename T>
class DoubleEndedPriorityQueue {
private:
    multiset<T> data;
public:
    // 要素の挿入
    void push(const T& value) {
        data.insert(value);
    }
    
    // 最小値（先頭要素）の参照を返す
    const T& top_min() const {
        if (data.empty()) throw runtime_error("Queue is empty");
        return *data.begin();
    }
    
    // 最大値（末尾要素）の参照を返す
    const T& top_max() const {
        if (data.empty()) throw runtime_error("Queue is empty");
        return *prev(data.end());
    }
    
    // 最小値を削除
    void pop_min() {
        if (data.empty()) throw runtime_error("Queue is empty");
        data.erase(data.begin());
    }
    
    // 最大値を削除
    void pop_max() {
        if (data.empty()) throw runtime_error("Queue is empty");
        auto it = prev(data.end());
        data.erase(it);
    }
    
    // キューが空かどうかを判定
    bool empty() const {
        return data.empty();
    }
    
    // 要素数を返す
    size_t size() const {
        return data.size();
    }
};