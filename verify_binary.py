# verify RGB565 binary image output

import struct
import numpy as np
import cv2
import sys

def verify_binary(bin_path, output_path='verified.png'):
    """
    Read RGB565 binary file and convert back to PNG image for verification.
    
    Args:
        bin_path: Path to input binary file
        output_path: Path to save the verification image (default: verified.png)
    """
    
    try:
        with open(bin_path, 'rb') as f:
            data = f.read()
    except FileNotFoundError:
        print(f'Error: Could not open binary file: {bin_path}')
        return
    
    # Expected size: 240x240x2 bytes (RGB565)
    expected_size = 240 * 240 * 2
    
    if len(data) != expected_size:
        print(f'Warning: Binary size {len(data)} bytes does not match expected size {expected_size} bytes')
    
    # Convert RGB565 to RGB888
    img_array = np.zeros((240, 240, 3), dtype=np.uint8)
    
    for i in range(0, len(data), 2):
        # Read 2 bytes (little-endian)
        rgb565 = struct.unpack('<H', data[i:i+2])[0]
        
        # Extract RGB565 components
        r = (rgb565 >> 11) & 0x1F  # 5 bits
        g = (rgb565 >> 5) & 0x3F   # 6 bits
        b = rgb565 & 0x1F          # 5 bits
        
        # Convert to 8-bit values
        r = (r << 3) | (r >> 2)    # Expand 5-bit to 8-bit
        g = (g << 2) | (g >> 4)    # Expand 6-bit to 8-bit
        b = (b << 3) | (b >> 2)    # Expand 5-bit to 8-bit
        
        # Calculate pixel position
        pixel_idx = i // 2
        row = pixel_idx // 240
        col = pixel_idx % 240
        
        img_array[row, col] = [b, g, r]  # BGR format for cv2
    
    # Save as PNG
    cv2.imwrite(output_path, img_array)
    print(f'Verification image saved to: {output_path}')
    print(f'Image size: {img_array.shape[1]}x{img_array.shape[0]}')
    
    # Print first few pixels for debugging
    print('\nFirst 5 pixels (BGR)：')
    for i in range(5):
        print(f'  Pixel {i}: B={img_array[0, i, 0]}, G={img_array[0, i, 1]}, R={img_array[0, i, 2]}')

if __name__ == '__main__':
    if len(sys.argv) > 1:
        binary_file = sys.argv[1]
        output_file = sys.argv[2] if len(sys.argv) > 2 else 'verified.png'
        verify_binary(binary_file, output_file)
    else:
        # Default usage
        verify_binary('img.bin', 'verified.png')
