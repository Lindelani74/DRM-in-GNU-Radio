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
#include "QAM16_Mapper_impl.h"

namespace gr {
  namespace DRM {

    QAM16_Mapper::sptr
    QAM16_Mapper::make()
    {
      return gnuradio::get_initial_sptr
        (new QAM16_Mapper_impl());
    }


    /*
     * The private constructor
     */
    QAM16_Mapper_impl::QAM16_Mapper_impl()
      : gr::sync_decimator("QAM16_Mapper",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), 4)
    {}

    /*
     * Our virtual destructor.
     */
    QAM16_Mapper_impl::~QAM16_Mapper_impl()
    {
    }

    int
    QAM16_Mapper_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
	float input[4];
	for(int i = 0; i < noutput_items; i++){
		for(int j = 0; j<4; j++){
			input[j] = *in++;
		}
		if(input[0] == (float)(0) && input[1] == (float)(0) && input[2] == (float)(0) && input[3] == (float)(0)){
			out[i].real(3/sqrt(10));
			out[i].imag(3/sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(0) && input[2] == (float)(0) && input[3] == (float)(0)){
			out[i].real(1/sqrt(10));
			out[i].imag(3/sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(0) && input[2] == (float)(1) && input[3] == (float)(0)){
			out[i].real(1/sqrt(10));
			out[i].imag(1/sqrt(10));
		}
		if(input[0] == (float)(0) && input[1] == (float)(0) && input[2] == (float)(1) && input[3] == (float)(0)){
			out[i].real(3/sqrt(10));
			out[i].imag(1/sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(0) && input[2] == (float)(0) && input[3] == (float)(1)){
			out[i].real(1/sqrt(10));
			out[i].imag(1/-sqrt(10));
		}
		if(input[0] == (float)(0) && input[1] == (float)(0) && input[2] == (float)(0) && input[3] == (float)(1)){
			out[i].real(3/sqrt(10));
			out[i].imag(1/-sqrt(10));
		}
		if(input[0] == (float)(0) && input[1] == (float)(0) && input[2] == (float)(1) && input[3] == (float)(1)){
			out[i].real(3/sqrt(10));
			out[i].imag(3/-sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(0) && input[2] == (float)(1) && input[3] == (float)(1)){
			out[i].real(1/sqrt(10));
			out[i].imag(3/-sqrt(10));
		}
		if(input[0] == (float)(0) && input[1] == (float)(1) && input[2] == (float)(0) && input[3] == (float)(0)){
			out[i].real(1/-sqrt(10));
			out[i].imag(3/sqrt(10));
		}
		if(input[0] == (float)(0) && input[1] == (float)(1) && input[2] == (float)(1) && input[3] == (float)(0)){
			out[i].real(1/-sqrt(10));
			out[i].imag(1/sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(1) && input[2] == (float)(1) && input[3] == (float)(0)){
			out[i].real(3/-sqrt(10));
			out[i].imag(1/sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(1) && input[2] == (float)(0) && input[3] == (float)(0)){
			out[i].real(3/-sqrt(10));
			out[i].imag(3/sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(1) && input[2] == (float)(0) && input[3] == (float)(1)){
			out[i].real(3/-sqrt(10));
			out[i].imag(1/-sqrt(10));
		}
		if(input[0] == (float)(0) && input[1] == (float)(1) && input[2] == (float)(0) && input[3] == (float)(1)){
			out[i].real(1/-sqrt(10));
			out[i].imag(1/-sqrt(10));
		}
		if(input[0] == (float)(0) && input[1] == (float)(1) && input[2] == (float)(1) && input[3] == (float)(1)){
			out[i].real(1/-sqrt(10));
			out[i].imag(3/-sqrt(10));
		}
		if(input[0] == (float)(1) && input[1] == (float)(1) && input[2] == (float)(1) && input[3] == (float)(1)){
			out[i].real(3/-sqrt(10));
			out[i].imag(3/-sqrt(10));
		}
	}
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

