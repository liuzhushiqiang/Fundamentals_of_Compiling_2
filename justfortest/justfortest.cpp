#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef map<char, int> Index;


/**
* 产生式类
*/
class Production{
public:
	//产生式左部
	char left;	
	//产生式右部（联合形式，如"S-->a|b|c", 只保存产生式右部字符串）
	string production_union;
	//产生式右部（孤立形式, 如"S-->a, S-->b, S-->c"，只保存产生式右部字符串）
	vector<string> production_isolation;	

public:
	//初始化
	void init(char l, string s){
		left = l;
		production_union = s;
		union_to_isolation(production_union, production_isolation);
	}



	//字符串分割函数，用于union_to_isolation函数
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
	* [union_to_isolation 从产生式的union形式导出isolation形式]
	* @param s [description]
	* @param v [description]
	*/
	void union_to_isolation(string s, std::vector<string>& v){
		v = split(production_union, "|");
		sort(v.begin(), v.end());
	}

	/**
	* [isolation_to_union 从产生式的isolation形式导出union形式]
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

	//对isolation重新排序格式化，并导出union，相当与重新格式化production类
	void format_from_production_isolation(){
		sort(production_isolation.begin(), production_isolation.end());
		isolation_to_union(production_union, production_isolation);
	}

};

/************************************************************************/
/* 文法类                                                               */
/************************************************************************/
class CFG{
public:
	char terminals[50];		//终结符
	int terninals_count;	
	char nonterminals[50];		//非终结符
	int nonterminals_count;
	Production pro[100];	//产生式
	int productions_count;		
	vector<char> first[50]; 	//first集合
	vector<char> follow[50];	//follow集合

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
	/* 消除左递归                                                            */
	/************************************************************************/
	void eleminate_left_recursion(){
		//i从n-2到0
		for(int i = nonterminals_count - 2; i >= 0; i--){
			Production temp = pro[i];
			//j从n-1到i+1
			for(int j = nonterminals_count - 1; j >= i + 1; j--){
				//先执行替换
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


				//如果有直接进左递归则做这一步
				if(flag == 1){
					//先把所有的产生式后移，并修改相应的非终结符
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

					//创建新的非终结符，并创建新production对象。
					nonterminals[i + 1] = nonterminals[i] + 1;

					vector<string> new_production_isolation1;
					vector<string> new_production_isolation2;
					pro[i + 1].left = pro[i].left + 1;	//pro[i]就是temp
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

					//更新下标i以前的产生式
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

					//更新productions_count,nonterminals_count
					productions_count ++;
					nonterminals_count++;
				}else{
					continue;
				}
			}
		}
	}

	/************************************************************************/
	/* 消除左因子                                                           */
	/************************************************************************/
	void eleminate_left_divisor(){

	}

	/************************************************************************/
	/* 打印产生式（调试）                                                   */
	/************************************************************************/
	void display_productions(){
		cout<<"产生式："<<endl;
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
	/* 计算first集合                                                   */
	/************************************************************************/
	void calculate_first(){
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
				calculate_first_genaral(first[i], *it);
			}
			sort(first[i].begin(), first[i].end());
		}
	}

	/************************************************************************/
	/* 打印first集合（调试）                                        */
	/************************************************************************/
	void display_fisrt(){
		for(int i = 0; i < nonterminals_count; i++){
			cout<<nonterminals[i]<<"的first：";
			for(vector<char>::iterator it = first[i].begin(); it != first[i].end(); it++ ){
				cout<<*it;
			}
			cout<<endl;
		}
	}

