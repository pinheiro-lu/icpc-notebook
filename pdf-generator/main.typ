#import "template.typ": *
#import sym

// Change here
#show: project.with(
  title: "Universidade de São Paulo - Escola de Artes, Ciências e Humanidades - Olá,sou Marcus Peixoto",
  date: datetime.today().display(),
  authors: (
    "Gabriel Dimant",
    "Marcus Peixoto",
    "Luan Pereira Pinheiro",
  ),
  logo: "marcus_peixoto.png"
)

= Template hashing

```bash
sed -n $2','$3' p' $1 | cpp -dD -P -fpreprocessed | tr -d '[:space:]' | md5sum | cut -c-6
```
The cli command `md5sum` is used to validate if the template was written correctly.

Write the one liner above without line breaks and save it as `hash.sh`, and run
```bash
chmod +x hash.sh
``` 
to make it executable.
vai luan

To get the hash in `file.cpp` between lines `<l1>` and `<l2>`, run
```sh
./hash.sh <file.cpp> <l1> <l2>
```

Hash is made by scope, delimited by the curly brackets (`"{"`, `"}"`).

The empty hash is 
`d41d8`
while
the `m5dsum` of `hash.sh` is
`9cd12`.

== Matrix Inverse 2x2

$A = mat(a,b;c,d)$, $A^(-1) = 1/(a d-b c)mat(d,-b;-c,a)$

== Pick Theorem

$A = i + b/2 -1$, where i is the number of lattice points inside the polygon
and b is the number on boundary. Work for simple polygon.

== Burnside Lemma

Let $G$ be a finite group that acts on set $X$. Let $X^g := { x in X | g.x = x }$. The number of orbits $|X slash G|$ is

$|X slash G| = 1/(|G|) sum_(g in G) |X^g| $

== Lucas Theorem

For prime p,
$binom(n, m) = product_(i=0)^n binom(n_i, m_i) space (mod p)$

== Fermat's Little Theorem

Let $p$ be a prime number. If $gcd(a, p) = 1$, then:
$ a^(p-1) equiv 1 space (mod p) quad "and" quad a^p equiv a space (mod p) "for all" a $

- *Modular Inverse:* $a^(-1) equiv a^(p-2) space (mod p)$
- *Exponent Reduction:* Since $a^(p-1) equiv 1$, the exponent cycles with period $p-1$:
$ a^b equiv a^(b space mod space (p-1)) space (mod p) quad ("if" gcd(a, p) = 1) $
*Example (Power Tower $a^(b^c) space mod space p$):* Compute exponent $E = b^c space mod space (p-1)$, then the answer is $a^E space mod space p$.

== Euler's Totient Theorem

Euler's totient function $phi(n)$ counts integers $k in [1, n]$ such that $gcd(k, n) = 1$.
$ phi(n) = n product_(p | n) (1 - 1/p) = product_(i) p_i^(k_i - 1) (p_i - 1) $
- *Properties:* Multiplicative: $phi(a b) = phi(a) phi(b)$ if $gcd(a, b) = 1$. Also $sum_(d | n) phi(d) = n$.
- *Euler's Theorem:* If $gcd(a, m) = 1$, then $a^(phi(m)) equiv 1 space (mod m)$, so $a^(-1) equiv a^(phi(m)-1) space (mod m)$.

*General Exponent Reduction (Power Towers / CSES Exponentiation II):*
For any base $a$:
$ a^b equiv cases(
  a^(b space mod space phi(m)) space (mod m) &"if" gcd(a, m) = 1,
  a^(b space mod space phi(m) + phi(m)) space (mod m) &"if" b >= phi(m)
) $
To evaluate towers $a_1^(a_2^(a_3^(dots))) space mod space m$, recursively reduce the modulo with $m_(i+1) = phi(m_i)$, which reaches 1 in $O(log m)$ steps.

== Catalan

$C_n = 1 / (n+1) mat(2n;n) = (2n)!/(n!*(n+1)!)$
|
$C_n^k = (k+1)/(n+k+1) mat(2n +k;n)$

== Stirling numbers of the first kind

These are the number of permutation of size n with exactly k cycles

$vec(n,k,delim: "[") = (n-1)vec(n-1,k,delim: "[") + vec(n-1,k-1,delim: "[")$

$vec(0,0,delim: "[") = 1, vec(n,0,delim: "[") = vec(0,n,delim: "[") = 1$

== Stirling numbers of the second kind

These are the number of ways to partition n into exactly k non-empty
sets.

$vec(n,k,delim: "[") = (k)vec(n-1,k,delim: "[") + vec(n-1,k-1,delim: "[")$

$vec(n,k,delim: "[") = 1/k! sum_(j=0)^k (-1)^(k-j) mat(k;j)j^n$

