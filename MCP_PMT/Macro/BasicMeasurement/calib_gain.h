// amp input = par_amp[0] x ADC^par_amp[1] + par_amp[2] x ADC^5
const double par_amp[2][16][3][3] = {
{ // LNA amp
{{0.046, 0.540, 9.32e-13},{0.124, 0.411, 4.03e-12}},
{{0.045, 0.529, 2.45e-12},{0.114, 0.417, 9.26e-12}},
{{0.045, 0.538, 9.19e-13},{0.117, 0.418, 3.87e-12}},
{{0.054, 0.532, 8.45e-13},{0.151, 0.391, 7.26e-12}},
{{0.044, 0.536, 9.64e-13},{0.119, 0.412, 4.01e-12}},
{{0.044, 0.510, 1.73e-12},{0.103, 0.405, 8.63e-12}},
{{0.073, 0.455, 2.11e-12},{0.155, 0.362, 8.99e-12}},
{{0.064, 0.491, 1.35e-12},{0.124, 0.410, 3.49e-12}},
{{0.074, 0.461, 9.02e-13},{0.134, 0.397, 1.23e-12}},
{{0.064, 0.458, 2.42e-12},{0.117, 0.387, 7.08e-12}},
{{0.050, 0.472, 1.39e-12},{0.099, 0.393, 4.88e-12}},
{{0.066, 0.470, 1.49e-12},{0.130, 0.384, 4.18e-12}},
{{0.031, 0.622, 4.19e-13},{0.090, 0.500, 2.90e-12}},
{{0.029, 0.615, 1.06e-12},{0.085, 0.492, 6.93e-12}},
{{0.063, 0.502, 1.25e-12},{0.142, 0.410, 1.39e-11}},
{{0.030, 0.603, 1.16e-12},{0.088, 0.484, 6.92e-12}}},
{ // variable amp
{{0.009, 0.871, 1.35e-14},{0.013, 1.000, 9.36e-12},{0.028, 1.000, 1.00e-10}},
{{0.012, 0.830, 6.88e-25},{0.016, 0.971, 8.33e-25},{0.035, 0.981, 1.00e-10}},
{{0.010, 0.843, 2.78e-25},{0.014, 0.981, 1.15e-22},{0.030, 1.000, 1.00e-10}},
{{0.011, 0.841, 3.83e-25},{0.015, 0.977, 3.75e-22},{0.028, 1.000, 1.00e-10}},
{{0.012, 0.814, 2.22e-26},{0.017, 0.938, 1.24e-22},{0.034, 0.968, 1.00e-10}},
{{0.015, 0.748, 0.00e+00},{0.018, 0.928, 1.86e-12},{0.031, 1.000, 1.00e-10}},
{{0.014, 0.777, 2.33e-25},{0.019, 0.917, 1.50e-25},{0.032, 1.000, 1.00e-10}},
{{0.016, 0.756, 5.00e-26},{0.023, 0.877, 4.16e-22},{0.062, 0.820, 1.00e-10}},
{{0.011, 0.821, 0.00e+00},{0.015, 0.967, 1.39e-23},{0.030, 1.000, 1.00e-10}},
{{0.018, 0.717, 1.49e-24},{0.017, 0.961, 1.67e-26},{0.033, 1.000, 1.00e-10}},
{{0.016, 0.732, 0.00e+00},{0.017, 0.922, 2.78e-26},{0.031, 1.000, 1.00e-10}},
{{0.012, 0.812, 0.00e+00},{0.014, 0.984, 2.28e-25},{0.030, 1.000, 1.00e-10}},
{{0.016, 0.753, 4.44e-26},{0.013, 1.000, 1.83e-11},{0.029, 1.000, 1.00e-10}},
{{0.011, 0.825, 1.67e-26},{0.014, 0.982, 1.35e-22},{0.030, 1.000, 1.00e-10}},
{{0.012, 0.801, 0.00e+00},{0.015, 0.965, 3.12e-23},{0.030, 1.000, 1.00e-10}},
{{0.010, 0.854, 5.55e-27},{0.013, 1.000, 1.66e-12},{0.029, 1.000, 1.00e-10}}}
};

const double correction_ch[2][16] = {
{ // LNA amp (JT0763_20131224_??.root)
//1.02, 0.96, 0.98, 1.12, 1.00, 0.88, 1.09, 1.16,
//1.13, 0.97, 0.87, 1.03, 0.95, 0.90, 1.03, 0.91},
//0.98, 0.92, 0.95, 1.07, 0.95, 0.85, 1.08, 1.22,
//1.19, 1.01, 0.90, 1.09, 0.95, 0.90, 1.04, 0.89},
1.03, 1.00, 1.00, 1.13, 1.04, 0.88, 1.09, 1.13,
1.10, 0.93, 0.88, 0.98, 0.99, 0.91, 1.01, 0.91},
{ // variable amp (JT0763_20140502_??.root)
0.99, 1.02, 0.96, 1.01, 0.96, 1.02, 1.08, 1.05,
0.96, 1.04, 0.96, 1.01, 0.99, 0.94, 0.97, 1.02}
};

// Correct the LNA amp LV high/low difference
// JT0743, JT0744, JT0852
const double correction_amplv[16] = {
//1.28, 1.29, 1.27, 1.29, 1.30, 1.27, 1.25, 1.17,
//1.17, 1.18, 1.19, 1.16, 1.22, 1.23, 1.22, 1.26};
//1.18, 1.19, 1.15, 1.22, 1.19, 1.16, 1.10, 1.07,
//1.15, 1.16, 1.13, 1.20, 1.15, 1.19, 1.16, 1.21};
1.10, 1.01, 0.99, 1.11, 1.07, 1.05, 1.03, 1.01,
1.11, 1.12, 1.06, 1.16, 1.08, 1.10, 1.10, 1.12};
const double err_correction_amplv = 0.06;


// Correct the gain difference of LNA from the variable amp
// LNA/variable, JT0763
const double correction_amp[16] = {
1.15, 1.11, 1.20, 1.12, 1.08, 1.29, 1.30, 1.24,
1.33, 1.30, 1.24, 1.37, 1.16, 1.19, 1.31, 1.35};

// Correct the absolute gain
// ADC/oscilloscope, KT0117_20140709
const double correction_gain = 0.77;

// Correct the gain of the old data
// ratio of JT0558_wocalib_20120531.root to JT0558_20141127.root
const double correction_old[16] = {
1.53, 1.57, 1.53, 1.32, 1.30, 1.37, 1.33, 1.41,
1.50, 1.51, 1.35, 1.41, 1.43, 1.59, 1.30, 1.30};
