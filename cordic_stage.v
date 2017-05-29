`timescale 1ns/100ps

module cordic_stage(clk, x_i, y_i, z_i, x_o, y_o, z_o, arctan);
  
  parameter z_width = 12; 
  parameter iter_width = 17;  	// iteration width
  parameter i=0;  		// power of two shift amount

  input signed [z_width:0] arctan;
  
  input clk;
  input signed [iter_width:0] x_i, y_i;
  input signed [z_width:0] z_i;
  output [iter_width:0] x_o, y_o;
  output [z_width:0] z_o;
  
  reg d;
  reg signed [iter_width:0] x_o, x_next;
  reg signed [iter_width:0] y_o, y_next;
  reg signed [z_width:0] z_o, z_next;
  
  always @ (posedge clk)
    begin
      x_o <= x_next;
      y_o <= y_next;
      z_o <= z_next; 
    end

  always @(*)
    begin    

      d = (z_i[11] == 0) ? 0 : 1;

      if(d == 0) begin
        x_next = x_i - (y_i >>> i);
        y_next = y_i + (x_i >>> i);
        z_next = z_i - arctan;
      end

      else begin
        x_next = x_i + (y_i >>> i);
        y_next = y_i - (x_i >>> i);
        z_next = z_i + arctan;
      end
      
  end

endmodule
