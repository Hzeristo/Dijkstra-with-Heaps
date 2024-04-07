#include "default.h"
#include "Fib_Heap.h"
#include "Binomial_Queue.h"
#include "Dijkstra.h"
#include "read_gr_txt.h"

using namespace std;

int main(){
    try{
        Graph t({});
        t.buildGraphFromtxt(".\\docs\\USA-road-d.NY.txt");
        double sum = 0;
        for(int i = 0; i < 100; i++)
        {
            FibHeap fh;
            Binomial_Queue bq;
            LARGE_INTEGER t1,t2,tc;
            QueryPerformanceFrequency(&tc);
            QueryPerformanceCounter(&t1);
            int d = dijkstra(t, 30, 23, bq);
            cout <<"distance = "<< d;
            QueryPerformanceCounter(&t2);
            double time = (double)(t2.QuadPart-t1.QuadPart)/(double)tc.QuadPart; 
            sum += time;
            cout<<" time = "<<time<<"s"<<endl;
        }
        cout << "total time = "<<sum<<"s"<<endl;
    }catch(const char* msg){
        cout << msg << endl;
    }
    return 0;
}