#include "graphics.h"

#define MAX_ROTATION 4

#define GRIDX 3 
#define GRIDY 3
#define GRIDZ 3 

typedef enum {
    GRASS,
    SAND,
    WATER,
    AIR,
    BLOCK_SIZE
} BLOCK;

typedef struct {
    int grid[GRIDX][GRIDY][GRIDZ];
} WFC_Grid;

typedef struct {
    WFC_Module** modules;
} WFC_Prototype;

void wfc_draw(Model* m, unsigned int shaderId) {
    mat4 model = GLM_MAT4_IDENTITY;
    for (int i = 0; i < GRIDX; i++) {
        for (int j = 0; j < GRIDY; j++) {
            for (int k = 0; k < GRIDZ; k++) {
                glm_mat4_identity(model);
                glm_translate(model, (vec3) {-5.0f + (i * 2), 0.0f + (j * 2), 0.0f + (k * 2)});
                cg_shader_uniform_matrix4fv(shaderId, "model", &model);
                cg_model_draw(m, shaderId);
            }
        }
    }
}

void wfc_grid_create(WFC_Grid* g) {
    for (int i = 0; i < GRIDX; i++) {
        for (int j = 0; j < GRIDY; j++) {
            for (int k = 0; k < GRIDZ; k++) {
                g->grid[i][j][k] = 0b1111;
            }
        }
    }
}

bool wfc_collapsed(WFC_Grid* g) {
    for (int i = 0; i < GRIDX; i++) {
        for (int j = 0; j < GRIDY; j++) {
            for (int k = 0; k < GRIDZ; k++) {
                if (g->grid[i][j][k] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}


void wfc_grid_find_empty_space(WFC_Grid* g, int* iind, int* jind, int* kind) {
    for (int i = 0; i < GRIDX; i++) {
        for (int j = 0; j < GRIDY; j++) {
            for (int k = 0; k < GRIDZ; k++) {
                if (g->grid[i][j][k] > 1) {
                    *iind = i; 
                    *jind = j; 
                    *kind = k; 
                }
            }
        }
    }
}

void wfc_grid_least_entropy(WFC_Grid* g, int* iind, int* jind, int* kind) {
    int minEntropy = 0b1111; // maximum entropy possible
    for (int i = 0; i < GRIDX; i++) {
        for (int j = 0; j < GRIDY; j++) {
            for (int k = 0; k < GRIDZ; k++) {
                if (g->grid[i][j][k] < minEntropy) {
                    *iind = i;
                    *jind = j;
                    *kind = k;
                    minEntropy = g->grid[i][j][k];
                }
            }
        }
    }
}

void wfc_collapse_at(WFC_Grid* g, int i, int j, int k) {
    int n = g->grid[i][j][k];
    int bits = cg_tool_count_bits(n);
    int setBits[BLOCK_SIZE] = {-1};
    int m = 0;
    for (int l = 0; l < BLOCK_SIZE; l++) {
        if (n & 1) {
            setBits[m++] = l;
        } 
        n >>= 1;
    }

    int index = rand() % bits;
    assert(m > index);
    g->grid[i][j][k] = 1 << setBits[index];
}

void wfc_propogate(WFC_Grid* g, int i, int j, int k) {
    if (i < 0 || i > GRIDX) {
        return;
    }
    if (j < 0 || j > GRIDY) {
        return;
    }
    if (k < 0 || k > GRIDZ) {
        return;
    }

    wfc_collapse_at(g, i, j, k);
    wfc_propogate(g, i-1, j, k);
    wfc_propogate(g, i+1, j, k);
}

// TODO: You need to have a list of possible prototypes not modules.
// There are different rotations that exist, and that's important to take into account
// Store an array of bools with available slots perhaps,
void wfc_prototype_create(WFC_Prototype* prototype, WFC_Module module) {
    //arrput(prototype->modules, module);
}


void wfc_wfc(WFC_Grid* g) {
    while (!wfc_collapsed(g)) {
        int i, j, k;
        wfc_grid_least_entropy(g, &i, &j, &k);
        wfc_collapse_at(g, i, j, k);
        wfc_propogate(g, i, j, k);
    }
}

WFC_Module* wfc_module_create_grass() {
    WFC_Module* grass = NULL;
    WFC_Module g1;
    g1.px = 0;
    g1.nx = 0;

    g1.py = 0;
    g1.ny = 0;

    g1.pz = 0;
    g1.nz = 0;

    WFC_Module g2;
    g2.px = -1;
    g2.nx = 0;

    g2.py = 0;
    g2.ny = 0;

    g2.pz = 0;
    g2.nz = 0;

    arrput(grass, g1);
    arrput(grass, g2);

    return grass;
}

WFC_Module* wfc_module_create_sand() {
    WFC_Module* sand = NULL;

    // sand : grass, water
    WFC_Module sand1;
    sand1.px = 1; 
    sand1.nx = 0; 

    sand1.py = -1;
    sand1.ny = 0;

    sand1.pz = 0;
    sand1.nz = 0;

    WFC_Module sand2; // rotation 90
    sand2.px = 0; 
    sand2.nx = 0; 

    sand2.py = -1;
    sand2.ny = 0;

    sand2.pz = 1;
    sand2.nz = 0;

    WFC_Module sand3; // rotation 180 
    sand3.px = 0; 
    sand3.nx = 1; 

    sand3.py = -1;
    sand3.ny = 0;

    sand3.pz = 0;
    sand3.nz = 0;

    WFC_Module sand4; // rotation 270 
    sand4.px = 0; 
    sand4.nx = 0; 

    sand4.py = -1;
    sand4.ny = 0;

    sand4.pz = 0;
    sand4.nz = 1;

    arrput(sand, sand1);
    arrput(sand, sand2);
    arrput(sand, sand3);
    arrput(sand, sand4);
    return sand;
}

WFC_Module* wfc_module_create_water() {
    WFC_Module* water = NULL;
    // water : sand, water
    WFC_Module w;
    w.px = 1; 
    w.nx = 1; 

    w.py = 3;
    w.ny = 0;

    w.pz = 1;
    w.nz = 1;

    arrput(water, w);
    return water;
}

WFC_Module* wfc_module_create_air() {
    WFC_Module* air = NULL;
    WFC_Module a;
    a.px = -1; 
    a.nx = -1; 

    a.py = -1;
    a.ny = -1;

    a.pz = -1;
    a.nz = -1;
    arrput(air, a);
    return air;
}

void wfc_module_destroy_module(WFC_Module* module) {
    arrfree(module);
}
