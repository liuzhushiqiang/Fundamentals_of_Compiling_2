#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef map<char, int> Index;
typedef std::vector<string>::iterator Vector_Int_Iteraror;

/**
* ����ʽ��
*/
class Production{
public:
	char left;
	string production_union;
	vector<string> production_isolation;
	int isolation_count;

public:

	void init(char l, string s){
		left = l;
		production_union = s;
		union_to_isolation(s, production_isolation);
		isolation_count = production_isolation.size();
	}



	//�ַ����ָ��
	std::vector<std::string> split(std::string str,std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str+=pattern;//��չ�ַ����Է������
		int size=str.size();	
		for(int i=0; i<size; i++)
		{
			pos=str.find(pattern,i);
			if(pos<size){
				std::string s=str.substr(i,pos-i);
				result.push_back(s);
				i=pos+pattern.size()-1;
			}
		}	
		return result;
	}

	/**
	* [union_to_isolation description]
	* @param s [description]
	* @param v [description]
	*/
	void union_to_isolation(string s, std::vector<string>& v){
		v = split(production_union, "|");
		sort(v.begin(), v.end());
	}

	/**
	* [isolation_to_union description]
	* @param s [description]
	* @param v [description]
	*/
	void isolation_to_union(string& s, std::vector<string> v){
		std::vector<string>::iterator it = v.begin();
		s = *it;
		for(it++; it != v.end(); it++){
			s += *it;
			s += "|";
		}
		s += *it;
	}

	void format_from_production_isolation(){
		sort(production_isolation.begin(), production_isolation.end());
		isolation_to_union(production_union, production_isolation);
		isolation_count = production_isolation.size();
	}

};

class CFG{		//�ķ���
private:
	char terminals[50];		//�ս��
	int terninals_count;	
	char nonterminals[50];		//���ս��
	int nonterminals_count;
	Production pro[100];	//����ʽ
	int productions_count;		
	vector<char> first[50]; 	//first����
	vector<char> follow[50];	//follow����
	Index terminals_index;	//�ս��������
	Index nonterminals_index;	//���ռ���������

public:

	CFG(char* t, int t_count, char* n, int n_count, string* p, int p_count){
		terninals_count = t_count;
		for(int i = 0; i < t_count; i++){
			terminals[i] = t[i];
			terminals_index[terminals[i]] = i;
		}
		nonterminals_count = n_count;
		for(int i = 0; i < n_count; i++){
			nonterminals[i] = n[i];
			nonterminals_index[nonterminals[i]] = i;
		}

		productions_count = p_count;
		for(int i = 0; i < p_count; i++){
			pro[i].init(nonterminals[i], p[i]);
		}

		//������ݹ��������
		//eleminate_left_recursion();
		//eleminate_left_divisor();
	}

