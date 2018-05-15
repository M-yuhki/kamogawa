
//鴨川河川敷シミュレーター

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double exp_rand(double x);

int main(){
	//変数
	double la,mu; //平均到着・サービス率
	int K; //サービス容量
	double t_be,t_en; //システム開始・終了時刻
	int check; //判定の分岐
	double t; //現在の時刻
	double t_x;//平均パケット数の計算用
	double t_w;//平均滞在時間の計算用
	double In , Out; //次の時間を指す
	double ave_pack; //シミュレート値の計算用
	double rho,N,W,P; //理論値の計算用
	int wait = 0; //待ち行列
	int pack = 0; // パケット受理回数
	int loss = 0; //パケット破棄回数
	int service; //サービス状態の判定
	int i, j,k,l,m,n; //汎用変数

	//新規追加変数
	int data; //詳細表示用のフラグ
	int river[160]; //川の状態を判定
	int check_in[160]; //到着時の判定用
	int check_out[160];//退去時の判定用
	int space=160; //座れる場所の数
	int couple=0; //現在のカップル数

	srand((unsigned)time(NULL));

	printf("\n*****鴨川カップルシミュレータ*****\n");

	//入力値の格納

	
	printf("カップルの鴨川訪問率（平均到着率）");
	scanf("%lf",&la);
	printf("鴨川河川敷でのカップルの滞在率　（サービス率）");
	scanf("%lf",&mu);
	printf("測定時間（単位:分）");
	scanf("%lf",&t_en);
	printf("詳細表示モード(OFF...0/時刻とイベントを表示...1/河川の状況も表示...2)");
	scanf("%d",&data);


	t_be=0;

	K = 0;
	
	double t_sum[160]; //平均パケット数計算用

	for(i=0;i<160;i++){
		river[i]=0;
		check_in[i]=0;
		check_out[i]=0;
	}
	for(i=0;i<160;i++) t_sum[i] = 0;
	t = 0;
	//最初の到着の処理
	t =  exp_rand(la);// 最初のパケット到着までの時間（実質的な開始時間）
	In = t + exp_rand(la); //最初のパケット到着時刻
	Out = t + exp_rand(mu); //最初のパケット退去時刻
	//シミュレート開始
		while(t< t_en){
			if(In > t_en && Out >t_en){//終了時間を超える場合はループを抜ける
				break;
			}
			if(In < Out){//到着の発生
				t_x = In - t;
				t = In;
				pack++;

				if(space != 0){//座れる場所がある
					couple++;
					k=0;

					for(l=0;l<160;l++){
						if(river[l]==0){
							check_in[k]=l;
							k++;
						}
					}
				//	printf("\n");
					m = rand()%k;
					n = check_in[m];
					river[n]=1; //ランダムで決まった箇所に座る
					space--;
					//横の情報変更		
		
					if(n>4){river[n-5]=river[n-5]+2;if(river[n-5]==2)space--;}
					if(n>3){river[n-4]=river[n-4]+2;if(river[n-4]==2)space--;}
					if(n>2){river[n-3]=river[n-3]+2;if(river[n-3]==2)space--;}
					if(n>1){river[n-2]=river[n-2]+2;if(river[n-2]==2)space--;}
					if(n>0){river[n-1]=river[n-1]+2;if(river[n-1]==2)space--;}
					if(n<159){river[n+1]=river[n+1]+2;if(river[n+1]==2)space--;}
					if(n<158){river[n+2]=river[n+2]+2;if(river[n+2]==2)space--;}
					if(n<157){river[n+3]=river[n+3]+2;if(river[n+3]==2)space--;}
					if(n<156){river[n+4]=river[n+4]+2;if(river[n+4]==2)space--;}
					if(n<155){river[n+5]=river[n+5]+2;if(river[n+5]==2)space--;}



					if(data>=1){
					printf("カップル到着,観測地点から%d mの箇所に座った。\n",n+1);
            if(data == 2){
					    printf("S:");
					    for(l=0;l<160;l++){
					    	if(l==n)printf("◎");
					    	else if(river[l]==0)printf("□");
					    	else printf("■");
					    }
					  printf(":G\n");
            }
					}				

					t_sum[couple-1] = t_sum[couple-1] + t_x;
					t_w = t_w + (t_x*(couple-1));
					In = t + exp_rand(la);
				}
				else if(space==0) {//あきがない→撤退
						t_sum[couple]=t_sum[couple] + t_x;
						t_w = t_w + (t_x*couple);
						In = t + exp_rand(la);
						pack--;
						loss++;
					if(data==1){printf("カップル到着,ただし座れる場所がなかったので帰った。\n");}

				}
			}else if(In > Out){//退去の発生
				t_x = Out - t;
				t =  Out;
				if(couple!=0){//座ってる人がいる
					couple--;
					k=0;

					for(l=0;l<160;l++){
						if(river[l]==1){
							check_out[k]=l;
							k++;
						}
					}

					m = rand()%k;
					n = check_out[m];
					river[n]=0; //ランダムで決まった箇所から出ていく
					space++;
					//横の情報変更


					if(n>4){river[n-5]=river[n-5]-2;if(river[n-5]==0)space++;}
					if(n>3){river[n-4]=river[n-4]-2;if(river[n-4]==0)space++;}
					if(n>2){river[n-3]=river[n-3]-2;if(river[n-3]==0)space++;}
					if(n>1){river[n-2]=river[n-2]-2;if(river[n-2]==0)space++;}
					if(n>0){river[n-1]=river[n-1]-2;if(river[n-1]==0)space++;}
					if(n<159){river[n+1]=river[n+1]-2;if(river[n+1]==0)space++;}
					if(n<158){river[n+2]=river[n+2]-2;if(river[n+2]==0)space++;}
					if(n<157){river[n+3]=river[n+3]-2;if(river[n+3]==0)space++;}
					if(n<156){river[n+4]=river[n+4]-2;if(river[n+4]==0)space++;}
					if(n<155){river[n+5]=river[n+5]-2;if(river[n+5]==0)space++;}


					if(data>=1){
					printf("カップル退出,観測場所から%d mの箇所から帰って行った。\n",n+1);
            if(data == 2){
					    printf("S:");
					    for(l=0;l<160;l++){

						    if(l==n)printf("×");
						    else if(river[l]==0)printf("□");
					  	  else printf("■");
					    }
					    printf(":G\n");	
            }
					}		
					t_sum[couple+1]=t_sum[couple+1] + t_x;
					t_w = t_w + (t_x*(couple+1));
					Out = t + exp_rand(mu);
					
				}				
				else{//誰もいない
					t_sum[0]=t_sum[0]+ t_x;
					t_w = t_w + t_x;
					Out = In + exp_rand(mu); 
				}

			}



			else{//同時発生
				t_x = Out - t;
				t =  Out;
				t_sum[couple]=t_sum[couple]+t_x;
				t_w = t_w + (t_x*(couple));
				pack++;
				In = t + exp_rand(la);
				Out = t + exp_rand(mu); 
			}	
	//printf("space:%d\n",space);
		}


	//開始時間から最初のイベントまでの時間
	 t_sum[couple] = t_sum[couple] - t + t_en;

	//計算

	for(j=0;j<160;j++) ave_pack = ave_pack + j * t_sum[j] / (t_en - t_be) ;
	printf("\n*****シミュレーション結果*****\n");
	printf("河川敷にいるカップル数の平均（平均パケット数）:%f(組)\n", ave_pack);
	printf("カップルが河川敷に居座る時間の平均（平均滞在時間）:%f(分)\n", t_w/pack);
	printf("訪れたカップルの数:%d（組）\n",pack+loss);
	printf("座れたカップルの数:%d（組）\n",pack);
	printf("座れずに帰ったカップルの数:%d（組）\n",loss);


	return 0;

}

double exp_rand(double x) {//指数分布に従う乱数生成
	return ( -log( 1.0 - ( (double)rand()/((double)RAND_MAX)) ) )/ x;

}


