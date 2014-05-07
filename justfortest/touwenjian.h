#include <map>
#include <stack>
typedef map<char, int> Index;

class CFG{		//�ķ���
private:
	char terminals[50];		//�ս��
	int terninals_count;	
	char nonterminals[50];		//���ս��
	int nonterminals_count;
	string productions[100];	//����ʽ
	int productions_count;
	string first[50];	//first����
	string follow[50];	//follow����
	Index terminals_index;	//�ս��������
	Index nonterminals_index;	//���ռ���������
	Index nonterminals_index_in_productions;	//����ʽ������

public:
	CFG(char* T, int T_count, char* N, int N_count, string* P, int P_count){
		//��ʵ�֣���ʼ��CFG������������ݹ�������Ӳ���������ݹ��������Ҳ���Ե���дһ��������
	}

	string calculate_first(string s1){
		string ret;
		//��ʵ�֣�����first���ϣ�
		return ret;
	}

	string calculate_follow(string s1){
		string ret;
		//��ʵ�֣�����follow���ϣ�
		return ret;
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
};

class Analysis_Tree_Node{		//�������������еĽ�㣬��ָ�����ʹ���һ�ŷ�����
private:
	int index_in_symbol_table;	//�ս���ڷ��ű��е��±���߷��ս����CFG�е��±�
	Analysis_Tree_Node *firstchild, *nextsibling;		//�������Ķ��������ʾ��
};

typedef Analysis_Tree_Node *Analysis_Tree;		//������

class LL1_Driver{		//�������࣬ʵ��LL(1)�������㷨
private:
	stack<char> symbol_stack;	//����ջ

public:
	Analysis_Tree run_driver(Symbol_Table_Manager stm, Predicting_Analysis_Table pat){
		Analysis_Tree ret = new Analysis_Tree_Node();
		//��ʵ��(�������㷨)
		return ret;
	}
};