#include <unistd.h>
#include <stdlib.h>
#include <map>
#include <cstring>
#include <cstdio>
#include "rating.h"

#define STRSIZE 40

//Get rating from filename, map to dict
void getRating(const char *filename, std::map<int, std::pair<float, int> > &dict, bool parent)
{
    FILE *fptr = fopen(filename, "r");
    char str[STRSIZE];

    while (fgets(str, STRSIZE, fptr))
    {
        float arr[4];
        int i = parent ? 1 : 0;
        str[strlen(str) - 1] = '\0';

        char *token = strtok(str, " \t");

        while (token != NULL)
        {
            arr[i] = atof(token);
            token = strtok(NULL, " \t");
            i++;
        }

        if (dict.find(arr[1]) == dict.end())
            dict[arr[1]] = std::make_pair(0, 0);

        dict[arr[1]].first += arr[2];
        dict[arr[1]].second++;
    }
    fclose(fptr);
}

//Calculate average rating, write to file
void calcRating(const char *filename, std::map<int, std::pair<float, int> > &dict)
{
    FILE *rf = fopen(filename, "w+");

    std::map<int, std::pair<float, int> >::iterator it;

    for (it = dict.begin(); it != dict.end(); it++)
    {
        float rating = it->second.first / it->second.second;

        fprintf(rf, "%d\t%f\n", it->first, rating);
    }
    fclose(rf);
}
