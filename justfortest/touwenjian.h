#include <map>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
typedef map<char, int> Index;

class Production{
private:
	string production_union;
	vector<string> production_isolation;
	int isolation_count;

public:
	Production(string s){
		production_union = s;
		production_isolation = split(production_union, "|");
		sort(production_isolation.begin(), production_isolation.end());
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
   		    if(pos<size)
   		    {
   		        std::string s=str.substr(i,pos-i);
   		        result.push_back(s);
   		        i=pos+pattern.size()-1;
   		    }
    	}	
     	return result;
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
	string first[50]; 	//first集合
	string follow[50];	//follow集合
	Index terminals_index;	//终结符的索引
	Index nonterminals_index;	//非终极符的索引

public:

	CFG(char* t, int t_count, char* n, int n_count, string* p, int p_count){
		for(int i = 0; i < t_count; i++){
			terminals[i] = t[i];
			terminals_index[terminals[i]] = i;
		}
		for(int i = 0; i < n_count; i++){
			nonterminals[i] = n[i];
			nonterminals_index[nonterminals[i]] = i;
		}
		
		for(int i = 0; i < p_count; i++){
			pro[i] = new Production(p[i]);
		}

		//消除左递归和左因子
		eleminate_left_recursion();
		eleminate_left_divisor();
	}

	void eleminate_left_recursion(){
		for(int i = nonterminals_count - 2; i >= 0; i--){
			for(int j = nonterminals_count - 1; j >= i + 1; j--){
				
			}
		}
	}

	void eleminate_left_divisor(){

	}

	string calculate_first(string s1){
		string ret;
		//待实现（计算first集合）
		return ret;
	}

	string calculate_follow(string s1){
		string ret;
		//待实现（计算follow集合）
		return ret;
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
	char terminals[50] = {'k', 'i', 'n', '+', '-', '=', '*', '/', '(', ')', '{', '}'};		//终结符
	int terninals_count = 12;	
	char nonterminals[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};		//非终结符
	int nonterminals_count = 8;		
	string pro_array[50] = {"B(){C}", "kiD", ",iD|;|!", "E;C|!", "i=F|B", "F+G|F-G|G", "G*H|G/H|H", "(F)|i|n"};
	int productions_count = 8;

	return 0;
}