	void eleminate_left_recursion(){
		for(int i = nonterminals_count - 2; i >= 0; i--){
			//i��n-2��0��temp���±�Ϊi�Ĳ���ʽ��it��temp.production�ĵ�����
			Production temp = pro[i];
			Vector_Int_Iteraror it;
			std::vector<string> v;
			for(int j = nonterminals_count - 1; j >= i + 1; j--){
				//j��n-1��i-1
				for(it = temp.production_isolation.begin(); it != temp.production_isolation.end(); it++){
					if((*it)[0] == pro[j].left){
						for(int ind_1 = 0; ind_1 < pro[j].isolation_count; ind_1++){
							temp.production_isolation.push_back(pro[j].production_isolation[ind_1] + (*it).substr(1, (*it).size() - 1));
							temp.production_isolation.erase(it);
						}
					}

				}
			}
			/**
			* ����Ai��ֱ����ݹ�
			*/
			//���ж��Ƿ���ֱ����ݹ�
			int flag = 0;
			for(std::vector<string>::iterator it1 = temp.production_isolation.begin(); 
				it1 != temp.production_isolation.end(); it1++){
					if((*it1)[0] == temp.left){
						flag = 1;
						break;
					}
			}


			//�����ֱ�ӽ����ݹ�������һ��
			if(flag == 1){
				//�Ȱ����еĲ���ʽ���ƣ����޸���Ӧ�ķ��ս��
				for(int k1 = nonterminals_count; k1 >= i + 2; k1--){
					pro[k1] = pro[k1 - 1];
					pro[k1].left++;
					for(int k2 = 0; k2 < pro[k1].production_union.size(); k2++){
						if(pro[k1].production_union[k2] - pro[i].left > 0 && pro[k1].production_union[k2] - '\0' < 91){
							pro[k1].production_union[k2]++;
						}
					}
					pro[k1].union_to_isolation(pro[k1].production_union, pro[k1].production_isolation);
					pro[k1].isolation_count = pro[k1].production_isolation.size();
				}

				//�����µķ��ս����������production����
				vector<string> new_production_isolation1;
				vector<string> new_production_isolation2;
				pro[i + 1].left = pro[i].left + 1;	//pro[i]����temp
				for(std::vector<string>::iterator it1 = temp.production_isolation.begin(); 
					it1 != temp.production_isolation.end(); it1++){
						if((*it1)[0] == temp.left){
							new_production_isolation2.push_back((*it).substr(1, 
								(*it).size() - 1) + pro[i + 1].left);
							//temp.production_isolation.erase(it);
						}else{
							new_production_isolation1.push_back((*it) + pro[i + 1].left);
						}
				}
				pro[i].production_isolation = new_production_isolation1;
				pro[i].format_from_production_isolation();
				pro[i + 1].production_isolation = new_production_isolation2;
				pro[i + 1].format_from_production_isolation();

				//�����±�i��ǰ��pro
				for(int k1 = nonterminals_count - 1; k1 >= 0; k1--){
					for(int k2 = 0; k2 < pro[k1].production_union.size(); k2++){
						if(pro[k1].production_union[k2] - pro[i].left > 0 && pro[k1].production_union[k2] - '\0' < 91){
							pro[k1].production_union[k2]++;
						}
					}
					pro[k1].union_to_isolation(pro[k1].production_union, pro[k1].production_isolation);
					pro[k1].isolation_count = pro[k1].production_isolation.size();
				}
			}else{
				continue;
			}
		}
	}

	void eleminate_left_divisor(){

	}

	void calculate_first(){
		//��ʵ�֣�����first���ϣ�
		for(int i = productions_count - 1; i >= 0; i--){
			Production p_temp = pro[i];
			vector<string>::iterator it;

			//���жϵ�һ��isolation�ǲ��ǣ���������򵥶�����
			it = p_temp.production_isolation.begin();
			if(*it == "!"){
				first[i].push_back('!');
				it++;
			}

			//���������������isolation
			for( ; it != p_temp.production_isolation.end(); it++){
				for(int j = 0; j < (*it).size(); j++){
					if((*it)[j] >= 97){
						//���ս��
						first[i].push_back((*it)[j]);
						break;
					}else if((*it)[j] >= 65){
						//�Ƿ��ս��
						for(vector<char>::iterator it_char = 
							first[nonterminals_index[(*it)[j]]].begin();
							it_char != first[nonterminals_index[(*it)[j]]].end(); it_char++){
								first[i].push_back(*it_char);
						}
						if(first[nonterminals_index[(*it)[j]]][0] == '!'){
							//����ս����first���ϰ��������������һ���ַ�
							continue;
						}else{
							//����ս����first���ϲ������������˳�
							break;
						}
					}
				}
			}
			sort(first[i].begin(), first[i].end());
		}
	}

	void display_fisrt(){
		for(int i = 0; i < nonterminals_count; i++){
			cout<<nonterminals[i]<<"��first��";
			for(vector<char>::iterator it = first[i].begin(); it != first[i].end(); it++ ){
				cout<<*it;
			}
			cout<<endl;
		}
	}

	void calculate_first_genaral(vector<char>& vc, string s){
		for(int j = 0; j < s.size(); j++){
			if(s[j] >= 97){
				//���ս��
				vc.push_back(s[j]);
				break;
			}else if(s[j] >= 65){
				//�Ƿ��ս��
				for(vector<char>::iterator it_char = 
					first[nonterminals_index[s[j]]].begin();
					it_char != first[nonterminals_index[s[j]]].end(); it_char++){
						vc.push_back(*it_char);
				}
				if(first[nonterminals_index[s[j]]][0] == '!'){
					//����ս����first���ϰ��������������һ���ַ�
					continue;
				}else{
					//����ս����first���ϲ������������˳�
					break;
				}
			}
		}
		sort(vc.begin(), vc.end());
	}

	void display_vc(vector<char>& vc){
		for(vector<char>::iterator it = vc.begin(); it != vc.end(); it++){
			cout<<"vc��ֵ��"<<*it;
		}
		cout<<endl;
	}

