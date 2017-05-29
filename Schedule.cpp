#include <vector>
#include "include/Conv_layer.hpp"
#include "include/Schedule.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int Schedule::Load_Instruction(int layer, int mode1, int mode2) {

	int load_bytes = 0;
	int load_times = 0; 
	int load_cycles = 0;
	
	// load input neurons
	load_bytes += net_.at(layer).Load_in_neuron();
	load_times += 1;
	
	// load output neurons
	load_bytes += net_.at(layer).Load_out_neuron(mode1)/cores_per_sample_;
	load_times += (mode1 != 0);
	
	// load synapses
	load_bytes += net_.at(layer).Load_synapse(mode2)/cores_per_sample_;
	load_times += (mode2 == 0);
	
	load_cycles = load_bytes/bandwidth_ + load_times*latency_;
	
	return load_cycles;

}

int Schedule::Store_Instruction(int layer) {
 	int store_bytes = 0;
	int store_times = 0;
	int store_cycles = 0;

	store_bytes = net_.at(layer).Store_neuron()/cores_per_sample_;
	store_times = 1;
	store_cycles = store_bytes/bandwidth_ + store_times * latency_; 

	return store_cycles;

}

int Schedule::Compute_Instruction(int layer, int mode) {
 	int conv_ops = 0;
	int add_ops= 0;
	int cycles = 0;
 	// convolution 
	conv_ops = net_.at(layer).Conv()/cores_per_sample_;

	// add
	add_ops = net_.at(layer).Add(mode)/cores_per_sample_;

	
	cycles = conv_ops/nfus_ + add_ops/pes_;

	return cycles;
}


void Schedule::step(int layer) {
 	int i,j,k;

	vector<int> load_inst;
	vector<int> store_inst;
	vector<int> comp_inst;

	// Instructions per core
	for (k = 0; k < (samples_*cores_per_sample_/cores_ + 1); k++) {
		for (i = 0; i < net_.at(layer).ky_; i++) {
			for (j = 0; j < net_.at(layer).oy_; j++) {
				load_inst.push_back(Load_Instruction(layer,i,j));
				store_inst.push_back(Store_Instruction(layer));
				comp_inst.push_back(Compute_Instruction(layer,i));
			}
		}
	}


	int iter1 = (samples_*cores_per_sample_/cores_ + 1)*net_.at(layer).ky_*net_.at(layer).oy_;
	int iter2 = samples_*cores_per_sample_/cores_ *net_.at(layer).ky_*net_.at(layer).oy_;
	

	string file_name;
	string index;
	stringstream ss;
	ss << layer;
	ss >> index;
	file_name = "layer" + index;

	ofstream fout;

	fout.open(file_name.c_str());

	int res = samples_*cores_per_sample_%cores_;
	

	for (i = 0; i < res; i++) {
		for (j = 0; j < iter1; j++) {
			fout << "Load," << load_inst.at(j) << "," << i << "\n";
			if (j > 1) {
				fout << "Store," << store_inst.at(j-2) << "," << i << "\n";
			}
			fout << "Sync," << i << "\n";
			fout << "Comp," << comp_inst.at(j) << "," << i << "\n";
		}

		fout << "Sotre," << store_inst.at(iter1-2) << "," << i << "\n";
		fout << "Sync," << i << "\n";
		fout << "Store," << store_inst.at(iter1-1) << "," << i << "\n";

	}

	for (i = res; i < cores_; i++) {
		for (j = 0; j < iter2; j++) {
			fout << "Load," << load_inst.at(j) << "," << i << "\n";
			if (j > 1) {
				fout << "Store," << store_inst.at(j-2) << "," << i << "\n";
			}
			fout << "Sync," << i << "\n";
			fout << "Comp," << comp_inst.at(j) << "," << i << "\n";
		}
		fout << "Sotre," << store_inst.at(iter2-2) << "," << i << "\n";
		fout << "Sync," << i << "\n";
		fout << "Store," << store_inst.at(iter2-1) << "," << i << "\n";

	}

	fout << flush;
	fout.close();


}

