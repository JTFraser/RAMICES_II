#include "YieldRidge.h"

YieldRidge::YieldRidge(): Source(Unknown), Z(-10)
{
	
}
YieldRidge::YieldRidge(SourceID source, double z, int nPoints): Source(source), Z(z)
{
	Points.resize(nPoints);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

double interp(double x, double x1, double x2)
{
	double extrap = 0.4;
	double scale = 5;
	double inter = (x - x1)/(x2 - x1);
	inter = std::min(std::max(inter,-extrap),1.0+extrap);
	return inter;
	//~ if (x >= x1 && x <= x2)
	//~ {
		//~ return inter; 
	//~ }
	
	//~ if (x < x1)
	//~ {
		//~ if (inter 
		//~ return 0;
	//~ }
	//~ if (x > x2)
	//~ {
		//~ double d = abs(x2 - x);
		//~ return 1;
	//~ }
}

double YieldBracket::Interpolate(double mass, double z)
{
	int lowZMass = 0;
	int highZMass = 0;
	while (lowZMass < LowerRidge.Points.size() -2 && LowerRidge.Points[lowZMass+1].Mass <= mass)
	{
		++lowZMass;
	}
	while (highZMass < UpperRidge.Points.size() -2 && UpperRidge.Points[highZMass+1].Mass <= mass)
	{
		++highZMass;
	}
	
	
	double lowZLowMass = LowerRidge.Points[lowZMass].Mass;
	double lowZHighMass = LowerRidge.Points[lowZMass+1].Mass;
	
	
	double lowZInterp = interp(mass,lowZLowMass,lowZHighMass);
	
	double lowZValue = LowerRidge.Points[lowZMass].Yield + lowZInterp * (LowerRidge.Points[lowZMass+1].Yield - LowerRidge.Points[lowZMass].Yield);
	if (sgn(LowerRidge.Points[lowZMass].Yield) == sgn(LowerRidge.Points[lowZMass+1].Yield) && sgn(LowerRidge.Points[lowZMass].Yield) != sgn(lowZValue))
	{
		lowZValue = 0;
	}
	
	double highZLowMass = UpperRidge.Points[highZMass].Mass;
	double highZHighMass = UpperRidge.Points[highZMass+1].Mass;
	
	
	
	double highZInterp = interp(mass,highZLowMass,highZHighMass);
	double highZValue = UpperRidge.Points[highZMass].Yield + highZInterp * (UpperRidge.Points[highZMass+1].Yield - UpperRidge.Points[highZMass].Yield);
	
	if (sgn(UpperRidge.Points[highZMass].Yield) == sgn(UpperRidge.Points[highZMass+1].Yield) && sgn(UpperRidge.Points[highZMass].Yield) != sgn(highZValue))
	{
		highZValue = 0;
	}
	
	double logZInterp;
	double upZ = 0;
	double downZ = 0;
	if (hasSingle)
	{
		logZInterp = 0;
	}
	else
	{
		upZ = log10(UpperRidge.Z) ;
		downZ = log10(LowerRidge.Z) ;
		logZInterp = (log10(std::min(UpperRidge.Z,z)) - downZ)/(upZ - downZ);
	}
	
	double interpolatedValue =  lowZValue + logZInterp * (highZValue - lowZValue);
	
	return interpolatedValue;
}
