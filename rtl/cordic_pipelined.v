`timescale 1ns/100ps

module cordic_pipelined(clk, z0, xn, yn);
  
  parameter z_width = 12; 
  parameter iter_width = 17;  // iteration width

  input clk;
  input signed [z_width:0] z0;
  output reg signed [iter_width:0] xn, yn;

  parameter N = 10; // number of iterations
  
  reg [(N-1):0] d;
  wire signed [iter_width:0] x [N:0];
  wire signed [iter_width:0] y [N:0];
  wire signed [z_width:0] z [N:0];
  wire signed [z_width:0] current_angle;

  assign x[0] = 'b01_0011_0110_1110_1001;  // 0.60725 in binary
  assign y[0] = 0;

  assign current_angle = z0 - 'h0A0;

  always @ (current_angle, x[N], y[N]) begin
   	
    // cordic only works in first and fourth quadrant

    if (((current_angle[z_width]==1) && (current_angle < 'h1A60)) || 
   	((current_angle[z_width]==0) && (current_angle > 'h05A0))) begin   // if  < -90 ||  > +90
	//$display("at 1");

  	// flip the sign for second or third quadrant using two-complement rule
  	xn = ~( x[N]-1 ); // xn = cos(z0)
	yn = ~( y[N]-1 ); // yn = sin(z0)  
    end

    else begin		// z0 is already in first and fourth quadrant
	xn = x[N];	// xn = cos(z0)  
	yn = y[N];	// yn = sin(z0)  
    end
  end

  always @ (z0) begin
    if (((z0[z_width]==1) && (z0 < 'h1A60)) || 
   	((z0[z_width]==0) && (z0 > 'h05A0))) begin   // if  < -90 ||  > +90

	if (z0[z_width] == 1) begin 			// if z0 < 0
	    z[0] = z0 + 'hB40; 	  // third quadrant, so add 180 degrees
	    //$display("at 2");
	end
	else begin
	    z[0] = z0 - 'hB40;	  // second quadrant, so subtract 180 degrees
	    //$display("at 3");
	end
    end

    else begin
	z[0] = z0; 
    end
    //$display("z[0] = ", z[0]);
  end
  
  wire signed [z_width:0] arctan [(N-1):0];
    
  assign arctan[0] = 'b0_0010_1101_0000; // 45.000;
  assign arctan[1] = 'b0_0001_1010_1001; // 26.565;
  assign arctan[2] = 'b0_0000_1110_0000; // 14.036;
  assign arctan[3] = 'b0_0000_0111_0010; // 7.1250;
  assign arctan[4] = 'b0_0000_0011_1001; // 3.5763;
  assign arctan[5] = 'b0_0000_0001_1100; // 1.7899;
  assign arctan[6] = 'b0_0000_0000_1110; // 0.8952;
  assign arctan[7] = 'b0_0000_0000_0111; // 0.4476;
  assign arctan[8] = 'b0_0000_0000_0011; // 0.2238;
  assign arctan[9] = 'b0_0000_0000_0001; // 0.1119;

  genvar i;
  generate
    for(i=0; i<N; i=i+1)
      begin: stage_generate
        cordic_stage #(.i(i)) stage_i
        (
          .clk(clk), 
          .x_i(x[i]), .x_o(x[i+1]), 
          .y_i(y[i]), .y_o(y[i+1]), 
          .z_i(z[i]), .z_o(z[i+1]),
          .arctan(arctan[i])
        );

      end
  endgenerate
     
endmodule
