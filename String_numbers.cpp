#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
//i know you need this: -std=c++98 -pedantic
//also this: g++ .\String_numbers.cpp -std=c++98 -pedantic -o .\String_numbers.exe
// 123456 -> [456, 123]
// negative numbers: [1, 456, 123]
class LNum {
        vector <int> num_vec; // number
        bool type; // 0 = +; 1 = -
        friend ostream& operator<<(ostream& os, LNum num);
        friend istream& operator>>(istream& is, LNum& num);
    public:
        LNum(){
            num_vec.push_back(0);
            type = 0;
        }

        void put(long int a){
            num_vec.clear();
            while (a>0){
                num_vec.push_back(a%1000);
                a/=1000;
            }
        }

        void put(string a){
            // a= '123456789'
            // [789, 456, 123]
            //a = 1350000
            num_vec.clear();
            int ins = 0;
            int rev_ins = 0;
            int k = 0;
            int koeff = 1;
            for (long int i =a.size()-1; i>=0; i--){//1 350 000
                if (k<3){
                    cout << a[i] << "this is a[i]";
                    ins = ins*10 + (a[i]-'0'); //100 -> 1
                    cout << ins << "ins" << "\n"; 
                    k++;
                }
                if (k==3 ){
                    
                    if (ins <10)
                        koeff = 100;
                    else if (ins >=10 && ins<100)
                        koeff = 10;
                    
                    while (ins >0){
                    rev_ins = rev_ins*10 + ins%10; 
                    ins/=10;
                    }
                    rev_ins*=koeff;
                    cout << rev_ins << "\n";
                    num_vec.push_back(rev_ins);
                    rev_ins = 0;
                    k = 0;
                    koeff = 1;
                }
                for (int i=0; i<num_vec.size(); i++){
                    cout << num_vec[i] << " ";
                }
            }       
            while (ins >0){
            rev_ins = rev_ins*10 + ins%10; 
            ins/=10;
            }
            cout << rev_ins << "\n";
            num_vec.push_back(rev_ins);
            rev_ins = 0;
            k = 0;
            
        }

        void pop(){
            if (num_vec.size()>0)
                num_vec.pop_back();
            else
                cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
        }

        void out(){
            if (type == 1)
                cout << "-";
            for  (int i=0; i<num_vec.size(); i++)
                cout << num_vec[i] << " ";
        }
        // 10 - 5 == 10 + -5 == -5 + 10                         
        LNum operator +(LNum a){
            LNum sum;
            sum.pop();
            while (max(a.num_vec.size(),num_vec.size())>min(a.num_vec.size(), num_vec.size())){
                if (a.num_vec.size()>num_vec.size())
                    num_vec.push_back(0);
                else
                    a.num_vec.push_back(0);
            } // 100 100 1 ; 900 900 0
            int leftover = 0;
            for (int i=0; i<num_vec.size(); i++){
                int fir = a.num_vec[i];
                int sec = num_vec[i];
                sum.num_vec.push_back((fir+sec)%1000+leftover);
                leftover = (fir+sec)/1000;
            }
            if (leftover !=0 )
                sum.num_vec.push_back(leftover);
            return sum;
        }

        bool operator <(LNum a){
            if (num_vec.size()<a.num_vec.size())
                return true;
            else if (num_vec.size()>a.num_vec.size())
                return false;
            else if (type == 1 && a.type == 0)
                return true;
            else if (type == 0 && a.type == 1)
                return false;
            else{
                if (type == 1){
                    for (int i=num_vec.size()-1; i>=0; i--){
                        if (num_vec[i]>a.num_vec[i])
                            return true;
                        else if (num_vec[i]<a.num_vec[i])
                            return false;
                    }
                    return false;//in case "="
                }
                else {
                    for (int i=num_vec.size()-1; i>=0; i--){
                        if (num_vec[i]>a.num_vec[i])
                            return false;
                        else if (num_vec[i]<a.num_vec[i])
                            return true;
                    }
                    return false;//in case "="
                }
                
            }
        }

        bool operator >(LNum a){
            if (num_vec.size()>a.num_vec.size())
                return true;
            else if (num_vec.size()<a.num_vec.size())
                return false;
            else if (type == 0 && a.type == 1)
                return true;
            else if (type == 1 && a.type == 0)
                return false;
            else{
                if (type == 0){
                    for (int i=num_vec.size()-1; i>=0; i--){
                        if (num_vec[i]>a.num_vec[i])
                            return true;
                        else if (num_vec[i]<a.num_vec[i])
                            return false;
                    }
                    return false;//in case "="
                }
                else {
                    for (int i=num_vec.size()-1; i>=0; i--){
                        if (num_vec[i]>a.num_vec[i])
                            return false;
                        else if (num_vec[i]<a.num_vec[i])
                            return true;
                    }
                    return false;//in case "="
                }
                
            }
        }

        bool operator ==(LNum a){
            if (num_vec.size()>a.num_vec.size())
                return false;
            else if (num_vec.size()<a.num_vec.size())
                return false;
            else if (type != a.type)
                return false;
            else{
                for (int i=num_vec.size()-1; i>=0; i--){
                    if (num_vec[i]>a.num_vec[i])
                        return false;
                    else if (num_vec[i]<a.num_vec[i])
                        return false;
                }
                return true;
            }
        }
/*
        LNum operator -(LNum a){
            LNum diff;
            
        }
*/

        ~LNum(){};

};

ostream& operator<<(ostream& os, LNum num){
    if (num.type==1)
        os << "-";
    for (long int i=num.num_vec.size()-1; i>=0; i--){
        if (num.num_vec[i]<10 && i!=num.num_vec.size()-1)
            os << 0 << 0 << num.num_vec[i] << " "; // 010 100 1 - 1100100 
        else if (num.num_vec[i]>=10 && num.num_vec[i]<100 && i!=num.num_vec.size()-1)
            os << 0 << num.num_vec[i] << " ";
        else
            os << num.num_vec[i] << " ";
    } 
                
    os << "\n";
    return os;
}

istream& operator>>(istream& is, LNum& num){
    string inp, a = "";
    is >> inp;
    if (inp[0]=='-'){
        num.type=1;
        for (int i = 1; i<inp.size();i++)
            a+=inp[i];
    }
    else
        a = inp;
    num.num_vec.clear();
    int ins = 0;
    int rev_ins = 0;
    int k = 0;
    int koeff = 1;
    for (long int i =a.size()-1; i>=0; i--){//1 350 000
        if (k<3){
            ins = ins*10 + (a[i]-'0'); //100 -> 1
            k++;
        }
        if (k==3 ){
            if (ins <10)
                koeff = 100;
            else if (ins >=10 && ins<100)
                koeff = 10;
                    
            while (ins >0){
                rev_ins = rev_ins*10 + ins%10; 
                ins/=10;
            }
                rev_ins*=koeff;
                num.num_vec.push_back(rev_ins);
                rev_ins = 0;
                k = 0;
                koeff = 1;
        }
    }       
    while (ins >0){
        rev_ins = rev_ins*10 + ins%10; 
        ins/=10;
        }
    if (rev_ins !=0 )
        num.num_vec.push_back(rev_ins);
    rev_ins = 0;
    k = 0;


    return is;
            
}



int main(){
    
    LNum a, b;
    cin >> a >> b;
    if (a < b)
        cout << "yes" << "\n";
    cout << a << b;
    

    

    return 0;
}
