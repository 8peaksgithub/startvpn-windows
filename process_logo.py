#!/usr/bin/env python3
"""
Script to remove background from logo and create different sizes
Usage: python3 process_logo.py <input_logo.png>
"""

import sys
import os
from PIL import Image

def remove_background(input_path, output_path):
    """Remove white/light background from image and make it transparent"""
    try:
        # Open the image
        img = Image.open(input_path)
        img = img.convert("RGBA")
        
        # Get image data
        datas = img.getdata()
        
        new_data = []
        for item in datas:
            # Change all white/light gray (240+) pixels to transparent
            if item[0] > 240 and item[1] > 240 and item[2] > 240:
                new_data.append((255, 255, 255, 0))  # Transparent
            else:
                new_data.append(item)
        
        # Update image data
        img.putdata(new_data)
        
        # Save the image
        img.save(output_path, "PNG")
        print(f"Saved processed logo to: {output_path}")
        
        return img
        
    except Exception as e:
        print(f"Error processing image: {e}")
        return None

def create_resized_versions(img, base_path):
    """Create different sized versions of the logo"""
    sizes = {
        'small': (100, 100),
        'medium': (200, 200),
        'large': (400, 400),
    }
    
    for name, size in sizes.items():
        resized = img.copy()
        resized.thumbnail(size, Image.Resampling.LANCZOS)
        output = base_path.replace('.png', f'_{name}.png')
        resized.save(output, "PNG")
        print(f"Created {name} version: {output}")

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 process_logo.py <input_logo.png>")
        sys.exit(1)
    
    input_logo = sys.argv[1]
    
    if not os.path.exists(input_logo):
        print(f"Error: File not found: {input_logo}")
        sys.exit(1)
    
    # Process the logo
    output_logo = "/workspaces/startvpn-windows/resources/new_logo.png"
    processed_img = remove_background(input_logo, output_logo)
    
    if processed_img:
        # Create different sizes
        create_resized_versions(processed_img, output_logo)
        print("\nLogo processing complete!")
        print(f"Main logo: {output_logo}")

if __name__ == "__main__":
    main()
