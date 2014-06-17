#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef map<char, int> Index;


/**
* ����ʽ��
*/
class Production{
public:
	//����ʽ��
	char left;	
	//����ʽ�Ҳ���������ʽ����"S-->a|b|c", ֻ�������ʽ�Ҳ��ַ�����
	string production_union;
	//����ʽ�Ҳ���������ʽ, ��"S-->a, S-->b, S-->c"��ֻ�������ʽ�Ҳ��ַ�����
	vector<string> production_isolation;	

public:
	//��ʼ��
	void init(char l, string s){
		left = l;
		production_union = s;
		union_to_isolation(production_union, production_isolation);
	}



	//�ַ����ָ��������union_to_isolation����
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
	* [union_to_isolation �Ӳ���ʽ��union��ʽ����isolation��ʽ]
	* @param s [description]
	* @param v [description]
	*/
	void union_to_isolation(string s, std::vector<string>& v){
		v = split(production_union, "|");
		sort(v.begin(), v.end());
	}

	/**
	* [isolation_to_union �Ӳ���ʽ��isolation��ʽ����union��ʽ]
	* @param s [description]
	* @param v [description]
	*/
	void isolation_to_union(string& s, std::vector<string> v){
		std::vector<string>::iterator it = v.begin();
		s = *it;
		for(it++; it != v.end(); it++){
			s += "|";
			s += *it;
		}
	}

	//��isolation���������ʽ����������union���൱�����¸�ʽ��production��
	void format_from_production_isolation(){
		sort(production_isolation.begin(), production_isolation.end());
		isolation_to_union(production_union, production_isolation);
	}

};

/************************************************************************/
/* �ķ���                                                               */
/************************************************************************/
class CFG{
public:
	char terminals[50];		//�ս��
	int terninals_count;	
	char nonterminals[50];		//���ս��
	int nonterminals_count;
	Production pro[100];	//����ʽ
	int productions_count;		
	vector<char> first[50]; 	//first����
	vector<char> follow[50];	//follow����

public:

	CFG(char* t, int t_count, char* n, int n_count, string* p, int p_count){
		terninals_count = t_count;
		for(int i = 0; i < t_count; i++){
			terminals[i] = t[i];
		}
		nonterminals_count = n_count;
		for(int i = 0; i < n_count; i++){
			nonterminals[i] = n[i];
		}

		productions_count = p_count;
		for(int i = 0; i < p_count; i++){
			pro[i].init(nonterminals[i], p[i]);
		}
	}


	/************************************************************************/
	/* by liuzhushiqiang Start                                              */
	/************************************************************************/

