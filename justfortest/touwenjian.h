#include <map>
#include <stack>
typedef map<char, int> Index;

class CFG{		//�ķ���
private:
	char terminals[50] = {'k', 'i', 'n', '+', '-', '=', '*', '/', '(', ')', '{', '}'};		//�ս��
	int terninals_count = 12;	
	char nonterminals[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};		//���ս��
	int nonterminals_count = 8;
	string productions[100] = {"AB(){C}", "B!", "BkiD", "D!", 
	"D,iD", "D;", "C!", "CE;C", "Ei=F", "EB", "FF+G", "FF-G", "FG", "GG*H", "GG/H", "GH", "H(F)", "Hi", "Hn"};	//����ʽ
	int productions_count = 19;		
	string first[50];	//first����
	string follow[50];	//follow����
	Index terminals_index;	//�ս��������
	Index nonterminals_index;	//���ռ���������
	Index nonterminals_index_in_productions;	//����ʽ������

public:
	CFG(char* T, int T_count, char* N, int N_count, string* P, int P_count){
		//��ʵ�֣���ʼ��CFG������������ݹ�������Ӳ���������ݹ��������Ҳ���Ե���дһ��������
	}

	CFG(){
		//�ֶ���ʼ��һЩ˽������
		for(int i = 0; i < terninals_count; i++){
			terminals_index[terminals[i]] = i;
		}
		for(int i = 0; i < nonterminals_count; i++){
			nonterminals_index[nonterminals[i]] = i;
		}
		nonterminals_index_in_productions[nonterminals[0]] = 0;
		for(int i = 1, j = 0; i < productions_count; i++){
			if(productions[i][0] != productions[i - 1][0]){
				nonterminals_index_in_productions[nonterminals[j]] = i;
				j++;
			}
		}

		//������ݹ��������
		eleminate_left_recursion();
		eleminate_left_divisor();
	}

	void eleminate_left_recursion(){
		for(int i = 0; i < productions_count; i++){
			if(productions)
		}
	}

	void eleminate_left_divisor(){

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
	stack<int> index_in_symbol_table_stack	//�洢����ջ�е�Ԫ���ڷ��ű��ж�Ӧ���±� 

public:
	Analysis_Tree run_driver(Symbol_Table_Manager stm, Predicting_Analysis_Table pat){
		Analysis_Tree ret = new Analysis_Tree_Node();
		//��ʵ��(�������㷨)
		return ret;
	}
};