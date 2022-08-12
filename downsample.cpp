#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

typedef vector<vector<vector<vector<float>>>> tensor;
typedef vector<vector<int>> point;

tensor empty_image(int b, int c, int height, int width) {
    tensor ret;
    ret.resize(b);
    for (int x = 0; x < b; x++){
        for (int y = 0; y < c; y++){
            for (int i = 0; i < height; i ++){
                ret[x].resize(c);
                ret[x][y].resize(height);
                ret[x][y][i].resize(width);
            }
        }
    }
    return ret;
}

tensor bilinear_down(tensor& in, float scale){
    if (scale > 1){
        cout<<"只支持下采样,自动设置scale为0.5"<<endl;
        scale = 0.5;
    }
    int b = in.size();
    int c = in[0].size();
    int w = in[0][0].size();
    int h = in[0][0][0].size();
    int dst_height = static_cast<int>(h * scale);
    int dst_width = static_cast<int>(w * scale);
    tensor out = empty_image(b, c, dst_height, dst_width);
    for (int i = 0; i < b; i++){
        for (int j = 0; j < c; j++){
            for(int x = 0; x < dst_height; x++){
                for(int y = 0; y < dst_width; y++){
                    int corr_x = (x + 0.5) / dst_height * h - 0.5;
                    int corr_y = (y + 0.5) / dst_width * w - 0.5;
                    int point1[2] = {floor(corr_x),floor(corr_y)};
                    int point2[2] = {point1[0], point1[1] + 1};
                    int point3[2] = {point1[0] + 1, point1[1]};
                    int point4[2] = {point1[0] + 1, point1[1] + 1};
                    
                    float fr1 = (point2[1] - corr_y) * in[i][j][point1[0]][point1[1]] + (corr_y - point1[1]) * in[i][j][point2[0]][point2[1]];
                    float fr2 = (point2[1] - corr_y) * in[i][j][point3[0]][point3[1]] + (corr_y - point1[1]) * in[i][j][point4[0]][point4[1]];
                    out[i][j][x][y] = (point3[0] - corr_x) * fr1 + (corr_x - point1[0]) * fr2;
                }
            }
        }
    }
    return out;
}


void randomize_image(tensor& img) {
    int b = img.size();
    int c = img[0].size();
    int height = img[0][0].size();
    int width = img[0][0][0].size();
    for (int x = 0; x < b; x++) {
        for (int y = 0; y < c; y++) {
            for (int i = 0; i < height; i++){
                for (int j = 0; j < width; j++){
                    img[x][y][i][j] = rand() % 256 + 0.5;
                }
            }
            
        }
    }
}

int main(int argc, char** argv) {
    int height = 256;
    int width = 256;
    int c = 3;
    int b = 1;
    float scale = 0.5;
    cout<<"输入尺寸:"<<b<<" "<<c<<" "<<height<<" "<<width<<endl;
    tensor in = empty_image(b, c, height, width);
    randomize_image(in);
    tensor out = bilinear_down(in, scale);
    cout<<"输出尺寸:"<<out.size()<<" "<<out[0].size()<<" "<<out[0][0].size()<<" "<<out[0][0][0].size()<<endl;
    return 0;
}
