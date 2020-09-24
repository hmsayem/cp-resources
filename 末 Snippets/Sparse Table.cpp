template<class T, class Cmp>
struct Sparse {
    int N, K; Cmp cmp;
    vector<vector<T>> table; vector<vector<int>> idx;
    template<class Iter, class Func> 
    explicit Sparse(Iter beg, Iter end, Func&& cmp)
        : N(end-beg), K(lg(N)), cmp(forward<Func>(cmp)),
        table(vector<vector<T>>(K+1, vector<T>(N))),
        idx(vector<vector<int>>(K+1, vector<int>(N))) {

        for(int i = 0; i < N; i++) {
            table[0][i] = *(beg+i);
            idx[0][i] = i;
        }
        for(int k = 1; k <= K; k++) {
            for(int i = 0; i+(1<<k) <= N; i++) {
                if(cmp(table[k-1][i], table[k-1][i+(1<<(k-1))])) {
                    table[k][i] = table[k-1][i];
                    idx[k][i] = idx[k-1][i];
                }
                else {
                    table[k][i] = table[k-1][i+(1<<(k-1))];
                    idx[k][i] = idx[k-1][i+(1<<(k-1))];
                }
            }
        }
    }

    int lg(int x) {
        return 31 - __builtin_clz(x);
    }

    int query(int l, int r) {
        int k = lg(r-l+1);
        r = r-(1<<k)+1;
        return cmp(table[k][l], table[k][r])? idx[k][l] : idx[k][r];
    }
};

template<class T, class Cmp>
Sparse(T, T, Cmp) -> Sparse<typename iterator_traits<T>::value_type, Cmp>;

/*
int32_t main() {
    int arr[] = {1, 2, 3, 5};
    vector<int> a{1, 2, 3, 5};
    Sparse st1(a.begin(), a.end(), std::less{});
    Sparse st2(a.begin(), a.end(), [](int x, int y){ return x < y; });

    Sparse st3(arr, arr+n, std::less{});
    Sparse st4(arr, arr+n, [](int x, int y){ return x < y; });
}
*/
