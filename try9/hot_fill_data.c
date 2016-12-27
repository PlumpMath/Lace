#include <math.h>
#include <assert.h>

typedef struct {
    float x, y, z;
} Vert;

float height_at(float x, float z) {
    return sinf(x * 0.5) * 0.02 + x * cos(x * 1 + z * 1) * 0.7 + sinf(z * 0.2) * 0.3;
}

#define MAX_VERT_COUNT 99999

void fill_data(Vert *verts, int *vert_count) {
    float w = 0.1;
    for(float z = -0.8; z <= 0.8 - w; z += w) {
        for(float x = -0.8; x <= 0.8 - w; x += w) {

            float y0 = height_at(x, z);
            float y1 = height_at(x + w, z);
            float y2 = height_at(x, z + w);
            float y3 = height_at(x + w, z + w);
            
            verts[*vert_count] =  (Vert) { x, y0, z, };
            (*vert_count)++;
            verts[*vert_count] =  (Vert) { x + w, y1, z };
            (*vert_count)++;
            verts[*vert_count] =  (Vert) { x, y2, z + w };
            (*vert_count)++;

            verts[*vert_count] =  (Vert) { x + w, y1, z };
            (*vert_count)++;
            verts[*vert_count] =  (Vert) { x + w, y3, z + w };
            (*vert_count)++;
            verts[*vert_count] =  (Vert) { x, y2, z + w };
            (*vert_count)++;

            assert(*vert_count < MAX_VERT_COUNT);
        }
    }
}
