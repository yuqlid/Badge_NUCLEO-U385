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
    
    # Convert BGR to RGB for proper color handling
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    
    # convert to np array
    img = img.flatten()
    
    with open('img.bin', 'wb') as f:
        for i in range(0, img.shape[0], 3):
            # convert to 565 binary
            # Note: img is now in RGB order after cvtColor conversion
            r = img[i]      # Red
            g = img[i+1]    # Green
            b = img[i+2]    # Blue
            
            # Convert 8-bit to 5/6-bit
            r5 = int((r >> 3) & 0x1F)  # 5 bits
            g6 = int((g >> 2) & 0x3F)  # 6 bits
            b5 = int((b >> 3) & 0x1F)  # 5 bits
            
            # Pack as RGB565: RRRRRGGGGGGBBBBB (16-bit, little-endian)
            rgb565 = ((r5 << 11) | (g6 << 5) | b5) & 0xFFFF
            
            rgb = struct.pack('<H', rgb565)  # Explicit little-endian
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
