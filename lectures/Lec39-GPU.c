Lecture 39: GPU

CPU vs. GPU Overview
- GPU: graphics processing unit, which creates images in a frame buffer for a display device
- CPU: better performance by minimizing latency of threads w/ complicated control structure
- GPU: Intended to maximize throughput using scheduling across parallel threads
	- Has 100s of execution units, rather than CPU having ~4
- CPU characteristic: fewer cores, higher frequency, lower latency, instruction level parallelism,
  fewer registers, fewer execution units, execution control is more complicated, hardware managed
- Use SIMD for higher throughput & can use independent threads to execute same program

GPU: Image Processing
- GPU consists of vertex processing, rasterization, fragment processing, and the framebuffer
	- Vertex Processing: performing computation on individual vertices (transform vertices into triangles)
		- Triangles are easier to work w/ compared to true 3D rendering. Idea: take a collection of points and
		  transform them (using rotations, translations, projections, etc.)
		- Can use a tile-based rendering to estimate the cost of image processing for each pixel,
		  which allows for more efficient allocation of computational resources
	- Rasterize: transforming triangles into discrete pixels (scan conversion of triangles)
		- Process of performing work on individual tiles & scanning the conversion
	- Fragment Processing: run programs on individual pixels (ex: lighting, texturing, shading)
		- Fragment Processing: performing the depth test and stencil test to remove undesirable fragments
		- Run the shading program on every pixel, so can use SIMD (thread-level parallelism) for speedup
		- Texture mapping: Map each pixel to texels across multiple images
	- Framebuffer: output pixels to a memory address
		- Want to tell that the frame is ready for display

/* C: Multiply each element by a constant & store result */
void saxpy(int n, float a, float* x, float* y) {
	for (int i = 0; i < n; i++)
		y[i] = a*x[i];
}

/* Same program as above in Metal */
using namespace metal;
kernel void saxpy (
	constant float& a [[buffer(0)]],
	constant float *x [[buffer(1)]],
	device float *y [[buffer(2)]],
	uint threadIndex[[thread_position_in_grid]]) {
	y[threadIndex] += a * x[threadIndex];
}

Saxpy Comparison
- For saxpy, GPU does a much better job for large workloads (conclusion: fixed overhead for GPU)
- For smaller workloads, CPU does a better job b/c there is a lot less overhead


Conclusion
- GPU maximizes throughput by taking advantage of parallelism
- One good case study for parallel processing is image processing (looking at individual pixels)

