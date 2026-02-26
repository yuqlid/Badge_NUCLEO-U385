# read image and convert to 565 binary

import struct
import cv2
import sys

def convert(img_path):
    img = cv2.imread(img_path)
    
    if img is None:
        print(f'Error: Could not open image file: {img_path}')
        return
    
    # check image size is 240x240
    if img.shape[0] != 240 or img.shape[1] != 240:
        print('Image size must be 240x240')
        return
    
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    
    # convert to np array
    img = img.flatten()
    
    with open('img.bin', 'wb') as f:
        for i in range(0, img.shape[0], 3):
            # convert to 565 binary
            r = img[i]
            g = img[i+1]
            b = img[i+2]
            rgb = struct.pack('H', ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))
            f.write(rgb)
    print('Convert done')
    
    with open('img.bin', 'rb') as f:
        data = f.read()
        print(len(data))

if __name__ == '__main__':
    if len(sys.argv) > 1:
        convert(sys.argv[1])
    else:
        convert('im.png')
