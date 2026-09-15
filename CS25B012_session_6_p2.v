`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/15/2026 03:18:00 PM
// Design Name: 
// Module Name: CS25B012_session_6_p2
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


module CS25B012_session_6_p2(
    input A, input B, input C, input D,
    output Z
);

    wire w1, w2, w3, w4;
    wire nA, nB, nC, nD;

    and(w1, A, B, C);
    and(w2, D, B, C);

    not(nA, A);
    not(nB, B);
    not(nC, C);
    not(nD, D);

    and(w3, nA, nB, nC);
    and(w4, nD, nB, nC);

    or(Z, w1, w2, w3, w4);

endmodule