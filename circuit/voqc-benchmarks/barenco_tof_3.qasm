OPENQASM 2.0;
include "qelib1.inc";
qreg q[5];
h q[4];
ccz q[2], q[3], q[4];
h q[3];
ccz q[0], q[1], q[3];
h q[3];
ccz q[2], q[3], q[4];
h q[4];
h q[3];
ccz q[0], q[1], q[3];
h q[3];
