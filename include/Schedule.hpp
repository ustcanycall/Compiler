#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <vector>
#include "Conv_layer.hpp"
#include <iostream>
#include <fstream>
using namespace std;

class Schedule {
public:
 Schedule();
 Schedule(int cores, int pes, int nfu_per_pe, int bandwidth, int latency, int samples, int cores_per_sample) :
 cores_(cores), pes_(pes), nfu_per_pe_(nfu_per_pe), bandwidth_(bandwidth), latency_(latency), samples_(samples), cores_per_sample_(cores_per_sample), time_(0), load_n_(0), load_s_(0), cal_(0) {
 	nfus_ = pes * nfu_per_pe;
 }


 int Load_Instruction(int layer, int mode1, int mode2);

 int Store_Instruction(int layer);

 int Compute_Instruction(int layer, int mode); 

 void step(int layer);
 
 int cores_;				// Number of cores on the chip
 int nfus_;
 int pes_;					// Number of PEs in one core
 int nfu_per_pe_;			// Number of NUF in each PE

 int bandwidth_;			// Bandwidth of DDR
 int latency_;				// Latency of DDR
 
 int samples_;				// Number of samples
 int cores_per_sample_;		// Number of cores to execute each sample
 
 int time_;					// Execution time
 int load_n_;
 int load_s_;
 int cal_;
 
 vector<Conv_layer> net_;	// 

};

#endif