	void calculate_follow(){
		//��ʵ�֣�����follow���ϣ�
		follow[0].push_back('#');
		for(int i = 1; i < productions_count; i++){
			for(int j = 0; j < productions_count; j++){
				for(vector<string>::iterator vs = pro[j].production_isolation.begin(); 
					vs != pro[j].production_isolation.end(); vs++){
						for(int k = 0; k < (*vs).size(); k++){
							if((*vs)[k] == pro[i].left){
								calculate_first_genaral(follow[i], 
									(*vs).substr(k + 1, (*vs).size() - k - 1));
								if((follow[i].begin() == follow[i].end() || *(follow[i].begin()) == '!') && j != i){
									if((follow[i].begin()) != follow[i].end()){
										follow[i].erase(follow[i].begin());
									}
									for(vector<char>::iterator it = follow[j].begin(); 
										it != follow[j].end(); it++){
											follow[i].push_back(*it);
									}
									sort(follow[i].begin(), follow[i].end());
								}
							}
						}
				}
			}
		}
	}

	void display_follow(){
		for(int i = 0; i < nonterminals_count; i++){
			cout<<nonterminals[i]<<"��follow��";
			for(vector<char>::iterator it = follow[i].begin(); it != follow[i].end(); it++ ){
				cout<<*it;
			}
			cout<<endl;
		}
	}
};

class Predicting_Analysis_Table{		//Ԥ���������
private:
	Index terminals_index;	//�ս��������
	Index nonterminals_index;	//���ս��������
	string predicting_analysis_table[50][50];	//Ԥ�������

public:
	Predicting_Analysis_Table(CFG cfg){
		//��ʵ�֣�����Ԥ�������
	}
};

class Symbol_Table_Item{		//���ű��е�һ����¼
private:
	string internal_identification;		//�Ǻŵ��ڲ���ʾ
	string external_identification;		//�Ǻŵ��ⲿ��ʾ
	char grammar_identification;	//�Ǻ����ķ��еı�ʾ
	int token_index;	//�Ǻŵ����ͱ��
	int address;	//����Ǳ�ʶ������洢�Ǻŵĵ�ַ
	int size;	//����Ǳ�ʶ������洢���ڴ�����ռ�Ĵ�С
};

class Symbol_Table_Manager{		//���ű��������
private:
	Symbol_Table_Item symbol_table[1000];	//���ű�
	int Symbol_Table_Item_Count;
};

class Analysis_Tree_Node{		//�������������еĽ�㣬��ָ�����ʹ���һ�ŷ�����
private:
	int index_in_symbol_table;	//Ԫ���ڷ��ű��е��±�
	Analysis_Tree_Node *firstchild, *nextsibling;		//���Ķ��������ʾ��
};

typedef Analysis_Tree_Node *Analysis_Tree;		//������

class LL1_Driver{		//�������࣬ʵ��LL(1)�������㷨
private:
	stack<char> symbol_stack;	//����ջ
	stack<int> index_in_symbol_table_stack;	//�洢����ջ�е�Ԫ���ڷ��ű��ж�Ӧ���±�,����ջ���ֶ�����ͬ�� 

public:
	Analysis_Tree run_driver(Symbol_Table_Manager stm, Predicting_Analysis_Table pat){
		Analysis_Tree ret = new Analysis_Tree_Node();
		//��ʵ��(�������㷨)
		return ret;
	}
};

int main(){
	//char terminals[50] = {'k', 'i', 'n', '+', '-', '=', '*', '/', '(', ')', '{', '}'};		//�ս��
	//int terninals_count = 12;	
	//char nonterminals[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};		//���ս��
	//int nonterminals_count = 8;		
	//string pro_array[50] = {"B(){C}", "kiD", ",iD|;|!", "E;C|!", "i=F|B", "F+G|F-G|G", "G*H|G/H|H", "(F)|i|n"};
	//int productions_count = 8;

	char terminals[50] = "abcde";		//�ս��
	int terninals_count = 5;	
	char nonterminals[50] = {"ABCD"};		//���ս��
	int nonterminals_count = 4;		
	string pro_array[50] = {"aBDe", "bC", "!|bcC", "d"};
	int productions_count = 4;
	CFG cfg(terminals, terninals_count, nonterminals, nonterminals_count, pro_array, productions_count);
	cfg.calculate_first();
	cfg.display_fisrt();
	vector<char> vc;
	cfg.calculate_first_genaral(vc, "aBDe");
	cfg.display_vc(vc);
	cfg.calculate_follow();
	cfg.display_follow();

	return 0;
}