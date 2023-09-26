#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <thread>
#include <unordered_map>
#include <unordered_map>

#include <mutex>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
//#include <functional>


using namespace std;

class WaterManager{
    public:
    mutex mtx;
    condition_variable cv;
    atomic<int> gnusInside{0};
    atomic<int> hyenasInside{0};

    WaterManager(){}

    void hyenaEnters(int nr,string state){
        unique_lock<mutex> lck(mtx);
        if(state=="on"){
            cout<<"Hyena " <<nr<<" is thirsty"<<endl; 
        }      
        cv.wait(lck, [this] {return gnusInside==0;});
        hyenasInside +=1;
        if(state=="on"){
            cout<<"A hyena enters the water cave"<<"      " <<"hyenas="<<hyenasInside<<"   "<<"gnus="<<gnusInside<<endl;      
        } 
        cv.notify_all();
    }

    void gnuEnters(int nr,string state){
        unique_lock<mutex> lck(mtx);
        if(state=="on"){
            cout<<"Gnu " <<nr<<" is thirsty"<<endl;
        }
        cv.wait(lck, [this] {return hyenasInside==0;});
        gnusInside +=1;
        if(state=="on"){
            cout<<"A gnu enters the water cave"<<"      " <<"hyenas="<<hyenasInside<<"   "<<"gnus="<<gnusInside<<endl;
        }
        cv.notify_all();
    }


    void hyenaLeaves(int nr, string state){
        mtx.lock();
        hyenasInside-=1;
        if(state=="on"){
            cout<<"Hyena "<<nr<<" finished drinking and exits the water cave"<<endl;
        }
        mtx.unlock();
        cv.notify_all();

    }
    void gnuLeaves(int nr,string state){
        mtx.lock();
        gnusInside-=1;
        if(state=="on"){
            cout<<"Gnu "<<nr<<" finished drinking and exits the water cave"<<endl;
        }
        mtx.unlock();
        cv.notify_all();

    }


};

class Gnu{
    public:
    WaterManager & watermanager;
    int Gid;
    string state;
    Gnu(int id, WaterManager & waterMan,string s): Gid(id), watermanager(waterMan), state(s) {}
    void operator()() {
        this_thread::sleep_for(1000ms);
        watermanager.gnuEnters(Gid,state);
        this_thread::sleep_for(1000ms);
        watermanager.gnuLeaves(Gid,state);
        this_thread::sleep_for(1000ms);
    }
};

ostream & operator<<(ostream & os, const Gnu & g) {
    os<<"Gnu nr " << g.Gid<<endl;
    return os;
}

void gnu(int nr, WaterManager & watermanager, unordered_map<thread::id , string> & threadmap,string state){
        threadmap[this_thread::get_id()]=("Gnu nr " + to_string(nr));
        this_thread::sleep_for(1000ms);
        watermanager.gnuEnters(nr, state);
        this_thread::sleep_for(1000ms);
        watermanager.gnuLeaves(nr,  state);
        this_thread::sleep_for(1000ms);


}

void hyena(int nr,WaterManager & watermanager, unordered_map<thread::id , string> & threadmap,string state){          
        threadmap[this_thread::get_id()]=("Hyena nr " + to_string(nr));
        this_thread::sleep_for(1000ms);
        watermanager.hyenaEnters(nr, state);
        this_thread::sleep_for(1000ms);
        watermanager.hyenaLeaves(nr, state);
        this_thread::sleep_for(1000ms);
}
void execute(string state){
    unordered_map<thread::id , string> threadmap;
    vector<thread> threadvec;
    int nrGnus=5;   
    int nrHyenas=5;    
    WaterManager watermanager;
    for (int i = 0; i < nrGnus; i++){ 
        Gnu g=Gnu(i+1,watermanager,state);
        function<void(void)> threadjob = g;
        //threadvec.push_back(thread(threadjob));
        threadvec.push_back(thread(gnu, i+1, ref(watermanager),ref(threadmap), state));
        // if(state=="on"){
        //     cout<<g;
        // }

    }
    for (int i = 0; i < nrHyenas; i++){
        threadvec.push_back(thread(hyena, i+1,ref(watermanager),ref(threadmap), state));
    }   
    //this_thread::sleep_for(1000ms);  
    if(state=="on"){
        for(auto& x : threadmap){
             cout<< x.second << endl;
        }
    }
    for (auto& thread : threadvec){
         thread.join();
     }


}

int main(int argc, char * argv[]){
    if(argc==1){
        cout<<"You must specify if you want the trace outputs on or off by writing on or off"<<endl;
        return 0;
    } 
    std::string arg1(argv[1]);
    if(!((arg1=="on") || (arg1=="off"))){
        cout<<"You must write on or off"<<endl;
        return 0;
    }  
    execute(arg1);     

    return 0;
}

