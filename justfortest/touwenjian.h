#include <map>
#include <stack>
typedef map<char, int> Index;

class CFG{		//文法类
private:
	char terminals[50];		//终结符
	int terninals_count;	
	char nonterminals[50];		//非终结符
	int nonterminals_count;
	string productions[100];	//产生式
	int productions_count;
	string first[50];	//first集合
	string follow[50];	//follow集合
	Index terminals_index;	//终结符的索引
	Index nonterminals_index;	//非终极符的索引
	Index nonterminals_index_in_productions;	//产生式的索引

public:
	CFG(char* T, int T_count, char* N, int N_count, string* P, int P_count){
		//待实现，初始化CFG，并进行消左递归和左因子操作（消左递归和左因子也可以单独写一个函数）
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
};

class Analysis_Tree_Node{		//该类代表分析树中的结点，其指针类型代表一颗分析树
private:
	int index_in_symbol_table;	//终结符在符号表中的下标或者非终结符在CFG中的下标
	Analysis_Tree_Node *firstchild, *nextsibling;		//二叉树的二叉链表表示法
};

typedef Analysis_Tree_Node *Analysis_Tree;		//分析树

class LL1_Driver{		//驱动器类，实现LL(1)驱动器算法
private:
	stack<char> symbol_stack;	//符号栈

public:
	Analysis_Tree run_driver(Symbol_Table_Manager stm, Predicting_Analysis_Table pat){
		Analysis_Tree ret = new Analysis_Tree_Node();
		//待实现(驱动器算法)
		return ret;
	}
};