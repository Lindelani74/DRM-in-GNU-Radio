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
#include "FFT_impl.h"

//Added
#include <iostream>
using std::cerr;
using std::endl;
#include <fstream>
using std::ofstream;
#include <cstdlib> // for exit function
#include<math.h>
#include<cmath>

namespace gr {
  namespace DRM {

    FFT::sptr
    FFT::make()
    {
      return gnuradio::get_initial_sptr
        (new FFT_impl());
    }

    //Added
    int num_inputs1;
    int input_number;
    int symbol_no;
    int transmission_frame_number;
    int transmission_super_frame_number;
    std::string chan;
    
    //Variable will be 1 when we are filling up the fft block with 256 'symbols' 
    int filling;
    //Variable will be 1 when we are outputting the three fft outputs(MSC, SDC, FAC QAM cells)
    int outputting;
    //Variable for the 256-item input array
    gr_complex time[256];
    //Variable to keep count of carrier number on output
    int carrier;

    //Item production and consumption
    int inputs_consumed;
    int outputs_produced;
    
    int input_done;
    int output_done;
    
    //MSC and SDC cell array (to help 'fill the rest')
    int symbol_array[256];
    /*
     * The private constructor
     */
    FFT_impl::FFT_impl()
      : gr::block("FFT",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(3, 3, sizeof(gr_complex)))
    {
    	num_inputs1 = 0;
    	symbol_no = 0;
    	transmission_frame_number = 0;
	transmission_super_frame_number = 0;
	filling = 1;
	outputting = 0;
	carrier = 0;
	inputs_consumed = 0;
  	outputs_produced = 0;
  	//initialise symbol array 
    	for(int y = 0; y<256; y++){
    		//All unoccupied for now 
    		symbol_array[y] = 0;
    	}
    	chan = "H";
    	input_done = 0;
    	output_done = 0;
    	/*std::ofstream ofs;
	ofs.open("/home/lindelani/Project/FFT_input_real.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open("/home/lindelani/Project/FFT_input_imag.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open("/home/lindelani/Project/FFT_output_real.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open("/home/lindelani/Project/FFT_output_imag.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();*/
	
    }

    /*
     * Our virtual destructor.
     */
    FFT_impl::~FFT_impl()
    {
    }

    void
    FFT_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    FFT_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      //MSC
      gr_complex *out0 = (gr_complex *) output_items[0];
      //FAC
      gr_complex *out1 = (gr_complex *) output_items[1];
      //SDC
      gr_complex *out2 = (gr_complex *) output_items[2];
  	
