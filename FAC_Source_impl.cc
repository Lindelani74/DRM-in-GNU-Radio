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
#include "FAC_Source_impl.h"

namespace gr {
  namespace DRM {
  
  	//Added
  	int FAC_block[108];
  	int index;

    FAC_Source::sptr
    FAC_Source::make(int msc_modulation,int sdc_code_rate,int num_audio_services,int num_data_services)
    {
      return gnuradio::get_initial_sptr
        (new FAC_Source_impl(msc_modulation, sdc_code_rate, num_audio_services, num_data_services));
    }


    /*
     * The private constructor
     */
    FAC_Source_impl::FAC_Source_impl(int msc_modulation,int sdc_code_rate,int num_audio_services,int num_data_services)
      : gr::sync_block("FAC_Source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(int)))
    {
    	index = 0;
    	int channel_params[20] = {0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0};
    	int service_params[44];
    	for(int i = 0; i<44; i++){
    		service_params[i] = 0;
    	}
    	//Copy the Channel Parameters and the Service Prameters
    	for(int i = 0; i<108; i++){
		if(i<20){
			FAC_block[i] = channel_params[i];
		}    		
		else if(i>=20 && i<64){
			//Service parameters
			FAC_block[i] = service_params[i-20];
		}
		else{
			FAC_block[i] = service_params[i-64];
		}
    	}
    }

    /*
     * Our virtual destructor.
     */
    FAC_Source_impl::~FAC_Source_impl()
    {
    }

    int
    FAC_Source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      int *out = (int *) output_items[0];

      // Do <+signal processing+>
	for(int i = 0; i<noutput_items; i++){
		if(index == 108){
			index = 0;
		}
		*out++ = FAC_block[index];
		index += 1;
	}
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

