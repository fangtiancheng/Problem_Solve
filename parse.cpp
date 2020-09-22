#include <iostream>
#include <string>
#include <cstdio>
#include <stack>
#include <vector>
#include <cstdlib>
using namespace std;
const char proposition[] = {'P', 'Q', 'R', 'S', 'T', 'U'};
// 支持运算符号 ! v ^ -> <->
// 第零优先级 ( )
// 第一优先级 !
// 第二优先级 v ^ -> <->

inline bool imply(bool p, bool q){
    // -> 蕴含
    if(p == true && q == false)
        return false;
    else 
        return true;
}
inline bool equal(bool p, bool q){
    // <-> 同或
    return !(p ^ q);
}
/* // 语法树版本, 待完成
enum NODE_TYPE {PROPOSITION,SYMBOL};
// 命题还是符号
struct Node{
    NODE_TYPE node_type;
    char content;
    // -> : >       <-> : <
    Node* next;
    Node(NODE_TYPE n = PROPOSITION, char c = '\0'):node_type(n),content(c),next(NULL){}
};
*/
struct sym{
    int position;// 位置
    char content;

    sym(int p, char c):position(p),content(c){}
    bool operator<(const sym& other)const{
        // 越小越先算
        if(other.content == '!') return false;
        if(this->content == '!') return true;
        return false;
    }
};
int n = 0;// 命题个数
struct error{
    int line;
    string msg;
    error(int l,const string& str):line(l),msg(str){}
};
inline bool is_symbol(const char c){
    switch(c){
        case 'v': case '^': case '!': case '>': case '<':
        return true;
        default: 
        return false;
    }
}
inline string parse(const string& input){
    // 把中缀表达式转换为后缀表达式
    string output;
    stack<sym> stk;// 符号栈
    for(string::const_iterator it = input.cbegin();it != input.cend(); it++){
        const char c = *it;
        switch(c){
            case '(': {
                stk.push(sym(it - input.cbegin(),'('));
            }
            break;

            case ')': {
                while(!stk.empty()&&stk.top().content != '('){
                    output += stk.top().content;
                    stk.pop();
                }
                if(stk.empty()) throw(error(__LINE__,string("Bracket mismatch!")));
                stk.pop();
            }
            break;

            case '!':{
                while(!stk.empty()&&stk.top().content == '!'){
                    output += stk.top().content;
                    stk.pop();
                }
                stk.push(sym(it - input.cbegin(),'!'));
            }
            break;

            case 'v':{
                while(!stk.empty()&&stk.top().content != '('){
                    output += stk.top().content;
                    stk.pop();
                }
                stk.push(sym(it - input.cbegin(),'v'));
            }
            break;

            case '^':{
                while(!stk.empty()&&stk.top().content != '('){
                    output += stk.top().content;
                    stk.pop();
                }
                stk.push(sym(it - input.cbegin(),'^'));
            }
            break;

            case '-':{
                it++; if(*it != '>') throw error(__LINE__,string("illegal symbol!"));
                while(!stk.empty()&&stk.top().content != '('){
                    output += stk.top().content;
                    stk.pop();
                }
                stk.push(sym(it - input.cbegin(),'>'));
            }
            break;

            case '<':{
                it ++; if(*it != '-') throw error(__LINE__,string("illegal symbol!"));
                it ++; if(*it != '>') throw error(__LINE__,string("illegal symbol!"));
                while(!stk.empty()&&stk.top().content != '('){
                    output += stk.top().content;
                    stk.pop();
                }
                stk.push(sym(it - input.cbegin(),'<'));
            }
            break;

            default:
            switch(c){
                case 'P':{
                    n = n > 1?n: 1;
                }
                break;
                case 'Q':{
                    n = n > 2?n: 2;
                }
                break;
                case 'R':{
                    n = n > 3?n: 3;
                }
                break;
                case 'S':{
                    n = n > 4?n: 4;
                }
                break;
                case 'T':{
                    n = n > 5?n: 5;
                }
                break;
                case 'U':{
                    n = n > 6?n: 6;
                }
                break;
                case ' ':{continue;} break;
                default:
                    cout << "===ERROR===\nThere is illegal character(s) " << c <<" \n";
                    throw error(__LINE__,string("Charactor Not Exist!"));
                break;
            }
            output += c;

        }

    }
    while(!stk.empty()){
        output += stk.top().content;
        stk.pop();
    }
    return output;
}