  	inputs_consumed = 0;
  	outputs_produced = 0;
  	for(int i = 0; i<noutput_items; i++){
  		if(filling == 1){
  			gr_complex value = *in;
  			//Fill up the FFT block
  			if(num_inputs1 < 256){
  				//[0]-[255]
  				//time[num_inputs1] = in[i];
  				time[num_inputs1] = *in++;
  				//GR_LOG_INFO(this->d_logger,"Input: " + std::to_string(num_inputs1) + "  " + std::to_string(value.real()));
  				inputs_consumed += 1;
  			}
  			/*if(input_done < 2){
  				//GR_LOG_INFO(this->d_logger,input_done);
  				//Write to file
  				ofstream outdata; // outdata is like cin
				outdata.open("/home/lindelani/Project/FFT_input_real.txt", ofstream::app); // opens the file
				if( !outdata ) { // file couldn't be opened
				      cerr << "Error: file could not be opened" << std::endl;
				      exit(1);
				 }
				 outdata << std::fixed;
				 outdata << std::setprecision(6);
				 outdata << value.real() << std::endl;
				 //outdata << abs(*in++) << std::endl;
				 //GR_LOG_INFO(this->d_logger,*in++);
		 		outdata.close();

				outdata.open("/home/lindelani/Project/FFT_input_imag.txt", ofstream::app); // opens the file
				if( !outdata ) { // file couldn't be opened
				      cerr << "Error: file could not be opened" << std::endl;
				      exit(1);
				 }
				 outdata << std::fixed;
				 outdata << std::setprecision(6);
				 outdata << value.imag() << std::endl;
				 //outdata << abs(*in++) << std::endl;
				 //GR_LOG_INFO(this->d_logger,*in++);
		 		outdata.close();
  			}*/
  			//Update
  			if(num_inputs1 == 255){
  				//[255]
  				//Array filled up
  				num_inputs1 = 0;
  				filling = 0;
  				outputting = 1;
  				input_done += 1;
  			}
  			else{
  				//[0]-[254]
  				num_inputs1 += 1;
  			}
  		}
  		if(outputting == 1){
  			//GR_LOG_INFO(this->d_logger,std::to_string(symbol_no));
  			//Calculate FFT. Will happen 256 times
  			gr_complex sum = 0;
			gr_complex multiplier;
  			for(int p = 0; p<256; p++){
  				//Summation
  				multiplier.real(cos((-M_PI/128.00)*(float)(p)*(float)(carrier)));
				multiplier.imag(sin((-M_PI/128.00)*(float)(p)*(float)(carrier)));
				//GR_LOG_INFO(this->d_logger,multiplier);
				sum += time[p]*multiplier;
  			}
  			//Symbol output
  			gr_complex bin = sum;
  			//GR_LOG_INFO(this->d_logger,bin);
  			//The function lets us know to which channel the output belongs
  			gr_complex bin2 = resolution(bin);
  			//GR_LOG_INFO(this->d_logger,"Symbol " + std::to_string(symbol_no) + " Carrier " + std::to_string(carrier) + " Output " + std::to_string(bin2.real()));
  			/*if(output_done < 6){
  				//Write to file
  				ofstream outdata; // outdata is like cin
				outdata.open("/home/lindelani/Project/FFT_output_real.txt", ofstream::app); // opens the file
				if( !outdata ) { // file couldn't be opened
				      cerr << "Error: file could not be opened" << std::endl;
				      exit(1);
				 }
				 outdata << std::fixed;
				 outdata << std::setprecision(6);
				 if(chan == "M"){
				 	outdata << bin2.real() << " - MSC" << std::endl;
				 }
				 if(chan == "F"){
				 	outdata << bin2.real() << " - FAC" << std::endl;
				 }
				 if(chan == "S"){
				 	outdata << bin2.real() << " - SDC" << std::endl;
				 }
				 if(chan == "H"){
				 	outdata << bin2.real() << std::endl;
				 }
				 //outdata << abs(*in++) << std::endl;
				 //GR_LOG_INFO(this->d_logger,*in++);
		 		outdata.close();
		 		
		 		outdata.open("/home/lindelani/Project/FFT_output_imag.txt", ofstream::app); // opens the file
				if( !outdata ) { // file couldn't be opened
				      cerr << "Error: file could not be opened" << std::endl;
				      exit(1);
				 }
				 outdata << std::fixed;
				 outdata << std::setprecision(6);
				 outdata << bin2.imag()<< std::endl;
				 //outdata << abs(*in++) << std::endl;
				 //GR_LOG_INFO(this->d_logger,*in++);
		 		outdata.close();
  			}*/
  			//MSC
  			if(chan == "M"){
  				*out0++ = bin2;
  				outputs_produced += 1;
  			}
  			//FAC
  			if(chan == "F"){
  				*out1++ = bin2;
  				//GR_LOG_INFO(this->d_logger,bin2);
  				outputs_produced += 1;
  			}
  			//SDC
  			if(chan == "S"){
  				*out2++ = bin2;
  				outputs_produced += 1;
  			}
  			//These above cannot all be =1 at the same time!!
  			//Counter updates
  			//Update carrier count
  			if(carrier == 255){
  				output_done += 1;
  				//This condition triggers updates if needed
  				//Update the symbol_no count
  				if(symbol_no == 39){
  					//Update the TF count
  					if(transmission_frame_number == 3){
  						//Update the TSF count
  						transmission_super_frame_number += 1;
  						transmission_frame_number = 0;
  					}
  					else{
  						transmission_frame_number += 1;
  					}
  					symbol_no = 0;
  				}
  				else{
  					symbol_no += 1;
  				}
  				carrier = 0;
  				//Fillup another input
  				filling = 1;
  				outputting = 0;
  				//Reset the symbol_array array
				for(int y = 0; y<256; y++){
					//All unoccupied 
					symbol_array[y] = 0;
				}
  			}
  			else{
  				carrier += 1;
  			}
  		}
  	}
      // Tell runtime system how many input items we consumed on
      // each input stream.
      //consume_each (noutput_items);
      consume_each (inputs_consumed);

      // Tell runtime system how many output items we produced.
      //return noutput_items;
      return outputs_produced;
    }
    
