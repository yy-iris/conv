#include <stdio.h>
#include <stdlib.h>

struct padding{
    int up, left;
};

float valueOrDie(const float **in_Matrix, struct padding *pad_res, int _i, int _j){
    int map_i = _i - pad_res->up;
    int map_j = _j - pad_res->left;
    if( (map_i >= 0 ) && (map_j >= 0)
        )
        return in_Matrix[map_i][map_j];
    else
        return 0.0;
}

float innerConv(const float **in_Matrix, int i, int j, int s_w, int s_h, struct padding *pad_res,const float **filter, int f_w, int f_h){
    float res = 0;
    int _i = i*s_h, _j = j*s_w;
    for(int m = 0; m < f_w; m++)
        for(int n = 0; n < f_h; n++){
            res += filter[n][m] * valueOrDie(in_Matrix, pad_res, _i+n, _j+m);
        }
    return res;

}

void outConv( float **out_Matrix, const float **in_Matrix, int out_w, int out_h, int s_w, int s_h,
             struct padding *pad_res, const float **filter, int f_w, int f_h){
    for (int i = 0; i < out_h; i++) {
        for (int j = 0; j < out_w; j++) {
            out_Matrix[i][j] = innerConv(in_Matrix, i, j, s_w, s_h,pad_res, filter, f_w, f_h);
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

float ** create_matrix(int in_w, int in_h, float value){

    float **in_Matrix = (float**)malloc(in_h*sizeof(float*));

    for (int i = 0; i < in_h; i++) {
        in_Matrix[i] = (float*)malloc(in_w*sizeof(float));
        for (int j = 0; j < in_w; j++)
            in_Matrix[i][j] = value;
    }
    return in_Matrix;
}

void free_matrix(float **in_Matrix, int in_h){
    for (int i = 0; i < in_h; i++)
        free(in_Matrix[i]);
    free(in_Matrix);
}

int main() {
    int out_h, out_w, in_h = 24, in_w=129, s_h = 3, s_w = 2, f_h = 3, f_w = 4;



    out_h = my_ceil(in_h,s_h);
    out_w = my_ceil(in_w,s_w);

    const float **in_Matrix = create_matrix(in_w, in_h, 1.0);
    float **out_Matrix = create_matrix(out_w, out_h, 0.0);
    const float **filter = create_matrix(f_w, f_h, 2.0);



    struct padding pad_res={0,0};
    get_padding(&pad_res, in_h, in_w, out_h, out_w, s_h, s_w, f_h, f_w);



    outConv(out_Matrix,in_Matrix, out_w, out_h, s_w, s_h,&pad_res, filter, f_w, f_h);

    for (int i = 0; i < out_h; i++) {
//        out_h[i] = (float*)malloc(in_w*sizeof(float));
        for (int j = 0; j < out_w; j++)
//            in_Matrix[i][j] = value;
            printf("%f\t", out_Matrix[i][j]);
    }

    free_matrix(in_Matrix, in_h);
    free_matrix(out_Matrix, out_h);
    free_matrix(filter, f_h);
    printf("Hello, World!\n");
    return 0;
}