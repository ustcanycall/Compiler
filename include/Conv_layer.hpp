#ifndef CONV_LAYER_HPP_
#define CONV_LAYER_HPP_

class Conv_layer {

public:
 Conv_layer() {
 }

 Conv_layer(int inc, int inx, int iny, int kx, int ky, int oc, int ox, int oy) :
 inc_(inc), inx_(inx), iny_(iny), kx_(kx), ky_(ky), oc_(oc), ox_(ox), oy_(oy) {
 }

// ~Conv_layer();


 inline int Conv() {
 	return oc_*ox_*kx_*inc_;
 }

 inline int Add(int mode) {
 	if (mode == 0) return 0;
	else	return ox_*oc_;
 }

 inline int Load_in_neuron () {
 	return inc_*inx_*2;
 }

 inline int  Load_out_neuron (int mode) {
 	if (mode == 0) return 0;
	else	return ox_*oc_*2;
 }


 inline int Load_synapse(int mode) {
 	if (mode == 0) return kx_*inc_*oc_*2;
	else	return 0;
 }

 inline int Store_neuron() {
 	return ox_*oc_*2;
 }



 int inc_;
 int inx_;
 int iny_;
 int kx_;
 int ky_;
 //int stride_x_;
 //int stride_y_;
 //int pad_x_;
 //int pad_y_;
 int oc_;
 int ox_;
 int oy_;

};

#endif

