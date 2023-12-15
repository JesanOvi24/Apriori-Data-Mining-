#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#define minimumSupportThreshold 85


using namespace std;


struct pattern
{
    vector<string> item ;
    double support ;
};

int numOftran ;
vector<string>transaction[3200] ;
vector<string>itemList ;
vector<pattern>C[100], L[100] ;


void printTransaction()
{
    for(int i = 0 ; i < numOftran ; i++){
        for(int j = 0 ; j < transaction[i].size() ; j++)
            cout << transaction[i][j] << " " ;
        cout<<endl ;
    }

    cout<<numOftran<<endl ;
}

void printItem()
{
    for(int i = 0 ; i < itemList.size() ; i++){
        cout<<itemList[i] << " " ;
    }
    cout<<endl << itemList.size() << endl ;
}

double calculateSupport(string item)
{
    double sup = 0.0 ;
    for(int i = 0 ; i < numOftran ; i++){
        for(int j = 0 ; j < transaction[i].size() ; j++){
            if(item == transaction[i][j]){
                sup++ ;
                break ;
            }
        }
    }
    return sup ;
}

bool isJoinable(vector<string> itemi , vector<string> itemj, int currentLen)
{
    if(currentLen == 1) return true ;
    else{
        int count = 0 ;
        for(int i = 0 ; i < itemi.size() - 1 ; i++){
            if(itemi[i] == itemj[i]) count++ ;

        }
        if(count == currentLen-1) return true ;
        return false ;
    }
}
bool isFrequent(vector<string> t, int currentLen)
{
    for(int i = 0 ; i < L[currentLen].size() ; i++){
        int count = 0 ;
        for(int j = 0 ; j < L[currentLen][i].item.size() ; j++){
           if(t[j] == L[currentLen][i].item[j]) count++ ;
        }
        if(count == t.size()){
            return true ;
        }
    }
    return false ;
}
bool checkDownwardClosur(vector<string> str , int currentLen)
{

    for(int i = 0 ; i < str.size() ; i++){
        vector<string> t ;
        for(int j = 0 ; j < str.size() ; j++){
            if(j != i){
                t.push_back(str[j]) ;
            }
        }
        if(!isFrequent(t, currentLen)){
                return false ;
        }

    }
    return true ;
}

double calculateSupportPatt(vector<string> itemset)
{
    double sup = 0.0 ;
    for(int i = 0 ; i < numOftran ; i++){
        int count = 0 ;
        for(int j = 0 ; j < itemset.size() ; j++){
            for(int k = 0 ; k < transaction[i].size() ; k++){
                if(itemset[j] == transaction[i][k]){
                    count++ ;
                    break ;
                }
            }
        }
    if(count == itemset.size()) sup++ ;
    }
    return sup ;
}
void printCandidateList(int len)
{
    for(int i = 1 ; i <= len ; i++){
        cout << "Length : " << i << endl ;
        for(int j = 0 ; j < C[i].size() ; j++){

                for(int k = 0 ;  k < C[i][j].item.size() ; k++)
                    cout << C[i][j].item[k] << " " ;
                cout << "Support (Relative): " << C[i][j].support << endl ;

        }
    }
}
void printFrequentList(int len)
{
    for(int i = 1 ; i <= len ; i++){
        cout << "Length : " << i << endl ;
        for(int j = 0 ; j < L[i].size() ; j++){

                for(int k = 0 ;  k < L[i][j].item.size() ; k++)
                    cout << L[i][j].item[k] << " " ;
                cout << L[i][j].support << endl ;

        }
    }
}
int apriori(int currentLen)
{
    while(true){
        for(int i = 0 ; i < L[currentLen].size() - 1 ; i++){
            for(int j = i+1 ;  j < L[currentLen].size() ; j++){
                if(isJoinable(L[currentLen][i].item , L[currentLen][j].item , currentLen)){
                    vector<string> str ;
                    for(int k = 0 ; k < L[currentLen][i].item.size() ; k++)
                        str.push_back(L[currentLen][i].item[k]) ;
                    str.push_back(L[currentLen][j].item[currentLen-1]) ;
                    if(checkDownwardClosur(str, currentLen)){
                        double sup = calculateSupportPatt(str) ;
                        struct pattern temp ;
                        for(int l = 0 ; l < str.size() ; l++)
                            temp.item.push_back(str[l]) ;
                        temp.support = (sup/numOftran) * 100 ;
                        C[currentLen+1].push_back(temp) ;

                   }

                }
            }
        }
        if(C[currentLen+1].size() == 0) return currentLen;
        for(int i = 0 ; i < C[currentLen+1].size() ; i++){

            if(C[currentLen+1][i].support >= minimumSupportThreshold){
                L[currentLen+1].push_back(C[currentLen+1][i]) ;
            }
        }
        currentLen++ ;
        if(L[currentLen].size() == 0) return currentLen;

        for(int j = 0 ; j < L[currentLen].size() ; j++){
            for(int k = 0 ;  k < L[currentLen][j].item.size() ; k++)
                cout << L[currentLen][j].item[k] << " " ;
            cout << "Support (Relative): "<< L[currentLen][j].support << endl ;

        }
    }


   //  printCandidateList(currentLen+1) ;
  // printFrequentList(currentLen+1) ;
  return currentLen ;
}
int main()
{
    fstream newfile;
    newfile.open("chess.dat",ios::in);
    string tp;
    while(getline(newfile, tp)){
        stringstream check1(tp) ;
        string intermediate;
        while(getline(check1, intermediate, ' '))
        {
            transaction[numOftran].push_back(intermediate) ;
            bool flag = false ;
            for(int i = 0 ; i < itemList.size() ; i++){
                if(itemList[i] == intermediate){
                    flag = true ;
                    break ;
                }
            }
            if(!flag) itemList.push_back(intermediate) ;
        }
        numOftran++ ;
    }
    newfile.close();


    sort(itemList.begin() , itemList.end()) ;
    for(int i = 0 ; i < numOftran ; i++)
        sort(transaction[i].begin() , transaction[i].end()) ;

    for(int i = 0 ; i < itemList.size() ; i++){
        double sup = calculateSupport(itemList[i]) ;
        struct pattern temp ;
        temp.item.push_back(itemList[i])  ;
        temp.support = (sup/numOftran)*100 ;
        C[1].push_back(temp) ;
    }

    for(int i = 0 ; i < C[1].size() ; i++){

        if(C[1][i].support >= minimumSupportThreshold){
            L[1].push_back(C[1][i]) ;
        }
    }
    int currentLen = 1 ;

    for(int i = 0 ; i < L[1].size() ; i++){
        for(int j = 0 ; j < L[1][i].item.size() ; j++)
            cout<< L[1][i].item[j] ;
        cout<< " " << L[1][i].support << endl ;

    }

    int len = apriori(currentLen) ;
    printFrequentList(len) ;




}
