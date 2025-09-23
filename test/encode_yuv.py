#!/bin/python

import os, sys

codec = sys.argv[1]
encoder = {"vvc": "libvvenc", 
           "av1": "libaom-av1", 
           "vp9": "libvpx-vp9"}[codec]

for file in os.listdir("/home/lemur/projects/video_decoder/test/yuv"):
    filename = file[:-4]
    _, size, fps = filename.split("_")
    command = f"ffmpeg -f rawvideo -video_size {size} -framerate {fps} -pix_fmt yuv420p -i /home/lemur/projects/video_decoder/test/yuv/{file} -c:v {encoder} /home/lemur/projects/video_decoder/test/{codec}/{filename}.{codec}"
    os.system(command)