$vec(0,0,delim: "[") = 1, vec(n,0,delim: "[") = vec(0,n,delim: "[") = 1$

== Planar Graph

If G has k connected components, then $n-m+f=k+1$. 

$m <= 3n - 6$. If G has no triangles, $m ≤ 2n − 4$.

The minimum degree is less of equal 5. And can be 6 colored in O(n+m)/

=== Spherical coordinates

$x = r sin sym(theta) cos sym(phi)$  | $r = sqrt(x^2 + y^2 + z^2)$

$y = r sin sym(theta) sin sym(phi)$ | $sym(theta) = $acos$ (z/r)$

$z = r cos sym(theta)$ | $sym(phi) =$ atan2 $(y,x)$

== Series and sums

$1/(1-x) = sum_(k=0) x^k$ | 
$e^x = sum_(k=0) x^k/k!$ | 
$ln(1+x) = sum_(k=1) (-1)^(k+1) x^k/k$

$sqrt(1+x) = 1 + sum_(k=1) (-1)^(k+1)x^k/2^(2k-1)$ | 
$1/(sqrt(1-x)) = sum_(k=0) x^k/4^k mat(2k;k)$

$sin x = sum_(k=0) (-1)^(k) (x)^(2k+1)/(2k+1)!$ |
$cos x = sum_(k=0) (-1)^k x^(2k)/(2k)!$

$1+2+... = (n(n+1))/2$ | $1^2+2^2+.. = (n(2n+1)(n+1))/6$

$1^3+2^3+.. = (n^2(n+1)^2)/4$ | $1^4+2^4+... = (n(n+1)(2n+1)(3n^2+3n+1))/30$

== Erdos-Gallai Theorem

A sequence of non-negative integers $d_1 >= ... >= d_n$ can be represented as the degree sequence of a finite simple graph on $n$ vertices if and only if $d_1 + ... + d_n$ is even and
$sum_(i=1)^(k) d_i <= k(k-1) + sum_(i=k+1)^(n) min(d_i, k)$
holds for every $k$ in 1 $<= k <= n$.

== Prufer sequence

The Prüfer sequence is a bijection between labeled trees with $n$ vertices and sequences with $n-2$ numbers from 1 to $n$.
To get the sequence from the tree:
- While there are more than 2 vertices, remove the leaf with smallest label and append it's neighbour to the end of the sequence.
To get the tree from the sequence:
- The degree of each vertex is 1 more than the number of occurrences of that vertex in the sequence. Compute the degree $d$, then do the following: for every value $x$ in the sequence (in order), find the vertex with smallest label $y$ such that $d(y) = 1$ and add an edge between $x$ and $y$, and also decrease their degrees by 1. At the end of this procedure, there will be two vertices left with degree 1; add an edge between them.

== Tutte Matrix

The Tutte Matrix $A$ of a graph $G = (V, E)$ is a matrix where its determinant is non-zero if and only if a perfect matching exists. If |V| = n, then $A$ is a $n$-by-$n$ matrix where

$ A_(i j) = cases(
  x_(i j) "if" (i, j) in E "and" i < j,
  -x_(j i) "if" (i, j) in E "and" i > j,
  0 "otherwise"
)  $

where the $x_(i j)$ are indeterminates (can be anything).

== Laplacian Matrix

The Laplacian matrix $L$ of a graph $G = (V, E)$ where $|V| = n$ is a $n$-by-$n$ matrix where 

$ L_(i j) = cases(
  deg(v_i) "if" i = j,
  -1 "if" i != j "and" (i, j) in E,
  0 "otherwise"
)  $

If we delete one row and one column from the matrix, the determinant of the resulting $(n-1)$-by-$(n-1)$ matrix will be equal to the number of distinct spanning trees.

== Kőnig's theorem

To find a vertex cover equal to the maximum matching of a $(A, B)$-bipartite graph, let $U$ be the set of unmatched vertices in $A$, and let $Z$ be the set of vertices that are either in $U$ or are connected to $U$ by alternating paths (paths that alternate between edges that are in the matching and edges that are not in the matching). Then the vertex cover is

$ K = (A backslash Z) union (B inter Z)  $

== Bit Manipulation (CP-Algorithms)

=== Bit Operations (k-th bit)
- *Check bit:* `(x >> k) & 1` or `(x & (1LL << k)) != 0`
- *Set bit:* `x |= (1LL << k)`
- *Clear bit:* `x &= ~(1LL << k)`
- *Toggle bit:* `x ^= (1LL << k)`
- *Set to $v in {0, 1}$:* `x = (x & ~(1LL << k)) | ((ll)v << k)`

