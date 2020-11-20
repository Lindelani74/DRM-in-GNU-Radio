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
#include "To_complex_impl.h"

namespace gr {
  namespace DRM {

    To_complex::sptr
    To_complex::make()
    {
      return gnuradio::get_initial_sptr
        (new To_complex_impl());
    }

	//Added
	int output_no;
	int print;

    /*
     * The private constructor
     */
    To_complex_impl::To_complex_impl()
      : gr::sync_block("To_complex",
              gr::io_signature::make(2, 2, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
    	output_no = 0;
    	print = 1;
    }

    /*
     * Our virtual destructor.
     */
    To_complex_impl::~To_complex_impl()
    {
    }

    int
    To_complex_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in1 = (const float *) input_items[0];
      const float *in2 = (const float *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for(int i = 0; i < noutput_items; i++){
      	out[i].real(*in1++);
	out[i].imag(*in2++);
	/*if(output_no == 1){
		GR_LOG_INFO(this->d_logger,out[i]);
	}*/
	output_no++;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

