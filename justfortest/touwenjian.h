#include <map>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
typedef map<char, int> Index;
typedef std::vector<string>::iterator Vector_Int_Iteraror

/**
 * ����ʽ��
 */
class Production{
private:
	string left;
	string production_union;
	vector<string> production_isolation;
	int isolation_count;

public:
	Production(string l, string s){
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
   		    if(pos<size)
   		    {
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
 		s = "";
 		std::vector<string>::iterator it;
 		for(it = v.begin(); it.next() != v.end(); it++){
 			s += *it;
 			s += "|";
 		}
 		s += *it;
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
	string first[50]; 	//first����
	string follow[50];	//follow����
	Index terminals_index;	//�ս��������
	Index nonterminals_index;	//���ռ���������

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
			pro[i] = new Production(nonterminals[i], p[i]);
		}

		//������ݹ��������
		eleminate_left_recursion();
		eleminate_left_divisor();
	}

	void eleminate_left_recursion(){
		for(int i = nonterminals_count - 2; i >= 0; i--){
			//i��n-2��0��temp���±�Ϊi�Ĳ���ʽ��it��temp.production�ĵ�����
				Production temp = pro[i];
				Vector_Int_Iteraror it;
				std::vector<string> v;
			for(int j = nonterminals_count - 1; j >= i + 1; j--){
				//j��n-1��i-1
				for(it = temp.production_isolation.begin(); it != temp.end(); it++){
						if((*it)[0] == pro[j].left){
							for(int ind_1 = 0; ind_1 < pro[j].isolation_count; ind_1++){
								temp.production_isolation.push_back(pro[j].production_isolation[ind_1] + (*it).substr(1, (*it).size() - 1));
								temp.production_isolation.erase(it);
							}
						}

					}
				}
				/**
				 * ����Ai��ֱ�����ݹ�
				 */
				//���ж��Ƿ���ֱ�����ݹ�
				int flag = 0;
				for(std::vector<string> it1 = temp.production_isolation.begin(); it != temp.end(); it++)

				//�����ֱ�ӽ����ݹ�������һ��
				if(flag == 1){
					//�Ȱ����еĲ���ʽ���ƣ����޸���Ӧ�ķ��ս��
					for(int k1 = nonterminals_count; k1 >= i + 2; k--){
						pro[k1] = pro[k1 - 1];
						pro[k1].left++;
							for(int k2 = 0; k2 < pro[k1].size(); k2++){
								if(pro[k1].production_union[k2] - pro[i].left >= 0 && pro[k1].production_union[k2] - '\0' < 91){
									pro[k1].production_union[k2]++;
								}
							}
							union_to_isolation(pro[k1].production_union, pro[k1].production_isolation);
							pro[k1].isolation_count = pro[k1].production_isolation.size();
						}

						//�����µķ��ս����������production����
				

			}
		}
	}

	void eleminate_left_divisor(){

	}

	vector<char> calculate_first(string s1){
		vector<char> ret;
		//��ʵ�֣�����first���ϣ�
		for(int i = productions_count - 1; i >= 0; i--){
			Production p_temp = pro[i];
			vector<string>::Iterator it;
			for(it = p_temp.productio_isolation.start(); it != p_temp.production_isolation.end(); it++){
				for(int j = 0; j < (*it).size(); j++){
					if((*it)[j] >= 97){
						ret.put_back();
						break;
					}else if((*it)[j] >= 65 && first[nonterminal_index[(*it)[j]][0] == "!"){
						ret.put_back()
					}

				}
			}
		} 
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
	stack<int> index_in_symbol_table_stack;	//�洢����ջ�е�Ԫ���ڷ��ű��ж�Ӧ���±�,����ջ���ֶ�����ͬ�� 

public:
	Analysis_Tree run_driver(Symbol_Table_Manager stm, Predicting_Analysis_Table pat){
		Analysis_Tree ret = new Analysis_Tree_Node();
		//��ʵ��(�������㷨)
		return ret;
	}
};

int main(){
	char terminals[50] = {'k', 'i', 'n', '+', '-', '=', '*', '/', '(', ')', '{', '}'};		//�ս��
	int terninals_count = 12;	
	char nonterminals[50] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};		//���ս��
	int nonterminals_count = 8;		
	string pro_array[50] = {"B(){C}", "kiD", ",iD|;|!", "E;C|!", "i=F|B", "F+G|F-G|G", "G*H|G/H|H", "(F)|i|n"};
	int productions_count = 8;

	return 0;
}