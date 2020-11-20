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

#ifndef INCLUDED_DRM_QAM4_MAPPER_IMPL_H
#define INCLUDED_DRM_QAM4_MAPPER_IMPL_H

#include <DRM/QAM4_Mapper.h>

namespace gr {
  namespace DRM {
  
  	/**
 	*  The QAM4 Mapper class.
 	*/
    class QAM4_Mapper_impl : public QAM4_Mapper
    {
     private:
      // Nothing to declare in this block.

     public:
     /*!
      * The private constructor. 
      *
      */
      QAM4_Mapper_impl();
      /*!
      * Our virtual destructor.
      *
      */
      ~QAM4_Mapper_impl();

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

#endif /* INCLUDED_DRM_QAM4_MAPPER_IMPL_H */

