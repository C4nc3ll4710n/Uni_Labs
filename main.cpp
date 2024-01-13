#include <iostream>
#include <set>
#include <unistd.h>

using namespace std;

template <typename T>

ostream& operator<<(ostream& o, const set<T>& a) {
	typename set<T>::const_iterator i = a.begin();
    	o << "{" << *i;
    	for (++i; i != a.end(); ++i)
        	o << ", " << *i;
    	o << "}\n";
    
    	return o;
}

int main(){
    set<int> s;
    for (int i = 0; i < 10; i++)
        s.insert(i);
    cout << s;
    
    char spinner[4]  = {'\\', '|', '/', '-'};
    int index = 0;
    while (true){
    	//usleep(500);
    	cout << '\r' << spinner[index++];
    	if (index == 4) index = 0;
    	
    }
    
    
    
    
    
    return 0;
}