    gr_complex
    FFT_impl::resolution (gr_complex in)
    {
    	/*
    	Note: symbol_array[index] means the index carrier is now occupied (for SDC and MSC cell fillup)
    	*/
    	gr_complex produced = in;
    	if(carrier >= 213 && carrier <= 255){
    		//Null carriers
    		//GR_LOG_INFO(this->d_logger,produced);
    		symbol_array[carrier] = 1;
    		//produced.real(0.00);
    		//produced.imag(0.00);
		chan = "H";
		return produced;
    	}
    	//AFS Reference Conditions. Just consume the input.
	//AFS References(Only in Symbol 4 of the first TF and Symbol 39 of the fourth TF)
	if((transmission_frame_number == 0 && symbol_no == 4) || (transmission_frame_number == 3 && symbol_no == 39)){
		if(carrier % 4 == 0){
			//GR_LOG_INFO(this->d_logger,"AFS reference. Transmission Frame " + std::to_string(transmission_frame_number) + " Symbol " + std::to_string(symbol_no) + " Carrier " + std::to_string(carrier));
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
	}
    	//Time Reference Conditions. Just consume the input.
	if(symbol_no == 0){
		if(carrier >= 53 && carrier <= 55){
			//GR_LOG_INFO(this->d_logger,"Time Reference");
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
		else if(carrier >= 118 && carrier <= 120){
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
		else if(carrier >= 127 && carrier <= 129){
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
		else if(carrier >= 146 && carrier <= 148){
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
		else if(carrier == 26 || carrier == 27 || carrier == 29 || carrier == 74 || carrier == 75 || carrier == 173 || carrier == 174 || carrier == 185 || carrier == 186){
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
		else{
			//Do nothing. It's not Time reference's time.
		}
	}
	//Gain Reference Conditions. Just consume the input. 4 groups of symbols
	if(symbol_no % 4 == 0){
		//Group 1
		if(carrier == 12 || carrier == 28 || carrier == 44 || carrier == 60 || carrier == 76 || carrier == 92 || carrier == 108 || carrier == 124 || carrier == 140 || carrier == 156 || carrier == 172 || carrier == 188 || carrier == 204){
		//GR_LOG_INFO(this->d_logger,"Gain reference. Symbol " + std::to_string(symbol_no) + " Carrier " + std::to_string(carrier));
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
	}
	if(symbol_no % 4 == 1){
		//Group 2
		if(carrier == 0 || carrier == 16 || carrier == 32 || carrier == 48 || carrier == 64 || carrier == 80 || carrier == 96 || carrier == 112 || carrier == 128 || carrier == 144 || carrier == 160 || carrier == 176 || carrier == 192 || carrier == 208){
		//GR_LOG_INFO(this->d_logger,"Gain reference. Symbol " + std::to_string(symbol_no) + " Carrier " + std::to_string(carrier) + " Output " + std::to_string(produced.real()));
		//GR_LOG_INFO(this->d_logger,"Gain reference. Symbol " + std::to_string(symbol_no) + " Carrier " + std::to_string(carrier));
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
	}
	if(symbol_no % 4 == 2){
		//Group 3
		if(carrier == 4 || carrier == 20 || carrier == 36 || carrier == 52 || carrier == 68 || carrier == 84 || carrier == 100 || carrier == 116 || carrier == 132 || carrier == 148 || carrier == 164 || carrier == 180 || carrier == 196 || carrier == 212){
		//GR_LOG_INFO(this->d_logger,"Gain reference. Symbol " + std::to_string(symbol_no) + " Carrier " + std::to_string(carrier));
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
	}
	if(symbol_no % 4 == 3){
		//Group 4
		if(carrier == 8 || carrier == 24 || carrier == 40 || carrier == 56 || carrier == 72 || carrier == 88 || carrier == 104 || carrier == 120 || carrier == 136 || carrier == 152 || carrier == 168 || carrier == 184 || carrier == 200){
		//GR_LOG_INFO(this->d_logger,"Gain reference. Symbol " + std::to_string(symbol_no) + " Carrier " + std::to_string(carrier));
			symbol_array[carrier] = 1;
			chan = "H";
			return produced;
		}
	}
	//Data cells
	//FAC Cells
	if(symbol_no >= 5 && symbol_no <= 26){
		//Group 1
		if(symbol_no % 4 == 1){
			if(carrier == 28 || carrier == 44 || carrier == 60 || carrier == 76 || carrier == 92 || carrier == 108 || carrier == 124 || carrier == 140 || carrier == 156 || carrier == 172 || carrier == 188){
				produced = euclidean(in);
				symbol_array[carrier] = 1;
				chan = "F";
				//GR_LOG_INFO(this->d_logger,"FAC");
				//GR_LOG_INFO(this->d_logger,produced);
				return produced;
			}
		}
		//Group 2
		if(symbol_no % 4 == 2){
			if(symbol_no != 26){
				if(carrier == 16 || carrier == 32 || carrier == 48 || carrier == 64 || carrier == 80 || carrier == 96 || carrier == 112 || carrier == 128 || carrier == 144 || carrier == 160 || carrier == 176 || carrier == 196){
					produced = euclidean(in);
					symbol_array[carrier] = 1;
					chan = "F";
					//GR_LOG_INFO(this->d_logger,produced);
					return produced;
				}
			}
			else{
				//Symbol [26]
				if(carrier == 16 || carrier == 32 || carrier == 48){
					produced = euclidean(in);
					symbol_array[carrier] = 1;
					chan = "F";
					//GR_LOG_INFO(this->d_logger,produced);
					return produced;
				}
			}
		}
		//Group 3
		if(symbol_no % 4 == 3){
			if(carrier == 20 || carrier == 36 || carrier == 52 || carrier == 68 || carrier == 84 || carrier == 100 || carrier == 116 || carrier == 132 || carrier == 148 || carrier == 164 || carrier == 180 || carrier == 196){
				produced = euclidean(in);
				symbol_array[carrier] = 1;
				chan = "F";
				//GR_LOG_INFO(this->d_logger,produced);
				return produced;
			}
		}
		//Group 4
		if(symbol_no % 4 == 0){
			if(carrier == 24 || carrier == 40 || carrier == 56 || carrier == 72 || carrier == 88 || carrier == 104 || carrier == 120 || carrier == 136 || carrier == 152 || carrier == 168 || carrier == 184){
				produced = euclidean(in);
				symbol_array[carrier] = 1;
				chan = "F";
				//GR_LOG_INFO(this->d_logger,produced);
				return produced;
			}
		}
	}
	//SDC Cells
	if(transmission_frame_number == 0 && (symbol_no == 0 || symbol_no == 1 || symbol_no == 2 ||symbol_no == 3 || symbol_no == 4)){
		//Look for the first open slot
		for(int h = 0; h<256; h++){
			if(symbol_array[h] == 0){
				//GR_LOG_INFO(this->d_logger,"SDC");
				produced = euclidean(in);
				//Now occupied
				symbol_array[h] = 1;
				//GR_LOG_INFO(this->d_logger,produced);
				chan = "S";
				return produced;
			}
		}
	}
	//MSC Cells
	else{
		//Look for the first open slot
		for(int h = 0; h<256; h++){
			if(symbol_array[h] == 0){
				produced = euclidean(in);
				//Now occupied
				symbol_array[h] = 1;
				chan = "M";
				return produced;
			}
		}
	}
    }
    
    gr_complex 
    FFT_impl::euclidean(gr_complex number){
	std::cout<<std::fixed;
   	std::cout<<std::setprecision(6);
    	//Value returned
    	gr_complex point;
    	//Detect using euclidean distance
	float point_one;
	float point_two;
	float point_three;
	float point_four;
	//Point one
	point_one = sqrt(pow(sqrt(1.00/2.00)-number.real(),2)+pow(sqrt(1.00/2.00)-number.imag(),2));
	//Point two
	point_two = sqrt(pow(-sqrt(1.00/2.00)-number.real(),2)+pow(sqrt(1.00/2.00)-number.imag(),2));
	//Point three
	point_three = sqrt(pow(-sqrt(1.00/2.00)-number.real(),2)+pow(-sqrt(1.00/2.00)-number.imag(),2));
	//Point four
	point_four = sqrt(pow(sqrt(1.00/2.00)-number.real(),2)+pow(-sqrt(1.00/2.00)-number.imag(),2));
	
	//GR_LOG_INFO(this->d_logger,number);
	/*GR_LOG_INFO(this->d_logger,point_one);
	GR_LOG_INFO(this->d_logger,point_two);
	GR_LOG_INFO(this->d_logger,point_three);
	GR_LOG_INFO(this->d_logger,point_four);*/
	
	//Find the lowest
	if(point_one < point_two && point_one < point_three && point_one < point_four){
		point.real(sqrt(1.00/2.00));
		point.imag(sqrt(1.00/2.00)); 
		return point;
		/*GR_LOG_INFO(this->d_logger,"1st condition ")
		GR_LOG_INFO(this->d_logger, point);*/
	}
	if(point_two < point_one && point_two < point_three && point_two < point_four){
		point.real(-sqrt(1.00/2.00));
		point.imag(sqrt(1.00/2.00)); 
		return point;
		/*GR_LOG_INFO(this->d_logger,"2nd condition ");
		GR_LOG_INFO(this->d_logger, point);*/
	}
	if(point_three < point_one && point_three < point_two && point_three < point_four){
		point.real(-sqrt(1.00/2.00));
		point.imag(-sqrt(1.00/2.00)); 
		return point;
		/*GR_LOG_INFO(this->d_logger,"3rd condition ");
		GR_LOG_INFO(this->d_logger, point);*/
	}
	if(point_four < point_one && point_four < point_two && point_four < point_three){
		point.real(sqrt(1.00/2.00));
		point.imag(-sqrt(1.00/2.00)); 
		return point;
		/*GR_LOG_INFO(this->d_logger,"4th condition ");
		GR_LOG_INFO(this->d_logger, point);*/
	}
	/*point.real(-sqrt(1.00/2.00));
	point.imag(-sqrt(1.00/2.00)); 
	return point;*/
    }

  } /* namespace DRM */
} /* namespace gr */

