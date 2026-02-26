# pad transparent PNG image to 240x240 with transparent background

import cv2
import numpy as np
import sys

def pad_image(img_path, output_path='output.png'):
    """
    Pad a transparent PNG image to 240x240 by placing it in the center
    with transparent padding on top and bottom.
    
    Args:
        img_path: Path to input PNG image (should have transparent background)
        output_path: Path to save the output image (default: output.png)
    """
    
    # Read image with alpha channel
    img = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)
    
    if img is None:
        print(f'Error: Could not open image file: {img_path}')
        return
    
    print(f'Original image size: {img.shape[1]}x{img.shape[0]}')
    
    # Target size
    target_width = 240
    target_height = 240
    
    # Current image dimensions
    img_height, img_width = img.shape[0], img.shape[1]
    
    # Check if image width matches target width
    if img_width != target_width:
        print(f'Warning: Image width {img_width} does not match target width {target_width}')
    
    # Calculate padding
    pad_top = (target_height - img_height) // 2
    pad_bottom = target_height - img_height - pad_top
    pad_left = (target_width - img_width) // 2
    pad_right = target_width - img_width - pad_left
    
    # Create canvas with RGBA (transparent background)
    if img.shape[2] == 4:  # If image already has alpha channel
        canvas = np.zeros((target_height, target_width, 4), dtype=np.uint8)
    else:  # If no alpha channel, add one
        img = cv2.cvtColor(img, cv2.COLOR_BGR2BGRA)
        canvas = np.zeros((target_height, target_width, 4), dtype=np.uint8)
    
    # Place original image in center
    canvas[pad_top:pad_top+img_height, pad_left:pad_left+img_width] = img
    
    # Save the result
    cv2.imwrite(output_path, canvas)
    print(f'Padded image saved to: {output_path}')
    print(f'Output image size: {canvas.shape[1]}x{canvas.shape[0]}')

if __name__ == '__main__':
    if len(sys.argv) > 1:
        input_file = sys.argv[1]
        output_file = sys.argv[2] if len(sys.argv) > 2 else 'output.png'
        pad_image(input_file, output_file)
    else:
        # Default usage
        pad_image('input.png', 'output.png')
