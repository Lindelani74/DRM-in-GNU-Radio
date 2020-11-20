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

#ifndef INCLUDED_DRM_WRITE_TO_FILE_INT_IMPL_H
#define INCLUDED_DRM_WRITE_TO_FILE_INT_IMPL_H

#include <DRM/Write_To_File_Int.h>

namespace gr {
  namespace DRM {
  
	/**
 	*  This class presents a utility block that takes in the first specified number of lines from the input stream and writes it out to a file. Implemented specifically for a float input. 
 	*/
    class Write_To_File_Int_impl : public Write_To_File_Int
    {
     private:
      // Nothing to declare in this block.

     public:
     /*!
      * The private constructor. 
      *
      * @param num_it the number of lines to write into the text file. These lines will be the first n input items to the block.
      * @param file the name of the text file to which the data must be written.
      */
      Write_To_File_Int_impl(int num_it, std::string file);
      /*!
      * Our virtual destructor.
      *
      */
      ~Write_To_File_Int_impl();

       /*!
      * This function is where the DSP algorithm is implemented. It produces output items from processing the input items.
      *
      * @param noutput_items the number of items that may be produced for this current call to general_work() which represents the number of output items to write for each output stream.
      * @param input_items a vector of pointers to the input items, one entry per input stream.
      * @param output_items a vector of pointers to the output items, one entry per output stream.
      * @return The number of items actually written to the output stream.
      */
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace DRM
} // namespace gr

#endif /* INCLUDED_DRM_WRITE_TO_FILE_INT_IMPL_H */

