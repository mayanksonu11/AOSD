// CPP program to demonstrate multithreading
// using three different callables.
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include <queue>

#define MAX_CUSTOMERS 100

using namespace std;

sem_t max_capacity;
sem_t sofa;
sem_t barber_chair,coord;
sem_t mutex1, mutex2;
sem_t customer_ready,leave_b_chair,payment,reciept;
sem_t finished[100];
int count;
queue<int> q;

// void customer();
// void cashier();
// void barber();

void enter_shop(int cust_number){
	cout << "Customer " << cust_number << " entering to the shop" << endl;
}
void sit_on_sofa(int cust_num){
	cout << "Customer " << cust_num << " sitting on the sofa" << endl;
}
void get_up_from_sofa(int cust_num){
	
	cout << "Customer " << cust_num << " getting up from the sofa" << endl;
}
void sit_on_barber_chair(int cust_num){
	cout << "Customer " << cust_num << " sitting on the barber chair" << endl;
}
void leave_barber_chair(int cust_num){
	cout << "Customer " << cust_num << " leaving fonrom the barber chair" << endl;
}
void pay(int cust_num){
	cout << "Customer " << cust_num << " paying " << endl;
}
void exit_shop(int cust_num){
	cout << "Customer " << cust_num << " exitting shop" << endl;
}
void cut_hair(int b_cust){
	cout << "Customer " << b_cust << " getting it's hair cut" << endl;
}
void accept_pay(){
	cout << "Accepting payment" << endl;
}
// A callable object
class customer {
public:
	void operator()()
	{

		int cust_number;
    	sem_wait(&max_capacity);
    	count++;
    	cust_number =count;
    	enter_shop(cust_number);
    	sem_wait(&mutex1);
    	sem_post(&mutex1);
    	sem_wait(&sofa);
    	sit_on_sofa(cust_number);
    	sem_wait(&barber_chair);
    	get_up_from_sofa(cust_number);
    	sem_post(&sofa);
    	sit_on_barber_chair(cust_number);
    	sem_wait(&mutex2); // for queuing the user at barber chair
    	q.push(cust_number);
    	sem_post(&customer_ready);
    	sem_post(&mutex2);
    	sem_wait(&finished[cust_number]);
    	leave_barber_chair(cust_number);
    	sem_post(&leave_b_chair);
    	pay(cust_number);
    	sem_post(&payment);
    	sem_wait(&reciept);
    	exit_shop(cust_number);
    	sem_post(&max_capacity);
	}
};

class barber {
public:
    barber(string msg){
        cout << msg << endl;
    }
	void operator()()
	{
		int b_cust;
    	while(true)
    	{
        	sem_wait(&customer_ready);
        	sem_wait(&mutex2);
        	b_cust = q.front(); q.pop();
        	sem_post(&mutex2);
        	sem_wait(&coord);
        	cut_hair(b_cust); // added parameter to the function
        	sem_post(&coord);
        	sem_post(&finished[b_cust]);
        	sem_wait(&leave_b_chair);
        	sem_post(&barber_chair);
    	}
	}
};

class cashier {
public:
    cashier(int _flag){};
	void operator()()
	{
    	while(true)
    	{
        	sem_wait(&payment);
        	sem_wait(&coord);
        	accept_pay();
        	sem_post(&coord);
        	sem_post(&reciept);
    	}
	}	
};


int main()
{
	
	sem_init(&max_capacity,0,20);
	sem_init(&sofa,0,4);
	sem_init(&barber_chair,0,3);
	sem_init(&coord,0,3);
	sem_init(&mutex1,0,1);
	sem_init(&mutex2,0,1);
	sem_init(&customer_ready,0,0);
	sem_init(&leave_b_chair,0,0);
	sem_init(&payment,0,0);
	sem_init(&reciept,0,0);
	for(int i=0;i<MAX_CUSTOMERS;i++){
		sem_init(&finished[i],0,0);
	}
	// count=0;
	

	thread th1(barber("Welcome to Barberland!"));
	thread th2(cashier(0));

	vector<thread> th;

	for(int i=0;i<MAX_CUSTOMERS;i++){
		th.push_back(thread (customer()));
		// cust.detach();
	}

	for(int i=0;i<MAX_CUSTOMERS;i++){
		th[i].join();
	}
	th1.join();
	th2.join();

	sem_destroy(&max_capacity);
	sem_destroy(&sofa);
	sem_destroy(&barber_chair);
	sem_destroy(&coord);
	sem_destroy(&mutex1);
	sem_destroy(&mutex2);
	sem_destroy(&customer_ready);
	sem_destroy(&leave_b_chair);
	sem_destroy(&payment);
	sem_destroy(&reciept);
	for(int i=0;i<MAX_CUSTOMERS;i++){
		sem_destroy(&finished[i]);
	}
	return 0;
}



// void customer()
// {
//     int cust_number;
//     sem_wait(&max_capacity);
//     count++;
//     cust_number =count;
//     enter_shop(cust_number);
//     sem_wait(&mutex1);
//     sem_post(&mutex1);
//     sem_wait(&sofa);
//     sit_on_sofa(cust_number);
//     sem_wait(&barber_chair);
//     get_up_from_sofa(cust_number);
//     sem_post(&sofa);
//     sit_on_barber_chair(cust_number);
//     sem_wait(&mutex2); // for queuing the user at barber chair
//     q.push(cust_number);
//     sem_post(&customer_ready);
//     sem_post(&mutex2);
//     sem_wait(&finished[cust_number]);
//     leave_barber_chair(cust_number);
//     sem_post(&leave_b_chair);
//     pay(cust_number);
//     sem_post(&payment);
//     sem_wait(&reciept);
//     exit_shop(cust_number);
//     sem_post(&max_capacity);
// }

// void barber()
// {
//     int b_cust;
//     while(true)
//     {
//         sem_wait(&customer_ready);
//         sem_wait(&mutex2);
//         b_cust = q.front(); q.pop();
//         sem_post(&mutex2);
//         sem_wait(&coord);
//         cut_hair(b_cust); // added parameter to the function
//         sem_post(&coord);
//         sem_post(&finished[b_cust]);
//         sem_wait(&leave_b_chair);
//         sem_post(&barber_chair);
//     }
// }

// void cashier()
// {
//     while(true)
//     {	
//         sem_wait(&payment);
//         sem_wait(&coord);
//         accept_pay();
//         sem_post(&coord);
//         sem_post(&reciept);
//     }
// }