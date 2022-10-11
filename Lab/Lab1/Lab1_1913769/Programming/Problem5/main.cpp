#include <unistd.h>
#include <stdlib.h>
#include <map>
#include "rating.h"


//? map<> dict structure
/*
    {
        key: Int = MovieID
        value: Pair <float, int>  = {
            first: Float = sum of Ratings,
            second: Int = count of Ratings 
        }
    }

*/

int main()
{

    pid_t child1 = fork();
    pid_t child2 = -2;

    if (child1 == -1)
    {
        perror("fork 1");
    }

    if (child1 != 0)
    {
        child2 = fork();
        if (child2 == -1)
        {
            perror("fork 2");
        }
    }

    std::map<int, std::pair<float, int> > dict;
    if (child1 == 0)
    {
        // Read from file 1 and calc
        getRating("movie-100k_1.txt", dict);

        // Calculate result and write to resultFile1
        calcRating("resultFile1.txt", dict);
    }
    else if (child2 == 0)
    {
        // Read from file 2 and calc
        getRating("movie-100k_2.txt", dict);

        //Calculate result and write to resultFile2
        calcRating("resultFile2.txt", dict);
    }
    else if (child1 != 0 && child2 != 0)
    {
        while(wait(NULL) > 0);
        getRating("resultFile1.txt", dict, true);
        getRating("resultFile2.txt", dict, true);
        calcRating("finaloutput.txt", dict);
    }

    return 0;
}
