#include <stdio.h>
#include <stdlib.h>

struct padding{
    int up, left;
};

float valueOrDie(float ***in_Matrix, struct padding *pad_res, int _i, int _j, int _k){
    int map_i = _i - pad_res->up;
    int map_j = _j - pad_res->left;
    if( (map_i >= 0 ) && (map_j >= 0)
        )
        return in_Matrix[map_i][map_j][_k];
    else
        return 0.0;
}

float innerConv(float **in_Matrix, int i, int j, int s_w, int s_h, struct padding *pad_res,float ***filter, int f_w, int f_h, int f_c){
    float res = 0;
    int _i = i*s_h, _j = j*s_w;
	for (int k = 0; k < f_c; k++) {
		for(int m = 0; m < f_w; m++)
			for(int n = 0; n < f_h; n++){
				res += filter[n][m][k] * valueOrDie(in_Matrix, pad_res, _i + n, _j + m, k);
			}
        }
    return res;

}

void outConv( float ***out_Matrix, float ***in_Matrix, int out_w, int out_h, int out_c, int s_w, int s_h,
             struct padding *pad_res, float ****filter, int f_w, int f_h, int f_c){
	for (int k = 0; k < out_c; k++) {
		float *** kernel = filter[k];
		for (int i = 0; i < out_h; i++) {
			for (int j = 0; j < out_w; j++) {
				out_Matrix[i][j][k] = innerConv(in_Matrix, i, j, s_w, s_h, pad_res, kernel, f_w, f_h, f_c);
			}
		}
	}
}



int my_ceil(int x, int y){
    int result = --x/y;
    result++;
    return result;
}



void get_padding(struct padding *pad_res,int in_h, int in_w, int out_h, int out_w, int s_h, int s_w, int f_h, int f_w){

    int pad_up_down = (out_h-1)*s_h+f_h-in_h;
    if (pad_up_down)
        (*pad_res).up = my_ceil(pad_up_down,2);

    int pad_left_right = (out_w -1)*s_w+f_w-in_w;
    if (pad_left_right)
        (*pad_res).left = my_ceil(pad_left_right,2);

}

float *** create_matrix(int in_w, int in_h, int in_c, float value){

	float ***in_Matrix = (float***)malloc(in_c *sizeof(float**));

	for (int c = 0; c < in_c; c++){
		in_Matrix[c] = (float**)malloc(in_h * sizeof(float*));	
		for (int j = 0; j < in_h; j++) {
			in_Matrix[c][j] = (float*)malloc(in_w*sizeof(float));
			for (int i = 0; i < in_w; i++)
				in_Matrix[c][j][i] = value;
		}
	}
    return in_Matrix;
	
}

void free_matrix(float ***in_Matrix, int in_w, int in_h, int in_c){
	for (int i = 0; i < in_w; i++){
		for (int j = 0; j < in_h; j++) {
			free(in_Matrix[i][j]);
		}
	}
    for (int i = 0; i < in_h; i++)
        free(in_Matrix[i]);
    free(in_Matrix);
}

int main() {
	int out_h, out_w, out_c = 5, in_h = 4, in_w = 5, in_c = 3, s_h = 2, s_w = 2, f_h = 2, f_w = 2, f_c = 3;

    out_h = my_ceil(in_h,s_h);
    out_w = my_ceil(in_w,s_w);

	// for testing
    float ***in_Matrix = create_matrix(in_w, in_h, in_c, 1.0);
    float ***out_Matrix = create_matrix(out_w, out_h, out_c, 0.0);
	float ****filter = (float****)malloc(out_c * sizeof(float***));
	for (int k = 0; k < out_c; k++) {
		filter[k] = create_matrix(f_w, f_h, in_c, 2.0);
	}

    struct padding pad_res={0,0};
    get_padding(&pad_res, in_h, in_w, out_h, out_w, s_h, s_w, f_h, f_w);

    outConv(out_Matrix,in_Matrix, out_w, out_h, out_c, s_w, s_h,&pad_res, filter, f_w, f_h, f_c);

    for (int i = 0; i < out_h; i++) {
//        out_h[i] = (float*)malloc(in_w*sizeof(float));
        for (int j = 0; j < out_w; j++)
//            in_Matrix[i][j] = value;
            printf("%f\t", out_Matrix[i][j]);
    }

    free_matrix(in_Matrix, in_w, in_h, in_c);
    free_matrix(out_Matrix, out_w, out_h, out_c);
	for (int k = 0; k < out_c; k++) {
		free_matrix(filter, f_w, f_h, in_c);
	}
	free(filter);
    printf("Hello, World!\n");
    return 0;
}