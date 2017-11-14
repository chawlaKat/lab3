//Lab 3
//Kathy Chawla
//W07681074

#include <cstdio> //needed for perror
#include <iostream>
#include <pthread.h>
#include <cstdlib>

//GLOBAL VARIABLES for the statistics
//Threads can access global data
//It is important to note that no two threads are accessing
//these variables at the same time!
double mean;
double min;
double max;

//A structure to hold parameters to pass to a thread
struct StatData
{
    //The number of numbers
    int N;
    //The array of numbers
    double* numbers;
};

void* minimum(void* data)
{
    //You should calculate the minimum here and store in min
    StatData* myData = (StatData*)data;
    min = myData->numbers[0];
    for(int i = 1; i < myData->N; i++)
    {
        if(myData->numbers[i] < min)
        {
            min = myData->numbers[i];
        }
    }
}

/*
    You should should write the function to calculate
    the maximum here.  This function will take a void *
    argument that is castable to a StatData* and use
    this data to calculate the maximum value.
*/
void* maximum(void* data) 
{
    StatData* myData = (StatData*)data;
    max = myData->numbers[0];
    for(int i = 1; i < myData->N; i++)
    {
	if(myData->numbers[i] > max)
	{
	    max = myData->numbers[i];
	}
    }
}


/*
    You should should write the function to calculate
    the mean here.  This function will take a void *
    argument that is castable to a StatData* and use
    this data to calculate the mean value.
*/
void* average(void* data)
{
    StatData* myData = (StatData*)data;
    int sum = 0;

    for(int i = 0; i< myData->N; i++)
    {
	sum += myData->numbers[i];
    }

    mean = sum / (myData->N);
}

/*
    You need to modify the main function so that it creates
    the maximum and average worker threads which execute the
    correct worker functions you created above.  Remember to
    make sure the main processes waits until they are done before
    printing the results.

    As an interesting excercise, what happens if you don't wait?
*/
int main(int argc, char** argv)
{
    //Declare worker threads
    pthread_t min_t;
    pthread_t max_t;
    pthread_t mean_t;
   
    //Initialize the stat_thread_data structure to hold 
    //the input parameters    
    StatData data;
    //N is the number of numbers
    data.N = argc - 1;
    //Allocate memory for N doubles
    double* numbers = new double[data.N];

    //Fill in the numbers, using atof
    for(int i = 1; i < argc; i++)
    {
        //Fill in the array starting at 0, reading args from 1
        numbers[i-1] = atof(argv[i]);
    }
    //Ponit the StatData number pointer to the filled in array
	data.numbers = numbers;
    
    //Create min worker thread
    if (pthread_create(&min_t, NULL, &minimum, &data)) 
    {
        perror("error creating the min worker thread");
        exit(1);
    }

    //Create max worker thread
    if (pthread_create(&max_t, NULL, &maximum, &data))
    {
	perror("error creating max thread");
	exit(1);
    }

    //Create mean thread
    if (pthread_create(&mean_t, NULL, &average, &data))
    {
	perror("error creating mean thread");
	exit(1);
    }

    //wait for threads to finish
    pthread_join(min_t, 0);
    pthread_join(max_t, 0);
    pthread_join(mean_t, 0);

    std::cout<<"The average value is "<<mean<<std::endl;
    std::cout<<"The minimum value is "<<min<<std::endl;
    std::cout<<"The maximum value is "<<max<<std::endl;

    //Free up the dynamically allocated memory created to hold the numbers
    delete numbers;
}
