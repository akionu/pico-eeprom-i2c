#include "i2c_write_nors_blocking.h"

// write without Repeated Start condition
int i2c_write_nors_blocking(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len) {
	
	// send without a Repeated Start conditions between chunks
	// code below originally comes from "pico-sdk/src/rp2_common/hardware_i2c/i2c.c"
	// which is under BSD 3-Clause "New" or "Reviced" License:
	/*
	Copyright 2020 (c) 2020 Raspberry Pi (Trading) Ltd.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
	following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
	disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products
	derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	*/
	bool abort = false;
	int byte_ctr;
    for (byte_ctr = 0; byte_ctr < len; ++byte_ctr) {
        bool first = byte_ctr == 0;
        bool last = byte_ctr == len - 1;

        i2c->hw->data_cmd =
                bool_to_bit(last) << I2C_IC_DATA_CMD_STOP_LSB |
                *src++;

        // Wait until the transmission of the address/data from the internal
        // shift register has completed. For this to function correctly, the
        // TX_EMPTY_CTRL flag in IC_CON must be set. The TX_EMPTY_CTRL flag
        // was set in i2c_init.
        do {
            tight_loop_contents();
        } while (!(i2c->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_TX_EMPTY_BITS));

        // If there was a timeout, don't attempt to do anything else.
        bool abort_reason = i2c->hw->tx_abrt_source;
        if (abort_reason) {
            // Note clearing the abort flag also clears the reason, and
            // this instance of flag is clear-on-read! Note also the
            // IC_CLR_TX_ABRT register always reads as 0.
            i2c->hw->clr_tx_abrt;
            abort = true;
        }

        if (abort || last) {
			// If the transaction was aborted or if it completed
			// successfully wait until the STOP condition has occured.

			// TODO Could there be an abort while waiting for the STOP
			// condition here? If so, additional code would be needed here
			// to take care of the abort.
			do {
				tight_loop_contents();
			} while (!(i2c->hw->raw_intr_stat & I2C_IC_RAW_INTR_STAT_STOP_DET_BITS));
        }

        // Note the hardware issues a STOP automatically on an abort condition.
        // Note also the hardware clears RX FIFO as well as TX on abort,
        // because we set hwparam IC_AVOID_RX_FIFO_FLUSH_ON_TX_ABRT to 0.
        if (abort) return PICO_ERROR_GENERIC;
    }

	// do not send "Restart"
	i2c->restart_on_next = false;
	// code originally from pico-sdk until here

	return len;
}