	/************************************************************************/
	/* 求first集合（通用），输入字符串s， 返回vector<char> vc.              */
	/************************************************************************/
	void calculate_first_genaral(vector<char>& vc, string s){
		for(int j = 0; j < s.size(); j++){
			if(s[j] >= 97){
				//是终结符
				vc.push_back(s[j]);
				break;
			}else if(s[j] >= 65){
				//是非终结符
				for(vector<char>::iterator it_char = 
					first[s[j] - 'A'].begin();
					it_char != first[s[j] - 'A'].end(); it_char++){
						vc.push_back(*it_char);
				}
				if(first[s[j] - 'A'][0] == '!'){
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

	/************************************************************************/
	/* 打印一个vector<char>的内容  （调试）                               */
	/************************************************************************/
	void display_vc(vector<char>& vc){
		cout<<"vc的值:";
		for(vector<char>::iterator it = vc.begin(); it != vc.end(); it++){
			cout<<*it;
		}
		cout<<endl;
	}

	/************************************************************************/
	/*   计算follow集合                                                  */
	/************************************************************************/
	void calculate_follow(){
		//为开始终结符加入'#'
		follow[0].push_back('#');
		for(int i = 1; i < productions_count; i++){
			for(int j = 0; j < productions_count; j++){
				for(vector<string>::iterator vs = pro[j].production_isolation.begin(); 
					vs != pro[j].production_isolation.end(); vs++){
						for(int k = 0; k < (*vs).size(); k++){
							//找到特定非终结符在产生式中的一个出现
							if((*vs)[k] == pro[i].left){
								//先计算后面字符串的first集合，加入到follow[i]
								calculate_first_genaral(follow[i], 
									(*vs).substr(k + 1, (*vs).size() - k - 1));
								//如果后面字符串的first集合包含！，
								//则把产生式的左部的follow集合加入到follow[i]
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
	/* 打印follow集合（调试）                                                */
	/************************************************************************/
	void display_follow(){
		for(int i = 0; i < nonterminals_count; i++){
			cout<<nonterminals[i]<<"的follow：";
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
 * 预测分析表类
 */
class Predicting_Analysis_Table{
public:
	Index terminals_index;	//终结符的索引
	Index nonterminals_index;	//非终结符的索引
	string predicting_analysis_table[50][50];	//预测分析表
	char terminals[50];
	char nonterminals[50];

public:

	/************************************************************************/
	/* by chenfangfang Start                                              */
	/************************************************************************/

	Predicting_Analysis_Table(CFG& cfg){
		//初始化
		for(int i = 0; i < 50; i++){
			for(int j = 0; j < 50; j++){
				predicting_analysis_table[i][j] = "";
			}
		}

		/*
		 *从cfg构造预测分析表
		 */
		//初始化列标
		for(int i = 0; i < cfg.terninals_count; i++){
			terminals[i] = cfg.terminals[i];
			terminals_index[cfg.terminals[i]] = i;
		}
		terminals[terminals_index.size()] = '#';
		terminals_index['#'] = terminals_index.size();

		//初始化行标
		for(int i = 0; i < cfg.nonterminals_count; i++){
			nonterminals[i] = cfg.nonterminals[i];
			nonterminals_index[cfg.nonterminals[i]] = i;
		}

		//填预测分析表
		for(int i = 0; i < cfg.nonterminals_count; i++){
			//根据first集合填表
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
			//根据follow集合填表
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
	/* 打印预测分析表（调试）                                            */
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
	/* 查预测分析表                                                     */
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


class LL1_Driver{		//驱动器类，实现LL(1)驱动器算法
public:
	stack<char> symbol_stack;	//符号栈 

public:
	//(驱动器算法)

	/************************************************************************/
	/* by chenyannan Start                                              */
	/************************************************************************/
	int run_driver(string s, Predicting_Analysis_Table& pat){
		int error_code = 0;
		string present_input = s + "#";

		//初始化第一个格局
		symbol_stack.push('#');
		symbol_stack.push('A');

		int ind = 0;
		//格局转换
		while(1){
			char c1 = symbol_stack.top();
			char c2 = present_input[ind];
			if(c1 == '#'){
				if(c2 == '#'){
					//成功退出，代码0表示正确结束
					error_code = 0;
					break;
				}else{
					//出错退出，代码3表示“其他错误”
					error_code = 3;
					break;
				}
			}
			string result = "";
			//栈顶是非终结符则执行if，终结符则执行else
			if(c1 - 'A' >= 0 && 'Z' - c1 >=0){
				//search_table查表，产生式匹配返回1执行if，并返回产生式字符串给result，
				//产生式不匹配返回0执行else
				if(pat.search_table(c1, c2, result)){
					if(result == "!"){
						char temp;
						temp = symbol_stack.top();
						symbol_stack.pop();
						//打印展开过程
						cout<<"按"<<temp<<"-->"<<result<<"展开"<<endl;
					}else{
						char temp = symbol_stack.top();
						symbol_stack.pop();
						for(int i = result.size() - 1; i >= 0; i--){
							symbol_stack.push(result[i]);		
						}
						//打印展开过程
						cout<<"按"<<temp<<"-->"<<result<<"展开"<<endl;
					}
					continue;
				}else{
					//出错退出，代码1表示“产生式不匹配”
					error_code = 1;
					break;
				}
			}else{
				//是终结符
				if(c1 == c2){
					char temp = symbol_stack.top();
					symbol_stack.pop();
					//打印过程
					cout<<"匹配"<<temp<<endl;
					ind++;
					continue;
				}else{
					//出错退出，代码2表示“栈顶终结符不匹配”
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

	//char terminals[50] = "abcde";		//终结符
	//int terninals_count = 5;	
	//char nonterminals[50] = {"ABCD"};		//非终结符
	//int nonterminals_count = 4;		
	//string pro_array[50] = {"aBDe", "bC", "!|bcC", "d"};
	//int productions_count = 4;

	char terminals[50] = "abcde";		//终结符
	int terninals_count = 5;	
	char nonterminals[50] = {"ABC"};		//非终结符
	int nonterminals_count = 3;		
	string pro_array[50] = {"aBCe", "b|Bbc", "d"};
	int productions_count = 3;

	CFG cfg(terminals, terninals_count, nonterminals, 
		nonterminals_count, pro_array, productions_count);
	cout<<"消除左递归，左因子之前";
	cfg.display_productions();
	cfg.eleminate_left_recursion();
	cfg.eleminate_left_divisor();
	cout<<"消除左递归，左因子之后";
	cfg.display_productions();
	cout<<endl;

	cfg.calculate_first();
	cout<<"文法的First集合："<<endl;
	cfg.display_fisrt();
	cfg.calculate_follow();
	cout<<"文法的Follow集合："<<endl;
	cfg.display_follow();
	cout<<endl;

	Predicting_Analysis_Table pat(cfg);
	cout<<"文法的预测分析表："<<endl;
	pat.display_predicting_analysis_table();
	cout<<endl;

	cout<<"格局转换的过程："<<endl;
	LL1_Driver ll1_driver;
	int error_code = ll1_driver.run_driver("abbcde", pat);
	if(error_code == 0){
		cout<<"正确结束"<<endl;
	}else if(error_code == 1){
		cout<<"出错，产生式不匹配"<<endl;
	}else if(error_code == 2){
		cout<<"出错，栈顶终结符不匹配"<<endl;
	}else if(error_code == 3){
		cout<<"出错，其他错误"<<endl;
	}
	cout<<endl;
	return 0;
}