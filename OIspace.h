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
			build(rs, mid + 1, r);122537063
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
}

#endif
