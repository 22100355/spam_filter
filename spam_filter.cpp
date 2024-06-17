#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>
using namespace std;

vector<string> spam_tr;
vector<string> ham_tr;
vector<string> m_specialCharList;


typedef struct _specialchar
{
    char start;
    char end;
    _specialchar(char _s,char _e){ start=_s; end=_e; }
}ST_SPECIALCHAR;

string replaceAll(string &str, const string &pattern, const string &replace) 
{ 
    size_t start_pos = 0;
    while ((start_pos = str.find(pattern, start_pos)) != std::string::npos){
        str.replace(start_pos,pattern.length(),replace);
        start_pos +=replace.length();
    }
    return str;
}
string Eliminate(string isString, int WordCount){
    for(int i=0; i<(int)m_specialCharList.size(); i++){
        replaceAll(isString,m_specialCharList[i],std::string(" "));
    }

    for(int i=0;i<WordCount;i++) replaceAll(isString,std::string("  "),std::string(" "));

    return isString;
} 

string filter(string &str){
    for(int i=0;i<str.size();i++) str[i]=std::toupper(str[i]);
    str=replaceAll(str, " -", "");
    str=replaceAll(str, " .", "");
    str=replaceAll(str, " '", "");
    str=replaceAll(str, " !", "");
    str=replaceAll(str, " @", "");
    str=replaceAll(str,"\r","");
    str=replaceAll(str,"\n"," ");

    int tempWordCount=10;
    str=Eliminate(str,tempWordCount);
    return str;
}

vector<string> split(string str, char Delimiter) {
    istringstream iss(str);
    string buffer;

    vector<string> result;
    
    while (getline(iss, buffer, Delimiter)) {
        result.push_back(buffer);
    }

    return result;
    }
    
