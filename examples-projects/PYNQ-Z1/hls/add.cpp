void add (int a, int b, int *y)
{
#pragma HLS interface s_axilite port=a
#pragma HLS interface s_axilite port=b
#pragma HLS interface s_axilite port=y
#pragma HLS interface ap_ctrl_none port=return
	*y = a + b;
}
