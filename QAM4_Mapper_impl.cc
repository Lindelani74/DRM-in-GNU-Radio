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
#include "QAM4_Mapper_impl.h"

namespace gr {
  namespace DRM {

    QAM4_Mapper::sptr
    QAM4_Mapper::make()
    {
      return gnuradio::get_initial_sptr
        (new QAM4_Mapper_impl());
    }


    /*
     * The private constructor
     */
    QAM4_Mapper_impl::QAM4_Mapper_impl()
      : gr::sync_decimator("QAM4_Mapper",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), 6)
    {}

    /*
     * Our virtual destructor.
     */
    QAM4_Mapper_impl::~QAM4_Mapper_impl()
    {
    }

    int
    QAM4_Mapper_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
	float input[2];
	for(int i = 0; i < noutput_items; i++){
		if(((nitems_written(0) + i) % 7460) == 0){
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
				tag.key = pmt::string_to_symbol("next");
			}
			//tag.value = pmt::PMT_T;
			tag.value = pmt::from_long(7460);
			tag.srcid = alias_pmt();
			//Add the tag to the stream output [0]
			add_item_tag(0,tag);
		}
		for(int j = 0; j<2; j++){
			input[j] = *in++;
		}
		if(input[0] == (float)(0) && input[1] == (float)(0)){
			out[i].real(1/sqrt(2));
			out[i].imag(1/sqrt(2));
		}
		if(input[0] == (float)(1) && input[1] == (float)(0)){
			out[i].real(1/((-1.000)*sqrt(2)));
			out[i].imag(1/sqrt(2));
		}
		if(input[0] == (float)(1) && input[1] == (float)(1)){
			out[i].real(1/((-1.000)*sqrt(2)));
			out[i].imag(1/((-1.000)*sqrt(2)));
		}
		if(input[0] == (float)(0) && input[1] == (float)(1)){
			out[i].real(1/sqrt(2));
			out[i].imag(1/((-1.000)*sqrt(2)));
		}
	}
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

