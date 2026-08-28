// Z-Algorithm
//
// Descrição:
//     z[i] é o comprimento do maior prefixo comum (LCP) entre a
//     string inteira s e o sufixo iniciando na posição i (s[i...sz-1]).
//     Por definição, z[0] = 0.
//
// Aplicações:
//     - Pattern Matching: montar S = P + '#' + T. Todo i > |P| com z[i] == |P|
//       é uma ocorrência de P em T.
//     - Prefixo Palíndromo: rodar em S + '#' + reverse(S). Se i + z[i] == |T|,
//       o prefixo de tamanho z[i] é palíndromo.
//     - Menor Período: menor k > 0 tal que k + z[k] == sz e sz % k == 0.
//
// Complexidade: O(N) tempo e espaço

template <typename T> vector<int> z_alg(int sz, const T s[]) {
	vector<int> ret(sz);
	for(int l=0,r=0,i=1;i<sz;i++) {
		auto expand = [&]() {
			while(r<sz && s[r-l]==s[r]) r++;
			ret[i] = r-l;
		};
		if(i >= r) {
			l=r=i;
			expand();
		} else {
			if(ret[i-l] < r-i) ret[i] = ret[i-l];
			else {
				l=i;
				expand();
			}
		}
	}
	return ret;
};
