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
* 产生式类
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



	//字符串分割函数
	std::vector<std::string> split(std::string str,std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str+=pattern;//扩展字符串以方便操作
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

class CFG{		//文法类
private:
	char terminals[50];		//终结符
	int terninals_count;	
	char nonterminals[50];		//非终结符
	int nonterminals_count;
	Production pro[100];	//产生式
	int productions_count;		
	vector<char> first[50]; 	//first集合
	vector<char> follow[50];	//follow集合
	Index terminals_index;	//终结符的索引
	Index nonterminals_index;	//非终极符的索引

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

		//消除左递归和左因子
		//eleminate_left_recursion();
		//eleminate_left_divisor();
	}

	void eleminate_left_recursion(){
		for(int i = nonterminals_count - 2; i >= 0; i--){
			//i从n-2到0，temp是下标为i的产生式，it是temp.production的迭代器
			Production temp = pro[i];
			Vector_Int_Iteraror it;
			std::vector<string> v;
			for(int j = nonterminals_count - 1; j >= i + 1; j--){
				//j从n-1到i-1
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
			* 消除Ai的直接左递归
			*/
			//先判断是否有直接左递归
			int flag = 0;
			for(std::vector<string>::iterator it1 = temp.production_isolation.begin(); 
				it1 != temp.production_isolation.end(); it1++){
					if((*it1)[0] == temp.left){
						flag = 1;
						break;
					}
			}


			//如果有直接进做递归则做这一步
			if(flag == 1){
				//先把所有的产生式后移，并修改相应的非终结符
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

				//创建新的非终结符，并创建production对象。
				vector<string> new_production_isolation1;
				vector<string> new_production_isolation2;
				pro[i + 1].left = pro[i].left + 1;	//pro[i]就是temp
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

				//更新下标i以前的pro
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
		//待实现（计算first集合）
		for(int i = productions_count - 1; i >= 0; i--){
			Production p_temp = pro[i];
			vector<string>::iterator it;

			//先判断第一个isolation是不是！，如果是则单独处理
			it = p_temp.production_isolation.begin();
			if(*it == "!"){
				first[i].push_back('!');
				it++;
			}

			//接下来处理！以外的isolation
			for( ; it != p_temp.production_isolation.end(); it++){
				for(int j = 0; j < (*it).size(); j++){
					if((*it)[j] >= 97){
						//是终结符
						first[i].push_back((*it)[j]);
						break;
					}else if((*it)[j] >= 65){
						//是非终结符
						for(vector<char>::iterator it_char = 
							first[nonterminals_index[(*it)[j]]].begin();
							it_char != first[nonterminals_index[(*it)[j]]].end(); it_char++){
								first[i].push_back(*it_char);
						}
						if(first[nonterminals_index[(*it)[j]]][0] == '!'){
							//如果终结符的first集合包含！，则继续下一个字符
							continue;
						}else{
							//如果终结符的first集合不包含！，则退出
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
			cout<<nonterminals[i]<<"的first：";
			for(vector<char>::iterator it = first[i].begin(); it != first[i].end(); it++ ){
				cout<<*it;
			}
			cout<<endl;
		}
	}

	void calculate_first_genaral(vector<char>& vc, string s){
		for(int j = 0; j < s.size(); j++){
			if(s[j] >= 97){
				//是终结符
				vc.push_back(s[j]);
				break;
			}else if(s[j] >= 65){
				//是非终结符
				for(vector<char>::iterator it_char = 
					first[nonterminals_index[s[j]]].begin();
					it_char != first[nonterminals_index[s[j]]].end(); it_char++){
						vc.push_back(*it_char);
				}
				if(first[nonterminals_index[s[j]]][0] == '!'){
					//如果终结符的first集合包含！，则继续下一个字符
					continue;
				}else{
					//如果终结符的first集合不包含！，则退出
					break;
				}
			}
		}
		sort(vc.begin(), vc.end());
	}

	void display_vc(vector<char>& vc){
		for(vector<char>::iterator it = vc.begin(); it != vc.end(); it++){
			cout<<"vc的值是"<<*it;
		}
		cout<<endl;
	}

	void calculate_follow(){
		//待实现（计算follow集合）
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
			cout<<nonterminals[i]<<"的follow：";
			for(vector<char>::iterator it = follow[i].begin(); it != follow[i].end(); it++ ){
				cout<<*it;
			}
			cout<<endl;
		}
	}
};

class Predicting_Analysis_Table{		//预测分析表类
private:
	Index terminals_index;	//终结符的索引
	Index nonterminals_index;	//非终结符的索引
	string predicting_analysis_table[50][50];	//预测分析表

public:
	Predicting_Analysis_Table(CFG cfg){
		//待实现，构造预测分析表
	}
};

class Symbol_Table_Item{		//符号表中的一条记录
private:
	string internal_identification;		//记号的内部表示
	string external_identification;		//记号的外部表示
	char grammar_identification;	//记号在文法中的表示
	int token_index;	//记号的类型编号
	int address;	//如果是标识符，则存储记号的地址
	int size;	//如果是标识符，则存储在内存中所占的大小
};

class Symbol_Table_Manager{		//符号表管理器类
private:
	Symbol_Table_Item symbol_table[1000];	//符号表
	int Symbol_Table_Item_Count;
};

class Analysis_Tree_Node{		//该类代表分析树中的结点，其指针类型代表一颗分析树
private:
	int index_in_symbol_table;	//元素在符号表中的下标
	Analysis_Tree_Node *firstchild, *nextsibling;		//树的二叉链表表示法
};

typedef Analysis_Tree_Node *Analysis_Tree;		//分析树

class LL1_Driver{		//驱动器类，实现LL(1)驱动器算法
private:
	stack<char> symbol_stack;	//符号栈
	stack<int> index_in_symbol_table_stack;	//存储符号栈中的元素在符号表中对应的下标,两个栈保持动作的同步 

public:
	Analysis_Tree run_driver(Symbol_Table_Manager stm, Predicting_Analysis_Table pat){
		Analysis_Tree ret = new Analysis_Tree_Node();
		//待实现(驱动器算法)
		return ret;
	}
};

int main(){
	//char terminals[50] = {'k', 'i', 'n', '+', '-', '=', '*', '/', '(', ')', '{', '}'};		//终结符
	//int terninals_count = 12;	
	//char nonterminals[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};		//非终结符
	//int nonterminals_count = 8;		
	//string pro_array[50] = {"B(){C}", "kiD", ",iD|;|!", "E;C|!", "i=F|B", "F+G|F-G|G", "G*H|G/H|H", "(F)|i|n"};
	//int productions_count = 8;

	char terminals[50] = "abcde";		//终结符
	int terninals_count = 5;	
	char nonterminals[50] = {"ABCD"};		//非终结符
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