/*
    -　互いに重なりうる半開区間[x,y)が何個か与えられて，それらをマージして
    重なりのない区間の集合として表したい 

    ex)[3,5),[4,7),[8,9) -> [3,7),[8,9)

    -> set<pair<ll,ll>> の元で[a,b)を表現する

    
*/

struct segment_set{
    set<pair<long long, long long>> segset;
    const long long LARGE = 2e18;
    segment_set() { //初期化はコンストラクタで
        segset.insert(make_pair(-LARGE, -LARGE));
        segset.insert(make_pair(LARGE, LARGE));
    }
    
    void add(long long x, long long y){ //区間[x,y)を加える
        auto it=segset.lower_bound(make_pair(x,y)); it--;
        // 今見ている区間の左端が他の区間と被っていたらマージする
        // 右側の条件の等号は場面によって変わることがある
        // [3,4),[4,5) を [3,5)と扱うかどうか(扱うなら等号がつく)
        if(it->first <= x && x <= it->second){
            x=min(x,it->first); y=max(y,it->second);
            segset.erase(it);
        }
        it=segset.lower_bound(make_pair(x,y));
        while(1){ // 再帰的に区間を処理
            if(x <= it->first && it->first <= y){ 
                y=max(y,it->second);
                it=segset.erase(it); // 削除された要素の次を指すイテレータを返す
            }
            else break;
        }
        segset.insert(make_pair(x,y));
    }
};