vector<string> Conjunction;// 合取范式
vector<string> Disjunction;// 析取范式
void process(string input,const string& TF){
    // 第一步, 把input的命题变项替换为真值
    for(string::iterator it = input.begin();it != input.end();it++){
        for(int i = 0;i < n;i++){
            char c = 'P' + i;
            if(*it == c){
                *it = TF[i];
            }
        }
    }
    // 第二步, 运算
    stack<bool> stk;// 命题栈
    for(string::iterator it = input.begin();it != input.end();it++ ){
        const char c = *it;
        if( is_symbol(c)){
            switch(c){
                case '!':{
                    if(stk.empty()) throw error(__LINE__,string("Operator and number don't match!"));
                    bool tmp = !stk.top();
                    stk.pop();
                    stk.push(tmp);
                }
                break;

                case 'v':{
                    if(stk.size() < 2) throw error(__LINE__,string("Operator and number don't match!"));
                    bool a = stk.top(); stk.pop();
                    bool b = stk.top(); stk.pop();
                    stk.push(a||b);
                }
                break;

                case '^':{
                    if(stk.size() < 2) throw error(__LINE__,string("Operator and number don't match!"));
                    bool a = stk.top(); stk.pop();
                    bool b = stk.top(); stk.pop();
                    stk.push(a&&b);
                }
                break;

                case '>':{
                    if(stk.size() < 2) throw error(__LINE__,string("Operator and number don't match!"));
                    bool b = stk.top(); stk.pop();
                    bool a = stk.top(); stk.pop();
                    stk.push(imply(a,b));
                }
                break;

                case '<':{
                    if(stk.size() < 2) throw error(__LINE__,string("Operator and number don't match!"));
                    bool a = stk.top(); stk.pop();
                    bool b = stk.top(); stk.pop();
                    stk.push(equal(a,b));
                }
                break;
                default:
                    throw error(__LINE__,string("The operator don't exist!"));
                break;
            }
        }
        else{
            if(c == 'T') stk.push(true);
            else if(c == 'F') stk.push(false);
            else throw error(__LINE__,string("The operator don't exist!"));
        }// c is not symbol
    }// for
    if (stk.size()!=1) throw error(__LINE__,string("Calculate error!"));
    for(char c:TF){
        cout << c <<'\t';
    }
    cout << (stk.top()?'T':'F') << '\n';
    if(stk.top()) Disjunction.push_back(TF);// T -> 析取范式
    else Conjunction.push_back(TF);// F -> 合取范式
    return;
}

bool add_TF(string& TF, int loc = 0){
    if(loc >= TF.size()) return false;
    if(TF[loc] == 'F'){
        TF[loc] = 'T';
        return true;
    }
    else {
        TF[loc] = 'F';
        return add_TF(TF,loc+1);
    }
}

void generate_TF(const string& input){
    string TF;
    for(int i = 0;i < n;i++){
        // 初始化为FFFFFF
        cout << "=========";
        TF.push_back('F');
    }
    cout << '\n';
    for(int i = 0;i < n;i++){
        cout << char('P'+i) <<'\t';
    }
    cout << "value" << '\n';
    do{
        process(input, TF);
    }
    while(add_TF(TF));
    cout << "Disjunction  ";// 析取范式
    for(vector<string>::iterator it1 = Disjunction.begin();it1 != Disjunction.end();it1++){
        const string& str = *it1;
        cout << '(';
        for(string::const_iterator it2 = str.cbegin();it2 != str.cend();it2++){
            if(*it2 != 'T'){
                cout << '!';
            }
            if(it2 == str.cend()-1) cout << char('P' + (it2 - str.cbegin()));
            else cout << char('P' + (it2 - str.cbegin())) << '^';
        }
        if(it1 == Disjunction.cend()-1) cout << ")\n";
        else cout << ")v";
    }
    cout <<"Conjunction  ";// 合取范式
    for(vector<string>::iterator it1 = Conjunction.begin();it1 != Conjunction.end();it1++){
        const string& str = *it1;
        cout << '(';
        for(string::const_iterator it2 = str.cbegin();it2 != str.cend();it2++){
            if(*it2 != 'T'){
                cout << '!';
            }
            if(it2 == str.cend()-1) cout << char('P' + (it2 - str.cbegin()));
            else cout << char('P' + (it2 - str.cbegin())) << 'v';
        }
        if(it1 == Conjunction.cend()-1) cout << ")\n";
        else cout << ")^";
    }
    cout << '\n';
}
void welcome(){
    cout << "==========================================================================================================================\n";
    cout << "* Created by ftc                                                                                                         *\n";
    cout << "* usage:                                                                                                                 *\n";
    cout << "* Input a proposition, and then use PQR(in order and upper case) and other characters to represent proposition variables *\n";
    cout << "* Here is an example:  (P->Q)^(R->Q)^(S->Q)->(P^R^!S->Q)                                                                 *\n";
    cout << "* The legal operator is '!' '^' 'v' '->' '<->'                                                                           *\n";
    cout << "==========================================================================================================================" << endl;
}
int main(){
    welcome();
    string input;
    // cin >> n;// 输入命题个数
    // n = 0;
    getline(cin,input);// 输入命题
    // 允许大写的P、Q、R为命题
    try{
        string output = parse(input);
        cout << output <<endl;
        generate_TF(output);
    }
    catch(error e){
        cout << "error in line "<<e.line<<'\n';
        cout << "error message : "<<e.msg<<'\n';
        return 1;
    }
    return 0;
}