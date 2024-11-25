#ifndef HIST_K_DIM_ARG_HH
#define HIST_K_DIM_ARG_HH

#include "infra/argbase.hh"
#include "infra/cb.hh"

typedef std::vector<argdescbase_t<H2D::Cb>*> argdesc_vt;

void construct_arg_desc(argdesc_vt& aArgDesc);

#endif

