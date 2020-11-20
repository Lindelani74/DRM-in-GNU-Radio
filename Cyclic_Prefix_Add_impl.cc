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
#include "Cyclic_Prefix_Add_impl.h"

namespace gr {
  namespace DRM {

    Cyclic_Prefix_Add::sptr
    Cyclic_Prefix_Add::make(int length)
    {
      return gnuradio::get_initial_sptr
        (new Cyclic_Prefix_Add_impl(length));
    }


	//Added
	std::vector<gr_complex> pre_cyclic;
	gr_complex copy[28];
	int fillup;
	int setup;
	gr_complex symbol[284];
	int pos;
	int len;

    /*
     * The private constructor
     */
    Cyclic_Prefix_Add_impl::Cyclic_Prefix_Add_impl(int length)
      : gr::block("Cyclic_Prefix_Add",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
    	fillup = 1;
    	setup = 0;
    	pos = 0;
    	len = length;
    }

    /*
     * Our virtual destructor.
     */
    Cyclic_Prefix_Add_impl::~Cyclic_Prefix_Add_impl()
    {
    }

    void
    Cyclic_Prefix_Add_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      //ninput_items_required[0] = noutput_items;
    }

    int
    Cyclic_Prefix_Add_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

	int used_in = 0;
	int used_out = 0;
      // Do <+signal processing+>
      for(int n = 0; n<noutput_items; n++){
	      if(fillup == 1){
	      	//Consume 256 items on the input
	      	//For the input items we have available
	      	//for(int i = 0; i<noutput_items; i++){
	      		pre_cyclic.push_back(*in++);
	      		used_in += 1;
	      		if(pre_cyclic.size() == 256){
	      			fillup = 0;
	      			setup = 1;
	      			break;
	      		}
	      	//}
	      }
	      if(setup == 1){
	      		int addition = 256-len;
	      		//addition = 228(256-28)
		      //Copy the cyclic prefix and make the (256+28) length array
		      for(int p = 0; p<len; p++){
		      	//copy[0]-[27] = pre_cyclic[228]-[255]
		      	copy[p] = pre_cyclic.at(p+addition);
		      }
		      //Construct the full symbol
		      for(int o = 0; o<(256+len); o++){
	      		if(o < len){
	      			symbol[o] = copy[o];
	      		}
	      		else{
	      			symbol[o] = pre_cyclic.at(o-len);
	      		}
	      	      }
	      	      setup = 0;
	      }
	      if(setup == 0 && fillup == 0){
	      	*out++ = symbol[pos];
	      	used_out += 1;
	      	if(pos == 255+len){
	      		pos = 0;
	      		fillup = 1;
	      		pre_cyclic.clear();
	      	}
	      	else{
	      		pos += 1;
	      	}
	      }
      }
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (used_in);
      //consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      //return noutput_items;
      return used_out;
    }

  } /* namespace DRM */
} /* namespace gr */

