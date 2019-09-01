#include "NFA.h"
#include "Regex.h"

#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

int main(int argc, char const *argv[]){
    ofstream filea("newfile");

    {
        boost::archive::text_oarchive oa(filea);
        oa << Regex::compile("a*b*c");
    }

	return 0;
}
