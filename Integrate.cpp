#include <iostream>
#include <cmath>
using namespace std;
using ll = long long;
double func(double x){
    return 1.0/(1.0+x);
}
double Matrix(double (*fn)(double),double x_0,double x_1){
    double delta = 1e-4;
    double ans = 0;
    for(double x = x_0;x < x_1;x += delta){
        ans += delta * fn(x);
    }
    return ans;
}
inline ll exp4(int x){
    return 1<<(2*x);
}
inline ll exp2(int x){
    return 1<<x;
}
double Tn[1000];
inline double sigma(double(*fn)(double),int k,double x_0,double x_1){
    double ans = 0;
    for(int j = 0;j < exp2(k-1);j++){
        ans += fn(x_0+double(2*j+1)*(x_1-x_0)/(double)exp2(k));
    }
    return ans;
}
inline void T_m_n(double(*fn)(double),int k,double x_0,double x_1){
    // 计算一层
    // 下面这个是按照T_k-1[0]做的
    Tn[k] = 0.5*Tn[0]+(x_1-x_0)/(double)exp2(k)*sigma(fn,k,x_0,x_1);
    // 下面这个是按照T_0[k-1]做的
    // Tn[k] = 0.5*Tn[k-1]+(x_1-x_0)/(double)exp2(k)*sigma(fn,k,x_0,x_1);
    for(int m = k;m > 0;m--){
        Tn[m-1] = 1.0/double(exp4(m)-1)*((double)exp4(m)*Tn[m]-Tn[m-1]);
    }
}
double Romberg(double(*fn)(double),double x_0,double x_1){
    // k从0开始
    Tn[0] = (x_1-x_0)*0.5*(fn(x_0)+fn(x_1));
    const int max_depth = 10;
    for(int k = 0;k < max_depth;k++){
        T_m_n(fn,k,x_0,x_1);
    }
    return Tn[0];
}
double Simpson(double (*fn)(double),double x_0,double x_1){
    double delta = 1e-4;
    double ans = 0;
    double prev = x_0;
    for(double x = x_0;x < x_1;x += delta){
        ans += delta * (fn(prev)+fn(x)+4.0*fn((prev+x)/2.0))/6.0;
        prev = x;
    }
    return ans;
}
int main(){
    printf("begin\n");
    double ans_sim = Simpson(func,0.0,1.5);
    printf("ans_sim = %lf\n",ans_sim);
    double ans_mat = Matrix(func,0.0,1.5);
    printf("ans_mat = %lf\n",ans_mat);
    double ans_rom = Romberg(func,0.0,1.5);
    printf("ans_rom = %lf\n",ans_rom);
    printf("ans = %lf\n",log(2.5));
    return 0;
}