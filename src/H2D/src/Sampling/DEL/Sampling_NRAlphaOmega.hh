#ifndef Sampling_NRAlphaOmega_HH
#define Sampling_NRAlphaOmega_HH


#include "Sample2dim.hh"
#include "SamplingAlphaOmegaNewton.hh"
#include "SamplingAlphaOmegaBisection.hh"
#include "../infra/tmath.hh"
#include "../infra/linfun.hh"
#include "../infra/newton_tt.hh"
#include "infra/FukushimaLambertW.hh"

namespace H2D{



class Sampling_NRAlphaOmega : public Sample2dim{
	public:
		Sampling_NRAlphaOmega(const Data2dim& aData,
		//       const uint      aSize,       	
                       const uint      aSampleSize,
                       const double    aQ,
                       const double    aTheta,
                       const bool      aTrace,
		       const double aEps);
		virtual ~Sampling_NRAlphaOmega();
	private:
        	double _aEps;
		//Data2dim _data;
                //SamplingAlphaOmegaNewton _alphaSampler;
	public:
		virtual double estimate(const H2D::query_t&) const;
	


};


			    
} //end namespace


#endif
