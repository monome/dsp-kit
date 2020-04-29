//
// Created by emb on 4/28/20.
//

#include "FastMover.hpp"

using dspkit::FastMover;
const float FastMover::shapeTables[FastMover::numTables][FastMover::tableSize] = {
    {
#include "inc/easing_linear.inc"
    },
    {
#include "inc/easing_in_back.inc"
    },
    {
#include "inc/easing_in_bounce.inc"
    },
    {
#include "inc/easing_in_circular.inc"
    },
    {
#include "inc/easing_in_cubic.inc"
    },
    {
#include "inc/easing_in_elastic.inc"
    },
    {
#include "inc/easing_in_exponential.inc"
    },
    {
#include "inc/easing_in_quadratic.inc"
    },
    {
#include "inc/easing_in_quartic.inc"
    },
    {
#include "inc/easing_in_quintic.inc"
    },
    {
#include "inc/easing_in_sine.inc"
    },
    {
#include "inc/easing_in_out_back.inc"
    },
    {
#include "inc/easing_in_out_bounce.inc"
    },
    {
#include "inc/easing_in_out_circular.inc"
    },
    {
#include "inc/easing_in_out_cubic.inc"
    },
    {
#include "inc/easing_in_out_elastic.inc"
    },
    {
#include "inc/easing_in_out_exponential.inc"
    },
    {
#include "inc/easing_in_out_quadratic.inc"
    },
    {
#include "inc/easing_in_out_quartic.inc"
    },
    {
#include "inc/easing_in_out_quintic.inc"
    },
    {
#include "inc/easing_in_out_sine.inc"
    },
    {
#include "inc/easing_out_back.inc"
    },
    {
#include "inc/easing_out_bounce.inc"
    },
    {
#include "inc/easing_out_circular.inc"
    },
    {
#include "inc/easing_out_cubic.inc"
    },
    {
#include "inc/easing_out_elastic.inc"
    },
    {
#include "inc/easing_out_exponential.inc"
    },
    {
#include "inc/easing_out_quadratic.inc"
    },
    {
#include "inc/easing_out_quartic.inc"
    },
    {
#include "inc/easing_out_quintic.inc"
    },
    {
#include "inc/easing_out_sine.inc"
    },
};
