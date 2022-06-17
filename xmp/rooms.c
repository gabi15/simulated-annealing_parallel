#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <xmp.h>
#define N 20
#define PACE 0.999
#define MASTER_PROCESS_ID 0



void red () {
  fprintf(stderr, "\033[1;31m");
}
void yellow() {
  fprintf(stderr, "\033[1;33m");
}
void reset () {
  fprintf(stderr, "\033[0m");
}


int get_random_number(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

double get_rand_normalized(void)
{
    return (double)rand()/(double)RAND_MAX;
}

float get_dislike_sum(float d[N][N], int a[N]){
    float dislike_sum = 0.0;
    for (int i =0; i<N; i++){
        for (int j=0; j<N; j++){
            if (i > j) continue;
            if (a[i]==a[j])
			{
				dislike_sum += d[i][j];
			}
		}
    }
    return dislike_sum;
}

void fill_matrix_randomly(float d[N][N], int length, int width)
{
    for(int i = 0; i < length; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(i < j)
            {
                d[i][j] = (float)get_random_number(1, 10);
            }
            if(i==j)
            {
                d[i][i] = 0.;
                continue;
            }
        }
    }
}

void print_1d_array_stderr(float *d, int length)
{
    for(int i = 0; i < length; ++i)
    {
        fprintf(stderr, "%.2f ", d[i]);
    }
}

void print_1d_array_int(int d[N], int length)
{
    for(int i = 0; i < length; ++i)
    {
        fprintf(stdout, "%d ", d[i]);
    }
}

void print_1d_array_int_stderr(int d[N], int length)
{
    for(int i = 0; i < length; ++i)
    {
        fprintf(stderr, "%d ", d[i]);
    }
}

void print_2d_array_stderr(float d[N][N], int length, int width)
{
    for(int i = 0; i < length; ++i)
    {
        print_1d_array_stderr(d[i], N);
        fprintf(stderr, "\n");
    }
}

void print_2d_array_stderr_color(float d[N][N], int a[N], int length, int width)
{
    for(int i = 0; i < length; ++i)
    {
        for(int j = 0; j < length; ++j)
        {
          if(a[i] == a[j] && i < j)
          {
            red();
            fprintf(stderr, "%.2f ", d[i][j]);
            reset();
          }
          else 
            fprintf(stderr, "%.2f ", d[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

int find_min(int * a, int size)
{
	int min = a[0];
	for(int j = 0; j<size; j++)
		if(a[j] < min) min = a[j];

	return min;
}

float solve(float d[N][N], int a[N], float T)
{
    float dislike_sum = 0.0;
    float new_dislike_sum = 0.0;
    int c1;
    int c2;
    float u;
    int i = 0;

    dislike_sum = get_dislike_sum(d,a);
    while (i<1000){
        while (1) {
            c1 = get_random_number(0,N-1);
            c2 = get_random_number(0,N-1);
            if (a[c1] != a[c2]) break;
        }
        //new sum for swapped rooms
        int a_check[N];
        for (int k = 0; k<N; k++){
            a_check[k] = a[k];
        }
        a_check[c1] = a[c2];
        a_check[c2] = a[c1];
        new_dislike_sum = get_dislike_sum(d,a_check);

        if (new_dislike_sum<dislike_sum || get_rand_normalized()<=exp((dislike_sum-new_dislike_sum)/T)){
            int temp = a[c1];
            a[c1] = a[c2];
            a[c2] = temp;
            dislike_sum = new_dislike_sum;
            i=0;
        }
        else{
            i +=1;
        }
        T *= PACE;
    }

    return dislike_sum;
}

void read_d_array(float D[N][N], int width, char *filename)
{
    FILE * fp;
    fp = fopen (filename, "r");

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
            fscanf(fp, "%f", &D[i][j]);
    }
    fclose(fp);
}

void read_a_array(int a[N], int width, char *filename)
{
    FILE * fp;
    fp = fopen (filename, "r");

    for (int i = 0; i < width; i++)
    {
        fscanf(fp, "%d", &a[i]);
    }
   fclose(fp);
}


#pragma xmp nodes p[*]
int main(int argc, char *argv[])
{
    // MPI Init config
	int numprocs, myid;
    struct
    {
        float  value;
        int	   process_id;
    } current_solution, best_solution;

	// MPI_Init(&argc, &argv);
	// MPI_Comm_rank(MPI_COMM_WORLD, &current_solution.process_id);
	// MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    // end of MPI initalization
    srandom (current_solution.process_id);
    current_solution.process_id = xmpc_node_num();
    
    #pragma xmp task on p[0]
    {
    fprintf(stderr, "Node number: %d\n", current_solution.process_id);
    }

    float T = 1.0;
    //float d[N][N] = {0}; //dislike table
    float d[N][N]; //dislike table
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            d[i][j] = 0;
        }
    }
    int a[N] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9}; //room assignments
    float *solutions;

    #pragma xmp task on p[0]
    {
        if (argc > 1 && strlen(argv[1]) != 0)
        {
            read_d_array(d, N, argv[1]);
            if (argc > 2 && strlen(argv[2]) != 0)
                read_a_array(a, N, argv[2]);
            else
                fprintf(stderr, "room assignment array not provided, using default\n");
        }
        else
        {
            fprintf(stderr, "dislike array not provided. using random one\n");
            fill_matrix_randomly(d, N, N);
        }
        fprintf(stderr, "\nroom assignment array: \n");
        print_1d_array_int_stderr(a, N);
        fprintf(stderr, "\nCalculating best room division...\n");
        fflush(stderr);
    }

    #pragma xmp bcast(d)
    //fprintf(stderr, "Node number: %d\n", current_solution.process_id);

    float solution = solve(d, a, T);
    current_solution.value = solution;
    // MPI_Reduce(
    //     &current_solution,
    //     &best_solution,
    //     1,
    //     MPI_FLOAT_INT,
    //     MPI_MINLOC,
    //     MASTER_PROCESS_ID,
    //     MPI_COMM_WORLD
    // );

    // MPI_Bcast(&best_solution, 1, MPI_FLOAT_INT, MASTER_PROCESS_ID, MPI_COMM_WORLD);
    best_solution.value = 5.55;
    best_solution.process_id = 2;
    #pragma xmp bcast(best_solution)

    #pragma xmp task on p[2] //cant compile when passinng variable here
    {
        // fprintf(stderr, "\nbest found division for process: %d\n", best_solution.process_id);
        // fprintf(stderr, "least dislike : %.2f\n", best_solution.value);
        // fprintf(stderr, "\ndislike array: \n");
        // print_2d_array_stderr_color(d, a, N, N);
        // fprintf(stderr, "solution:\n");
        // print_1d_array_int_stderr(a, N);
        // fflush(NULL);
        // it should be uncommented but stderr is not compatible with stdout.
        // print_1d_array_int(a, N);
    }

    return 0;
}