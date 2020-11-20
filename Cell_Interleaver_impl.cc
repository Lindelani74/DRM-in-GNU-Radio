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
#include "Cell_Interleaver_impl.h"

namespace gr {
  namespace DRM {

    Cell_Interleaver::sptr
    Cell_Interleaver::make()
    {
      return gnuradio::get_initial_sptr
        (new Cell_Interleaver_impl());
    }

	//Added: The permutation vector
	int p_index[7460];
	//Added vector to store all inputs of frame to date
	gr_complex incoming[7460];
	//For debugging, just writing how many inputs have been input so far
	int counter;
	int num_in;
	int arr_no;
	int ind;
	int produced;

    /*
     * The private constructor
     */
    Cell_Interleaver_impl::Cell_Interleaver_impl()
      : gr::sync_block("Cell_Interleaver",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
    	//set_min_output_buffer(33554432);
    	//s = 2^5
    	int s_value = 32;
    	//q = s/4 - 1
    	int q_value = 7;
    	//Index 0 = 0
    	//Set up the permutation array
    	p_index[0] = 0;
    	for(int i = 1; i < 7460; i++){
    		p_index[i] = (((5*p_index[i-1]) + 7) % 32);
    		while(p_index[i] >= 7460){
    			p_index[i] = (((5*p_index[i]) + 7) % 32);
    		}
    	}
    	counter = 0;
    	num_in = 0;
    	arr_no = 0;
    	ind = 0;
    	produced = 0;
    	set_min_noutput_items(1);
    }

    /*
     * Our virtual destructor.
     */
    Cell_Interleaver_impl::~Cell_Interleaver_impl()
    {
    }

    int
    Cell_Interleaver_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
	int consumed = 0;
      if(num_in < 7460){
      	//noutput_items is the length in items of all input and output buffers
      	incoming[num_in] = *in;
      	num_in += 1;
      	counter += 1;
      	consumed = 1;
      }
      else{
      	for(int i = 0; i<noutput_items; i++){
      		//Add tag if start of multiplex frame
      		if(((nitems_written(0) + i) % 7460) == 0){
	      		GR_LOG_INFO(this->d_logger,"nitems_written(0) + i = " + std::to_string(nitems_written(0) + i));
			//Add tag
			//Do processing of output (codeword)
			//b_{0}
			gr::tag_t tag;
			//tag.offset = item_number + i;
			tag.offset = nitems_written(0) + i;
			if((nitems_written(0) + i) == 0){
				tag.key = pmt::string_to_symbol("packet_len");
			}
			else{
				tag.key = pmt::string_to_symbol("other");
			}
			//tag.value = pmt::PMT_T;
			tag.value = pmt::from_long(7460);
			tag.srcid = alias_pmt();
			//Add the tag to the stream output [0]
			add_item_tag(0,tag);
		}
      		*out++ = incoming[p_index[ind]];
      		produced++;
      		if(ind == 7459){
      			ind = 0;
      			num_in = 0;
      			arr_no += 1;
      		}
      		else{
			ind += 1;
		}
      	}
      }
      //Added this
      // Tell runtime system how many input items we consumed on each input stream.
	consume_each (consumed);
      if(num_in == 0){
      	return noutput_items;
      }
      else{
      	return 0;
      }
      // Tell runtime system how many output items we produced.
      //return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

