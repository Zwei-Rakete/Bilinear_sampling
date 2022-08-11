import numpy as np
import math

def bilinear_interpolation(image, scale):
    height = image.shape[-2]
    width = image.shape[-1]
    c = image.shape[1]
    dst_height = int(scale * height)
    dst_width = int(scale * width)
    h_s = np.linspace(0, height-1, dst_height)
    w_s = np.linspace(0, width-1, dst_width)
    new_image = np.zeros((1, c, dst_height, dst_width))
    for k in range(c):
        for i in range(dst_height):
            for j in range(dst_width):
                h0, w0 = math.floor(h_s[i]),math.floor(w_s[j])
                # 如果到了右侧边界，直接取原图像素点放入目标图像中
                if j == dst_width - 1:
                    new_image[0, k, i, j] = image[0, k, h0, w0]
                    continue
                elif i == dst_height - 1:
                # 如果到了底部边界，直接取原图像素点放入目标图像中
                    new_image[0, k, i, j] = image[0, k, h0, w0]
                    continue
                
                point_x = (w0)
                point_y = (h0)
                q_00 = image[0, k, point_y, point_x]          #左上角坐标
                q_01 = image[0, k, point_y, point_x + 1]      #右上角
                q_10 = image[0, k, point_y + 1, point_x]      #左下角
                q_11 = image[0, k, point_y + 1, point_x + 1]  #右下角
                
                fr0 = ((q_10 - q_00) * (point_y - h0)) + q_00
                fr1 = ((q_11 - q_01) * (point_y - h0)) + q_01
                fp = (fr1 - fr0) * (point_x - w0) + fr0
                new_image[0, k, i, j] = fp
                
    return new_image