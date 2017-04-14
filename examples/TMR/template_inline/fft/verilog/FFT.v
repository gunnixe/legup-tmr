/* 64 Point FFT

State 1: Accept inputs 1 per clock cycle
State 2: Time Decimation, swap input values in place with index obtained by reversing the last 6 bits of current index
States 3-7: Each is a stage of the butterfly computation, 32 parallel butterflies per stage
Stage 8: Output values 1 per clock cycle

Simulation is currently performed using Quartus Simulator because Modelsim produces the error 
"Enhanced FOR loop is not enabled for verilog". */

module FFT (clock, in_real, in_imag, out_real, out_imag);

  integer m, i, temp_count, sin_index; // indices for nested for loops

  input clock;
  input [15:0] in_real;
  input [15:0] in_imag;
  output reg [15:0] out_real;
  output reg [15:0] out_imag;
    
  reg [3:0] state; //For FSM
  reg [15:0] count;
  reg [15:0] state_count;
  reg [15:0] Real [63:0];
  reg [15:0] Imag [63:0];
  reg finished;
  
  reg [15:0] sin_lookup [125:0];	// // Can be placed in RAM (.mif file)
  
  reg [15:0] mcnd1 [31:0]; // multiplicands
  reg [15:0] mcnd2 [31:0];
  reg [15:0] mcnd3 [31:0];
  reg [15:0] mcnd4 [31:0];
  wire [17:0] prod1 [63:0]; // products
  wire [17:0] prod2 [63:0];
  wire [17:0] prod3 [63:0];
  wire [17:0] prod4 [63:0];
  reg [15:0] temp1 [63:0]; // products following bit-shifting
  reg [15:0] temp2 [63:0];
  reg [15:0] temp3 [63:0];
  reg [15:0] temp4 [63:0];
  reg [15:0] qr [63:0]; // more temporary registers
  reg [15:0] qi [63:0];
  reg [15:0] tr [63:0];
  reg [15:0] ti [63:0];
 
  initial
  begin
    finished <= 0;
    state <= 0;
    count <= 0;
    state_count <= 0;
 	
    sin_lookup[0] <= 32767; // These are the sin values needed in order of usage. Note that they are scaled by 
    sin_lookup[1] <= 0;	// 2^15 = 32768 to avoid floating point numbers, e.g. 32767 corresponds to sin(pi/2)=1.
    sin_lookup[2] <= 32767;
    sin_lookup[3] <= 0;
    sin_lookup[4] <= 0;
    sin_lookup[5] <= 32767;
    sin_lookup[6] <= 32767;
    sin_lookup[7] <= 0;
    sin_lookup[8] <= 23169;
    sin_lookup[9] <= 23169;
    sin_lookup[10] <= 0;
    sin_lookup[11] <= 32767;
    sin_lookup[12] <= -23169;
    sin_lookup[13] <= 23169;
    sin_lookup[14] <= 32767;
    sin_lookup[15] <= 0;
    sin_lookup[16] <= 30272;
    sin_lookup[17] <= 12539;
    sin_lookup[18] <= 23169;
    sin_lookup[19] <= 23169;
    sin_lookup[20] <= 12539;
    sin_lookup[21] <= 30272;
    sin_lookup[22] <= 0;
    sin_lookup[23] <= 32767;
    sin_lookup[24] <= -12539;
    sin_lookup[25] <= 30272;
    sin_lookup[26] <= -23169;
    sin_lookup[27] <= 23169;
    sin_lookup[28] <= -30272;
    sin_lookup[29] <= 12539;
    sin_lookup[30] <= 32767;
    sin_lookup[31] <= 0;
    sin_lookup[32] <= 32137;
    sin_lookup[33] <= 6392;
    sin_lookup[34] <= 30272;
    sin_lookup[35] <= 12539;
    sin_lookup[36] <= 27244;
    sin_lookup[37] <= 18204;
    sin_lookup[38] <= 23169;
    sin_lookup[39] <= 23169;
    sin_lookup[40] <= 18204;
    sin_lookup[41] <= 27244;
    sin_lookup[42] <= 12539;
    sin_lookup[43] <= 30272;
    sin_lookup[44] <= 6392;
    sin_lookup[45] <= 32137;
    sin_lookup[46] <= 0;
    sin_lookup[47] <= 32767;
    sin_lookup[48] <= -6392;
    sin_lookup[49] <= 32137;
    sin_lookup[50] <= -12539;
    sin_lookup[51] <= 30272;
    sin_lookup[52] <= -18204;
    sin_lookup[53] <= 27244;
    sin_lookup[54] <= -23169;
    sin_lookup[55] <= 23169;
    sin_lookup[56] <= -27244;
    sin_lookup[57] <= 18204;
    sin_lookup[58] <= -30272;
    sin_lookup[59] <= 12539;
    sin_lookup[60] <= -32137;
    sin_lookup[61] <= 6392;
    sin_lookup[62] <= 32767;
    sin_lookup[63] <= 0;
    sin_lookup[64] <= 32609;
    sin_lookup[65] <= 3211;
    sin_lookup[66] <= 32137;
    sin_lookup[67] <= 6392;
    sin_lookup[68] <= 31356;
    sin_lookup[69] <= 9511;
    sin_lookup[70] <= 30272;
    sin_lookup[71] <= 12539;
    sin_lookup[72] <= 28897;
    sin_lookup[73] <= 15446;
    sin_lookup[74] <= 27244;
    sin_lookup[75] <= 18204;
    sin_lookup[76] <= 25329;
    sin_lookup[77] <= 20787;
    sin_lookup[78] <= 23169;
    sin_lookup[79] <= 23169;
    sin_lookup[80] <= 20787;
    sin_lookup[81] <= 25329;
    sin_lookup[82] <= 18204;
    sin_lookup[83] <= 27244;
    sin_lookup[84] <= 15446;
    sin_lookup[85] <= 28897;
    sin_lookup[86] <= 12539;
    sin_lookup[87] <= 30272;
    sin_lookup[88] <= 9511;
    sin_lookup[89] <= 31356;
    sin_lookup[90] <= 6392;
    sin_lookup[91] <= 32137;
    sin_lookup[92] <= 3211;
    sin_lookup[93] <= 32609;
    sin_lookup[94] <= 0;
    sin_lookup[95] <= 32767;
    sin_lookup[96] <= -3211;
    sin_lookup[97] <= 32609;
    sin_lookup[98] <= -6392;
    sin_lookup[99] <= 32137;
    sin_lookup[100] <= -9511;
    sin_lookup[101] <= 31356;
    sin_lookup[102] <= -12539;
    sin_lookup[103] <= 30272;
    sin_lookup[104] <= -15446;
    sin_lookup[105] <= 28897;
    sin_lookup[106] <= -18204;
    sin_lookup[107] <= 27244;
    sin_lookup[108] <= -20787;
    sin_lookup[109] <= 25329;
    sin_lookup[110] <= -23169;
    sin_lookup[111] <= 23169;
    sin_lookup[112] <= -25329;
    sin_lookup[113] <= 20787;
    sin_lookup[114] <= -27244;
    sin_lookup[115] <= 18204;
    sin_lookup[116] <= -28897;
    sin_lookup[117] <= 15446;
    sin_lookup[118] <= -30272;
    sin_lookup[119] <= 12539;
    sin_lookup[120] <= -31356;
    sin_lookup[121] <= 9511;
    sin_lookup[122] <= -32137;
    sin_lookup[123] <= 6392;
    sin_lookup[124] <= -32609;
    sin_lookup[125] <= 3211;
  end

  
  genvar gen_count;

  // Ensures the 128 multipliers are always re-used
  // Multiplication setting can be modified. By default, latency is 16 clock cycles
  generate
    for (gen_count=0; gen_count<32; gen_count=gen_count+1)
    begin : loop
      mult M1 (clock, mcnd1[gen_count], mcnd2[gen_count], prod1[gen_count]);
      mult M2 (clock, mcnd3[gen_count], mcnd4[gen_count], prod2[gen_count]);
      mult M3 (clock, mcnd1[gen_count], mcnd4[gen_count], prod3[gen_count]);
      mult M4 (clock, mcnd3[gen_count], mcnd2[gen_count], prod4[gen_count]);
    end
  endgenerate
  
  always @ (posedge clock)
  begin
  // STATE 0: Every clock cycle, a new input is inserted into the shift register
    if (state == 0)
    begin
      for (i=63; i > 0; i = i - 1)
      begin
        Real[i] <= Real[i-1];
        Imag[i] <= Imag[i-1];
      end

      Real[0] <= count;//in_real;		// Temporary
      Imag[0] <= 0;//in_imag;			// Temporary      

      if (count == 63) // Mux: if all bits of count match 63, then state <- 1 and count <- 0, else count <- count + 1
      begin			   // (mux is connected to an adder)
        state <= 1;	  
        count <= 0;	  
      end
      else 
      begin
        count <= count + 1;
      end
    end

  // STATE 1: One clock cycle, time decimation (required swaps are stored)
    else if ( state == 1 )
    begin
      Real[0] <= Real[63];
      Imag[0] <= Imag[63];
      Real[1] <= Real[31];
      Imag[1] <= Imag[31];
      Real[2] <= Real[47];
      Imag[2] <= Imag[47];
      Real[3] <= Real[15];
      Imag[3] <= Imag[15];
      Real[4] <= Real[55];
      Imag[4] <= Imag[55];
      Real[5] <= Real[23];
      Imag[5] <= Imag[23];
      Real[6] <= Real[39];
      Imag[6] <= Imag[39];
      Real[7] <= Real[7];
      Imag[7] <= Imag[7];
      Real[8] <= Real[59];
      Imag[8] <= Imag[59];
      Real[9] <= Real[27];
      Imag[9] <= Imag[27];
      Real[10] <= Real[43];
      Imag[10] <= Imag[43];
      Real[11] <= Real[11];
      Imag[11] <= Imag[11];
      Real[12] <= Real[51];
      Imag[12] <= Imag[51];
      Real[13] <= Real[19];
      Imag[13] <= Imag[19];
      Real[14] <= Real[35];
      Imag[14] <= Imag[35];
      Real[15] <= Real[3];
      Imag[15] <= Imag[3];
      Real[16] <= Real[61];
      Imag[16] <= Imag[61];
      Real[17] <= Real[29];
      Imag[17] <= Imag[29];
      Real[18] <= Real[45];
      Imag[18] <= Imag[45];
      Real[19] <= Real[13];
      Imag[19] <= Imag[13];
      Real[20] <= Real[53];
      Imag[20] <= Imag[53];
      Real[21] <= Real[21];
      Imag[21] <= Imag[21];
      Real[22] <= Real[37];
      Imag[22] <= Imag[37];
      Real[23] <= Real[5];
      Imag[23] <= Imag[5];
      Real[24] <= Real[57];
      Imag[24] <= Imag[57];
      Real[25] <= Real[25];
      Imag[25] <= Imag[25];
      Real[26] <= Real[41];
      Imag[26] <= Imag[41];
      Real[27] <= Real[9];
      Imag[27] <= Imag[9];
      Real[28] <= Real[49];
      Imag[28] <= Imag[49];
      Real[29] <= Real[17];
      Imag[29] <= Imag[17];
      Real[30] <= Real[33];
      Imag[30] <= Imag[33];
      Real[31] <= Real[1];
      Imag[31] <= Imag[1];
      Real[32] <= Real[62];
      Imag[32] <= Imag[62];
      Real[33] <= Real[30];
      Imag[33] <= Imag[30];
      Real[34] <= Real[46];
      Imag[34] <= Imag[46];
      Real[35] <= Real[14];
      Imag[35] <= Imag[14];
      Real[36] <= Real[54];
      Imag[36] <= Imag[54];
      Real[37] <= Real[22];
      Imag[37] <= Imag[22];
      Real[38] <= Real[38];
      Imag[38] <= Imag[38];
      Real[39] <= Real[6];
      Imag[39] <= Imag[6];
      Real[40] <= Real[58];
      Imag[40] <= Imag[58];
      Real[41] <= Real[26];
      Imag[41] <= Imag[26];
      Real[42] <= Real[42];
      Imag[42] <= Imag[42];
      Real[43] <= Real[10];
      Imag[43] <= Imag[10];
      Real[44] <= Real[50];
      Imag[44] <= Imag[50];
      Real[45] <= Real[18];
      Imag[45] <= Imag[18];
      Real[46] <= Real[34];
      Imag[46] <= Imag[34];
      Real[47] <= Real[2];
      Imag[47] <= Imag[2];
      Real[48] <= Real[60];
      Imag[48] <= Imag[60];
      Real[49] <= Real[28];
      Imag[49] <= Imag[28];
      Real[50] <= Real[44];
      Imag[50] <= Imag[44];
      Real[51] <= Real[12];
      Imag[51] <= Imag[12];
      Real[53] <= Real[20];
      Imag[53] <= Imag[20];
      Real[54] <= Real[36];
      Imag[54] <= Imag[36];
      Real[55] <= Real[4];
      Imag[55] <= Imag[4];
      Real[56] <= Real[56];
      Imag[56] <= Imag[56];
      Real[57] <= Real[24];
      Imag[57] <= Imag[24];
      Real[58] <= Real[40];
      Imag[58] <= Imag[40];
      Real[59] <= Real[8];
      Imag[59] <= Imag[8];
      Real[60] <= Real[48];
      Imag[60] <= Imag[48];
      Real[61] <= Real[16];
      Imag[61] <= Imag[16];
      Real[62] <= Real[32];
      Imag[62] <= Imag[32];
      Real[63] <= Real[0];
      Imag[63] <= Imag[0];
	  state <= 2;
    end

	// State 2: First "recursive" (non-dependent) stage of the butterfly: 32 butterflies in parellel
    else if ( state == 2 )
	begin
      for (i=0; i<64; i=i+2) // Computation is simplified for this state as sin coefficient is always 1 or 0
      begin
        Imag[i+1] <= Imag[i] - Imag[i+1];
        Real[i+1] <= Real[i] - Real[i+1];
        Imag[i] <= Imag[i] + Imag[i+1];
        Real[i] <= Real[i] + Real[i+1];
      end
      state <= 3;
    end // state 2

    else if ( state == 3 ) // second of six butterflu stages, 32 more parallel butterflies
	begin
      for (i=0; i<64; i=i+4) // Computation is simplified for this state also (sin = 1 or 0)
      begin
        Imag[i+2] <= Imag[i] - Imag[i+2];
        Real[i+2] <= Real[i] - Real[i+2];
        Imag[i] <= Imag[i] + Imag[i+2];
        Real[i] <= Real[i] + Real[i+2];
      end

      for (i=1; i<64; i=i+4)
      begin
        Imag[i+2] <= Imag[i] + Real[i+2];
        Real[i+2] <= Real[i] - Imag[i+2];
        Imag[i] <= Imag[i] - Real[i+2];
        Real[i] <= Real[i] + Imag[i+2];
      end

      state <= 4;
    end // state 3
    
    else if ( state == 4 ) // First state for which butterflies are performed without simplification
	begin
      temp_count = 0; // integer acting as array index 
		  // The following 2 nested for loops are the same as in the C implementation (see fft.c). Note that
		  // the limits are known constants hence these loops can be unrolled manually, and each butterfly
		  // should be performed in parallel. For loops are used only to save space
      for (m=0, sin_index=6; m<4; m=m+1, sin_index=sin_index+2)		// (Next state: m<8 and sin=14 (then 30,62))
      begin
        for (i=m; i<64; i=i+8, temp_count = temp_count + 1)	// (Next state: i = i+16)
        begin	// Inner for loop: the butterfly computation is performed
          mcnd1[temp_count] <= sin_lookup[sin_index]; // 1st clock cycle in this state: set multiplicands for
          mcnd2[temp_count] <= Imag[i+4];							// the mult module
          mcnd3[temp_count] <= sin_lookup[sin_index+1];
          mcnd4[temp_count] <= Real[i+4];					// Next state: set these indices to i+8

		  temp1[temp_count] <= (prod1[temp_count] >> 1) + prod1[temp_count][0]; // multiplies have latency of 16 
		  temp2[temp_count] <= (prod2[temp_count] >> 1) + prod2[temp_count][0]; // clock cycles, so these bit shifts
		  temp3[temp_count] <= (prod3[temp_count] >> 1) + prod3[temp_count][0]; // produce correct values only on
		  temp4[temp_count] <= (prod4[temp_count] >> 1) + prod4[temp_count][0]; // the 17th clock cycle
          
          ti[temp_count] <= temp1[temp_count] - temp2[temp_count]; // 18th clock cycle
          tr[temp_count] <= temp3[temp_count] + temp4[temp_count];		            
          qi[temp_count] <= Imag[i]; // Note that because the FFT is in place, i.e. values in Real / Imag are
          qr[temp_count] <= Real[i]; // repeatedly stored and accessed, this assignment is the critical path
																		 // of the circuit (a large mux in front of the Real and Imag registers).

			// The critical path can be removed by storing each set of butterflies in new registers rather than in
			// place, but this would increase area.

   		  if (state_count > 18) // 19th clock cycle, store new butterflied values in-place
		  begin
            Imag[i+4] <= qi[temp_count] - ti[temp_count];
            Real[i+4] <= qr[temp_count] - tr[temp_count];
            Imag[i] <= qi[temp_count] + ti[temp_count];
            Real[i] <= qr[temp_count] + tr[temp_count];
          end
        end
      end 
    
	  if (state_count == 20) // 20th clock cycle the values in Real and Imag reflect the new butterflies, hence
	  begin									 // move to the next state
        state_count <= 0;
        state <= 5;
      end
      else 
      begin
		state_count <= state_count + 1;
	  end
    end // state 4

    else if ( state == 5 )
	begin
      temp_count = 0;
      for (m=0, sin_index=14; m<8; m=m+1, sin_index=sin_index+2)				// Next, m < 16, sin=30,62
      begin
        for (i=m; i<64; i=i+16, temp_count = temp_count + 1)			// Next, i = i+32
        begin
          mcnd1[temp_count] <= sin_lookup[sin_index];
          mcnd2[temp_count] <= Imag[i+8];							// Change to i + 16
          mcnd3[temp_count] <= sin_lookup[sin_index+1];
          mcnd4[temp_count] <= Real[i+8];

		  temp1[temp_count] <= (prod1[temp_count] >> 1) + prod1[temp_count][0];
		  temp2[temp_count] <= (prod2[temp_count] >> 1) + prod2[temp_count][0];
		  temp3[temp_count] <= (prod3[temp_count] >> 1) + prod3[temp_count][0];
		  temp4[temp_count] <= (prod4[temp_count] >> 1) + prod4[temp_count][0];
          
          ti[temp_count] <= temp1[temp_count] - temp2[temp_count];
          tr[temp_count] <= temp3[temp_count] + temp4[temp_count];		            
          qi[temp_count] <= Imag[i];
          qr[temp_count] <= Real[i];

   		  if (state_count > 18)
		  begin
            Imag[i+8] <= qi[temp_count] - ti[temp_count];
            Real[i+8] <= qr[temp_count] - tr[temp_count];
            Imag[i] <= qi[temp_count] + ti[temp_count];
            Real[i] <= qr[temp_count] + tr[temp_count];
          end
        end
      end
    
	  if (state_count == 20)
	  begin
        state_count <= 0;
        state <= 6;
      end
      else 
      begin
		state_count <= state_count + 1;
	  end
    end // state 5

    else if ( state == 6 )
	begin
      temp_count = 0;
      for (m=0, sin_index=30; m<16; m=m+1, sin_index=sin_index+2)				// Next, m < 32, sin=62
      begin
        for (i=m; i<64; i=i+32, temp_count = temp_count + 1)			// Next, i = i+64
        begin
          mcnd1[temp_count] <= sin_lookup[sin_index];
          mcnd2[temp_count] <= Imag[i+16];							// Change to i + 32
          mcnd3[temp_count] <= sin_lookup[sin_index+1];
          mcnd4[temp_count] <= Real[i+16];

		  temp1[temp_count] <= (prod1[temp_count] >> 1) + prod1[temp_count][0];
		  temp2[temp_count] <= (prod2[temp_count] >> 1) + prod2[temp_count][0];
		  temp3[temp_count] <= (prod3[temp_count] >> 1) + prod3[temp_count][0];
		  temp4[temp_count] <= (prod4[temp_count] >> 1) + prod4[temp_count][0];
          
          ti[temp_count] <= temp1[temp_count] - temp2[temp_count];
          tr[temp_count] <= temp3[temp_count] + temp4[temp_count];		            
          qi[temp_count] <= Imag[i];
          qr[temp_count] <= Real[i];

   		  if (state_count > 18)
		  begin
            Imag[i+16] <= qi[temp_count] - ti[temp_count];
            Real[i+16] <= qr[temp_count] - tr[temp_count];
            Imag[i] <= qi[temp_count] + ti[temp_count];
            Real[i] <= qr[temp_count] + tr[temp_count];
          end
        end
      end
    
	  if (state_count == 20)
	  begin
        state_count <= 0;
        state <= 7;
      end
      else 
      begin
		state_count <= state_count + 1;
	  end
    end // state 6

    else if ( state == 7 )
	begin
      temp_count = 0;
      for (m=0, sin_index=62; m<32; m=m+1, sin_index=sin_index+2)
      begin
        for (i=m; i<64; i=i+64, temp_count = temp_count + 1)
        begin
          mcnd1[temp_count] <= sin_lookup[sin_index];
          mcnd2[temp_count] <= Imag[i+32];
          mcnd3[temp_count] <= sin_lookup[sin_index+1];
          mcnd4[temp_count] <= Real[i+32];

		  temp1[temp_count] <= (prod1[temp_count] >> 1) + prod1[temp_count][0];
		  temp2[temp_count] <= (prod2[temp_count] >> 1) + prod2[temp_count][0];
		  temp3[temp_count] <= (prod3[temp_count] >> 1) + prod3[temp_count][0];
		  temp4[temp_count] <= (prod4[temp_count] >> 1) + prod4[temp_count][0];
          
          ti[temp_count] <= temp1[temp_count] - temp2[temp_count];
          tr[temp_count] <= temp3[temp_count] + temp4[temp_count];		            
          qi[temp_count] <= Imag[i];
          qr[temp_count] <= Real[i];

   		  if (state_count > 18)
		  begin
            Imag[i+32] <= qi[temp_count] - ti[temp_count];
            Real[i+32] <= qr[temp_count] - tr[temp_count];
            Imag[i] <= qi[temp_count] + ti[temp_count];
            Real[i] <= qr[temp_count] + tr[temp_count];
          end
        end
      end
    
	  if (state_count == 20)
	  begin
        state_count <= 0;
        state <= 8;
      end
      else 
      begin
		state_count <= state_count + 1;
	  end
    end // state 7

    // State 8: Computations finished, one output value for each of 64 clock cycles
    else if ( state == 8 )
    begin
	  out_real <= Real[count];
	  out_imag <= Imag[count];
	  count <= count + 1;
	  if (count == 64)
	  begin
	    state <= 9; // From now on nothing happens, but can return to state 0 for new input
	    finished <= 1;
	  end
	end
  end   // always block
endmodule
