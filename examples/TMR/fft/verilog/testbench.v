`timescale 1 ns / 1 ns // Check
module main_tb ();
  reg  clock;
  reg finished; // In addition to setting state = 9 at the end, set finished (1 bit reg) = 1
  reg [15:0] in_real;
  reg [15:0] in_imag;
  reg [15:0] out_real;
  reg [15:0] out_imag;
  reg [15:0] r[63:0];
  reg [15:0] i[63:0];
  reg [8:0] count;

  top top_inst (
    .clock(clock),
  	.in_real(in_real)
  	.in_imag(in_imag)
	  .finished(finished)
  	.out_real(out_real)
  	.out_imag(out_imag)
  );

  initial 
    clock <= #0 0;

  always @(*)
  begin
    clock <= #1 ~clock;
  end

  always @ (posedge clock)
  begin
    if (count < 64)
	  begin
	    in_real <= count; // set to any input
      in_imag <= 0;
  	  count <= count + 1;
  	end
  end  
  
  always @ (finish)
  begin
    if (finish == 1) 
	  begin
//        $display("At t=%t clk=%b finish=%b return_val=%d", $time, clk, finish, return_val);
      $finish;
    end
  end
endmodule 

