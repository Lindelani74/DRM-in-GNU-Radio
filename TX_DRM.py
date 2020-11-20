#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# GNU Radio version: 3.8.1.0

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from PyQt5.QtCore import QObject, pyqtSlot
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import analog
from gnuradio import blocks
import numpy
from gnuradio import filter
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio.qtgui import Range, RangeWidget
import DRM
import osmosdr
import time

from gnuradio import qtgui

class TX_DRM(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "TX_DRM")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.tx_freq = tx_freq = 1000
        self.samp_rate_0 = samp_rate_0 = 113779
        self.samp_rate = samp_rate = 32000
        self.mod_freq = mod_freq = 20000
        self.items_int_write = items_int_write = 400
        self.in_val = in_val = 0
        self.chan = chan = 'MSC'
        self.audio_rate = audio_rate = 10000

        ##################################################
        # Blocks
        ##################################################
        self._tx_freq_range = Range(0, 16000, 500, 1000, 200)
        self._tx_freq_win = RangeWidget(self._tx_freq_range, self.set_tx_freq, 'Frequency of Transmitted signal', "counter_slider", float)
        self.top_grid_layout.addWidget(self._tx_freq_win)
        # Create the options list
        self._in_val_options = (0, 1, 2, )
        # Create the labels list
        self._in_val_labels = ('DRM', 'Wave', 'Sound', )
        # Create the combo box
        self._in_val_tool_bar = Qt.QToolBar(self)
        self._in_val_tool_bar.addWidget(Qt.QLabel('Input' + ": "))
        self._in_val_combo_box = Qt.QComboBox()
        self._in_val_tool_bar.addWidget(self._in_val_combo_box)
        for _label in self._in_val_labels: self._in_val_combo_box.addItem(_label)
        self._in_val_callback = lambda i: Qt.QMetaObject.invokeMethod(self._in_val_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._in_val_options.index(i)))
        self._in_val_callback(self.in_val)
        self._in_val_combo_box.currentIndexChanged.connect(
            lambda i: self.set_in_val(self._in_val_options[i]))
        # Create the radio buttons
        self.top_grid_layout.addWidget(self._in_val_tool_bar)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=100,
                decimation=1,
                taps=None,
                fractional_bw=None)
        self.qtgui_sink_x_0 = qtgui.sink_f(
            1024, #fftsize
            firdes.WIN_BLACKMAN_hARRIS, #wintype
            0, #fc
            samp_rate, #bw
            "", #name
            True, #plotfreq
            True, #plotwaterfall
            True, #plottime
            True #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.top_grid_layout.addWidget(self._qtgui_sink_x_0_win)
        self.osmosdr_sink_0 = osmosdr.sink(
            args="numchan=" + str(1) + " " + ""
        )
        self.osmosdr_sink_0.set_time_unknown_pps(osmosdr.time_spec_t())
        self.osmosdr_sink_0.set_sample_rate(samp_rate*100)
        self.osmosdr_sink_0.set_center_freq(90000000, 0)
        self.osmosdr_sink_0.set_freq_corr(0, 0)
        self.osmosdr_sink_0.set_gain(1000, 0)
        self.osmosdr_sink_0.set_if_gain(1, 0)
        self.osmosdr_sink_0.set_bb_gain(1, 0)
        self.osmosdr_sink_0.set_antenna('', 0)
        self.osmosdr_sink_0.set_bandwidth(100000, 0)
        # Create the options list
        self._chan_options = ('MSC', 'FAC', 'SDC', )
        # Create the labels list
        self._chan_labels = ('MSC', 'FAC', 'SDC', )
        # Create the combo box
        self._chan_tool_bar = Qt.QToolBar(self)
        self._chan_tool_bar.addWidget(Qt.QLabel('Channel' + ": "))
        self._chan_combo_box = Qt.QComboBox()
        self._chan_tool_bar.addWidget(self._chan_combo_box)
        for _label in self._chan_labels: self._chan_combo_box.addItem(_label)
        self._chan_callback = lambda i: Qt.QMetaObject.invokeMethod(self._chan_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._chan_options.index(i)))
        self._chan_callback(self.chan)
        self._chan_combo_box.currentIndexChanged.connect(
            lambda i: self.set_chan(self._chan_options[i]))
        # Create the radio buttons
        self.top_grid_layout.addWidget(self._chan_tool_bar)
        self.blocks_xor_xx_0_0_0 = blocks.xor_ii()
        self.blocks_xor_xx_0_0 = blocks.xor_ii()
        self.blocks_xor_xx_0 = blocks.xor_ii()
        self.blocks_wavfile_source_0 = blocks.wavfile_source('/home/lindelani/Downloads/02. John Redcorn - (SongsLover.com).wav', True)
        self.blocks_tag_debug_1_0_0 = blocks.tag_debug(gr.sizeof_gr_complex*1, 'OFDM Symbol', 'Symbol_Start')
        self.blocks_tag_debug_1_0_0.set_display(False)
        self.blocks_tag_debug_1_0 = blocks.tag_debug(gr.sizeof_gr_complex*1, 'Transmission Frame', 'TF_Start')
        self.blocks_tag_debug_1_0.set_display(False)
        self.blocks_selector_0 = blocks.selector(gr.sizeof_float*1,in_val,0)
        self.blocks_selector_0.set_enabled(True)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vff(1)
        self.blocks_multiply_xx_0 = blocks.multiply_vff(1)
        self.blocks_int_to_float_0_0_0 = blocks.int_to_float(1, 1)
        self.blocks_int_to_float_0_0 = blocks.int_to_float(1, 1)
        self.blocks_int_to_float_0 = blocks.int_to_float(1, 1)
        self.blocks_float_to_int_1_0_0 = blocks.float_to_int(1, 1)
        self.blocks_float_to_int_1_0 = blocks.float_to_int(1, 1)
        self.blocks_float_to_int_0 = blocks.float_to_int(1, 1)
        self.blocks_complex_to_real_0 = blocks.complex_to_real(1)
        self.blocks_complex_to_imag_0 = blocks.complex_to_imag(1)
        self.blocks_add_xx_0 = blocks.add_vff(1)
        self.analog_wfm_tx_0 = analog.wfm_tx(
        	audio_rate=samp_rate,
        	quad_rate=samp_rate,
        	tau=75e-6,
        	max_dev=5000,
        	fh=-1.0,
        )
        self.analog_sig_source_x_1 = analog.sig_source_f(samp_rate, analog.GR_SAW_WAVE, tx_freq, 1, 0, 0)
        self.analog_sig_source_x_0_0 = analog.sig_source_f(113779, analog.GR_SIN_WAVE, mod_freq, 1, 0, 0)
        self.analog_sig_source_x_0 = analog.sig_source_f(113779, analog.GR_COS_WAVE, mod_freq, 1, 0, 0)
        self.analog_sig_source_x_0.set_block_alias("Upconversion")
        self.analog_random_source_x_0 = blocks.vector_source_i(list(map(int, numpy.random.randint(0, 1, 1000))), True)
        self.DRM_Spectral_Efficiency_0 = DRM.Spectral_Efficiency()
        self.DRM_SDC_Source_0 = DRM.SDC_Source(0)
        self.DRM_QAM4_Mapper_0_0_0 = DRM.QAM4_Mapper()
        self.DRM_QAM4_Mapper_0_0 = DRM.QAM4_Mapper()
        self.DRM_QAM4_Mapper_0 = DRM.QAM4_Mapper()
        self.DRM_PRBS_Dispersal_1_0 = DRM.PRBS_Dispersal('SDC')
        self.DRM_PRBS_Dispersal_1 = DRM.PRBS_Dispersal('FAC')
        self.DRM_PRBS_Dispersal_0 = DRM.PRBS_Dispersal('MSC')
        self.DRM_IFFT_0 = DRM.IFFT()
        self.DRM_FAC_Source_0 = DRM.FAC_Source(0, 0, 1, 0)
        self.DRM_Cyclic_Prefix_Add_0 = DRM.Cyclic_Prefix_Add(28)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.DRM_Cyclic_Prefix_Add_0, 0), (self.blocks_complex_to_imag_0, 0))
        self.connect((self.DRM_Cyclic_Prefix_Add_0, 0), (self.blocks_complex_to_real_0, 0))
        self.connect((self.DRM_FAC_Source_0, 0), (self.blocks_xor_xx_0_0, 0))
        self.connect((self.DRM_IFFT_0, 0), (self.DRM_Cyclic_Prefix_Add_0, 0))
        self.connect((self.DRM_IFFT_0, 0), (self.blocks_tag_debug_1_0, 0))
        self.connect((self.DRM_IFFT_0, 0), (self.blocks_tag_debug_1_0_0, 0))
        self.connect((self.DRM_PRBS_Dispersal_0, 0), (self.blocks_float_to_int_0, 0))
        self.connect((self.DRM_PRBS_Dispersal_1, 0), (self.blocks_float_to_int_1_0, 0))
        self.connect((self.DRM_PRBS_Dispersal_1_0, 0), (self.blocks_float_to_int_1_0_0, 0))
        self.connect((self.DRM_QAM4_Mapper_0, 0), (self.DRM_IFFT_0, 0))
        self.connect((self.DRM_QAM4_Mapper_0_0, 0), (self.DRM_IFFT_0, 1))
        self.connect((self.DRM_QAM4_Mapper_0_0_0, 0), (self.DRM_IFFT_0, 2))
        self.connect((self.DRM_SDC_Source_0, 0), (self.blocks_xor_xx_0_0_0, 1))
        self.connect((self.analog_random_source_x_0, 0), (self.blocks_xor_xx_0, 1))
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_multiply_xx_0, 1))
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.connect((self.analog_sig_source_x_1, 0), (self.blocks_selector_0, 1))
        self.connect((self.analog_wfm_tx_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_selector_0, 2))
        self.connect((self.blocks_complex_to_imag_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.connect((self.blocks_complex_to_real_0, 0), (self.blocks_multiply_xx_0, 0))
        self.connect((self.blocks_float_to_int_0, 0), (self.blocks_xor_xx_0, 0))
        self.connect((self.blocks_float_to_int_1_0, 0), (self.blocks_xor_xx_0_0, 1))
        self.connect((self.blocks_float_to_int_1_0_0, 0), (self.blocks_xor_xx_0_0_0, 0))
        self.connect((self.blocks_int_to_float_0, 0), (self.DRM_QAM4_Mapper_0, 0))
        self.connect((self.blocks_int_to_float_0_0, 0), (self.DRM_QAM4_Mapper_0_0, 0))
        self.connect((self.blocks_int_to_float_0_0_0, 0), (self.DRM_QAM4_Mapper_0_0_0, 0))
        self.connect((self.blocks_multiply_xx_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_selector_0, 0), (self.analog_wfm_tx_0, 0))
        self.connect((self.blocks_selector_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_selector_0, 0))
        self.connect((self.blocks_xor_xx_0, 0), (self.blocks_int_to_float_0, 0))
        self.connect((self.blocks_xor_xx_0_0, 0), (self.blocks_int_to_float_0_0, 0))
        self.connect((self.blocks_xor_xx_0_0_0, 0), (self.blocks_int_to_float_0_0_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.DRM_Spectral_Efficiency_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.osmosdr_sink_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "TX_DRM")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_tx_freq(self):
        return self.tx_freq

    def set_tx_freq(self, tx_freq):
        self.tx_freq = tx_freq
        self.analog_sig_source_x_1.set_frequency(self.tx_freq)

    def get_samp_rate_0(self):
        return self.samp_rate_0

    def set_samp_rate_0(self, samp_rate_0):
        self.samp_rate_0 = samp_rate_0

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_sig_source_x_1.set_sampling_freq(self.samp_rate)
        self.osmosdr_sink_0.set_sample_rate(self.samp_rate*100)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)

    def get_mod_freq(self):
        return self.mod_freq

    def set_mod_freq(self, mod_freq):
        self.mod_freq = mod_freq
        self.analog_sig_source_x_0.set_frequency(self.mod_freq)
        self.analog_sig_source_x_0_0.set_frequency(self.mod_freq)

    def get_items_int_write(self):
        return self.items_int_write

    def set_items_int_write(self, items_int_write):
        self.items_int_write = items_int_write

    def get_in_val(self):
        return self.in_val

    def set_in_val(self, in_val):
        self.in_val = in_val
        self._in_val_callback(self.in_val)
        self.blocks_selector_0.set_input_index(self.in_val)

    def get_chan(self):
        return self.chan

    def set_chan(self, chan):
        self.chan = chan
        self._chan_callback(self.chan)

    def get_audio_rate(self):
        return self.audio_rate

    def set_audio_rate(self, audio_rate):
        self.audio_rate = audio_rate





def main(top_block_cls=TX_DRM, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    def quitting():
        tb.stop()
        tb.wait()

    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()

if __name__ == '__main__':
    main()
