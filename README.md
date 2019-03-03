# Median Threshold Bitmaps Alignment

## Comparison - Edge detection

1. Edge-detection filters are dependent on image exposure
2. It takes longer time to align
3. Failed on the majority of high-resolution images

## Concepts

1. The response function of a camera is __monotonic__
2. Then, their __median__ should be on the same or similar spots
3. Use integer pixel offsets and ignore rotations

## Method

1. Use the green channel only or the following conversion formula:
   $$
   \text{grey} = (54 \times \text{red} + 183 \times \text{green} + 19 \times \text{blue}) / 256
   $$

2. Initialize the limit of shifted bits, and adopt _image pyramids_ to reduce computation

   1. Compute bitmaps
      - Threshold bitmap:
        0 if <= median
        1 otherwise
      - Exclusion bitmap: to ignore noise near the median
        0 if it locates in the range of $\pm4$ pixel near a median-valued pixel
        1 otherwise
   2. Shift the second image's bitmaps $\pm1$ (with offset from smaller image) on both dimension to find lower error
      - Error computation:
        - `XOR` the first image's threshold bitmap and the second's shifted one as `bm_result`
        - `AND` `bm_result` with both the first and the shifted second's exclusion bitmaps
        - Sum the total 1s on the generated bitmap
   3. Time the lowest offset by 2 and make it the initial offset for its larger image