#ifndef __TRANSPOSED_H__
#define __TRANSPOSED_H__

double coeff_data_b[3] = {
    0.271275515076715,
    0.376532612594379,
    0.271275515076715
};

double coeff_data_a[3] = {
    1,
    -0.343320701630157,
    0.316863473960287
};

Word16 coeff_data_a_fix_q15[3] = {
    32767, -11250,10383
};
Word16 coeff_data_b_fix_q15[3] = {
    8889, 12338, 8889
};

Word32 coeff_data_a_fix_q31[3] = {
2147483647,  -737275593 ,  680459129
};

Word32 coeff_data_b_fix_q31[3] = {
    582559733,   808597628,   582559733
};

#endif