	/************************************************************************/
	/* ������ݹ�                                                            */
	/************************************************************************/
	void eleminate_left_recursion(){
		//i��n-2��0
		for(int i = nonterminals_count - 2; i >= 0; i--){
			Production temp = pro[i];
			//j��n-1��i+1
			for(int j = nonterminals_count - 1; j >= i + 1; j--){
				//��ִ���滻
				for(vector<string>::iterator it = temp.production_isolation.begin(); 
					it != temp.production_isolation.end(); 
					it++){
						if((*it)[0] == pro[j].left){
							for(vector<string>::iterator it_vs_j = 
								pro[j].production_isolation.begin(); 
								it_vs_j != pro[j].production_isolation.end(); it_vs_j++){
									temp.production_isolation.push_back(*it_vs_j 
										+ (*it).substr(1, (*it).size() - 1));
							}
							temp.production_isolation.erase(it);
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


				//�����ֱ�ӽ���ݹ�������һ��
				if(flag == 1){
					//�Ȱ����еĲ���ʽ���ƣ����޸���Ӧ�ķ��ս��
					for(int k1 = nonterminals_count; k1 >= i + 2; k1--){
						nonterminals[k1] = nonterminals[k1 - 1];
						pro[k1] = pro[k1 - 1];
						pro[k1].left++;
						for(int k2 = 0; k2 < pro[k1].production_union.size(); k2++){
							if(pro[k1].production_union[k2] - pro[i].left > 0 
								&& pro[k1].production_union[k2] - '\0' < 91){
									pro[k1].production_union[k2]++;
							}
						}
						pro[k1].union_to_isolation(pro[k1].production_union, 
							pro[k1].production_isolation);
					}

					//�����µķ��ս������������production����
					nonterminals[i + 1] = nonterminals[i] + 1;

					vector<string> new_production_isolation1;
					vector<string> new_production_isolation2;
					pro[i + 1].left = pro[i].left + 1;	//pro[i]����temp
					for(std::vector<string>::iterator it1 = temp.production_isolation.begin(); 
						it1 != temp.production_isolation.end(); it1++){
							if((*it1)[0] == temp.left){
								new_production_isolation2.push_back((*it1).substr(1, 
									(*it1).size() - 1) + pro[i + 1].left);
							}else{
								new_production_isolation1.push_back((*it1) + pro[i + 1].left);
							}
					}
					pro[i].production_isolation = new_production_isolation1;
					pro[i].format_from_production_isolation();
					new_production_isolation2.push_back("!");
					pro[i + 1].production_isolation = new_production_isolation2;
					pro[i + 1].format_from_production_isolation();

					//�����±�i��ǰ�Ĳ���ʽ
					for(int k1 = i - 1; k1 >= 0; k1--){
						for(int k2 = 0; k2 < pro[k1].production_union.size(); k2++){
							if(pro[k1].production_union[k2] - pro[i].left > 0 && 
								pro[k1].production_union[k2] - '\0' < 91){
									pro[k1].production_union[k2]++;
							}
						}
						pro[k1].union_to_isolation(pro[k1].production_union, 
							pro[k1].production_isolation);
					}

					//����productions_count,nonterminals_count
					productions_count ++;
					nonterminals_count++;
				}else{
					continue;
				}
			}
		}
	}

	/************************************************************************/
	/* ����������                                                           */
	/************************************************************************/
	void eleminate_left_divisor(){

	}

	/************************************************************************/
	/* ��ӡ����ʽ�����ԣ�                                                   */
	/************************************************************************/
	void display_productions(){
		cout<<"����ʽ��"<<endl;
		for(int i = 0; i < nonterminals_count; i++){
			cout<<pro[i].left<<"	";
			for(vector<string>::iterator it_vs = pro[i].production_isolation.begin(); 
				it_vs != pro[i].production_isolation.end(); it_vs++){
					cout<<*it_vs<<"	";
			}
			cout<<endl;
		}
	}

	/************************************************************************/
	/* by liuzhushiqiang End                                              */
	/************************************************************************/



	/************************************************************************/
	/* by zhangyuling Start                                              */
	/************************************************************************/


	/************************************************************************/
	/* ����first����                                                   */
	/************************************************************************/
	void calculate_first(){
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
				calculate_first_genaral(first[i], *it);
			}
			sort(first[i].begin(), first[i].end());
		}
	}

	/************************************************************************/
	/* ��ӡfirst���ϣ����ԣ�                                        */
	/************************************************************************/
	void display_fisrt(){
		for(int i = 0; i < nonterminals_count; i++){
			cout<<nonterminals[i]<<"��first��";
			for(vector<char>::iterator it = first[i].begin(); it != first[i].end(); it++ ){
				cout<<*it;
			}
			cout<<endl;
		}
	}

