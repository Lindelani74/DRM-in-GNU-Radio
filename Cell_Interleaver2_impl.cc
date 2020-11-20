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
#include "Cell_Interleaver2_impl.h"

namespace gr {
  namespace DRM {

    Cell_Interleaver2::sptr
    Cell_Interleaver2::make()
    {
      return gnuradio::get_initial_sptr
        (new Cell_Interleaver2_impl());
    }
	int p[7460]; //!< The permutation array
	gr_complex inc[7460]; //!< Array to store all the inputs of a frame
	//For debugging, just writing how many inputs have been input so far
	int count; //!< An integer value for debugging
	int number_in; //!< An integer value for debugging
	int array_no; //!< An integer value for debugging
	int indie; //!< An integer value for debugging
	int prod; //!< An integer value for debugging

    Cell_Interleaver2_impl::Cell_Interleaver2_impl()
      : gr::block("Cell_Interleaver2",
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
    	p[0] = 0;
    	for(int i = 1; i < 7460; i++){
    		p[i] = (((5*p[i-1]) + 7) % 32);
    		while(p[i] >= 7460){
    			p[i] = (((5*p[i]) + 7) % 32);
    		}
    	}
    	count = 0;
    	number_in = 0;
    	array_no = 0;
    	indie = 0;
    	prod = 0;
    	set_min_noutput_items(1);
    }

    Cell_Interleaver2_impl::~Cell_Interleaver2_impl()
    {
    }

    void
    Cell_Interleaver2_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = noutput_items;
    }

    int
    Cell_Interleaver2_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
	int consumed = 0; 
      if(number_in < 7460){
      	//noutput_items is the length in items of all input and output buffers
      	inc[number_in] = *in;
      	number_in += 1;
      	count += 1;
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
      		*out++ = inc[p[indie]];
      		prod++;
      		if(indie == 7459){
      			indie = 0;
      			number_in = 0;
      			array_no += 1;
      		}
      		else{
			indie += 1;
		}
      	}
      }
      // Tell runtime system how many input items we consumed on each input stream.
	consume_each (consumed);
      if(number_in == 0){
      	return noutput_items;
      }
      else{
      	return 0;
      }
    }

  } /* namespace DRM */
} /* namespace gr */

