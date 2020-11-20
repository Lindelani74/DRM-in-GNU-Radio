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
#include "Write_To_File_Int_impl.h"

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

    Write_To_File_Int::sptr
    Write_To_File_Int::make(int num_it, std::string file)
    {
      return gnuradio::get_initial_sptr
        (new Write_To_File_Int_impl(num_it, file));
    }


	int maxim;
	int max_lim;
	std::string f;
    /*
     * The private constructor
     */
    Write_To_File_Int_impl::Write_To_File_Int_impl(int num_it, std::string file)
      : gr::sync_block("Write_To_File_Int",
              gr::io_signature::make(1, 1, sizeof(int)),
              gr::io_signature::make(0, 0, 0))
    {
    	maxim = 0;
	max_lim = num_it;
	f = file;
	std::ofstream ofs;
	ofs.open(f, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
    }

    /*
     * Our virtual destructor.
     */
    Write_To_File_Int_impl::~Write_To_File_Int_impl()
    {
    }

    int
    Write_To_File_Int_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const int *in = (const int *) input_items[0];

	// Do <+signal processing+>
	for(int i = 0; i<noutput_items; i++){
		if(maxim < max_lim){
			//Write input to file
			ofstream outdata; // outdata is like cin
			outdata.open(f, ofstream::app); // opens the file
			if( !outdata ) { // file couldn't be opened
			      cerr << "Error: file could not be opened" << std::endl;
			      exit(1);
			 }
			 outdata << std::fixed;
			 outdata << std::setprecision(6);
			 outdata << in[i]<< std::endl;
			 //outdata << abs(*in++) << std::endl;
			 //GR_LOG_INFO(this->d_logger,*in++);
			 outdata.close();
		}
		maxim++;
	}
	// Tell runtime system how many output items we produced.
	return noutput_items;
	//return 0;
    }

  } /* namespace DRM */
} /* namespace gr */

