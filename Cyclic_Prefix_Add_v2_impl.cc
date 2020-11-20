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
#include "Cyclic_Prefix_Add_v2_impl.h"

namespace gr {
  namespace DRM {

    Cyclic_Prefix_Add_v2::sptr
    Cyclic_Prefix_Add_v2::make(int length)
    {
      return gnuradio::get_initial_sptr
        (new Cyclic_Prefix_Add_v2_impl(length));
    }
    
    
    	//Added
	std::vector<gr_complex> pre_cyclic2;
	gr_complex copy2[28];
	int fillup2;
	int setup2;
	gr_complex symbol2[284];
	int pos2;
	int len2;
	int ys;
	int num_in2;
	int symb_num;


    /*
     * The private constructor
     */
    Cyclic_Prefix_Add_v2_impl::Cyclic_Prefix_Add_v2_impl(int length)
      : gr::sync_block("Cyclic_Prefix_Add_v2",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
    	fillup2 = 1;
    	setup2 = 0;
    	pos2 = 0;
    	len2 = length;
    	ys = 1;
    	//'num_in' is in Cell_interleaver block
    	num_in2 = 0;
    	symb_num = 1;
    }

    /*
     * Our virtual destructor.
     */
    Cyclic_Prefix_Add_v2_impl::~Cyclic_Prefix_Add_v2_impl()
    {
    }

    int
    Cyclic_Prefix_Add_v2_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
	
	int used_in = 0;
	int used_out = 0;
      // Do <+signal processing+>
      for(int i = 0; i<noutput_items; i++){
      	if(num_in2 < 256){
      		pre_cyclic2.push_back(*in++);
      		num_in2 += 1;
      		//GR_LOG_INFO(this->d_logger,num_in2);
      	}
      	if(num_in2 == 256){
      		//GR_LOG_INFO(this->d_logger,num_in2);
      		//Set up 284-item array
      		int addition = 256-len2;
      		/*GR_LOG_INFO(this->d_logger,"setup2 = 1(2)");
      		//addition = 228(256-28)
      	      GR_LOG_INFO(this->d_logger,"--------------------------------------------------------------------");
      	      for(int h = 0; h<256; h++){
      	      	GR_LOG_INFO(this->d_logger,pre_cyclic2.at(h));
      	      }
      	      GR_LOG_INFO(this->d_logger,"--------------------------------------------------------------------");*/
	      //Copy the cyclic prefix and make the (256+28) length array
	      for(int p = 0; p<len2; p++){
	      	//copy2[0]-[27] = pre_cyclic2[228]-[255]
	      	copy2[p] = pre_cyclic2.at(p+addition);
	      }
	      //Construct the full symbol
	      for(int o = 0; o<(256+len2); o++){
      		if(o < len2){
      			//GR_LOG_INFO(this->d_logger,"First if. o = " + std::to_string(o));
      			symbol2[o] = copy2[o];
      		}
      		else{
      			//GR_LOG_INFO(this->d_logger,"Second if. o = " + std::to_string(o));
      			symbol2[o] = pre_cyclic2.at(o-len2);
      		}
      	      }
      	      //Print symbol[]
      	      /*GR_LOG_INFO(this->d_logger,"--------------------------------------------------------------------");
      	      GR_LOG_INFO(this->d_logger,symb_num);
      	      for(int h = 0; h<284; h++){
      	      	GR_LOG_INFO(this->d_logger,symbol2[h]);
      	      }
      	      GR_LOG_INFO(this->d_logger,"--------------------------------------------------------------------");
      	      symb_num += 1;*/
      	     
      	     //Output
      	     //check
      	     for(int j = 0; j<256 + len2; j++){
      	     	*out++ = symbol2[j];
      	     	//GR_LOG_INFO(this->d_logger,symbol2[j]);
      	     	used_out += 1;
      	     }
      	     num_in2 = 0;
      	     pre_cyclic2.clear();
      	}
      }
      
      //---------------------------------------------------------------------------------------------------------------------
      /*for(int n = 0; n<noutput_items; n++){
	      if(fillup2 == 1){
	      	//Consume 256 items on the input
	      	//For the input items we have available
	      	for(int i = 0; i<256; i++){
	      		pre_cyclic2.push_back(*in);
	      		*in++;
	      		used_in += 1;
	      	}
	      	fillup2 = 0;
		setup2 = 1;
		gr_complex inc = *in;
      		//GR_LOG_INFO(this->d_logger,"Input: " + std::to_string(inc.real()));
	      }
	      if(setup2 == 1){
	      		int addition = 256-len2;
		      //Copy the cyclic prefix and make the (256+28) length array
		      for(int p = 0; p<len2; p++){
		      	//copy2[0]-[27] = pre_cyclic2[228]-[255]
		      	copy2[p] = pre_cyclic2.at(p+addition);
		      }
		      //Construct the full symbol
		      for(int o = 0; o<(256+len2); o++){
	      		if(o < len2){
	      			//GR_LOG_INFO(this->d_logger,"First if. o = " + std::to_string(o));
	      			symbol2[o] = copy2[o];
	      		}
	      		else{
	      			//GR_LOG_INFO(this->d_logger,"Second if. o = " + std::to_string(o));
	      			symbol2[o] = pre_cyclic2.at(o-len2);
	      		}
	      	      }
	      	      setup2 = 0;
	      }
	      if(setup2 == 0 && fillup2 == 0){
	      	*out = symbol2[pos2];
	      	*out++;
	      	//GR_LOG_INFO(this->d_logger,symbol2[pos2]);
	      	used_out += 1;
	      	if(pos2 == (255+len2)){
	      		//GR_LOG_INFO(this->d_logger,"Input marker left at: " + std::to_string(in->real()));
	      		pos2 = 0;
	      		fillup2 = 1;
	      		pre_cyclic2.clear();
	      	}
	      	else{
	      		pos2 += 1;
	      	}
	      }
      }*/
      
      // Tell runtime system how many output items we produced.
      //return noutput_items;
      return used_out;
    }

  } /* namespace DRM */
} /* namespace gr */

