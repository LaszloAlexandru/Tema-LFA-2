#include <iostream>
#include<stack>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;
ifstream in("date.in");
class tran {
	char sym;
	int notran;
public:
	int tostates[50];
	void readNo(int);
	void readSym(char);
	void readToState(int);
	int getNoTran() { return notran; }
	int getState(int);
	char getSym();
};
class state {
	int no;
	int notrlist;
public:
	state() :no(0) {};
	tran tranlist[50];
	void read(int);
	int getNo() { return no; }
	void identNoTr() {
		notrlist++;
	}
	int getTr() {
		return notrlist;
	}
};
class dstate {
	char name;
	string statestring;
	int is_final;
public:
	char trans[10];
	void setName(char c) {
		name = c;
	}
	void addString(const char c) {
		statestring+=c;
	}
	void cpyString(string x) {
		statestring = x;
	}
	string getstring() {
		return statestring;
	}
	char getName() {
		return name;
	}
	int getFin() {
		return is_final;
	}
	void setFin(int x) {
		is_final = x;
	}

};

dstate dstates[50];
state states[50];
stack<int> A, B;
int cptr = -1, aptr = -1, bptr = -1;
string temp;
vector<char> s;
int nos, nof, noi, j, k, nods = -1;

void state::read(int x) {
	no = x;
}
void tran::readNo(int x) {
	notran = x;
}
void tran::readSym(char s) {
	sym = s;
}
void tran::readToState(int x) {
	in >> tostates[x];
}
int tran::getState(int x) {
	return tostates[x];
}
char tran::getSym() {
	return sym;
}
int seek(stack<int> x, int st) {
	vector<int> t;
	while (!x.empty()) {
		t.push_back(x.top());
		x.pop();
	}
	for (int i = 0; i < t.size(); i++) {
		if (st == t[i])return 1;
	}
	return 0;
}
void tostring(stack<int>y) {
	vector<int> x;
	temp.clear();
	while (!y.empty()) {
		x.push_back(y.top());
		y.pop();
	}
	sort(x.begin(), x.end());
	for (int i = 0; i < x.size(); i++) {
		char c;
		c = x[i] + '0';
		temp += c;
	}
}
void copy(int i) {
	string temp;
	int k = 0,l;
	while (!B.empty())B.pop();
	temp = dstates[i].getstring();
	while (k < temp.size()) {
		l = temp[k] - '0';
		B.push(l);
		k++;
	}
}
void move(int st, int j) {
	int ctr = 0;
	while (ctr < states[st].tranlist[j].getNoTran()) {
		A.push(states[st].tranlist[j].tostates[ctr++]);
	}
}
void lambdaClosure(int st) {
	int ctr = 0, in_state = st, curst = st, chk;
	while (!A.empty()) {
		curst = A.top();
		A.pop();
		ctr = 0;
		in_state = curst;
		while (ctr <= states[curst].tranlist[noi].getNoTran()) {
			chk = seek(B, in_state);
			if (chk == 0)B.push(in_state);
			in_state = states[curst].tranlist[noi].tostates[ctr++];
			chk = seek(A, in_state);
			if (chk == 0 && ctr <= states[curst].tranlist[noi].getNoTran())A.push(in_state);
		}
	}
}
void display() {
	cout<<"\n\t\t DFA Transition Table ";
	cout<<"\n\t\t -------------------- ";
	cout<<"\nStates\tString\tInputs\n ";
	for (int i = 0; i<noi; i++)
	{
	cout<<s[i];
	}
	cout<<"\n \t----------";
	for (int i = 0; i<nods; i++)
	{
		if (dstates[i].getFin()== 0)
			cout<<"\n"<<dstates[i].getName();
		else
			cout<<"\n*"<<dstates[i].getName();
		cout<<"\t"<< dstates[i].getstring();
		for (j = 0; j<noi; j++)
		{
			cout<<"\t"<< dstates[i].trans[j];
		}
	}
	cout << '\n';
}
int main() {
	vector<int> finals;
	char v, ans;
	int  start, x, fin, i;
	cout << "Citeste nr de stari: ";
	cin >> nos;
	for (i = 0; i < nos; i++) {
		states[i].read(i);
	}
	cout << "Citeste starea initiala: ";
	cin >> start;
	cout << "Citeste nr de stari finale: ";
	cin >> nof;
	cout << "Citeste starile finale: ";
	for (i = 0; i < nof; i++) {
		cin >> x;
		finals.push_back(x);
	}
	cout << "Citeste nr de simboluri: ";
	cin >> noi;
	cout << "Citeste simbolurile:\n";
	for (i = 0; i < noi; i++) {
		cin >> v;
		s.push_back(v);
	}
	s.push_back('e');
	for (i = 0; i < nos; i++) {
		for (j = 0; j <= noi; j++) {
			states[i].tranlist[j].readSym(s[j]);
			k = 0;
			ans = 'y';
			while (ans == 'y')
			{
				cout << "Move " << i << " " << s[j] << " ";
				states[i].tranlist[j].readToState(k++);
				if (states[i].tranlist[j].getState(k - 1) == -1)
				{
					k--; ans = 'n';
					break;
				}
				states[i].tranlist[j].readNo(k);
			}
		}
	}
	i = 0; nods = 0; fin = 0;
	A.push(start);
	lambdaClosure(A.top());
	tostring(B);
	dstates[nods].setName('A');
	nods++;
	dstates[0].cpyString(temp);
	while (i < nods) {
		for (j = 0; j < noi; j++) {
			fin = 0;
			copy(i);
			while (!B.empty()) {
				move(B.top(), j);
				B.pop();
			}
			while (!A.empty())lambdaClosure(A.top());
			tostring(B);
			for (k = 0; k < nods; k++) {
				if (temp == dstates[k].getstring()) {
					dstates[i].trans[j] = dstates[k].getName();
					break;
				}
			}
			if (k == nods) {
				nods++;
				for (k = 0; k < nof; k++)
				{
					fin = seek(B, finals[k]);
					if (fin == 1)
					{
						dstates[nods-1].setFin(1);
						break;
					}
				}
				char a;
				a = 'A' + nods-1;
				dstates[nods-1].cpyString(temp);
				dstates[nods-1].setName(a);
				dstates[i].trans[j] = dstates[nods-1].getName();
				
			}
		}
		i++;
	}
	display();

	in.close();
	cout << "\n\n\n\nACESTASTA TEMA A NECESITAT 6 ORE DE DEBUGGING SI 3 BERI PENTRU A FI REALIZATA";
	system("pause");
	return 0;

}