=== Arithmetic & LSB Tricks
- *Isolate LSB (lowest 1):* `x & -x` or `x & (~x + 1)`
- *Clear lowest 1:* `x & (x - 1)`
- *Power of 2 check:* `x > 0 and (x & (x - 1)) == 0`
- *Mask of 1s up to LSB:* `x ^ (x - 1)`
- *Mask of 1s below LSB:* `(x & -x) - 1`
- *Clear trailing 1s:* `x & (x + 1)` (ex: $010111_2 arrow.r 010000_2$)
- *Set lowest 0:* `x | (x + 1)` (ex: $010110_2 arrow.r 010111_2$)
- *Isolate lowest 0:* `~x & (x + 1)`

=== Fast Branchless Tricks
- *Swap:* `a ^= b; b ^= a; a ^= b;`
- *Modulo $2^k$:* `x & ((1LL << k) - 1)`
- *Opposite signs check:* `(x ^ y) < 0`
- *Branchless Min/Max:*
  - $min(a, b) = b xor ((a xor b) and -(a < b))$
  - $max(a, b) = a xor ((a xor b) and -(a < b))$

=== Mask Operations as Sets
- *Intersection ($A inter B$):* `A & B`
- *Union ($A union B$):* `A | B`
- *Difference ($A backslash B$):* `A & ~B`
- *Symmetric Difference ($A Delta B$):* `A ^ B`
- *Complement ($overline(A)$ in $N$ bits):* `(~A) & ((1LL << N) - 1)`
- *Subset check ($A subset.eq B$):* `(A & B) == A` or `(A & ~B) == 0`

=== GCC Builtins (64-bit with `ll`) & C++20 `<bit>`
- `__builtin_popcountll(x)` / `std::popcount(x)`: number of 1-bits.
- `__builtin_ctzll(x)` / `std::countr_zero(x)`: trailing zeros (index of LSB).
- `__builtin_clzll(x)` / `std::countl_zero(x)`: leading zeros ($63 - floor(log_2(x))$).
- `__builtin_ffsll(x)`: 1st set bit ($1$-indexed, 0 if $x = 0$).
- `__builtin_parityll(x)`: parity of 1s ($1$ if odd, $0$ if par).
- `std::bit_width(x)` ($64 - "clz"(x)$), `std::bit_ceil(x)` ($>= x$), `std::bit_floor(x)` ($<= x$).
- `std::rotl(x, s)` / `std::rotr(x, s)`: circular bit rotation.

=== Submask Enumeration
- *Iterate submasks of $m$ in descending order ($O(3^N)$ total):*
```cpp
for (int s = m; s; s = (s - 1) & m) { /* use s */ }
```
- *Iterate submasks of $m$ including 0:*
```cpp
for (int s = m; ; s = (s - 1) & m) { /* use s */ if (!s) break; }
```
- *Iterate supermasks of $s$ in universe of $N$ bits:*
```cpp
for (int m = s; m < (1 << n); m = (m + 1) | s) { /* use m */ }
```

=== Gosper's Hack ($O(binom(n, k))$)
Iterates all masks of $n$ bits with exactly $k$ set bits in ascending order:
```cpp
int mask = (1 << k) - 1;
while (mask < (1 << n)) {
	/* use mask */
	int c = mask & -mask, r = mask + c;
	mask = (((r ^ mask) >> 2) / c) | r;
}
```

== Game Theory

- *Sprague-Grundy Theorem:* Any impartial finite game under normal play is equivalent to a Nim pile of size $G(S) = "mex"({G(S')})$. Independent games sum with XOR: $G(A + B) = G(A) xor G(B)$. First player wins iff $G(S) != 0$.
- *Standard Nim:* Piles $a_1, dots, a_n$. First player wins iff $a_1 xor a_2 xor dots xor a_n != 0$.
- *Another Game (remove 1 from any subset of piles):* First player wins iff there is at least one odd pile ($exists a_i equiv 1 space (mod 2)$).
- *Subtraction Game (remove $1 dots k$ items):* First player wins iff $n space mod space (k + 1) != 0$.
- *Staircase Nim (move coins down steps):* Equivalent to Nim on *odd-indexed* steps ($xor_(i "odd") a_i != 0$). Moving from odd to even is a normal Nim move; moving from even to odd can be mirrored by the opponent.
- *Tree Nim (move coins towards root):* Equivalent to Nim with coins at odd depths ($xor_(u "odd depth") "coins"[u] != 0$).
- *Misère Nim (normal play, but last move loses):* If all piles have size $<= 1$, win iff number of piles is even; otherwise, play normal Nim ($xor a_i != 0$).
- *Moore's $"Nim"_k$ (remove from up to $k$ piles):* Expand pile sizes in binary; win iff there is some bit position where the sum of bits is not divisible by $k + 1$.

= Code

// Script will be used to insert code here

