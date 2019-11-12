#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ameutils.h"
 
#define _SUBMODELNAME_ "BASIC_AERO_DRAG_FORMULA"
 
void basic_aero_drag_formulain_(int *n, double *DragCoefficient
	, double *ContactArea, double *Speed, double *FluidDensity)
 
{
	*DragCoefficient = 7.00000000000000e-001;
	*ContactArea = 1.00000000000000e+000;
	*Speed = 0.00000000000000e+000;
}
 
void basic_aero_drag_formula_(int *n, double *DragCoefficient
	, double *ContactArea, double *Speed, double *AeroDragForce
	, double *FluidDensity, int *flag)
 
{
	*AeroDragForce = -1.0 * (*ContactArea) * (*DragCoefficient) * (*FluidDensity) * (*Speed) * (*Speed) * 0.5;
}
