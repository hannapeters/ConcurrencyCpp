#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <thread>

using namespace std;

int main(){
    vector<int> v = {1, 3, 4, 5, 6, 7, 8, 9};
    cout << "vector v odd elements:" << endl;
    for_each(v.cbegin(), v.cend(),[&] (int const & i){
        if(v[i]%2==1){
            cout<< v[i]<<" ";
        }
    }
    );
    cout<<endl;
    for_each(v.cbegin(), v.cend(),[&] (int const & i){
        if(v[i]%2==1){
            v[i]=2*v[i];
        }
    }
    );
    int outSideData=0;
    auto x=[& outSideData]() mutable { outSideData++; return outSideData;};
    cout<<x();

    return 0;
}