vector<string> Laplace(vector<string> a){
        vector<string> temp;
        int i=3;
        int index=0;
        while (true)
        {
            if(atoi(a[i].c_str()) != 0 || a[i].compare("0") == 0) {
                //temp.push_back(a[i]);
                int ind= atoi(a[i].c_str());
                ind++;
                i=i+2;
                while(true){
                    if(i>=a.size() || atoi(a[i].c_str())==ind) break;
                    if(a[i] !="\r") temp.push_back(a[i]);
                    i++;
                }
                if(i>=a.size()) break;
            }
        }
        temp.erase(unique(temp.begin(),temp.end()),temp.end());
        return temp;
    }

    vector<vector<string> > mail(vector<string> a){
        vector<vector<string> > sp_data;
        int i=3;
        int index=0;
        while (true)
        {
            vector<string> temp;
            if(atoi(a[i].c_str()) != 0 || a[i].compare("0") == 0) {
                temp.push_back(a[i]);
                int ind= atoi(a[i].c_str());
                ind++;
                i=i+2;
                while(true){
                    if(i>=a.size() || atoi(a[i].c_str())==ind) break;
                    if(a[i] !="\r") temp.push_back(a[i]);
                    i++;
                }
                sp_data.push_back(temp);
                if(i>=a.size()) break;
                temp.clear();
            }
        }
        return sp_data;
    }

    long double cal_p(string word,vector<vector<string> > text,vector<string> laplace){
        long double count=0;
        for(int i=0;i<text.size();i++){
            for(int j=1;j<text[i].size();j++){
                if(text[i][j].compare(word)==0) {
                    count+=1.0;
                }
            }
        }
        return (count+1.0)/((float)(text.size()+laplace.size()));
    }


    int main(void) {
        vector<ST_SPECIALCHAR> vList; vList.clear();
        vList.push_back(ST_SPECIALCHAR(0x21,0x2f));
        vList.push_back(ST_SPECIALCHAR(0x3a,0x40));
        vList.push_back(ST_SPECIALCHAR(0x5b,0x60));
        vList.push_back(ST_SPECIALCHAR(0x7b,0x7e));
        
        m_specialCharList.clear();
        for(int i=0;i<(int)vList.size();i++){
            char _start=vList[i].start;
            char _end=vList[i].end;
            
            for(char _ch=_start;_ch<=_end;_ch++){
                string _str;
                _str=_ch;
                m_specialCharList.push_back(_str);
            }
        }

        string str_buf;         
        fstream fs; 
        
        fs.open("csv/train/dataset_spam_train100.csv",ios::in);
        string spam_train;
        if (fs.is_open())
        {
            fs.seekg(0, std::ios::end);
            int size = fs.tellg();
            spam_train.resize(size);
            fs.seekg(0, std::ios::beg);
            fs.read(&spam_train[0], size);
        }
        spam_train=filter(spam_train);
        fs.close();
        
        spam_tr=split(spam_train, ' ');
        vector<string> Laplace_sp=Laplace(spam_tr);
        vector<vector<string> > spam_train_vec=mail(spam_tr);


        fs.open("csv/train/dataset_ham_train100.csv", ios::in);
        string ham_train;
        if (fs.is_open())
        {
            fs.seekg(0, std::ios::end);
            int size = fs.tellg();
            ham_train.resize(size);
            fs.seekg(0, std::ios::beg);
            fs.read(&ham_train[0], size);
        }
        ham_train=filter(ham_train);
        fs.close();
        
        ham_tr=split(ham_train, ' ');
        vector<string> Laplace_ham=Laplace(ham_tr);
        vector<vector<string> >  ham_train_vec=mail(ham_tr);


        //----------train 데이터 처리--------------------


        fs.open("csv/test/dataset_spam_test20.csv",ios::in);
        string spam_test;
        if (fs.is_open())
        {
            fs.seekg(0, std::ios::end);
            int size = fs.tellg();
            spam_test.resize(size);
            fs.seekg(0, std::ios::beg);
            fs.read(&spam_test[0], size);
        }
        fs.close();
        spam_test=filter(spam_test);
        
        vector<string> te_spam=split(spam_test,' ');
        vector<vector<string> > sp_data=mail(te_spam);

        fs.open("csv/test/dataset_ham_test20.csv",ios::in);
        string ham_test;
        if (fs.is_open())
        {
            fs.seekg(0, std::ios::end);
            int size = fs.tellg();
            ham_test.resize(size);
            fs.seekg(0, std::ios::beg);
            fs.read(&ham_test[0], size);
        }
        fs.close();
        ham_test=filter(ham_test);
        vector<string> te_ham=split(ham_test,' ');
        vector<vector<string> > ham_data=mail(te_ham);
        
        
        long double sp_P[20]={0};
        for(int k=0;k<sp_data.size();k++){
            long double po=1;
            for(int j=1;j<sp_data[k].size();j++){
                po *= cal_p(sp_data[k][j],spam_train_vec,Laplace_sp);
            }
            sp_P[k]=po*0.5;
        }

        long double sp_Q[20]={0};
        for(int k=0;k<sp_data.size();k++){
            long double po=1;
            for(int j=1;j<sp_data[k].size();j++){
                po *= cal_p(sp_data[k][j],ham_train_vec,Laplace_ham);
            }
            sp_Q[k]=po*0.5;
        }
        long double sp_R[20]={0};
        for(int i=0;i<20;i++){
            sp_R[i]=sp_P[i]/(sp_P[i]+sp_Q[i]);
        }

//------------------------test_spam--------------------------

         long double ham_P[20]={0};
        for(int k=0;k<ham_data.size();k++){
            long double po=1;
            for(int j=1;j<ham_data[k].size();j++){
                po *= cal_p(ham_data[k][j],spam_train_vec,Laplace_sp);
            }
            ham_P[k]=po*0.5;
        }

        long double ham_Q[20]={0};
        for(int k=0;k<ham_data.size();k++){
            long double po=1;
            for(int j=1;j<ham_data[k].size();j++){
                po *= cal_p(ham_data[k][j],ham_train_vec,Laplace_ham);
            }
            ham_Q[k]=po*0.5;
        }
        long double ham_R[20]={0};
        for(int i=0;i<20;i++){
            ham_R[i]=ham_P[i]/(ham_P[i]+ham_Q[i]);
        }
        
        //-----------------------print-------------------------

        for(int i=0;i<20;i++){
            cout<<"s"<<(i+101)<<" r: "<<sp_R[i]<<endl;
        }

        cout<<endl<<endl;

        for(int i=0;i<20;i++){
            cout<<"h"<<(i+101)<<" r: "<<ham_R[i]<<endl;
        }
        
        cout<<endl<<endl;

        double T=0.6;
        cout<<" case1 T:0.6"<<endl;
        for(int i=0;i<20;i++){
            if(sp_R[i]>T) cout<<"s"<<(i+101)<<": spam"<<endl;
            else cout<<"s"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.7;
        cout<<" case2 T:0.7"<<endl;
        for(int i=0;i<20;i++){
            if(sp_R[i]>T) cout<<"s"<<(i+101)<<": spam"<<endl;
            else cout<<"s"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.8;
        cout<<" case3 T:0.8"<<endl;
        for(int i=0;i<20;i++){
            if(sp_R[i]>T) cout<<"s"<<(i+101)<<": spam"<<endl;
            else cout<<"s"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.9;
        cout<<" case4 T:0.9"<<endl;
        for(int i=0;i<20;i++){
            if(sp_R[i]>T) cout<<"s"<<(i+101)<<": spam"<<endl;
            else cout<<"s"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.95;
        cout<<" case5 T:0.95"<<endl;
        for(int i=0;i<20;i++){
            if(sp_R[i]>T) cout<<"s"<<(i+101)<<": spam"<<endl;
            else cout<<"s"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.6;
        cout<<" case1 T:0.6"<<endl;
        for(int i=0;i<20;i++){
            if(ham_R[i]>T) cout<<"h"<<(i+101)<<": spam"<<endl;
            else cout<<"h"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.7;
        cout<<" case2 T:0.7"<<endl;
        for(int i=0;i<20;i++){
            if(ham_R[i]>T) cout<<"h"<<(i+101)<<": spam"<<endl;
            else cout<<"h"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.8;
        cout<<" case3 T:0.8"<<endl;
        for(int i=0;i<20;i++){
            if(ham_R[i]>T) cout<<"h"<<(i+101)<<": spam"<<endl;
            else cout<<"h"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.9;
        cout<<" case4 T:0.9"<<endl;
        for(int i=0;i<20;i++){
            if(ham_R[i]>T) cout<<"h"<<(i+101)<<": spam"<<endl;
            else cout<<"h"<<(i+101)<<": non-spam"<<endl;
        }

        cout<<endl<<endl;

        T=0.95;
        cout<<" case5 T:0.95"<<endl;
        for(int i=0;i<20;i++){
            if(ham_R[i]>T) cout<<"h"<<(i+101)<<": spam"<<endl;
            else cout<<"h"<<(i+101)<<": non-spam"<<endl;
        }

        return 0;
    }