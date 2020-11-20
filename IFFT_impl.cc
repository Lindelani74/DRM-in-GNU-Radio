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
#include "IFFT_impl.h"

//Added
#include<cmath>
#include<string>
#include <iostream>
using std::cerr;
using std::endl;
#include <fstream>
using std::ofstream;
#include <cstdlib> // for exit function

namespace gr {
  namespace DRM {

    IFFT::sptr
    IFFT::make()
    {
      return gnuradio::get_initial_sptr
        (new IFFT_impl());
    }
    
    //Added
    int symbol_number;
    int tf_number;
    int carrier_number;
    gr_complex input[256];
    int msc_count;
    //For printing
    int input_print;
    int output_print;

    /*
     * The private constructor
     */
    IFFT_impl::IFFT_impl()
      : gr::block("IFFT",
              gr::io_signature::make(3, 3, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
    	//No frames yet
    	symbol_number = 0;
    	tf_number = 0;
    	carrier_number = 0;
    	for(int i = 0; i<256;i++){
    		input[i].real(0.0);
    	}
    	msc_count = 0;
    	input_print = 0;
    	output_print = 0;
    	
    	std::ofstream ofs;
	ofs.open("/home/lindelani/Project/IFFT_input_real.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open("/home/lindelani/Project/IFFT_input_imag.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open("/home/lindelani/Project/IFFT_output_real.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open("/home/lindelani/Project/IFFT_output_imag.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
    }

    /*
     * Our virtual destructor.
     */
    IFFT_impl::~IFFT_impl()
    {
    }

    void
    IFFT_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    	//noutput_items is how many items you are asked to produce :/
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = noutput_items/256;
      ninput_items_required[1] = noutput_items/256;
      ninput_items_required[2] = noutput_items/256;
    }

    int
    IFFT_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in1 = (const gr_complex *) input_items[0];
      const gr_complex *in2 = (const gr_complex *) input_items[1];
      const gr_complex *in3 = (const gr_complex *) input_items[2];
      gr_complex *out = (gr_complex *) output_items[0];
      
      
      //noutput_items is how many items you're **allowed at most** to produce
	//GR_LOG_INFO(this->d_logger,noutput_items);
	/*GR_LOG_INFO(this->d_logger,"------------------------------------------------------");
	GR_LOG_INFO(this->d_logger,"noutput_items " + std::to_string(noutput_items));*/
	
	int sdc_consumed = 0;
	int fac_consumed = 0;
	int msc_consumed = 0;
	int produced_items = 0;
      // Do <+signal processing+>
     for(int i = 0; i<noutput_items; i++){
     	//Initialise a symbol if the output is the 256th
     	if((carrier_number % 256) == 0){
     		sdc_consumed = 0;
		fac_consumed = 0;
		msc_consumed = 0;
     		//Reinitialize the input array for the new symbol
     		for(int l = 0; l<256; l++){
    			input[l] = 0.0;
    		}
	      	//Construct the symbol via input[] array
	      	//Fill in gain cells depending in which symbol group we are dealing with
	      	gain_cells_init(symbol_number);
	      	//Check if we have AFS reference cell conditions. Will overwrite the gain cells where necessary.
	      	//Condition 1
	      	if((symbol_number == 4) && (tf_number == 0)){
	      		afs_cells_init();
	      	}
	      	//Condition 2
	      	if((symbol_number == 39) && (tf_number == 3)){
	      		afs_cells_init();
	      	}
	      //Check if we have time reference cell conditions.
	      if(symbol_number == 0){
	      		time_cells_init();
	      }
	      //Check if FAC cells condition present. If so, fill symbols with FAC cells.
	      if(symbol_number >= 5 && symbol_number <= 26){
	      		//GR_LOG_INFO(this->d_logger,"Calling fac_consumed()");
	      		fac_consumed = FAC_init(symbol_number,in2);
	      }
	      //Check if SDC cells condition present/ If so, fill u the remainder of the symol with SDC cells
	      if(tf_number == 0 && symbol_number < 5){
		      	sdc_consumed = SDC_init(in3);
	      }
	      else{
	      		//Fill with MSC cells
	      		msc_consumed = MSC_init(in1);
	      }
	      //Now the symbol has been initialised.
	      //Increment the number of symbols
	      if(symbol_number == 39){
	     	//Delete
	     	msc_count += msc_consumed;
	     	/*GR_LOG_INFO(this->d_logger,"msc cells consumed for TF " + std::to_string(tf_number) + " is " + std::to_string(msc_count));*/
	     	msc_count = 0;
	      	symbol_number = 0;
	      	//Update TF number
	      	if(tf_number == 3){
	      		tf_number = 0;
	      	}
	      	else{
	      		tf_number += 1;
	      	}
	      }
	      else{
	      	msc_count += msc_consumed;
	      	symbol_number += 1;
	      }
	      // Tell runtime system how many input items we consumed on
      		// each input stream.
      		//consume_each (noutput_items);
      		consume(0,msc_consumed);
     		consume(1,fac_consumed);
      		consume(2,sdc_consumed);
      		//Print the symbol
      		if(input_print < 6){
      			//Write to file
			ofstream outdata; // outdata is like cin
			outdata.open("/home/lindelani/Project/IFFT_input_real.txt", ofstream::app); // opens the file
			if( !outdata ) { // file couldn't be opened
			      cerr << "Error: file could not be opened" << std::endl;
			      exit(1);
			 }
			 outdata << std::fixed;
			 outdata << std::setprecision(6);
			 for(int g = 0; g<256; g++){
				 outdata << input[g].real() << std::endl;
			 }
			 //outdata << abs(*in++) << std::endl;
			 //GR_LOG_INFO(this->d_logger,*in++);
			outdata.close();

			outdata.open("/home/lindelani/Project/IFFT_input_imag.txt", ofstream::app); // opens the file
			if( !outdata ) { // file couldn't be opened
			      cerr << "Error: file could not be opened" << std::endl;
			      exit(1);
	 		}
			 outdata << std::fixed;
			 outdata << std::setprecision(6);
			 for(int t = 0; t<256; t++){
			 	outdata << input[t].imag() << std::endl;
			 }
			 //outdata << abs(*in++) << std::endl;
			 //GR_LOG_INFO(this->d_logger,*in++);
			outdata.close();
			input_print += 1;
      		}
	      //Debugging
     		//Reinitialise carrier number
     		carrier_number = 0;
     	}
     	//Put a tag on every 10240th output item. Start of TF. 
     	if(((nitems_written(0) + i) % 10240) == 0){
		//Add tag
		gr::tag_t tag;
		//tag.offset = item_number + i;
		tag.offset = nitems_written(0) + i;
		tag.key = pmt::string_to_symbol("TF_Start");
		tag.value = pmt::PMT_T;
		tag.srcid = alias_pmt();
		//Add the tag to the stream output [0]
		add_item_tag(0,tag);
	}
	//Put a tag on every 256th output item. Start of a symbol.
	if(((nitems_written(0) + i) % 256) == 0){
		//Add tag
		gr::tag_t tag;
		//tag.offset = item_number + i;
		tag.offset = nitems_written(0) + i;
		tag.key = pmt::string_to_symbol("Symbol_Start");
		tag.value = pmt::PMT_T;
		tag.srcid = alias_pmt();
		//Add the tag to the stream output [0]
		add_item_tag(0,tag);
	}
	//Calculate IFFT values that we can (the output values)
	gr_complex sum = 0;
	gr_complex multiplier;
	for(int k = 0; k<256; k++){
		multiplier.real(cos((M_PI/128.00)*k*carrier_number));
		multiplier.imag(sin((M_PI/128.00)*k*carrier_number));
		sum += input[k]*multiplier;
		//GR_LOG_INFO(this->d_logger,input[k]);
	}
	gr_complex divider;
	divider.real(1.00/256.00);
	/*if(i == 240){
      		GR_LOG_INFO(this->d_logger,divider * sum);
      	}*/
	*out++ = divider * sum;
	produced_items += 1;
	if(output_print < 6){
		//Write to file
		ofstream outdata; // outdata is like cin
		outdata.open("/home/lindelani/Project/IFFT_output_real.txt", ofstream::app); // opens the file
		if( !outdata ) { // file couldn't be opened
		      cerr << "Error: file could not be opened" << std::endl;
		      exit(1);
		 }
		 outdata << std::fixed;
		 outdata << std::setprecision(6);
		 outdata << (divider*sum).real() << std::endl;
		 //outdata << abs(*in++) << std::endl;
		 //GR_LOG_INFO(this->d_logger,*in++);
		outdata.close();

		outdata.open("/home/lindelani/Project/IFFT_output_imag.txt", ofstream::app); // opens the file
		if( !outdata ) { // file couldn't be opened
		      cerr << "Error: file could not be opened" << std::endl;
		      exit(1);
 		}
		 outdata << std::fixed;
		 outdata << std::setprecision(6);
		 outdata << (divider*sum).imag() << std::endl;
		 //outdata << abs(*in++) << std::endl;
		 //GR_LOG_INFO(this->d_logger,*in++);
		outdata.close();
		if(carrier_number == 255){
			output_print += 1;
		}
	}
	carrier_number += 1;
     }
     
      // Tell runtime system how many output items we produced.
      return noutput_items;
      //return produced_items;
    }
    
    /*
     * Function to initialise the symbol with SDC cells where appropriate. Returns the number of input SDC cells consumed
     */
    int IFFT_impl::SDC_init(const gr_complex *input_pointer){
    	int output = 0;
    	std::complex<float> zero = 0;
    	//int ind = 0;
    	for(int i = 0; i<213; i++){
    		if(input[i].real() == 0.0){
    			//GR_LOG_INFO(this->d_logger,*input_pointer);
    			input[i] = *input_pointer++;
    			//input[i] = input_pointer[ind];
    			//ind++;
    			//GR_LOG_INFO(this->d_logger,input[i]);
    			output++;
    		}
    	}
    	return output;
    }
    
    /*
     * Function to initialise the symbol with FAC cells where appropriate. Returns the number of input FAC cells consumed
     */
    int IFFT_impl::FAC_init(int symbol, const gr_complex *input_pointer){
    	int output = 0;
	if(symbol%4 == 0){
		//Symbols 8,12,16,20,24
		for(int i = 24; i<185; i += 16){
			input[i] = *input_pointer++;
			output++;
		}
		return output;
	}
	if(symbol%4 == 1){
		//Symbols 5,9,13,17,21,25
		for(int i = 28; i<189; i += 16){
			input[i] = *input_pointer++;
			output++;
		}
		return output;
	}
	if(symbol%4 == 2){
		if(symbol != 26){
			//Symbols 6,10,14,18,22
			for(int i = 16; i<193; i += 16){
				input[i] = *input_pointer++;
				output++;
			}
		}
		else{
			//Symbol 26
			for(int i = 16; i<49; i += 16){
				input[i] = *input_pointer++;
				output++;
			}
		}
		return output;
	}
	if(symbol%4 == 3){
		//Symbols 7,11,15,19,23
		for(int i = 20; i<197; i += 16){
			input[i] = *input_pointer++;
			output++;
		}
		return output;
	}
	return output;
    }
    
    /*
     * Function to initialise the symbol with MSC cells where appropriate. Returns the number of input MSC cells consumed
     */
    int IFFT_impl::MSC_init(const gr_complex *input_pointer){
    	//Debugging
     	//GR_LOG_INFO(this->d_logger,"In the MSC filling function");
    	int output = 0;
    	if(tf_number == 3 && symbol_number == 39){
    		for(int i = 0; i<213; i++){
	    		if(i == 210 || i == 211){
	    			if(i == 210){
	    				input[i].real(0.7071067812);
    					input[i].imag(0.7071067812);
	    			}
	    			else{
	    				input[i].real(0.7071067812); 
    					input[i].imag(-0.7071067812);
	    			}
	    		}
	    		else{
		    		if(input[i].real() == 0.0){
		    			input[i] = *input_pointer++;
		    			output += 1;
		    		}
	    		}
	    	}
    	}
    	else{
    		for(int i = 0; i<213; i++){
    			if(input[i].real() == 0.0){
	    			input[i] = *input_pointer++;
	    			output += 1;
	    		}
    		} 	
    	}
    	//GR_LOG_INFO(this->d_logger,"Return value = " + std::to_string(output));
   	//GR_LOG_INFO(this->d_logger,"------------------------------------------------------");
    	return output;
    }
    
    /*
     * Function to initialise the symbol with Gain reference cells where appropriate.
     */
    void IFFT_impl::gain_cells_init(int symbol){
    	//Can make this code more compact by using for loops in each if.
    	if(symbol%4 == 0){
    		//No special cells
    		input[12] = 1.414213562;
	    	input[28] = 1.414213562;
	    	input[44] = 1.414213562;
	    	input[60] = 1.414213562;
	    	input[76] = 1.414213562;
	    	input[92] = 1.414213562;
	    	input[108] = 1.414213562;
	    	input[124] = 1.414213562;
	    	input[140] = 1.414213562;
	    	input[156] = 1.414213562;
	    	input[172] = 1.414213562;
	    	input[188] = 1.414213562;
	    	input[204] = 1.414213562;
    	}
    	if(symbol%4 == 1){
    		//Special cells
    		input[0] = 2;
	    	input[16] = 1.414213562;
	    	input[32] = 1.414213562;
	    	input[48] = 1.414213562;
	    	input[64] = 1.414213562;
	    	input[80] = 1.414213562;
	    	input[96] = 1.414213562;
	    	input[112] = 1.414213562;
	    	input[128] = 1.414213562;
	    	input[144] = 1.414213562;
	    	input[160] = 1.414213562;
	    	input[176] = 1.414213562;
	    	input[192] = 1.414213562;
	    	input[208] = 2;
    	}
    	if(symbol%4 == 2){
    		//Special cells
    		input[4] = 2;
    		input[20] = 1.414213562;
    		input[36] = 1.414213562;
    		input[52] = 1.414213562;
    		input[68] = 1.414213562;
    		input[84] = 1.414213562;
    		input[100] = 1.414213562;
    		input[116] = 1.414213562;
    		input[132] = 1.414213562;
    		input[148] = 1.414213562;
    		input[164] = 1.414213562;
    		input[180] = 1.414213562;
    		input[196] = 1.414213562;
    		input[212] = 2;
    	}
    	if(symbol%4 == 3){
    		//No special cels
    		input[8] = 1.414213562;
    		input[24] = 1.414213562;
    		input[40] = 1.414213562;
    		input[56] = 1.414213562;
    		input[72] = 1.414213562;
    		input[88] = 1.414213562;
    		input[104] = 1.414213562;
    		input[120] = 1.414213562;
    		input[136] = 1.414213562;
    		input[152] = 1.414213562;
    		input[168] = 1.414213562;
    		input[184] = 1.414213562;
    		input[200] = 1.414213562;
    	}
    }
    
    void IFFT_impl::afs_cells_init(){
    	for(int i = 0; i<256; i+=4){
    		input[i] = 1.0;
    	}
    }
    
    void IFFT_impl::time_cells_init(){
    	input[26] = 1.414213562;
    	input[27] = 1.414213562;
    	input[29] = 1.414213562;
    	input[53] = 1.414213562;
    	input[54] = 1.414213562;
    	input[55] = 1.414213562;
    	input[74] = 1.414213562;
    	input[75] = 1.414213562;
    	input[118] = 1.414213562;
    	input[119] = 1.414213562;
    	input[120] = 1.414213562;
    	input[127] = 1.414213562;
    	input[128] = 1.414213562;
    	input[129] = 1.414213562;
    	input[146] = 1.414213562;
    	input[147] = 1.414213562;
    	input[148] = 1.414213562;
    	input[173] = 1.414213562;
    	input[174] = 1.414213562;
    	input[185] = 1.414213562;
    	input[186] = 1.414213562;	
    }


  } /* namespace DRM */
} /* namespace gr */

