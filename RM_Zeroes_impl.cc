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
#include "RM_Zeroes_impl.h"

namespace gr {
  namespace DRM {

    RM_Zeroes::sptr
    RM_Zeroes::make()
    {
      return gnuradio::get_initial_sptr
        (new RM_Zeroes_impl());
    }


    /*
     * The private constructor
     */
    RM_Zeroes_impl::RM_Zeroes_impl()
      : gr::sync_block("RM_Zeroes",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {}

    /*
     * Our virtual destructor.
     */
    RM_Zeroes_impl::~RM_Zeroes_impl()
    {
    }

    int
    RM_Zeroes_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

	int prod = 0;
      // Do <+signal processing+>
	for(int i = 0; i<noutput_items; i++){
		if(in[i].real() != 0){
			*out++ = in[i];
			prod += 1;
		}
	}
      // Tell runtime system how many output items we produced.
      //return noutput_items;
      return prod;
    }

  } /* namespace DRM */
} /* namespace gr */

