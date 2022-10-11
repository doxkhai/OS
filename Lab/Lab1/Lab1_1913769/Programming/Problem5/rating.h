#include <stdlib.h>
#include <map>

void getRating(const char *, std::map<int, std::pair<float, int> > &, bool = false);
void calcRating(const char *, std::map<int, std::pair<float, int> > &);