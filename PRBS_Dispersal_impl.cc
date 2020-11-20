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
#include "PRBS_Dispersal_impl.h"

//Added
#include <bitset>

namespace gr {
  namespace DRM {
  
  //Added here(Gotta have the shift register with initialization word: 0b111111111)
    int shift_register[9];
    int reset;
    int block_size;
    int first_run;

    PRBS_Dispersal::sptr
    PRBS_Dispersal::make(std::string channel)
    {
      return gnuradio::get_initial_sptr
        (new PRBS_Dispersal_impl(channel));
    }


    /*
     * The private constructor
     */
    PRBS_Dispersal_impl::PRBS_Dispersal_impl(std::string channel = "MSC")
      : gr::block("PRBS_Dispersal",
             gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
    	//Added
    	for(int i = 0; i<9; i++){
    		shift_register[i] = 1;
    	}
    	reset = 0;
    	if(channel == "MSC"){
    		block_size = 3727;
    	}
    	else if(channel == "FAC"){
    		block_size = 116;
    	}
    	else{
    		//SDC
    		block_size = 68;
    	}
    	first_run = 1;
    }

    /*
     * Our virtual destructor.
     */
    PRBS_Dispersal_impl::~PRBS_Dispersal_impl()
    {
    }

    void
    PRBS_Dispersal_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      
    }

    int
    PRBS_Dispersal_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	float *out = (float *) output_items[0];
	int produced = 0;
      // Do <+signal processing+>
      if(first_run == 1){
      	//Print the shift register
      	for(int h = 0; h<9; h++){
      		GR_LOG_INFO(this->d_logger, shift_register[h]);
      	}
      	GR_LOG_INFO(this->d_logger, "Shift Register");
      	first_run = 0;
      }
     	 for (int i = 0; i<noutput_items; i++){
	      	reset += 1;
	      	if((shift_register[4] == 1 && shift_register[8] == 0) || (shift_register[4] == 0 && shift_register[8] == 1)){
		      	out[i] = 1.00;
		      	produced += 1;
		}
	      	else{
	      		out[i] = 0.00;
	      		produced += 1;
	      	}
	      	//Shift right and insert out[i] if not a reset(end of block)
	      	if(reset < block_size){
		      	for (int j = 8; j>0; j--){
		      		shift_register[j] = shift_register[j-1];
		      	}
			shift_register[0] = out[i];
		}
		//Reset the shift register
		else{
			reset = 0;
			for(int p = 0; p<9; p++){
	    			shift_register[p] = 1;
	    		}
		}
      	}
      // Tell runtime system how many input items we consumed on
      // each input stream.
      //consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
      //return produced;
    }

  } /* namespace DRM */
} /* namespace gr */

