// Minimum Queue
//
// Descrição:
//     Fila que retorna o elemento mínimo em tempo constante.
//     Mantém no deque apenas os elementos que podem ser o mínimo
//     em algum momento futuro (elementos maiores que entram antes
//     são descartados, pois nunca serão o mínimo).
//
// Uso comum:
//     Sliding window (janela deslizante), two pointers.
//
// Complexidade:
//     push, pop, min - O(1) amortizado

template<class T> struct minqueue {
	deque<pair<T, int>> q;

	void push(T x) {
		int ct = 1;
		while (q.size() and x < q.back().f)
			ct += q.back().s, q.pop_back();
		q.emplace_back(x, ct);
	}
	void pop() {
		if (q.front().s > 1) q.front().s--;
		else q.pop_front();
	}
	T min() { return q.front().f; }
};
