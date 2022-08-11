import os
import cv2
import glob
import torch
import torch.nn.functional as F
import numpy as np
from interpolate import bilinear_interpolation

if __name__ == "__main__":
    # read origin image
    path = 'input/'
    output = 'output/'
    scale = 2   # 2 for upsample / 0.5 for downsample
    if path.endswith('/'):
        img_input = path[:-1]
    if os.path.isfile(img_input):
        img_list = [img_input]
    else:
        img_list = sorted(glob.glob(os.path.join(img_input, '*')))
    os.makedirs(output, exist_ok=True)

    for img_path in img_list:
        # read image
        img_name = os.path.basename(img_path)
        print(f'Processing {img_name} ...')
        basename, ext = os.path.splitext(img_name)
        input_img = np.array(cv2.imread(img_path)).transpose(2, 0 ,1)
        input_img = np.expand_dims(input_img, 0).astype("float32")

        # main process
        img1 = bilinear_interpolation(input_img, scale)

        # pytorch original interpolate function
        img2 = F.interpolate(torch.tensor(input_img), scale_factor=scale, mode = 'bilinear', align_corners=False)
        
        img1 = img1.squeeze().transpose(1,2,0)
        img2 = np.array(img2).squeeze().transpose(1,2,0)
        cv2.imwrite(f'output/{basename}_1.png',img1)
        cv2.imwrite(f'output/{basename}_torch.png',img2)
