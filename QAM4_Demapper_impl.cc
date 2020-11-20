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
#include "QAM4_Demapper_impl.h"

namespace gr {
  namespace DRM {

    QAM4_Demapper::sptr
    QAM4_Demapper::make()
    {
      return gnuradio::get_initial_sptr
        (new QAM4_Demapper_impl());
    }

	int bit4;

    /*
     * The private constructor
     */
    QAM4_Demapper_impl::QAM4_Demapper_impl()
      : gr::sync_block("QAM4_Demapper",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
    	bit4 = 2;
    }

    /*
     * Our virtual destructor.
     */
    QAM4_Demapper_impl::~QAM4_Demapper_impl()
    {
    }

    int
    QAM4_Demapper_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      float *out = (float *) output_items[0];

	float outp[2];
	gr_complex input;
	int num_prod = 0;
      // Do <+signal processing+>
	for(int i = 0; i<noutput_items; i++){
		if(bit4 == 2){
			//Colect input
			//GR_LOG_INFO(this->d_logger,*in);
			input = *in++;
			bit4  = 0;
		}
		//else{
		if(bit4 == 0){
			//Set up the two bits
			//Conditions
			if(input.real() > 0 && input.imag() > 0){
				outp[0] = 0;
				outp[1] = 0;
			}
			if(input.real() < 0 && input.imag() > 0){
				outp[0] = 1;
				//outp[0] = 0;
				outp[1] = 0;
			}
			if(input.real() < 0 && input.imag() < 0){
				outp[0] = 1;
				outp[1] = 1;
				/*outp[0] = 0;
				outp[1] = 0;*/
			}
			if(input.real() > 0 && input.imag() < 0){
				outp[0] = 0;
				outp[1] = 1;
				//outp[1] = 0;
			}
			out[i] = outp[0];
			num_prod += 1;
			//GR_LOG_INFO(this->d_logger,outp[0]);
			//GR_LOG_INFO(this->d_logger,output[0]);
			bit4++;
		}
		else{
			//GR_LOG_INFO(this->d_logger,"Bit 2: " + std::to_string(output[1]));
			out[i] = outp[1];
			//GR_LOG_INFO(this->d_logger,outp[1]);
			num_prod += 1;
			//GR_LOG_INFO(this->d_logger,output[1]);
			bit4++;
		}
		//}
	}
      // Tell runtime system how many output items we produced.
      //return noutput_items;
      return num_prod;
    }

  } /* namespace DRM */
} /* namespace gr */

