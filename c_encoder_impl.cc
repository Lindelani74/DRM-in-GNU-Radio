/* -*- c++ -*- */
/*
 * Copyright 2020 Lindelani Mahada.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "c_encoder_impl.h"

namespace gr {
  namespace DRM {

    c_encoder::sptr
    c_encoder::make(std::string channel)
    {
      return gnuradio::get_initial_sptr
        (new c_encoder_impl(channel));
    }


	//////////////////Added//////////////////////
	//The limit should change with the channel
	int limit;
	int shift_no;
	//Shift register set up
	float shift_reg[6];
	int item_number;
	/////////////////////////////////////////////

    /*
     * The private constructor
     */
    c_encoder_impl::c_encoder_impl(std::string channel)
      : gr::sync_interpolator("c_encoder",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)), 6)
    {
    	if(channel == "MSC"){
    		limit = 3730;
    	}
    	if(channel == "FAC"){
    		limit = 116;
    	}
    	if(channel == "SDC"){
  		limit = 68;  		
    	}
    	shift_no = 1;
    	for(int y = 0; y<6; y++){
    		shift_reg[y] = 0;
    	}
    	item_number = 0;
    }

    /*
     * Our virtual destructor.
     */
    c_encoder_impl::~c_encoder_impl()
    {
    }

    int
    c_encoder_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      // Do <+signal processing+>
      int bit_no;
      //Start at the first incoming bit
      int count = 0;
      //For debuging
      /*GR_LOG_INFO(this->d_logger, "nout_items: "+ std::to_string(noutput_items));
      GR_LOG_INFO(this->d_logger, "Assume "+ std::to_string(noutput_items/6) + " item");*/
      int produced = 0;
      for(int i = 0; i<noutput_items; i++){
      	bit_no = i % 6;
      	//Make provision for if it;s the extra bits
	int first_bit;
	if(shift_no <= limit){
		first_bit = in[count];
	}
	else{
		/*GR_LOG_INFO(this->d_logger, "Item number = : "+ std::to_string(item_number) + " - " + std::to_string(nitems_written(0)) +  " EMIT TAG");
		GR_LOG_INFO(this->d_logger, "Item number = : " + std::to_string(nitems_written(0) + i));*/
		first_bit = 0;
		shift_no = 1;
	}
	if(((nitems_written(0) + i) % 3727) == 0){
		//Add tag
		/*GR_LOG_INFO(this->d_logger, "Item number = : "+ std::to_string(item_number) + " - " + std::to_string(nitems_written(0)) +  " EMIT TAG");*/
		//Do processing of output (codeword)
		//b_{0}
		gr::tag_t tag;
		//tag.offset = item_number + i;
		tag.offset = nitems_written(0) + i;
		tag.key = pmt::string_to_symbol("Detected");
		tag.value = pmt::PMT_T;
		tag.srcid = alias_pmt();
		//Add the tag to the stream output [0]
		add_item_tag(0,tag);
	}
	if(bit_no == 0){
		if((first_bit + static_cast<int>(shift_reg[1]) + static_cast<int>(shift_reg[2]) + static_cast<int>(shift_reg[4]) + static_cast<int>(shift_reg[5])) % 2 == 0){	
			out[i] = 0;
			produced += 1;
		}
		else{
			out[i] = 1;
			produced += 1;
		}
	}
	//b_{1}
	if(bit_no == 1){
		if((first_bit + static_cast<int>(shift_reg[0]) + static_cast<int>(shift_reg[1]) + static_cast<int>(shift_reg[2]) + static_cast<int>(shift_reg[5])) % 2 == 0){	
			out[i] = 0;
			produced += 1;
		}
		else{
			out[i] = 1;
			produced += 1;
		}
	}
	//b_{2}
	if(bit_no == 2){
		if((first_bit + static_cast<int>(shift_reg[0]) + static_cast<int>(shift_reg[3]) + static_cast<int>(shift_reg[5])) % 2 == 0){	
			out[i] = 0;
			produced += 1;
		}
		else{
			out[i] = 1;
			produced += 1;
		}
	}
	//b_{3}
	if(bit_no == 3){
		if((first_bit + static_cast<int>(shift_reg[1]) + static_cast<int>(shift_reg[2]) + static_cast<int>(shift_reg[4]) + static_cast<int>(shift_reg[5])) % 2 == 0){	
			out[i] = 0;
			produced += 1;
		}
		else{
			out[i] = 1;
			produced += 1;
		}
	}
	//b_{4}
	if(bit_no == 4){
		if((first_bit + static_cast<int>(shift_reg[0]) + static_cast<int>(shift_reg[1]) + static_cast<int>(shift_reg[2]) + static_cast<int>(shift_reg[5])) % 2 == 0){	
			out[i] = 0;
			produced += 1;
		}
		else{
			out[i] = 1;
			produced += 1;
		}
	}
	//b_{5}
	if(bit_no == 5){
		if((first_bit + static_cast<int>(shift_reg[0]) + static_cast<int>(shift_reg[3]) + static_cast<int>(shift_reg[5])) % 2 == 0){	
			out[i] = 0;
			produced += 1;
		}
		else{
			out[i] = 1;
			produced += 1;
		}
		//Go to the next bit in the input stream
		count += 1;
		item_number += 1;
		//Signal shifting the register up and shift the register up
		shift_no += 1;	
		for(int q=5; q>0; q--){
			shift_reg[q] = shift_reg[q-1];
		}
		//Place the latest bit in the zeroeth position
		shift_reg[0] = first_bit;
    	}
    }
    // Tell runtime system how many output items we produced.
      	//return noutput_items;
      	return produced;
    //After this, we have noutput_items outputted
	}
  } /* namespace DRM */
} /* namespace gr */
