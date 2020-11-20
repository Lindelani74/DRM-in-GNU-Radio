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

#ifndef INCLUDED_DRM_IFFT_IMPL_H
#define INCLUDED_DRM_IFFT_IMPL_H

#include <DRM/IFFT.h>

namespace gr {
  namespace DRM {

	/**
 	*  The symbol constructor and IFFT class.
 	*/
    class IFFT_impl : public IFFT
    {
     private:
      // Nothing to declare in this block.

     public:
     /*!
      * The private constructor. 
      *
      */
      IFFT_impl();
      /*!
      * Our virtual destructor.
      *
      */
      ~IFFT_impl();

      /*!
      * This function communicates to the scheduler the ratio of input items to output items needed for a single call to general_work()
      *
      * @param noutput_items the number of input items required for each call to general_work().
      * @param ninput_items_required the number of output items required for each call to general_work()
      * @see general_work()
      */
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);
      
	/*!
      * This function is where the DSP algorithm is implemented. It produces output items from processing the input items.
      *
      * @param noutput_items the number of items that may be produced for this current call to general_work() which represents the number of output items to write for each output stream.
      * @param ninput_items the number of input items available on each input stream.
      * @param input_items a vector of pointers to the input items, one entry per input stream.
      * @param output_items a vector of pointers to the output items, one entry per output stream.
      * @return The number of items actually written to the output stream.
      */
      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
           
      /*!
      * This function is called in general_work() when the received QAM cell is coming from the SDC.
      *
      * @param input_pointer the pointer to the input item being considered on the SDC input stream.
      * @return The number of input SDC cells consumed.
      * @see general_work()
      */
      int SDC_init(const gr_complex *input_pointer);
       /*!
      * This function is called in general_work() when the received QAM cell is coming from the MSC.
      *
      * @param input_pointer the pointer to the input item being considered on the MSC input stream.
      * @return The number of input MSC cells consumed.
      * @see general_work()
      */
      int MSC_init(const gr_complex *input_pointer);
      /*!
      * This function is called in general_work() when the received QAM cell is coming from the FAC.
      *
      * @param symbol the symbol number of the symbol currently being constructed by the algorithm.
      * @param input_pointer the pointer to the input item being considered on the FAC input stream.
      * @return The number of input FAC cells consumed.
      * @see general_work()
      */
      int FAC_init(int symbol, const gr_complex *input_pointer);
      /*!
      * This function is called in general_work() when the carrier of a symbol requires a Gain Reference pilot cell.
      *
      * @param symbol the symbol number of the symbol currently being constructed by the algorithm.
      * @see general_work()
      */
      void gain_cells_init(int symbol);
      /*!
      * This function is called in general_work() when the carrier of a symbol requires an AFS pilot cell.
      *
      * @see general_work()
      */
      void afs_cells_init();
      /*!
      * This function is called in general_work() when the carrier of a symbol requires a Time Reference pilot cell.
      *
      * @see general_work()
      */
      void time_cells_init();
      

    };

  } // namespace DRM
} // namespace gr

#endif /* INCLUDED_DRM_IFFT_IMPL_H */