	/************************************************************************/
	/* ��first���ϣ�ͨ�ã��������ַ���s�� ����vector<char> vc.              */
	/************************************************************************/
	void calculate_first_genaral(vector<char>& vc, string s){
		for(int j = 0; j < s.size(); j++){
			if(s[j] >= 97){
				//���ս��
				vc.push_back(s[j]);
				break;
			}else if(s[j] >= 65){
				//�Ƿ��ս��
				for(vector<char>::iterator it_char = 
					first[s[j] - 'A'].begin();
					it_char != first[s[j] - 'A'].end(); it_char++){
						vc.push_back(*it_char);
				}
				if(first[s[j] - 'A'][0] == '!'){
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

	/************************************************************************/
	/* ��ӡһ��vector<char>������  �����ԣ�                               */
	/************************************************************************/
	void display_vc(vector<char>& vc){
		cout<<"vc��ֵ:";
		for(vector<char>::iterator it = vc.begin(); it != vc.end(); it++){
			cout<<*it;
		}
		cout<<endl;
	}

	/************************************************************************/
	/*   ����follow����                                                  */
	/************************************************************************/
	void calculate_follow(){
		//Ϊ��ʼ�ս������'#'
		follow[0].push_back('#');
		for(int i = 1; i < productions_count; i++){
			for(int j = 0; j < productions_count; j++){
				for(vector<string>::iterator vs = pro[j].production_isolation.begin(); 
					vs != pro[j].production_isolation.end(); vs++){
						for(int k = 0; k < (*vs).size(); k++){
							//�ҵ��ض����ս���ڲ���ʽ�е�һ������
							if((*vs)[k] == pro[i].left){
								//�ȼ�������ַ�����first���ϣ����뵽follow[i]
								calculate_first_genaral(follow[i], 
									(*vs).substr(k + 1, (*vs).size() - k - 1));
								//��������ַ�����first���ϰ�������
								//��Ѳ���ʽ���󲿵�follow���ϼ��뵽follow[i]
								if((follow[i].begin() == follow[i].end() || *(follow[i].begin()) 
									== '!') && j != i){
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

	/************************************************************************/
	/* ��ӡfollow���ϣ����ԣ�                                                */
	/************************************************************************/
	void display_follow(){
		for(int i = 0; i < nonterminals_count; i++){
			cout<<nonterminals[i]<<"��follow��";
			for(vector<char>::iterator it = follow[i].begin(); it != follow[i].end(); it++ ){
				cout<<*it;
			}
			cout<<endl;
		}
	}

	/************************************************************************/
	/* by zhangyuling End                                              */
	/************************************************************************/
};

/*
 * Ԥ���������
 */
class Predicting_Analysis_Table{
public:
	Index terminals_index;	//�ս��������
	Index nonterminals_index;	//���ս��������
	string predicting_analysis_table[50][50];	//Ԥ�������
	char terminals[50];
	char nonterminals[50];

public:

	/************************************************************************/
	/* by chenfangfang Start                                              */
	/************************************************************************/

	Predicting_Analysis_Table(CFG& cfg){
		//��ʼ��
		for(int i = 0; i < 50; i++){
			for(int j = 0; j < 50; j++){
				predicting_analysis_table[i][j] = "";
			}
		}

		/*
		 *��cfg����Ԥ�������
		 */
		//��ʼ���б�
		for(int i = 0; i < cfg.terninals_count; i++){
			terminals[i] = cfg.terminals[i];
			terminals_index[cfg.terminals[i]] = i;
		}
		terminals[terminals_index.size()] = '#';
		terminals_index['#'] = terminals_index.size();

		//��ʼ���б�
		for(int i = 0; i < cfg.nonterminals_count; i++){
			nonterminals[i] = cfg.nonterminals[i];
			nonterminals_index[cfg.nonterminals[i]] = i;
		}

		//��Ԥ�������
		for(int i = 0; i < cfg.nonterminals_count; i++){
			//����first�������
			for(vector<string>::iterator it_vs = cfg.pro[i].production_isolation.begin(); 
				it_vs != cfg.pro[i].production_isolation.end(); it_vs++){
					vector<char> vc;
					cfg.calculate_first_genaral(vc, *it_vs);
					for(vector<char>::iterator it_vc = vc.begin(); it_vc != vc.end(); it_vc++){
						if(*it_vc == '!'){
							continue;
						}else{
							predicting_analysis_table[nonterminals_index[cfg.pro[i].left]][
								terminals_index[*it_vc]] = *it_vs;
						}
					}
			}
			//����follow�������
			if(*(cfg.first[i].begin()) == '!'){
				for(vector<char>::iterator it_vc = cfg.follow[i].begin(); 
					it_vc != cfg.follow[i].end(); 
					it_vc++){
						predicting_analysis_table[nonterminals_index[
							cfg.pro[i].left]][terminals_index[*it_vc]] = "!";
				}
			}
		}
	}

	/************************************************************************/
	/* ��ӡԤ����������ԣ�                                            */
	/************************************************************************/
	void display_predicting_analysis_table(){
		cout<<"	";
		for(int i = 0; i < terminals_index.size(); i++){
			cout<<terminals[i]<<"	";
		}
		cout<<endl;
		for(int i = 0; i < nonterminals_index.size(); i++){
			cout<<nonterminals[i]<<":	";
			for(int j = 0; j < terminals_index.size(); j++){
				cout<<predicting_analysis_table[i][j]<<"	";
			}
			cout<<endl;
		}
	}

	/************************************************************************/
	/* ��Ԥ�������                                                     */
	/************************************************************************/
	int search_table(char c1, char c2, string& result){
		if(predicting_analysis_table[nonterminals_index[c1]][terminals_index[c2]] != ""){
			result = predicting_analysis_table[nonterminals_index[c1]][terminals_index[c2]];
			return 1;
		}else{
			return 0;
		}
	}

	/************************************************************************/
	/* by chenfangfang End                                              */
	/************************************************************************/
};


class LL1_Driver{		//�������࣬ʵ��LL(1)�������㷨
public:
	stack<char> symbol_stack;	//����ջ 

public:
	//(�������㷨)

	/************************************************************************/
	/* by chenyannan Start                                              */
	/************************************************************************/
	int run_driver(string s, Predicting_Analysis_Table& pat){
		int error_code = 0;
		string present_input = s + "#";

		//��ʼ����һ�����
		symbol_stack.push('#');
		symbol_stack.push('A');

		int ind = 0;
		//���ת��
		while(1){
			char c1 = symbol_stack.top();
			char c2 = present_input[ind];
			if(c1 == '#'){
				if(c2 == '#'){
					//�ɹ��˳�������0��ʾ��ȷ����
					error_code = 0;
					break;
				}else{
					//�����˳�������3��ʾ����������
					error_code = 3;
					break;
				}
			}
			string result = "";
			//ջ���Ƿ��ս����ִ��if���ս����ִ��else
			if(c1 - 'A' >= 0 && 'Z' - c1 >=0){
				//search_table�������ʽƥ�䷵��1ִ��if�������ز���ʽ�ַ�����result��
				//����ʽ��ƥ�䷵��0ִ��else
				if(pat.search_table(c1, c2, result)){
					if(result == "!"){
						char temp;
						temp = symbol_stack.top();
						symbol_stack.pop();
						//��ӡչ������
						cout<<"��"<<temp<<"-->"<<result<<"չ��"<<endl;
					}else{
						char temp = symbol_stack.top();
						symbol_stack.pop();
						for(int i = result.size() - 1; i >= 0; i--){
							symbol_stack.push(result[i]);		
						}
						//��ӡչ������
						cout<<"��"<<temp<<"-->"<<result<<"չ��"<<endl;
					}
					continue;
				}else{
					//�����˳�������1��ʾ������ʽ��ƥ�䡱
					error_code = 1;
					break;
				}
			}else{
				//���ս��
				if(c1 == c2){
					char temp = symbol_stack.top();
					symbol_stack.pop();
					//��ӡ����
					cout<<"ƥ��"<<temp<<endl;
					ind++;
					continue;
				}else{
					//�����˳�������2��ʾ��ջ���ս����ƥ�䡱
					error_code = 2;
					break;
				}
			}
		}

		return error_code;
	}
	/************************************************************************/
	/* by chenyannan End                                              */
	/************************************************************************/

};

int main(){

	//char terminals[50] = "abcde";		//�ս��
	//int terninals_count = 5;	
	//char nonterminals[50] = {"ABCD"};		//���ս��
	//int nonterminals_count = 4;		
	//string pro_array[50] = {"aBDe", "bC", "!|bcC", "d"};
	//int productions_count = 4;

	char terminals[50] = "abcde";		//�ս��
	int terninals_count = 5;	
	char nonterminals[50] = {"ABC"};		//���ս��
	int nonterminals_count = 3;		
	string pro_array[50] = {"aBCe", "b|Bbc", "d"};
	int productions_count = 3;

	CFG cfg(terminals, terninals_count, nonterminals, 
		nonterminals_count, pro_array, productions_count);
	cout<<"������ݹ飬������֮ǰ";
	cfg.display_productions();
	cfg.eleminate_left_recursion();
	cfg.eleminate_left_divisor();
	cout<<"������ݹ飬������֮��";
	cfg.display_productions();
	cout<<endl;

	cfg.calculate_first();
	cout<<"�ķ���First���ϣ�"<<endl;
	cfg.display_fisrt();
	cfg.calculate_follow();
	cout<<"�ķ���Follow���ϣ�"<<endl;
	cfg.display_follow();
	cout<<endl;

	Predicting_Analysis_Table pat(cfg);
	cout<<"�ķ���Ԥ�������"<<endl;
	pat.display_predicting_analysis_table();
	cout<<endl;

	cout<<"���ת���Ĺ��̣�"<<endl;
	LL1_Driver ll1_driver;
	int error_code = ll1_driver.run_driver("abbcde", pat);
	if(error_code == 0){
		cout<<"��ȷ����"<<endl;
	}else if(error_code == 1){
		cout<<"��������ʽ��ƥ��"<<endl;
	}else if(error_code == 2){
		cout<<"����ջ���ս����ƥ��"<<endl;
	}else if(error_code == 3){
		cout<<"������������"<<endl;
	}
	cout<<endl;
	return 0;
}