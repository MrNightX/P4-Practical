// P4 Practical.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#include <stdio.h>
#include <iostream>
#include "omp.h"
#define NUM_THREADS 16
static long num_steps = 100000000;
double step;

using namespace std;

double TimeTaken = 0.0;

double ComputePiFalseSharing()
{
    int i, nthreads;
    double pi = 0;
    double partial_sums[NUM_THREADS], sum = 0.0;
    step = 1.0 / (double)num_steps;
    omp_set_num_threads(NUM_THREADS);
    double start_time = omp_get_wtime();
#pragma omp parallel
    {
        int i, id, nthrds;
        double x;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) {
            nthreads = nthrds;
            printf("Total OMP threads: %d\n", nthreads);
        }

        for (i = id, partial_sums[id] = 0.0; i < num_steps; i = i + nthrds)
        {
            x = (i + 0.5) * step;
            partial_sums[id] += 4.0 / (1.0 + x * x);
        }
    }
    for (i = 0, pi = 0.0; i < nthreads; i++) {
        sum += partial_sums[i];
    }
    pi = sum * step;
    double end_time = omp_get_wtime();
    TimeTaken = end_time - start_time;
    //printf("%f\n", pi);
    cout << pi << endl;

    //printf("Work took %f seconds\n", end_time - start_time);
    cout << "Work took " << TimeTaken << " seconds\n";
    return TimeTaken;
}

double TestVer()
{
    int i, nthreads;
    double pi = 0;
    double partial_sums[NUM_THREADS], sum = 0.0;
    step = 1.0 / (double)num_steps;
    omp_set_num_threads(NUM_THREADS);
    double start_time = omp_get_wtime();
#pragma omp parallel
    {
        int i, id, nthrds;
        double x;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();

        double PS_V2 = 0.0; //Step 1

        if (id == 0) {
            nthreads = nthrds;
            printf("Total OMP threads: %d\n", nthreads);
        }

        for (i = id; i < num_steps; i = i + nthrds)
        {
            x = (i + 0.5) * step;
            PS_V2 += 4.0 / (1.0 + x * x); //Step 2
        }

#pragma omp critical(sum)//step 3 
        {
            sum += PS_V2;
        }
    }

    /*for (i = 0, pi = 0.0; i < nthreads; i++) {
        sum += partial_sums[i];
    }*/

    pi = sum * step;
    double end_time = omp_get_wtime();
    TimeTaken = end_time - start_time;
    //printf("%f\n", pi);
    cout << pi << endl;

    //printf("Work took %f seconds\n", end_time - start_time);
    cout << "Work took " << TimeTaken << " seconds\n";
    return TimeTaken;
}

int main()
{
    cout << "Ver 1" << endl;
    double Ver1Time = ComputePiFalseSharing();

    cout << endl << "Test Ver" << endl;
    double TestVerTime = TestVer();

    double improvement = Ver1Time / TestVerTime;
    cout << endl << "Improved performance by (ORI/MOD) : " << improvement;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
