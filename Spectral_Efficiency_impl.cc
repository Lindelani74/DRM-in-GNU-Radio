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
#include "Spectral_Efficiency_impl.h"
//Added
#include<cmath>
#include <complex>
#include <iomanip>
//---------------------
#include <iostream>
using std::cerr;
using std::endl;
#include <fstream>
//--------------------
using std::cerr;
using std::endl;
#include <fstream>
using std::ofstream;
#include <cstdlib> // for exit function
#include <ctime>
#include <chrono>
#include <random>
#include <numeric>

using namespace std::chrono;

namespace gr {
  namespace DRM {

    Spectral_Efficiency::sptr
    Spectral_Efficiency::make()
    {
      return gnuradio::get_initial_sptr
        (new Spectral_Efficiency_impl());
    }

	int num_inputbits;

    /*
     * The private constructor
     */
    Spectral_Efficiency_impl::Spectral_Efficiency_impl()
      : gr::sync_block("Spectral_Efficiency",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, sizeof(gr_complex)))
    {
    	num_inputbits = 0;
    	
    }

    /*
     * Our virtual destructor.
     */
    Spectral_Efficiency_impl::~Spectral_Efficiency_impl()
    {
    }

    int
    Spectral_Efficiency_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      
      // Do <+signal processing+>
      auto start = high_resolution_clock::now();
      for(int i = 0; i<noutput_items; i++){
      if(num_inputbits == 0){
      	start = high_resolution_clock::now();
      }
      if(num_inputbits == 999){
      		//Stop clock
      		auto stop_value = high_resolution_clock::now();
    		auto duration = duration_cast<microseconds>(stop_value - start);
    		//Write input to file
		ofstream outdata; // outdata is like cin
		outdata.open("/home/lindelani/Spectral_efficiency.txt", ofstream::app); // opens the file
		if( !outdata ) { // file couldn't be opened
		      cerr << "Error: file could not be opened" << std::endl;
		      exit(1);
		 }
		 outdata << "Time taken by function: "<< duration.count() << " microseconds" << endl;
		 outdata.close();
      	}
      	num_inputbits += 1;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace DRM */
} /* namespace gr */

