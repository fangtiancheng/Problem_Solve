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
    return 1LL<<(2*x);
}
inline ll exp2(int x){
    return 1LL<<x;
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
    Tn[k] = 0.5*Tn[0]+(x_1-x_0)/(double)exp2(k)*sigma(fn,k,x_0,x_1);
    for(int m = k;m > 0;m--){
        Tn[m-1] = 1.0/double(exp4(m)-1)*((double)exp4(m)*Tn[m]-Tn[m-1]);
    }
}
    const int max_depth = 20;

double Romberg(double(*fn)(double),double x_0,double x_1){
    // k从0开始
    Tn[0] = (x_1-x_0)*0.5*(fn(x_0)+fn(x_1));
    for(int k = 0;k <= max_depth;k++){
        // 一次算一层
        T_m_n(fn,k,x_0,x_1);
    }
    return Tn[0];
}
double Rn[max_depth+5][max_depth+5];
double Romberg_teacher(double(*fn)(double),double x_0,double x_1){
    // 老师的Romberg积分法
    Rn[0][0] = (x_1-x_0)*0.5*(fn(x_0)+fn(x_1));
    // printf("Rn[0][0] = %.09lf\n",Rn[0][0]);
    for(int k = 1;k < max_depth+3;k++){
        Rn[0][k] = 0.5 * Rn[0][k-1] + (x_1-x_0)/(double)exp2(k)*sigma(fn,k,x_0,x_1);
        // printf("Rn[0][%d] = %.09lf\n",k,Rn[0][k]);
    }
    for(int i= 1;i < max_depth;i++){
        // 一列一列算
        for(int j = 0;i + j < max_depth;j++){
            Rn[i][j] = 1.0/double(exp4(i)-1)*((double)exp4(i)*Rn[i-1][j+1]-Rn[i-1][j]);
            // printf("Rn[%d][%d] = %.09lf\n",i,j,Rn[i][j]);
        }
    }
    return Rn[0][max_depth-1];
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
    printf("ans_sim = %.09lf\n",ans_sim);
    double ans_mat = Matrix(func,0.0,1.5);
    printf("ans_mat = %.09lf\n",ans_mat);
    double ans_rom1 = Romberg_teacher(func,0.0,1.5);
    double ans_rom2 = Romberg(func,0.0,1.5);
    printf("ans_rom1 = %.020lf\n",ans_rom1);
    printf("ans_rom2 = %.020lf\n",ans_rom2);
    printf("ans_corr = %.020lf\n",log(2.5));
    return 0;
}