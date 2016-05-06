#include "statisticsmodule.h"
#include "SHindex.h"
#include "SHgeneral.h"
#include "SHselection.h"
#include "data.h"
#include <iostream>
#include <cstring>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

SHGeneral shg;
SHSelection shs;
SHIndex shi;
StatisticsModule st;

float data[datasize][D];
IO io;
Knn knn;

int main()
{
    char file_path[20]="Enron";
    char data_name[20]="enron";
    char data_path[100];
    sprintf(data_path,"~/Downloads/Dataset/%s/data/%s_base.dat",file_path,data_name);
    char query_path[100];
    sprintf(query_path,"~/Downloads/Dataset/%s/query/%s_query.dat",file_path,data_name);

    char index_path[100];
    sprintf(index_path,"indices/%s_%.1fc_index",data_name,c);

    char result_path[100];
    sprintf(result_path,"~/Downloads/Dataset/%s/exp_fig/result/DSH/k=%d/%.1fc.txt",file_path,K,c);

    char gnd_path[100];
    sprintf(gnd_path,"~/Downloads/Dataset/%s/gnd/%s_groundtruth.dat",file_path,data_name);
    char decision_path[50]="result/decision.dat";
    char query_result_path[50]="result/result.dat";

    io.diskread_float(data_path, data[0], datasize*D);
    cout<<"data read from disk"<<endl;


    int start_=clock();
	shg.init();
	shs.radius_selection(decision_path);
	//index
    shi.index_construct(decision_path);
    int finish_=clock();
    float indextime=(finish_-start_)*1.0/CLOCKS_PER_SEC;

    ofstream fout;
    fout.open(result_path,ios::app);
    fout<<indextime<<" #index_time "<<endl;

    shi.index_write(index_path);


    shi.index_load(index_path);
    
    //search
    int MaxChecked[20]={50,100,300,500,700,1000,2000,3000,5000,7000,8000,10000,12000,15000,20000,30000,50000,60000,70000,100000};
    int i=L;
    shi.query_load(query_path);
    cout<<"load query"<<endl;
    for(int j=0;j<20;j++)
    {
        st.begin();
        {
            shi.query_execute(i,MaxChecked[j]);
        }
        cout<<"query success";
        //shi.query_execute(i);
        st.finish();
        shi.result_write(query_result_path);

        //fout.open(result_path, ios::out | ios::app);
        //fout.close();

        st.stat_output(query_path,gnd_path,query_result_path,result_path,MaxChecked[j]);
    }
    cout<<"program finished"<<endl;
    //int forcin; cin>>forcin;
	return 0;
}


