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
#include "SDC_Source_impl.h"

namespace gr {
  namespace DRM {
  
  	int SDC_block[924];
  	int in;

    SDC_Source::sptr
    SDC_Source::make(int data_entity_type)
    {
      return gnuradio::get_initial_sptr
        (new SDC_Source_impl(data_entity_type));
    }


    /*
     * The private constructor
     */
    SDC_Source_impl::SDC_Source_impl(int data_entity_type)
      : gr::sync_block("SDC_Source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(int)))
    {
    	for(int i = 0; i<924; i++){
    		SDC_block[i] = 0;
    	}
    	in = 0;
    }

    /*
     * Our virtual destructor.
     */
    SDC_Source_impl::~SDC_Source_impl()
    {
    }

    int
    SDC_Source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      int *out = (int *) output_items[0];

      // Do <+signal processing+>
	for(int i = 0; i<noutput_items; i++){
		if(in == 924){
			in = 0;
		}
		*out++ = SDC_block[in];
		in += 1;
	}
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

