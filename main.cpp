#include <iostream>
#include <fstream>
#include "include/Schedule.hpp"
#include "include/Conv_layer.hpp"

using namespace std;

int main()
{
 ifstream fin("config");

 int cores = 32;
 int pes = 32;
 int nfu_per_pe = 32;
 int bandwidth = 256; 
 int latency = 100;
 int samples = 512;
 int cores_per_sample = 1;

 fin >> cores;
 fin >> pes;
 fin >> nfu_per_pe;
 fin >> bandwidth;
 fin >> latency;
 fin >> samples;
 fin >> cores_per_sample;

 fin.close();

 Schedule schedule(cores, pes, nfu_per_pe, bandwidth, latency, samples, cores_per_sample);  // cores, pes_per_core, bandwidth, samples, cores_per_sample

 Conv_layer *conv_layer = new Conv_layer[5];

 conv_layer[0] = Conv_layer(nfu_per_pe, 227, 227, 11, 11, 96, 55, 55);
 conv_layer[1] = Conv_layer(96, 27, 27, 5, 5, 256, 27, 27);
 conv_layer[2] = Conv_layer(256, 13, 13, 3, 3, 384, 13, 13);
 conv_layer[3] = Conv_layer(384, 13, 13, 3, 3, 384, 13, 13);
 conv_layer[4] = Conv_layer(384, 13, 13, 3, 3, 256, 13, 13);

 schedule.net_.push_back(conv_layer[0]);
 schedule.net_.push_back(conv_layer[1]);
 schedule.net_.push_back(conv_layer[2]);
 schedule.net_.push_back(conv_layer[3]);
 schedule.net_.push_back(conv_layer[4]);

 schedule.step(0);
 schedule.step(1);
 schedule.step(2);
 schedule.step(3);
 schedule.step(4);


 delete[] conv_layer;

}
