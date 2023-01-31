#pragma once

typedef struct nykGLPoint_t {
	double x, y, z, nx, ny, nz;
} nykGLPoint;

typedef short nykGLPolyIndex;

extern nykGLPoint* nyk_gltype_point_data[];
extern nykGLPolyIndex* nyk_gltype_poly_index[];