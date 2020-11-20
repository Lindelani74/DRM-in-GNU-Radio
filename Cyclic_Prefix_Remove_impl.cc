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
#include "Cyclic_Prefix_Remove_impl.h"

//Added
#include <iostream>
using std::cerr;
using std::endl;
#include <fstream>
using std::ofstream;
#include <cstdlib> // for exit function

namespace gr {
  namespace DRM {

    Cyclic_Prefix_Remove::sptr
    Cyclic_Prefix_Remove::make(int len)
    {
      return gnuradio::get_initial_sptr
        (new Cyclic_Prefix_Remove_impl(len));
    }


	//Added
	int counter2;
	int publish2;
	int le;
    /*
     * The private constructor
     */
    Cyclic_Prefix_Remove_impl::Cyclic_Prefix_Remove_impl(int len)
      : gr::sync_block("Cyclic_Prefix_Remove",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
    	counter2 = 0;
    	publish2 = 1;
    	le = len;
    }

    /*
     * Our virtual destructor.
     */
    Cyclic_Prefix_Remove_impl::~Cyclic_Prefix_Remove_impl()
    {
    }

    int
    Cyclic_Prefix_Remove_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
	
	int in_function_count = 0;
      // Do <+signal processing+>
      for(int i = 0; i<noutput_items; i++){
      	if (counter2 >= le && counter2 <= (255+le)){
      		//Pass symbol through
      		/*if(publish2 == 1){
		//Write to file
		ofstream outdata; // outdata is like cin
		outdata.open("/home/lindelani/FFT_INPUT.txt", ofstream::app); // opens the file
		   if( !outdata ) { // file couldn't be opened
		      cerr << "Error: file could not be opened" << endl;
		      exit(1);
		   }
		      outdata << std::abs(*in) << endl;
		      //outdata << *out << endl<<endl;
		   outdata.close();
		}*/
      		if(counter2 == (255+le)){
      			counter2 = 0;
      			publish2 = 0;
      		}
      		else{
      			counter2++;
      		}
      		in_function_count++;
		*out++ = *in++;
      	}
      	else{
      		//counter2 range: [0]-[27]
      		//Do nothing but check limits and iterate over the input symbols coming in
      		counter2++;
      		*in++;
      	}
      }

      // Tell runtime system how many output items we produced.
      return in_function_count;
//      return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

