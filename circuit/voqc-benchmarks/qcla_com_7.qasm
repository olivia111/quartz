OPENQASM 2.0;
include "qelib1.inc";
qreg q[24];
x q[0];
x q[3];
x q[6];
x q[10];
x q[13];
x q[17];
x q[21];
h q[2];
h q[5];
h q[8];
h q[9];
h q[12];
h q[15];
h q[16];
h q[19];
h q[20];
h q[23];
ccz q[0], q[1], q[2];
ccz q[3], q[4], q[5];
ccz q[6], q[7], q[9];
ccz q[10], q[11], q[12];
ccz q[13], q[14], q[16];
ccz q[17], q[18], q[20];
ccz q[21], q[22], q[23];
cx q[3], q[4];
cx q[6], q[7];
cx q[10], q[11];
cx q[13], q[14];
cx q[17], q[18];
cx q[21], q[22];
ccz q[7], q[11], q[8];
ccz q[14], q[18], q[15];
h q[2];
h q[8];
h q[9];
h q[15];
h q[16];
h q[19];
ccz q[15], q[22], q[19];
ccz q[2], q[4], q[5];
ccz q[16], q[18], q[20];
h q[19];
h q[5];
h q[20];
ccz q[9], q[11], q[12];
ccz q[5], q[8], q[12];
ccz q[20], q[22], q[23];
h q[12];
ccz q[12], q[19], q[23];
h q[23];
h q[12];
h q[19];
h q[20];
ccz q[5], q[8], q[12];
ccz q[16], q[18], q[20];
ccz q[15], q[22], q[19];
h q[5];
ccz q[2], q[4], q[5];
ccz q[9], q[11], q[12];
h q[2];
h q[8];
h q[9];
h q[15];
h q[16];
ccz q[7], q[11], q[8];
ccz q[14], q[18], q[15];
cx q[3], q[4];
cx q[6], q[7];
cx q[10], q[11];
cx q[13], q[14];
cx q[17], q[18];
cx q[21], q[22];
ccz q[0], q[1], q[2];
ccz q[3], q[4], q[5];
ccz q[6], q[7], q[9];
ccz q[10], q[11], q[12];
ccz q[13], q[14], q[16];
ccz q[17], q[18], q[20];
h q[2];
h q[5];
h q[8];
h q[9];
h q[12];
h q[15];
h q[16];
h q[19];
h q[20];
x q[0];
x q[3];
x q[6];
x q[10];
x q[13];
x q[17];
x q[21];
x q[23];
