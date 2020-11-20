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
#include "Write_To_File_impl.h"

//Added
#include <iostream>
using std::cerr;
using std::endl;
#include <fstream>
using std::ofstream;
#include <cstdlib> // for exit function
#include <iostream>
#include <iomanip>

namespace gr {
  namespace DRM {

    Write_To_File::sptr
    Write_To_File::make(int n, std::string filename)
    {
      return gnuradio::get_initial_sptr
        (new Write_To_File_impl(n, filename));
    }


	int max_limit;
	int max;
	std::string fn;
    /*
     * The private constructor
     */
    Write_To_File_impl::Write_To_File_impl(int n, std::string filename)
      : gr::sync_block("Write_To_File",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0))
    {
    	max_limit = 0;
    	max = n;
    	fn = filename;
    	std::ofstream ofs;
	ofs.open(fn, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
    }

    /*
     * Our virtual destructor.
     */
    Write_To_File_impl::~Write_To_File_impl()
    {
    }

    int
    Write_To_File_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];

      // Do <+signal processing+>
      for(int i = 0; i<noutput_items; i++){
	if(max_limit < max){
		//Write input to file
		ofstream outdata; // outdata is like cin
		outdata.open(fn, ofstream::app); // opens the file
		if( !outdata ) { // file couldn't be opened
		      cerr << "Error: file could not be opened" << std::endl;
		      exit(1);
		 }
		 outdata << std::fixed;
		 outdata << std::setprecision(6);
		 outdata << in[i].real() <<","<<in[i].imag()<< std::endl;
		 //outdata << abs(*in++) << std::endl;
		 //GR_LOG_INFO(this->d_logger,*in++);
		 outdata.close();
	}
	max_limit++;
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
      //return 0;
    }

  } /* namespace DRM */
} /* namespace gr */

