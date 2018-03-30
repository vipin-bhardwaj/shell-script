
#ifndef SUFFIX 
#define SUFFIX
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

class Suffix{
	int *SA;
	int n;
	int *LCP;
	char *str;
	struct node{
		int index;
		int rank[2];
		bool operator<(const node &a) {
			if(rank[0] == a.rank[0]) return rank[1] < a.rank[1];
			return rank[0] < a.rank[0];
		}
	};

	public:
	Suffix(const char *, const int len);
	Suffix(){
		n = 0;
	}
	void printSA(){
		for(int i = 0; i < n; ++i) {
			cout << SA[i] << " ";
		}
		cout << "\n";
	}
	void buildLCP();
	void printLCP() {
		int *temp = SA;
		SA = LCP;
		printSA();
		SA = temp;
	}

	~Suffix(){
		delete [] SA;
		delete [] LCP;
	}
};

Suffix::Suffix(const char * s, const int len): n(len) {
	str = new char[n+1];
	for(int i = 0; i < n; ++i) {
		str[i] = s[i];
	}

	Suffix::node suffix[n];

	// preparing for sorting according to first two characters
	for(int i = 0; i < n; ++i) {
		suffix[i].index = i;
		suffix[i].rank[0] = str[i] - 0;
		suffix[i].rank[1] = (i+1 < n)? str[i+1] - 0: -1;
	}
	sort(suffix, suffix + n);
	int location[n];

	for (int k = 2; k < n; k*= 2) {
		//2*k represents the no of charcters to be compared
		int prev_rank = suffix[0].rank[0], nw_rnk = 0;
		suffix[0].rank[0] = nw_rnk;
		location[suffix[0].index] = 0;
		//updating the new ranking 
		for(int i = 1; i < n; ++i) {
			if(suffix[i].rank[0] == prev_rank and suffix[i].rank[1] == suffix[i-1].rank[1]) {
				suffix[i].rank[0] = nw_rnk;
			} else {
				prev_rank = suffix[i].rank[0];
				suffix[i].rank[0] = ++nw_rnk;
			}
			location[suffix[i].index] = i;
		}

		// if this happens it means all are already sorted
		if(nw_rnk == n){
			break;
		}

		//prepare for the next sorting by updating rank[1]

		for(int i = 0; i < n; ++i) {
			
			int nxt_ind = suffix[i].index + k;
			suffix[i].rank[1] = (nxt_ind < n)? suffix[ location[nxt_ind] ].rank[0]: -1;
		}
		sort(suffix, suffix + n);

	}
	SA = new int[n];
	for(int i = 0; i < n; ++i) {
		SA[i] = suffix[i].index;
	}

}

void Suffix::buildLCP() {
	int phi[n];  // phi[i] stores the index of preceding suffix in the 
					//Suffix array for the suffix starting at ith index
	phi[ SA[0] ] = -1;
	LCP = new int[n];
	for(int i = 1; i < n; ++i) {
		phi[ SA[i] ] = SA[i-1];
	}
	
	int plcp[n];  // plcp[i] stores the lcp of the suffix starting at index i
	for(int i = 0, L = 0; i < n; ++i) {
		if(phi[i] == -1) {
			plcp[i] = 0;
			continue;
		}

		while( phi[i] + L < n and str[i + L] == str[phi[i] + L]) L++;
		plcp[i] = L;
		L = max(L-1, 0); //atleast L-1 characters will match for the next suffix
	}
	for(int i = 0; i < n; ++i) {
		LCP[i] = plcp[SA[i]];
		
	}
}
#endif