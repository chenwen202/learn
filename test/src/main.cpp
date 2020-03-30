#include <iostream>
#include <math.h>
#include <string>

int main() {
    int b = 12000;
    char a = b;
    std::string str; 
    std::cout<< "int b = 12000, actual "<<(int)a <<std::endl;
    std::cin>>str;
    std::cout<<str<<std::endl;
    std::getchar();
    return 0;
}





















// #include<iostream>
// #include<cstdio>
// #include<iomanip>
// using namespace std;
// #define e     0.00000001
// #define maxn 50

// int    n;//规模nXn
// double a[maxn][maxn];//系数矩阵
// double b[maxn];//b矩阵
// double m[maxn][maxn];//中间变量矩阵
// double x[maxn];//最终解
// int    H=1;//扩大H被结算（优化）
// /*读取数据*/
// void read(){
//     cout<<"请输入系数矩阵规模n:= ";
//     cin>>n;
//     cout<<"|-----------------------------\n";
//     cout<<"|请输入系数矩阵,如：\n";
//     cout<<"|1.1348 3.8326 1.1651 3.4017\n"; 
//     cout<<"|0.5301 1.7875 2.5330 1.5435\n";
//     cout<<"|3.4129 4.9317 8.7643 1.3142\n";
//     cout<<"|1.2371 4.9998 10.6721 0.0147\n";
//     cout<<"|-----------------------------\n";
//     for(int i=1;i<=n;i++)
//         for(int j=1;j<=n;j++){
//             cin>>a[i][j];
//             a[i][j]*=H;
//         }
//     cout<<"|-----------------------------\n";
//     cout<<"|请输入b矩阵，如：\n";
//     cout<<"|9.5342 6.3941 18.4231 16.9237\n";
//     cout<<"|-----------------------------\n";
//     for(int i=1;i<=n;i++){
//         cin>>b[i];
//         b[i]*=H;
//     }
// }

// /*
// 中间矩阵输出
// 参数：消元次数
// */
// void PrintProc(int cases){
//     printf("--------第%d次消元结果如下：\n",cases);
//     for(int i=1;i<=n;i++){
//         for(int j=1;j<=n;j++){
//             cout<<setw(10)<<a[i][j]<<' ';
//         }
//         cout<<setw(10)<<b[i]<<'\n';
//     }
//     cout<<"END THIS SHOW-------------\n";
// }

// /*
// 显示结果
// */
// void Print(){
//     cout<<"|-----------------------------\n";
//     cout<<"|结果为：\n";
//     for(int i=1;i<=n;i++){
//         printf("x[%d]=  %lf\n",i,x[i]);
//     }
//     cout<<"|-----------------------------\n\n";
// }

// /*
// 顺序消元法
// */
// void ShunXuXiaoYuan(){
//     //消元计算
//     for(int k=1;k<n;k++){
//         for(int i=k+1;i<=n;i++){
//             m[i][k]=a[i][k]/a[k][k];
//             for(int j=k+1;j<=n;j++){
//                 a[i][j]-=m[i][k]*a[k][j];
//             }
//         }
//         for(int i=k+1;i<=n;i++){
//             b[i]-=m[i][k]*b[k];
//         }
//         PrintProc(k);//输出中间计算过程
//     }
//     //回代求解
//     x[n]=b[n]/a[n][n];
//     for(int i=n-1;i>0;i--){
//         x[i]=b[i];
//         for(int j=i+1;j<=n;j++)
//             x[i]-=a[i][j]*x[j];
//         x[i]/=a[i][i];
//     }
//     //输出结果
//     Print();
// }

// /*
// 列主消元
// */
// void LieZhuXiaoYuan(){
//      double temp = 0.0;
//     for(int k=1;k<n;k++){
//         //选主元[这一列的绝对值最大值]
//         double ab_max=-1;
//         int       max_ik;
//         for(int i=k;i<=n;i++){
//             if(abs(a[i][k])>ab_max){
//                 ab_max=abs(a[i][k]);
//                 max_ik=i;
//             }
//         }
//         //交换行处理[先判断是否为0矩阵]
//         if(ab_max<e){//0矩阵情况
//             cout<<"det A=0\n";
//             break;
//         }else if(max_ik!=k) {//是否是当前行，不是交换
//             temp = 0.0;
//             for(int j=1;j<=n;j++){
//                 temp=a[max_ik][j];
//                 a[max_ik][j]=a[k][j];
//                 a[k][j]=temp;
//             }
//             temp=b[max_ik];
//             b[max_ik]=b[k];
//             b[k]=temp;
//         }
//         //消元计算
//         for(int i=k+1;i<=n;i++){
//             a[i][k]/=a[k][k];
//             for(int j=k+1;j<=n;j++){
//                 a[i][j]-=a[i][k]*a[k][j];
//             }
//             b[i]-=a[i][k]*b[k];
//         }
//         PrintProc(k);//输出中间计算过程
//         if(k<n-1)continue;
//         else{
//             if(abs(a[n][n])<e){
//                 cout<<"det A=0\n";
//                 break;
//             }else{//回代求解
//                 x[n]=b[n]/a[n][n];
//                 for(int i=n-1;i>0;i--){
//                     x[i]=b[i];
//                     for(int j=i+1;j<=n;j++)
//                         x[i]-=a[i][j]*x[j];
//                     x[i]/=a[i][i];
//                 }
//                 //输出结果
//                 Print();
//             }
//         }
//     }
// }

// /*
// 主函数
// */
// int main(){
//     while(1){
//         read();
//         LieZhuXiaoYuan();
//         //ShunXuXiaoYuan();
//     }return 0;
// }
// /*
// 书上高斯消元的例子：
// 1 1 1 
// 1 3 -2
// 2 -2 1

// 6 1 1
// */
// /*
// 书上列主消元的例子：
// -0.002 2 2
// 1 0.78125 0
// 3.996 5.5625 4

// 0.4 1.3816 7.4178
// */