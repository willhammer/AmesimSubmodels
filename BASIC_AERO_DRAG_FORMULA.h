#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ameutils.h"
 

void basic_aero_drag_formulain_(int *n, double *DragCoefficient, double *ContactArea, double *Speed, double *FluidDensity);
void basic_aero_drag_formula_(int *n, double *DragCoefficient, double *ContactArea, 
							double *Speed, double *AeroDragForce, double *FluidDensity, int *flag);

