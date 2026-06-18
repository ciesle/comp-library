/*
 <中央値を保持する>
    - multiset を2つ持つ構造体 
        - push(T a) : aを加える
        - erase(T a) : aを削除する
        - get_size() : lowerとhigherのsizeをpair型で得る
        - get_median() : 現在の中央値を得る 

*/

template<typename T>
struct median_value{
    // 常にlower.size()<=higher.size()を保つ様に実装した方が楽そう
    multiset<T> lower;
    multiset<T> higher;
    median_value() {}

    void push(T v){
        if(lower.size() == 0 || v <= *prev((lower.end()))){
            if(lower.size() > higher.size()){
                // lower.rbegin()はconst iteratorではなくreverse iteratorというもの！
                lower.insert(v); //先に加える
                T temp = *(prev(lower.end()));
                auto temp_itr = (prev(lower.end()));
                lower.erase(temp_itr);
                higher.insert(temp);
            }
            else{
                lower.insert(v);
            }
        }
        else{
            if(higher.size() > lower.size()){
                higher.insert(v);
                T temp = *higher.begin();
                auto temp_itr = higher.begin();
                higher.erase(temp_itr);
                lower.insert(temp);
            }
            else{
                higher.insert(v);
            }
        }
    }

    void erase(T v){
        if(lower.count(v)){
            lower.erase(lower.find(v));
            if((int)lower.size() + 1 < ((int)higher.size() )){
                // size()はint型ではないので，負になる様な処理をするとおかしくなる　-> intにキャストすること!!
                T temp = *higher.begin();
                auto temp_itr = higher.begin();
                higher.erase(temp_itr);
                lower.insert(temp);
            }
        }
        else if(higher.count(v)){
            higher.erase(higher.find(v));
            if((int)higher.size() + 1< (int)lower.size()){
                T temp = *(prev(lower.end()));
                auto temp_itr = (prev(lower.end()));
                lower.erase(temp_itr);
                higher.insert(temp);
            }
        }
        else{
            throw runtime_error("Your input is not in the set.");
        }
        return;
    }

    pair<int,int> get_size(){
        return make_pair((int)lower.size(), (int)higher.size());
    }

    T get_median(){
        if(lower.size() < higher.size()){
            return *higher.begin();
        }
        else if(lower.size() > higher.size()){
            return *(prev(lower.end()));
        }
        else{
            return (*(prev(lower.end())) + *higher.begin()) / 2;
        }
    }
};