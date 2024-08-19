#ifndef _OISPACE_H_
#define _OISPACE_H_

#include <bits/stdc++.h>

using namespace std;
using db = double;
using ll = long long;
//using lll = __int128;
using ldb = long double;
using ull = unsigned long long;
//using ulll = unsigned __int128;

namespace TempLib {
	template <class T>
	struct Mod {
		using value_type = T;
		inline void operator () (value_type &x, const value_type &y) {
			while (x < 0) x += y;
			while (x > y) x -= y;
		}
	};
	
	template <class T, int N>
	struct Atlantis {
		using value_type = T;
		static constexpr auto max_size = N + 5;
		using pvv = pair<value_type, value_type>;
		struct ScanLine {
			int flag;
			value_type xl, xr, y;
			ScanLine() = default;
			ScanLine(value_type _xl, value_type _xr, value_type _y, int _flag) {
				xl = _xl, xr = _xr, y = _y, flag = _flag;
			}
			friend bool operator < (const ScanLine &x, const ScanLine &y) {
				return x.y < y.y;
			}
		} line[max_size << 1];
		value_type xx[max_size << 1];
		#define ls (p << 1)
		#define rs (p << 1 | 1)
		struct Node {
			int l, r, tag, len;
			Node() = default;
			Node(int _l, int _r, int _tag, int _len) {
				l = _l, r = _r, tag = _tag, len = _len;
			}
		} tr[max_size << 2];
		inline void pushup(int p) {
			auto l = tr[p].l;
			auto r = tr[p].r;
			if (tr[p].tag) {
				tr[p].len = xx[r + 1] - xx[l];
			} else {
				tr[p].len = tr[ls].len + tr[rs].len;
			}
		}
		void build(int p, int l, int r) {
			tr[p] = Node(l, r, 0, 0);
			if (l == r) {
				return;
			}
			int mid = (l + r) >> 1;
			build(ls, l, mid);
			build(rs, mid + 1, r);
		}
		void modify(int p, int L, int R, int A) {
			auto l = tr[p].l;
			auto r = tr[p].r;
			if (xx[r + 1] <= L || R <= xx[l]) {
				return;
			}
			if (L <= xx[l] && xx[r + 1] <= R) {
				tr[p].tag += A;
				pushup(p);
				return;
			}
			modify(ls, L, R, A);
			modify(rs, L, R, A);
			pushup(p);
		}
		#undef ls
		#undef rs
		void maintask(int n) {
			int cnt = 0;
			for (int i = 0; i < n; i++) {
				pvv A, B;
				cin >> A.first >> A.second;
				cin >> B.first >> B.second;
				line[++cnt] = ScanLine(A.first, B.first, A.second, 1), xx[cnt] = A.first;
				line[++cnt] = ScanLine(A.first, B.first, B.second, -1), xx[cnt] = B.first;
			}			
			sort(line + 1, line + cnt + 1);
			sort(xx + 1, xx + cnt + 1);
			int m = unique(xx + 1, xx + cnt + 1) - (xx + 1);
			build(1, 1, m - 1);
			value_type ans = 0;
			for (int i = 1; i < cnt; i++) {
				modify(1, line[i].xl, line[i].xr, line[i].flag);
				ans += tr[1].len * (line[i + 1].y - line[i].y);
			}
			cout << ans << "\n";
		}
	};
	
	template <class T, int N>
	struct Blocks {
		using value_type = T;
		static constexpr auto max_size = N + 5;
		static constexpr auto bl_size = int(sqrt(N)) + 5;
		int st[bl_size], ed[bl_size], bel[max_size], bl, t;
		value_type A[max_size], B[max_size];		
		value_type add[bl_size];
		void init(int n) {
			bl = sqrt(n);
			t = n / bl;
			if (n % bl) {
				t++;
			}
			for (int i = 1; i <= t; i++) {
				st[i] = (i - 1) * bl + 1;
				ed[i] = i * bl;
			}
			ed[t] = n;
			for (int i = 1; i <= n; i++) {
				bel[i] = (i - 1) / bl + 1;
			}
		}
		inline void reset(int x) {
			auto p = bel[x];
			for (int i = st[p]; i <= ed[p]; i++) {
				B[i] = A[i];
			}
			sort(B + st[p], B + ed[p] + 1);
		}
		void modify(int l, int r, value_type w) {
			auto p = bel[l];
			auto q = bel[r];
			if (p == q) {
				for (int i = l; i <= r; i++) {
					A[i] += w;
				}
				reset(l);
				return;
			}
			for (int i = l; i <= ed[p]; i++) {
				A[i] += w;
			}
			reset(l);
			for (int i = st[q]; i <= r; i++) {
				A[i] += w;
			}
			reset(r);
			for (int i = p + 1; i <= q - 1; i++) {
				add[i] += w;
			}
		}
		int query(int l, int r, value_type w) {
			int ret = 0;
			auto p = bel[l];
			auto q = bel[r];
			if (p == q) {
				for (int i = l; i <= r; i++) {
					if (A[i] + add[p] >= w) {
						ret++;
					}
				}
				return ret;
			}
			for (int i = l; i <= ed[p]; i++) {
				if (A[i] + add[p] >= w) {
					ret++;
				}
			}
			for (int i = st[q]; i <= r; i++) {
				if (A[i] + add[q] >= w) {
					ret++;
				}
			}
			for (int i = p + 1; i <= q - 1; i++) {
				auto L = st[i];
				auto R = ed[i];
				int cnt = 0;
				while (L <= R) {
					int mid = (L + R) >> 1;
					if (B[mid] + add[i] >= w) {
						R = mid - 1, cnt = ed[i] - mid + 1;
					} else {
						L = mid + 1;
					}
				}
				ret += cnt;
			}
			return ret;
		}
		void maintask(int n, int m) {
			init(n);
			for (int i = 1; i <= n; i++) {
				cin >> A[i];
				B[i] = A[i];
			}
			for (int i = 1; i <= t; i++) {
				sort(B + st[i], B + ed[i] + 1);
			}
			while (m--) {
				char opt;
				int l, r, w;
				cin >> opt >> l >> r >> w;
				if (opt == 'M') {
					modify(l, r, w);
				} else {
					cout << query(l, r, w) << "\n";
				}
			}
		}
	};
	
