# Video Decoder
Decoder for international video formats.

This is just a fun project for me to fiddle with. It's nothing revolutionary but I enjoy the challenge.

## `ffplay` commands

### Playing YUV:
`ffplay -f rawvideo -pixel_format yuv420p -video_size <width>x<height> -i <input_yuv>`

### Convert YUV to VVC
`ffmpeg -f rawvideo -video_size <width>x<height> -framerate <frame_rate> -pix_fmt yuv420p -i <input_yuv> -c:v libvvenc <output_vvc>`

#### Encode Script
`test/encode_yuv.py`