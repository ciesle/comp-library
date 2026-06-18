/*
- rootを決めて根付き木を作り，訪れた順/親/深さ/子の頂点 の情報を返す O(n)
    - 重み付き木のdepthはコストの和ではなく頂点数の和(重みなしのdepthと同一)
        変えたければdepth[a]+1の代わりにdepth[a]+x.toとする
*/

template<typename Graph>
pair<vector<vector<int>>,vector<vector<int>>> rooted_tree(Graph &G, int root){
	int n = G.size();
	vector<int> order = {root}, pare(n,-1), depth(n);
	vector<vector<int>> child(n);
	pare[root]=-2; //根の親は-2とする
	for(int i = 0; i < n; i++){
		int a=order[i];
		for(auto x:G[a]){
            int to;
            if constexpr (is_same<decltype(x), int>::value) to = x;
            else to = x.to;
			if(pare[to]==-1){
				pare[to]=a;
				depth[to]=depth[a]+1;
				order.push_back(to);
			}
		}
	}
	for(int i=0;i<n;i++){
		if(pare[i] != -2) child[pare[i]].push_back(i);
	}
	vector<vector<int>> info(3);
	info[0]=order;
	info[1]=pare;
	info[2]=depth;
	return {info,child};
}