	template <class T, int N, int M>
	struct Modui {
		using value_type = T;
		static constexpr auto max_size = N + 5;
		static constexpr auto max_query = M + 5;
		int blo, bel[max_size];
		value_type A[max_size], tot[max_size];
		value_type sum, ans[max_query], _ans[max_query];
		struct qry {
			int l, r, id;
		} q[max_query];
		void init(int n) {
			blo = sqrt(n);
			for (int i = 1; i <= n; i++) {
				bel[i] = (i - 1) / blo + 1;
			}
		}
		inline void add(int x) {
			sum += tot[x];
			tot[x]++;
		}
		inline void del(int x) {
			tot[x]--;
			sum -= tot[x];
		}
		void maintask(int n, int m) {
			init(n);
			for (int i = 1; i <= n; i++) {
				cin >> A[i];
			}
			for (int i = 1; i <= m; i++) {
				cin >> q[i].l >> q[i].r;
				q[i].id = i;
			}
			sort(q + 1, q + m + 1, [&](const qry &x, const qry &y) {
				return bel[x.l] == bel[y.l] ? x.r < y.r : x.l < y.l;
			});
			for (int i = 1, l = 1, r = 0; i <= m; i++) {
				if (q[i].l == q[i].r) {
					ans[q[i].id] = 0, _ans[q[i].id] = 1;
					continue;
				}
				while (l > q[i].l) add(A[--l]);
				while (r < q[i].r) add(A[++r]);
				while (l < q[i].l) del(A[l++]);
				while (r > q[i].r) del(A[r--]);
				ans[q[i].id] = sum, _ans[q[i].id] = (ll)(r - l + 1) * (r - l) / 2;
			}
			for (int i = 1; i <= m; i++) {
				auto A = ans[i];
				auto B = _ans[i];
				if (!A) {
					cout << "0/1\n";
					continue;
				}
				auto g = __gcd(A, B);
				A /= g, B /= g;
				cout << A << "/" << B << "\n";
			}
		}
	};
	
	template <class T>
	struct Chtholly_tree {
		using value_type = T;
		struct node {
			int l, r;
			mutable value_type v;
			node(int _l, int _r = -1, value_type _v = 0):
				l(_l), r(_r), v(_v) { }
			friend bool operator < (const node &x, const node &y) {
				return x.l < y.l;
			}
		};
		set<node> s;
		void init(int n) {
			s.insert(node(1, n));
		}
		auto split(int x) {
			auto it = s.lower_bound(node(x));
			if (it != s.end() && it -> l == x) {
				return it;
			}
			--it;
			int _l = it -> l, _r = it -> r;
			auto _v = it -> v;
			s.erase(it);
			s.insert(node(_l, x - 1, _v));
			return s.insert(node(x, _r, _v)).first;
		}
		inline void assign(int l, int r, value_type v) {
			auto itr = split(r + 1), itl = split(l);
			s.erase(itl, itr);
			s.insert(node(l, r, v));
		}
		inline void add(int l, int r, value_type v) {
			auto itr = split(r + 1), itl = split(l);
			for (auto it = itl; it != itr; ++it) {
				it -> v += v;
			}
		}
		inline void mul(int l, int r, value_type v) {
			auto itr = split(r + 1), itl = split(l);
			for (auto it = itl; it != itr; ++it) {
				it -> v *= v;
			}
		}
		inline void mod(int l, int r, value_type v) {
			auto itr = split(r + 1), itl = split(l);
			for (auto it = itl; it != itr; ++it) {
				Mod<value_type>(it -> v, v);
			}
		}
		inline void sqrt(int l, int r) {
			auto itr = split(r + 1), itl = split(l);
			for (auto it = itl; it != itr; ++it) {
				it -> v = sqrt(it -> v);
			}
		}
		inline value_type sum(int l, int r) {
			auto itr = split(r + 1), itl = split(l);
			value_type ret = 0;
			for (auto it = itl; it != itr; ++it) {
				ret += it -> v;
			}
			return ret;
		}
		inline int kth(int l, int r, value_type k) {
			auto itr = split(r + 1), itl = split(l);
			vector<pair<value_type, int> > vec;
			for (auto it = itl; it != itr; ++it) {
				vec.emplace_back(it -> v, (it -> r) - (it -> l) + 1);
			}
			sort(vec.begin(), vec.end());
			for (const auto &u : vec) {
				k -= u.second;
				if (k <= 0) return u.first; 
			}
			return -1;
		}
		inline int count(int l, int r) {
			auto itr = split(r + 1), itl = split(l);
			set<value_type> st;
			for (auto it = itl; it != itr; ++it) {
				if (it -> v) {
					st.insert(it -> v);
				}
			}
			return st.size();
		}
		void maintask(int n, int m) {
			init();
			for (int i = 1; i <= m; i++) {
				int l, r;
				cin >> l >> r;
				assign(l, r, i);
			}
			cout << count(1, n) << "\n";
		}
	};
}

#endif
