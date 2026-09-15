`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/15/2026 03:27:14 PM
// Design Name: 
// Module Name: CS25B012_session6_p1
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module CS25B012_session6_p1(
    input A, input B, input C, input D, input E,
    output F
);

    wire w1, w2, w3, w4;
    wire nA, nB, nE;

    
    not(nA, A);
    not(nB, B);
    not(nE, E);

   
    and(w1, A, B, C, D);
    and(w2, nB, C, D, E);
    and(w3, B, C, nE);
    and(w4, nA, nB);

    
    or(F, w1, w2, w3, w4);

endmodule

